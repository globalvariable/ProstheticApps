#include "MovObjHandlerRtTask.h"

static RtTasksData *static_rt_tasks_data = NULL;

static MovObjStatus mov_obj_status = MOV_OBJ_STATUS_DISABLED;   // Only mov_obj handler can change status. 

static int mov_obj_handler_rt_thread = 0;
static bool rt_mov_obj_handler_stay_alive = 1;

static void *rt_mov_obj_handler(void *args);

static ThreeDofRobot *static_robot_arm = NULL;
static MovObjHandParadigmRobotReach *static_mov_obj_paradigm = NULL;

static Gui2MovObjHandMsg *static_msgs_gui_2_mov_obj_hand = NULL;
static MovObjHand2GuiMsg *static_msgs_mov_obj_hand_2_gui = NULL;

static TrialHand2MovObjHandMsg *msgs_trial_hand_2_mov_obj_hand = NULL;
static MovObjHand2TrialHandMsg *msgs_mov_obj_hand_2_trial_hand = NULL;

static MovObjDurHand2MovObjHandMsg *msgs_mov_obj_dur_hand_2_mov_obj_hand = NULL;
static MovObjHand2MovObjDurHandMsg *msgs_mov_obj_hand_2_mov_obj_dur_hand = NULL;   

static MovObjHand2NeuralNetMsg **msgs_mov_obj_hand_2_neural_net_multi_thread = NULL;
static NeuralNet2MovObjHandMsg **msgs_neural_net_2_mov_obj_hand_multi_thread = NULL;

static MovObjStatusHistory* static_mov_obj_status_history = NULL;
static ThreeDofRobotAngleHistory *static_robot_angle_history = NULL;
static ThreeDofRobotPulseHistory *static_robot_pulse_history = NULL;

static MessageLogBuffer *static_message_log = NULL;

static SpikeData *scheduled_spike_data = NULL;

static bool connect_to_trial_hand(void);
static bool connect_to_neural_net(void);

/*
static bool connect_to_mov_obj_interf(void );
*/


bool create_mov_obj_handler_rt_thread(RtTasksData *rt_tasks_data, ThreeDofRobot *robot_arm, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, MovObjHandParadigmRobotReach *mov_obj_paradigm, MessageLogBuffer *message_log, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history)
{
	static_rt_tasks_data = rt_tasks_data;

	static_robot_arm = robot_arm;
	static_mov_obj_paradigm = mov_obj_paradigm;

	static_msgs_gui_2_mov_obj_hand = msgs_gui_2_mov_obj_hand;
	static_msgs_mov_obj_hand_2_gui = msgs_mov_obj_hand_2_gui;

	static_mov_obj_status_history = mov_obj_status_history;
	static_robot_angle_history = robot_angle_history;
	static_robot_pulse_history = robot_pulse_history;

	static_message_log = message_log;

	msgs_trial_hand_2_mov_obj_hand = allocate_shm_server_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand);
	msgs_neural_net_2_mov_obj_hand_multi_thread = allocate_shm_server_neural_net_2_mov_obj_hand_multi_thread_msg_buffer(msgs_neural_net_2_mov_obj_hand_multi_thread, SNN_SIM_NUM_OF_DEDICATED_CPUS);
	msgs_mov_obj_hand_2_neural_net_multi_thread = g_new0(MovObjHand2NeuralNetMsg*, SNN_SIM_NUM_OF_DEDICATED_CPUS); 

	init_mov_obj_duration_handler();

	if (!connect_to_neural_net())
		return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "create_mov_obj_handler_rt_thread", "connect_to_neural_net().");	

	if (! connect_to_trial_hand())
		return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "create_mov_obj_handler_rt_thread", "connect_to_trial_hand().");	

	msgs_mov_obj_dur_hand_2_mov_obj_hand = allocate_mov_obj_dur_hand_2_mov_obj_hand_msg_buffer(msgs_mov_obj_dur_hand_2_mov_obj_hand);
	msgs_mov_obj_hand_2_mov_obj_dur_hand = allocate_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand);

	scheduled_spike_data = allocate_spike_data(scheduled_spike_data, NEURAL_NET_2_MOV_OBJ_HAND_MSG_BUFF_SIZE);

	if (mov_obj_handler_rt_thread !=0)
		return print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "create_mov_obj_handler_rt_thread", "CANNOT create rt_thread again.");	
	mov_obj_handler_rt_thread =  rt_thread_create(rt_mov_obj_handler, NULL, 10000);
	if (mov_obj_handler_rt_thread ==0)
		return print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "create_mov_obj_handler_rt_thread", "Couldn' t create rt_thread.");	
	return TRUE;
}

bool kill_mov_obj_handler_rt_thread(void)
{
	return TRUE;
}

static void *rt_mov_obj_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	RTIME prev_time, curr_time, execution_end;
	RTIME expected;
	unsigned int i, timer_cpuid, task_cpuid;
	RTIME mov_obj_time_ns;
	MovObjHand2MovObjDurHandMsgAdditional mov_obj_hand_2_mov_obj_dur_hand_additional_data;

	unsigned int run_time_cntr = 0;

	SEM *exp_envi_rx_buff_sem = NULL;
	SEM *exp_envi_tx_buff_sem = NULL;
	ExpEnviRxShm *exp_envi_rx_buff_shm = NULL;
	ExpEnviTxShm *exp_envi_tx_buff_shm = NULL;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;
	task_cpuid = (MOV_OBJ_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+MOV_OBJ_HANDLER_CPU_THREAD_ID;

	if (! check_rt_task_specs_to_init(static_rt_tasks_data, MOV_OBJ_HANDLER_CPU_ID, MOV_OBJ_HANDLER_CPU_THREAD_ID, MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID, MOV_OBJ_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(MOV_OBJ_HANDLER_TASK_NAME, MOV_OBJ_HANDLER_TASK_PRIORITY, MOV_OBJ_HANDLER_STACK_SIZE, MOV_OBJ_HANDLER_MSG_SIZE,MOV_OBJ_HANDLER_POLICY, 1 << task_cpuid))) {
		print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(static_rt_tasks_data, MOV_OBJ_HANDLER_CPU_ID, MOV_OBJ_HANDLER_CPU_THREAD_ID, MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID, MOV_OBJ_HANDLER_PERIOD, MOV_OBJ_HANDLER_POSITIVE_JITTER_THRES, MOV_OBJ_HANDLER_NEGATIVE_JITTER_THRES, MOV_OBJ_HANDLER_RUN_TIME_THRES, "MovObjHandler", FALSE))  {
		print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	curr_time = rt_get_time_cpuid(task_cpuid);	
	mov_obj_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

        period = nano2count(MOV_OBJ_HANDLER_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;

	// Initialization of semaphores should be done after initializing the rt task !!!!
	if (! init_rs232_buffers(static_robot_arm, &exp_envi_rx_buff_sem, &exp_envi_tx_buff_sem, &exp_envi_rx_buff_shm, &exp_envi_tx_buff_shm,  mov_obj_time_ns))  {
		print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! init_rs232_buffers()."); exit(1); }	

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	msgs_trial_hand_2_mov_obj_hand->buff_read_idx = msgs_trial_hand_2_mov_obj_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_mov_obj_dur_hand_2_mov_obj_hand->buff_read_idx = msgs_mov_obj_dur_hand_2_mov_obj_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_mov_obj_hand_2_mov_obj_dur_hand->buff_read_idx = msgs_mov_obj_hand_2_mov_obj_dur_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	static_msgs_gui_2_mov_obj_hand->buff_read_idx = static_msgs_gui_2_mov_obj_hand->buff_write_idx;
	static_msgs_mov_obj_hand_2_gui->buff_read_idx = static_msgs_mov_obj_hand_2_gui->buff_write_idx;
	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		(msgs_neural_net_2_mov_obj_hand_multi_thread[i])->buff_read_idx = (msgs_neural_net_2_mov_obj_hand_multi_thread[i])->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	}

	mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.schedule = mov_obj_time_ns;  // send this command now
	mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.item_idx = MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION;
	if (! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand, mov_obj_time_ns,  MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE, mov_obj_hand_2_mov_obj_dur_hand_additional_data)) {
		print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer()."); exit(1); }	

        while (rt_mov_obj_handler_stay_alive) 
	{
        	rt_task_wait_period();

		curr_time = rt_get_time_cpuid(task_cpuid);	
		mov_obj_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

		expected += period;
		evaluate_and_save_jitter(static_rt_tasks_data, MOV_OBJ_HANDLER_CPU_ID, MOV_OBJ_HANDLER_CPU_THREAD_ID, MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID, curr_time, expected);
		prev_time = curr_time;
		// routines
		if (! handle_gui_to_mov_obj_handler_msg(static_robot_arm, &mov_obj_status, mov_obj_time_ns, static_msgs_gui_2_mov_obj_hand)) {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! handle_gui_to_mov_obj_handler_msg()."); break; }
		if (! handle_trial_handler_to_mov_obj_handler_msg(static_robot_arm, &mov_obj_status, mov_obj_time_ns, msgs_trial_hand_2_mov_obj_hand, msgs_mov_obj_hand_2_mov_obj_dur_hand, msgs_mov_obj_hand_2_trial_hand, static_mov_obj_paradigm, static_message_log, static_mov_obj_status_history, static_msgs_mov_obj_hand_2_gui, msgs_mov_obj_hand_2_neural_net_multi_thread))  {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! handle_trial_handler_to_mov_obj_handler_msg()."); break; }
		if (! handle_mov_obj_handler_to_mov_obj_dur_handler_msg(mov_obj_time_ns, msgs_mov_obj_hand_2_mov_obj_dur_hand)) {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjDurationHandlerRtTask", "rt_mov_obj_duration_handler", "! handle_mov_obj_handler_to_mov_obj_duration_handler_msg()."); break; }
		if (! handle_mov_obj_handler_duration(mov_obj_time_ns, msgs_mov_obj_dur_hand_2_mov_obj_hand))  {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjDurationHandlerRtTask", "rt_mov_obj_duration_handler", "! handle_mov_obj_handler_duration()."); break; }
		if (! handle_mov_obj_dur_handler_to_mov_obj_handler_msg(static_robot_arm, mov_obj_time_ns, &mov_obj_status, msgs_mov_obj_dur_hand_2_mov_obj_hand, msgs_mov_obj_hand_2_trial_hand, msgs_mov_obj_hand_2_mov_obj_dur_hand, msgs_mov_obj_hand_2_neural_net_multi_thread, scheduled_spike_data, static_message_log, static_mov_obj_paradigm, static_robot_pulse_history, static_robot_angle_history, static_mov_obj_status_history))  {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! handle_mov_obj_dur_handler_to_mov_obj_handler_msg()."); break; }
		// first handle duration status and robot position which determine mov_obj_status. Later on hanle spike outputs of neural net.  it changes the target pulse width value to reset the position to target led during reseting status. 
		if (! handle_neural_net_to_mov_obj_handler_msg(static_robot_arm, mov_obj_time_ns, msgs_neural_net_2_mov_obj_hand_multi_thread, scheduled_spike_data))  {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! handle_neural_net_to_mov_obj_handler_msg()."); break; }
		if (! handle_spike_data_buff(mov_obj_status, mov_obj_time_ns, scheduled_spike_data, static_robot_arm, static_mov_obj_paradigm))  {
			print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "! handle_spike_data_buff()."); break; }
		// routines	
		execution_end = rt_get_time_cpuid(timer_cpuid);
		evaluate_and_save_period_run_time(static_rt_tasks_data, MOV_OBJ_HANDLER_CPU_ID, MOV_OBJ_HANDLER_CPU_THREAD_ID, MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID, curr_time, execution_end);		
		run_time_cntr++;
		if (run_time_cntr == NUM_OF_TASK_EXECUTIONS_4_PERFOMANCE_EVAL)
		{
			run_time_cntr = 0;
			write_run_time_to_averaging_buffer(static_rt_tasks_data, MOV_OBJ_HANDLER_CPU_ID, MOV_OBJ_HANDLER_CPU_THREAD_ID, MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID, curr_time, execution_end);	
		}	
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "rt_task_delete().");	

        return 0; 
}

static bool connect_to_trial_hand(void )
{
	TrialHand2MovObjHandMsgItem msg_item;
	char str_trial_hand_2_mov_obj_hand_msg[TRIAL_HAND_2_MOV_OBJ_HAND_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_mov_obj_hand_msg_buffer_item(msgs_trial_hand_2_mov_obj_hand, &msg_item))
		{
			get_trial_hand_2_mov_obj_hand_msg_type_string(msg_item.msg_type, str_trial_hand_2_mov_obj_hand_msg);
			print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", str_trial_hand_2_mov_obj_hand_msg);	
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_MOV_OBJ_HAND_MSG_ARE_YOU_ALIVE:
					msgs_mov_obj_hand_2_trial_hand = allocate_shm_client_mov_obj_hand_2_trial_hand_msg_buffer(msgs_mov_obj_hand_2_trial_hand);
					sleep(1);
					if (msgs_mov_obj_hand_2_trial_hand == NULL)
						return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", "msgs_mov_obj_hand_2_trial_hand == NULL.");	
					if (!write_to_mov_obj_hand_2_trial_hand_msg_buffer(msgs_mov_obj_hand_2_trial_hand, static_rt_tasks_data->current_system_time, MOV_OBJ_HAND_2_TRIAL_HAND_MSG_I_AM_ALIVE, 0))
						return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", "write_to_mov_obj_hand_2_trial_hand_msg_buffer().");	
					print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", "Connection to TRIAL_HANDLER is successful!!!");	
					return TRUE;		
				default:
					return print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", str_trial_hand_2_mov_obj_hand_msg);	
			}
		}
		print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", "Waiting for TRIAL_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_trial_hand", "Wrong hit in the code.");
}

static bool connect_to_neural_net(void)
{
	NeuralNet2MovObjHandMsgItem msg_item;
	char str_neural_net_2_mov_obj_hand_msg[NEURAL_NET_2_MOV_OBJ_HAND_MSG_STRING_LENGTH];
	unsigned int i, num_of_alive_responses = 0;
	MovObjHand2NeuralNetMsgAdditional	mov_obj_hand_2_neural_net_msg_add;
	for (i = 0; i <SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		msgs_mov_obj_hand_2_neural_net_multi_thread[i] = allocate_shm_client_mov_obj_hand_2_neural_net_multi_thread_msg_buffer_item(msgs_mov_obj_hand_2_neural_net_multi_thread, i, MIN_MOV_OBJ_HAND_2_NEURAL_NET_EVENT_SCHEDULING_DELAY, MIN_MOV_OBJ_HAND_2_NEURAL_NET_EVENT_SCHEDULING_DELAY);
		if (msgs_mov_obj_hand_2_neural_net_multi_thread[i] == NULL)
			return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "msgs_mov_obj_hand_2_neural_net == NULL.");
		mov_obj_hand_2_neural_net_msg_add.dummy = 0;
		if (!write_to_mov_obj_hand_2_neural_net_msg_buffer(msgs_mov_obj_hand_2_neural_net_multi_thread[i], static_rt_tasks_data->current_system_time, MOV_OBJ_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE, mov_obj_hand_2_neural_net_msg_add))
			return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "write_to_mov_obj_hand_2_neural_net_msg_buffer().");
	}
	while (1) 
	{
		for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
		{ 
			while (get_next_neural_net_2_mov_obj_hand_msg_buffer_item(msgs_neural_net_2_mov_obj_hand_multi_thread[i], &msg_item))
			{
				get_neural_net_2_mov_obj_hand_msg_type_string(msg_item.msg_type, str_neural_net_2_mov_obj_hand_msg);
				print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", str_neural_net_2_mov_obj_hand_msg);	
				switch (msg_item.msg_type)
				{
					case NEURAL_NET_2_MOV_OBJ_HAND_MSG_I_AM_ALIVE:
						print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "Connection to NEURAL_NET is successful!!!");	
						static_mov_obj_paradigm->num_of_output_neurons = msg_item.neuron_num;
						num_of_alive_responses++;
						if (num_of_alive_responses == SNN_SIM_NUM_OF_DEDICATED_CPUS)
							goto SEND_JOINT_MIN_MAX;
						break;			
					default:
						return print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", str_neural_net_2_mov_obj_hand_msg);	
				}
			}
		}
		sleep(1); 
	}
	SEND_JOINT_MIN_MAX: 
/*	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_min_joint_angles[BASE_SERVO] = static_mov_obj_paradigm->polar_space_limits[BASE_SERVO].min;
	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_min_joint_angles[SHOULDER_SERVO] = static_mov_obj_paradigm->polar_space_limits[SHOULDER_SERVO].min;
	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_min_joint_angles[ELBOW_SERVO] = static_mov_obj_paradigm->polar_space_limits[ELBOW_SERVO].min;
	if (!write_to_mov_obj_hand_2_neural_net_msg_buffer((*msgs_mov_obj_hand_2_neural_net_multi_thread)[0], *sys_time_ptr, MOV_OBJ_HAND_2_NEURAL_NET_MSG_JOINT_ANGLE_MIN, mov_obj_hand_2_neural_net_msg_add))	
		return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "write_to_mov_obj_hand_2_neural_net_msg_buffer().");

	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_max_joint_angles[BASE_SERVO] = static_mov_obj_paradigm->polar_space_limits[BASE_SERVO].max;
	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_max_joint_angles[SHOULDER_SERVO] = static_mov_obj_paradigm->polar_space_limits[SHOULDER_SERVO].max;
	mov_obj_hand_2_neural_net_msg_add.three_dof_robot_max_joint_angles[ELBOW_SERVO] = static_mov_obj_paradigm->polar_space_limits[ELBOW_SERVO].max;
	if (!write_to_mov_obj_hand_2_neural_net_msg_buffer((*msgs_mov_obj_hand_2_neural_net_multi_thread)[0], *sys_time_ptr, MOV_OBJ_HAND_2_NEURAL_NET_MSG_JOINT_ANGLE_MAX, mov_obj_hand_2_neural_net_msg_add))	
		return print_message(ERROR_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "write_to_mov_obj_hand_2_neural_net_msg_buffer().");
*/

	return print_message(INFO_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "Successfull !!!.");
}
