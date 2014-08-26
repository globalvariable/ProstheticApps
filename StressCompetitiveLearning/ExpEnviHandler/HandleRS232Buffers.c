#include "HandleRS232Buffers.h"


typedef union 			
{
	struct 
	{
		unsigned guide_led:1;  
		unsigned left_led:1;  
		unsigned right_led:1;  
		unsigned levers_available:1;  
		unsigned valve:1;  
		unsigned buzzer:1;
		unsigned exp_envi_switch:1;
		unsigned get_ready_led:1;
	} ;
	unsigned char all_cmd;  /// one byte (unsigned char, see it via Watch in debug mode)
} ExpEnviRS232Cmd;


typedef union  			
{
	struct 
	{
		unsigned ir_beam:1;  
		unsigned left_lever:1;  
		unsigned right_lever:1;  
		unsigned ir_beam_2:1;
		unsigned none1:1;
		unsigned none2:1;
		unsigned none3:1;
		unsigned none4:1;
	} ;
	unsigned char all_status;  /// one byte (unsigned char, see it via Watch in debug mode)
} ExpEnviRS232Status;



#define EXP_ENVI_STATUS_MSG_LEN			1
#define EXP_ENVI_STATUS_MSG_START_IDX	1
#define EXP_ENVI_CMD_MSG_LEN				1
#define EXP_ENVI_CMD_MSG_START_IDX		1

static SEM *static_exp_envi_rx_buff_sem = NULL;
static SEM *static_exp_envi_tx_buff_sem = NULL;
static ExpEnviRxShm *static_exp_envi_rx_buff_shm = NULL;
static ExpEnviTxShm *static_exp_envi_tx_buff_shm = NULL;

static ExpEnviRS232Cmd *exp_envi_rs232_cmd = NULL;
static ExpEnviRS232Status *exp_envi_rs232_status = NULL;

bool init_rs232_buffers(SEM **exp_envi_rx_buff_sem, SEM **exp_envi_tx_buff_sem, ExpEnviRxShm **exp_envi_rx_buff_shm, ExpEnviTxShm **exp_envi_tx_buff_shm)
{

	exp_envi_rs232_cmd = g_new0(ExpEnviRS232Cmd, 1);
 	exp_envi_rs232_status = g_new0(ExpEnviRS232Status, 1);

	if (! bind_to_exp_envi_rx_buffer_semaphore(exp_envi_rx_buff_sem))
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "init_rx_tx_buffer", "! bind_to_exp_envi_rx_buffer_semaphore().");	
	if (! bind_to_exp_envi_tx_buffer_semaphore(exp_envi_tx_buff_sem))
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "init_rx_tx_buffer", "! bind_to_exp_envi_tx_buffer_semaphore().");	
	if (! bind_to_exp_envi_tx_buffer_shm(exp_envi_tx_buff_shm) )
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "init_rx_tx_buffer", "! bind_to_exp_envi_tx_buffer_shm().");	
	if (! bind_to_exp_envi_rx_buffer_shm(exp_envi_rx_buff_shm) )
		return print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "init_rx_tx_buffer", "! bind_to_exp_envi_rx_buffer_shm().");	

	static_exp_envi_rx_buff_sem = *exp_envi_rx_buff_sem; 
	static_exp_envi_tx_buff_sem = *exp_envi_tx_buff_sem;
	static_exp_envi_rx_buff_shm = *exp_envi_rx_buff_shm;
	static_exp_envi_tx_buff_shm = *exp_envi_tx_buff_shm;

	return TRUE;
}

bool handle_exp_envi_tx_shm(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviOutputStatusHistory *exp_envi_output_status_history)
{
	ExpEnviTxShm exp_envi_tx_buffer;
	static ExpEnviCompStatus output_components_status_prev[NUM_OF_OUTPUT_COMPONENTS] = {0,0,0,0,0,0};

	if (exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[VALVE_IDX_IN_EXP_ENVI_DATA])
	{
		switch(exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->valve = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->valve = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "hhandle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[VALVE_IDX_IN_EXP_ENVI_DATA].status) - default");
		}
		output_components_status_prev[VALVE_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_VALVE_CENTER, exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}

	if (exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[BUZZER_IDX_IN_EXP_ENVI_DATA])
	{			
		switch(exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->buzzer = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->buzzer = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "hhandle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[BUZZER_IDX_IN_EXP_ENVI_DATA].status) - default");
		}
		output_components_status_prev[BUZZER_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_BUZZER, exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}
	
	if (exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[LEFT_LED_IDX_IN_EXP_ENVI_DATA])
	{	
		switch(exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->left_led = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->left_led = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "hhandle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
		}		
		output_components_status_prev[LEFT_LED_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_LEFT_TARGET_LED, exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}

	if (exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[RIGHT_LED_IDX_IN_EXP_ENVI_DATA])
	{	
		switch(exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->right_led = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->right_led = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "hhandle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
		}		
		output_components_status_prev[RIGHT_LED_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_RIGHT_TARGET_LED, exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}

	if (exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[GUIDE_LED_IDX_IN_EXP_ENVI_DATA])
	{	
		switch(exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->guide_led = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->guide_led = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "handle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
		}		
		output_components_status_prev[GUIDE_LED_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_GUIDE_LED, exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}
	
	if (exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA])
	{	
		switch(exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->levers_available = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->levers_available = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "handle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status) - default");
		}		
		output_components_status_prev[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_LEVER_SOLENOID, exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}

	if (exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status != output_components_status_prev[GET_READY_LED_IDX_IN_EXP_ENVI_DATA])
	{	
		switch(exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status)
		{
			case EXP_ENVI_COMP_STATUS_LOW:
				exp_envi_rs232_cmd->get_ready_led = 0;
				break;
			case EXP_ENVI_COMP_STATUS_HIGH:
				exp_envi_rs232_cmd->get_ready_led = 1;
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffer", "hhandle_exp_envi_tx_shm", "switch(exp_envi_data->outp_comps[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
		}		
		output_components_status_prev[GET_READY_LED_IDX_IN_EXP_ENVI_DATA] = exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status;
		if (! write_to_exp_envi_output_status_history(exp_envi_output_status_history, current_time, EXP_ENVI_OUTPUT_COMPONENT_GET_READY_LED, exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status))
			return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_output_status_history().");
	}

	exp_envi_tx_buffer.last_write_time = current_time;
	exp_envi_tx_buffer.exp_envi_tx_buff[0] = exp_envi_rs232_cmd->all_cmd;
	if (! write_to_exp_envi_tx_buff_shm(&exp_envi_tx_buffer, static_exp_envi_tx_buff_shm, EXP_ENVI_CMD_MSG_LEN, static_exp_envi_tx_buff_sem))  //  Exp Envi Handler writes its command to static_exp_envi_tx_buff for delivery by this process
		print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "handle_exp_envi_tx_shm", "! read_exp_envi_tx_buff_shm()."); 

	return TRUE;
}

bool handle_exp_envi_rx_shm(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, ExpEnviHand2TrialHandMsg* msgs_exp_envi_hand_2_trial_hand, ExpEnviInputStatusHistory *exp_envi_input_status_history)
{
	static ExpEnviRS232Status exp_envi_rs232_status_prev = { .all_status = 0 };
	ExpEnviRxShm exp_envi_rx_buffer;
	bool cancellation_reqiured_for_low_status_timer, setting_required_for_high_status_timer;
	bool cancellation_reqiured_for_high_status_timer, setting_required_for_low_status_timer;

	if (! read_exp_envi_rx_buff_shm(&exp_envi_rx_buffer, static_exp_envi_rx_buff_shm, EXP_ENVI_STATUS_MSG_LEN, static_exp_envi_rx_buff_sem)) 
		print_message(ERROR_MSG ,"BMIExpController", "HandleRxTxBuffer", "handle_exp_envi_rx_shm", "! read_exp_envi_tx_buff_shm()."); 
	exp_envi_rs232_status->all_status = exp_envi_rx_buffer.exp_envi_rx_buff[0];
//	printf ("%u\n", exp_envi_rs232_status->all_status);

	if (exp_envi_rs232_status_prev.ir_beam != exp_envi_rs232_status->ir_beam)
	{
		exp_envi_rs232_status_prev.ir_beam = exp_envi_rs232_status->ir_beam;

		if (exp_envi_rs232_status->ir_beam)
		{
			if (! exp_envi_input_low_2_high_event(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &cancellation_reqiured_for_low_status_timer, &setting_required_for_high_status_timer))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "exp_envi_input_low_2_high_event().");
			if (cancellation_reqiured_for_low_status_timer)	 
			{
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
			}
			if (setting_required_for_high_status_timer)
			{	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_high_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_high_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
			}
			if (! write_to_exp_envi_input_status_history(exp_envi_input_status_history, current_time, EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE, EXP_ENVI_COMP_STATUS_HIGH))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_input_status_history().");
		}
		else
		{
			if (! exp_envi_input_high_2_low_event(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &cancellation_reqiured_for_high_status_timer, &setting_required_for_low_status_timer))
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "exp_envi_input_high_2_low_event().");
			if (cancellation_reqiured_for_high_status_timer)	 
			{
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");		
			}
			if (setting_required_for_low_status_timer)
			{	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_low_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_low_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
			}	
			if (! write_to_exp_envi_input_status_history(exp_envi_input_status_history, current_time, EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE, EXP_ENVI_COMP_STATUS_LOW))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_input_status_history().");
		}
	}



	if (exp_envi_rs232_status_prev.left_lever != exp_envi_rs232_status->left_lever)
	{
		exp_envi_rs232_status_prev.left_lever = exp_envi_rs232_status->left_lever;
	}
	if (exp_envi_rs232_status_prev.right_lever != exp_envi_rs232_status->right_lever)
	{
		exp_envi_rs232_status_prev.right_lever = exp_envi_rs232_status->right_lever;
	}

	if (exp_envi_rs232_status_prev.ir_beam_2 != exp_envi_rs232_status->ir_beam_2)
	{
		exp_envi_rs232_status_prev.ir_beam_2 = exp_envi_rs232_status->ir_beam_2;

		if (exp_envi_rs232_status->ir_beam_2)
		{
			if (! exp_envi_input_low_2_high_event(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]), &cancellation_reqiured_for_low_status_timer, &setting_required_for_high_status_timer))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "exp_envi_input_low_2_high_event().");
			if (cancellation_reqiured_for_low_status_timer)	 
			{
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
			}
			if (setting_required_for_high_status_timer)
			{	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA].constraints.min_high_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA].constraints.max_high_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
			}
			if (! write_to_exp_envi_input_status_history(exp_envi_input_status_history, current_time, EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE, EXP_ENVI_COMP_STATUS_HIGH))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_input_status_history().");
		}
		else
		{
			if (! exp_envi_input_high_2_low_event(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]), &cancellation_reqiured_for_high_status_timer, &setting_required_for_low_status_timer))
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "exp_envi_input_high_2_low_event().");
			if (cancellation_reqiured_for_high_status_timer)	 
			{
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");		
			}
			if (setting_required_for_low_status_timer)
			{	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA].constraints.min_low_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA].constraints.max_low_status_duration + current_time))
					return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
			}	
			if (! write_to_exp_envi_input_status_history(exp_envi_input_status_history, current_time, EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE, EXP_ENVI_COMP_STATUS_LOW))
				return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_input_status_history().");
		}
	}
	return TRUE;
}
