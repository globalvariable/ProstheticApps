#include "HandleRS232Buffers.h"


#define EXP_ENVI_STATUS_MSG_LEN			1
#define EXP_ENVI_STATUS_MSG_START_IDX	1
#define EXP_ENVI_CMD_MSG_LEN				1
#define EXP_ENVI_CMD_MSG_START_IDX		1

#define ROBOT_POSITION_MSG_LEN		2*THREE_DOF_ROBOT_NUM_OF_SERVOS
#define ROBOT_POSITION_MSG_START_IDX	EXP_ENVI_STATUS_MSG_START_IDX+EXP_ENVI_STATUS_MSG_LEN

#define ROBOT_PW_CMD_MSG_LEN		2*THREE_DOF_ROBOT_NUM_OF_SERVOS
#define ROBOT_PW_CMD_MSG_START_IDX	EXP_ENVI_CMD_MSG_START_IDX+EXP_ENVI_CMD_MSG_LEN

#define RX_BUFF_SIZE				1 + EXP_ENVI_STATUS_MSG_LEN + (2*THREE_DOF_ROBOT_NUM_OF_SERVOS) + 2		// 0xFF + EXP_ENVI_STATUS (1 BYTE) + SERVO_0_POSITION_L_BYTE + SERVO_0_POSITION_H_BYTE + SERVO_1_POSITION_L_BYTE + SERVO_1_ POSITION_H_BYTE + SERVO_2_POSITION_L_BYTE + SERVO_2_ POSITION_H_BYTE + 0xFF + 0xFF

#define PW_TX_BUFF_SIZE			1 + EXP_ENVI_CMD_MSG_LEN + (2*THREE_DOF_ROBOT_NUM_OF_SERVOS) + 2		// 'P' + EXP_ENVI_COMND (1 BYTE) + SERVO_0_PW_L_BYTE + SERVO_0_PW_H_BYTE + SERVO_1_PW_L_BYTE + SERVO_1_ PW_H_BYTE + SERVO_2_PW_L_BYTE + SERVO_2_ PW_H_BYTE + 0xFF + 0xFF	

#define ADC_TX_BUFF_SIZE			1 + EXP_ENVI_CMD_MSG_LEN + 2		// 'A' + EXP_ENVI_COMND (1 BYTE) + 0xFF + 0xFF	

static ThreeDofRobot *static_robot_arm = NULL;
static SEM *static_exp_envi_rx_buff_sem = NULL;
static SEM *static_exp_envi_tx_buff_sem = NULL;
static ExpEnviRxShm *static_exp_envi_rx_buff_shm = NULL;
static ExpEnviTxShm *static_exp_envi_tx_buff_shm = NULL;
static unsigned char adc_tx_buffer[ADC_TX_BUFF_SIZE];
static unsigned char pw_tx_buffer[PW_TX_BUFF_SIZE];

static bool exp_envi_rx_switch = FALSE;

bool init_rs232_buffers(ThreeDofRobot *robot_arm, SEM **exp_envi_rx_buff_sem, SEM **exp_envi_tx_buff_sem, ExpEnviRxShm **exp_envi_rx_buff_shm, ExpEnviTxShm **exp_envi_tx_buff_shm, TimeStamp current_time)
{
	static_robot_arm = robot_arm;

	if (! init_rs232_com1(115200))
 		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "init_rs232_buffers", "! init_rs232_com1().");	
	if (! init_exp_envi_rx_buffer_semaphore(exp_envi_rx_buff_sem))
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "init_rs232_buffers", "! init_exp_envi_rx_buffer_semaphore().");	
	if (! init_exp_envi_tx_buffer_semaphore(exp_envi_tx_buff_sem))
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "init_rs232_buffers", "! init_exp_envi_tx_buffer_semaphore().");	
	if (! init_exp_envi_tx_buffer_shm(exp_envi_tx_buff_shm, EXP_ENVI_CMD_MSG_LEN, current_time) )
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "init_rs232_buffers", "! init_exp_envi_tx_buffer_shm().");	
	if (! init_exp_envi_rx_buffer_shm(exp_envi_rx_buff_shm, EXP_ENVI_STATUS_MSG_LEN) )
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "init_rs232_buffers", "! init_exp_envi_rx_buffer_shm().");	

	static_exp_envi_rx_buff_sem = *exp_envi_rx_buff_sem; 
	static_exp_envi_tx_buff_sem = *exp_envi_tx_buff_sem;
	static_exp_envi_rx_buff_shm = *exp_envi_rx_buff_shm;
	static_exp_envi_tx_buff_shm = *exp_envi_tx_buff_shm;

	adc_tx_buffer[0] = 'A';
	adc_tx_buffer[ADC_TX_BUFF_SIZE-2] = 0xFF;
	adc_tx_buffer[ADC_TX_BUFF_SIZE-1] = 0xFF;

	pw_tx_buffer[0] = 'P';
	pw_tx_buffer[PW_TX_BUFF_SIZE-2] = 0xFF;
	pw_tx_buffer[PW_TX_BUFF_SIZE-1] = 0xFF;	
	return TRUE;
}

bool handle_exp_envi_tx_shm_and_send_rs232_adc_command(TimeStamp current_time)
{
	unsigned int i;
	ExpEnviTxShm exp_envi_tx_buffer;
	if (! read_exp_envi_tx_buff_shm(&exp_envi_tx_buffer, static_exp_envi_tx_buff_shm, EXP_ENVI_CMD_MSG_LEN, static_exp_envi_tx_buff_sem))    //  Exp Envi Handler writes its command to static_exp_envi_tx_buff for delivery by this process
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "rs232_handle_exp_envi_tx_shm_and_send_adc_command", "! read_exp_envi_tx_buff_shm()."); 
	if ((current_time - exp_envi_tx_buffer.last_write_time) < 10000000)
	{
		if (exp_envi_rx_switch)
		{
			exp_envi_rx_switch = 0;
			exp_envi_tx_buffer.exp_envi_tx_buff[0] = exp_envi_tx_buffer.exp_envi_tx_buff[0] && 0b10111111;   // MCU check 7th bit swtiching to save outputs from burning (every ~ 50 ms)
		}
		else
		{
			exp_envi_rx_switch = 1;
			exp_envi_tx_buffer.exp_envi_tx_buff[0] = exp_envi_tx_buffer.exp_envi_tx_buff[0] || 0b01000000;		
		}
	}
//	printf ("%u\n", exp_envi_tx_buffer.exp_envi_tx_buff[0] );
	for (i = 0; i < EXP_ENVI_CMD_MSG_LEN; i++)
		adc_tx_buffer[EXP_ENVI_CMD_MSG_START_IDX+i] = exp_envi_tx_buffer.exp_envi_tx_buff[i];   // 'A' + EXP_ENVI_COMND (1 BYTE) + 0xFF + 0xFF	

	if (! write_to_rs232_com1(adc_tx_buffer, ADC_TX_BUFF_SIZE)) 
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "rs232_handle_exp_envi_tx_shm_and_send_adc_command", "! write_to_rs232_com1()."); 
	return TRUE;
}

bool handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command(TimeStamp current_time, ThreeDofRobotPulseHistory *robot_pulse_history, double max_servo_angle_change, double spike_count_threshold)
{
	unsigned int i;
	ExpEnviTxShm exp_envi_tx_buffer;
	unsigned char cmd_low_byte, cmd_high_byte;

	if (! read_exp_envi_tx_buff_shm(&exp_envi_tx_buffer, static_exp_envi_tx_buff_shm, EXP_ENVI_CMD_MSG_LEN, static_exp_envi_tx_buff_sem)) 
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command", "! read_exp_envi_tx_buff_shm()."); 
	if ((current_time - exp_envi_tx_buffer.last_write_time) < 10000000)
	{
		if (exp_envi_rx_switch)
		{
			exp_envi_rx_switch = 0;
			exp_envi_tx_buffer.exp_envi_tx_buff[0] = exp_envi_tx_buffer.exp_envi_tx_buff[0] & 0b10111111;   // MCU check 7th bit swtiching to save outputs from burning (every ~ 50 ms)
		}
		else
		{
			exp_envi_rx_switch = 1;
			exp_envi_tx_buffer.exp_envi_tx_buff[0] = exp_envi_tx_buffer.exp_envi_tx_buff[0] | 0b01000000;		
		}
	}

	for (i = 0; i < EXP_ENVI_CMD_MSG_LEN; i++)
		pw_tx_buffer[EXP_ENVI_CMD_MSG_START_IDX+i] = exp_envi_tx_buffer.exp_envi_tx_buff[i];

	if ( check_three_dof_robot_security_limits(static_robot_arm))
	{
		for (i = 0; i < THREE_DOF_ROBOT_NUM_OF_SERVOS; i++)
			evaluate_servo_pw_command_via_spike_count_with_limitation_count_thresholding(&(static_robot_arm->servos[i]), max_servo_angle_change, spike_count_threshold);


		if (! write_to_three_dof_robot_pulse_history(robot_pulse_history, current_time, static_robot_arm->servos[BASE_SERVO].pulse_current, static_robot_arm->servos[SHOULDER_SERVO].pulse_current, static_robot_arm->servos[ELBOW_SERVO].pulse_current))
			return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command", "! write_to_three_dof_robot_pulse_history()."); 
		for (i = 0; i < ROBOT_PW_CMD_MSG_LEN; i+=2)
		{
			get_servo_pw_val_bytes(&(static_robot_arm->servos[(unsigned int) (i/2)]), &cmd_low_byte, &cmd_high_byte);
			pw_tx_buffer[ROBOT_PW_CMD_MSG_START_IDX + i] = cmd_low_byte;
			pw_tx_buffer[ROBOT_PW_CMD_MSG_START_IDX + i +1] = cmd_high_byte;
		}
		if (! write_to_rs232_com1(pw_tx_buffer, PW_TX_BUFF_SIZE)) 
			return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command", "! write_to_rs232_com1()."); 
	}
	else
	{
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command", "! check_three_dof_robot_out_of_security_limits()."); 	
	}

	return TRUE;
}

bool handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm(void)
{
	unsigned int i;
	unsigned char rx_buffer[RX_BUFF_SIZE];
	clear_rx_buffer(rx_buffer, RX_BUFF_SIZE);
	if (! read_from_rs232_com1(rx_buffer, RX_BUFF_SIZE)) 
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm", "! read_from_rs232_com1().");
	if ((rx_buffer[0] != 0xFF) || (rx_buffer[RX_BUFF_SIZE-2] != 0xFF) || (rx_buffer[RX_BUFF_SIZE-1] != 0xFF)) 
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm", "Invalid rx message format."); 
	if (! write_to_exp_envi_rx_buff_shm(&(rx_buffer[EXP_ENVI_STATUS_MSG_START_IDX]), static_exp_envi_rx_buff_shm, EXP_ENVI_STATUS_MSG_LEN, static_exp_envi_rx_buff_sem)) 
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm", "! write_to_exp_envi_rx_buff_shm()."); 
	for (i = 0; i <  ROBOT_POSITION_MSG_LEN; i+=2)
		write_servo_position_val(&(static_robot_arm->servos[(unsigned int) (i/2)]), rx_buffer[ROBOT_POSITION_MSG_START_IDX + i], rx_buffer[ROBOT_POSITION_MSG_START_IDX + i + 1]);
	return TRUE;
}
