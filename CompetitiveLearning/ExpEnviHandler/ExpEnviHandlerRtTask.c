#include "ExpEnviHandlerRtTask.h"

static RtTasksData *static_rt_tasks_data = NULL;

static int exp_envi_handler_rt_thread = 0;
static bool rt_exp_envi_handler_stay_alive = 1;

static void *rt_exp_envi_handler(void *args);

static ExpEnviData *static_exp_envi_data = NULL;

static Gui2ExpEnviHandMsg *static_msgs_gui_2_exp_envi_hand = NULL;
static ExpEnviHand2GuiMsg *static_msgs_exp_envi_hand_2_gui = NULL;  
/*
static ExpEnviInterf2ExpEnviHandMsg *msgs_exp_envi_interf_2_exp_envi_hand = NULL;
static ExpEnviHand2ExpEnviInterfMsg *msgs_exp_envi_hand_2_exp_envi_interf = NULL;
*/

static TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand = NULL;
static ExpEnviHand2TrialHandMsg *msgs_exp_envi_hand_2_trial_hand = NULL;

static ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand = NULL;
static ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand = NULL;   

static ExpEnviInputStatusHistory *static_exp_envi_input_status_history = NULL;
static ExpEnviOutputStatusHistory *static_exp_envi_output_status_history = NULL;

static ExpEnviHandParadigmRobotReach *static_exp_envi_paradigm = NULL;

static bool connect_to_trial_hand(void);

bool create_exp_envi_handler_rt_thread(RtTasksData *rt_tasks_data, ExpEnviData *exp_envi_data, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui, ExpEnviInputStatusHistory *exp_envi_input_status_history, ExpEnviOutputStatusHistory *exp_envi_output_status_history, ExpEnviHandParadigmRobotReach *exp_envi_paradigm)
{
	static_rt_tasks_data = rt_tasks_data;

	static_exp_envi_data = exp_envi_data;

	static_msgs_gui_2_exp_envi_hand = msgs_gui_2_exp_envi_hand;
	static_msgs_exp_envi_hand_2_gui = msgs_exp_envi_hand_2_gui;

	static_exp_envi_input_status_history = exp_envi_input_status_history;
	static_exp_envi_output_status_history = exp_envi_output_status_history;

	static_exp_envi_paradigm = exp_envi_paradigm;

	msgs_trial_hand_2_exp_envi_hand = allocate_shm_server_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand);

	if (! connect_to_trial_hand())
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "create_exp_envi_handler_rt_thread", "connect_to_trial_hand().");	


	msgs_exp_envi_dur_hand_2_exp_envi_hand = allocate_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer(msgs_exp_envi_dur_hand_2_exp_envi_hand);
	msgs_exp_envi_hand_2_exp_envi_dur_hand = allocate_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand);

	if(! create_exp_envi_duration_handler_rt_thread(rt_tasks_data, msgs_exp_envi_dur_hand_2_exp_envi_hand, msgs_exp_envi_hand_2_exp_envi_dur_hand, exp_envi_data->num_of_inp_comps, exp_envi_data->num_of_outp_comps))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "create_exp_envi_handler_rt_thread", "create_exp_envi_duration_handler_rt_thread().");	

	if (exp_envi_handler_rt_thread !=0)
		return print_message(BUG_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "create_exp_envi_handler_rt_thread", "CANNOT create rt_thread again.");	
	exp_envi_handler_rt_thread =  rt_thread_create(rt_exp_envi_handler, NULL, 10000);
	if (exp_envi_handler_rt_thread ==0)
		return print_message(BUG_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "create_exp_envi_handler_rt_thread", "Couldn' t create rt_thread.");	
	return TRUE;
}

bool kill_exp_envi_handler_rt_thread(void)
{
	return TRUE;
}

static void *rt_exp_envi_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	TimeStamp curr_system_time;

	SEM *exp_envi_rx_buff_sem = NULL;
	SEM *exp_envi_tx_buff_sem = NULL;
	ExpEnviRxShm *exp_envi_rx_buff_shm = NULL;
	ExpEnviTxShm *exp_envi_tx_buff_shm = NULL;

	if (! check_rt_task_specs_to_init(static_rt_tasks_data, EXP_ENVI_HANDLER_CPU_ID, EXP_ENVI_HANDLER_CPU_THREAD_ID, EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID, EXP_ENVI_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(EXP_ENVI_HANDLER_TASK_NAME, EXP_ENVI_HANDLER_TASK_PRIORITY, EXP_ENVI_HANDLER_STACK_SIZE, EXP_ENVI_HANDLER_MSG_SIZE,EXP_ENVI_HANDLER_POLICY, 1 << ((EXP_ENVI_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+EXP_ENVI_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(static_rt_tasks_data, EXP_ENVI_HANDLER_CPU_ID, EXP_ENVI_HANDLER_CPU_THREAD_ID, EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID, EXP_ENVI_HANDLER_PERIOD, EXP_ENVI_HANDLER_POSITIVE_JITTER_THRES, EXP_ENVI_HANDLER_NEGATIVE_JITTER_THRES, "ExpEnviHandler", FALSE))  {
		print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	// Initialization of semaphores should be done after initializing the rt task !!!!
	if (! init_rs232_buffers(&exp_envi_rx_buff_sem, &exp_envi_tx_buff_sem, &exp_envi_rx_buff_shm, &exp_envi_tx_buff_shm ))  {
		print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! init_rs232_buffers()."); exit(1); }	

        period = nano2count(EXP_ENVI_HANDLER_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	msgs_trial_hand_2_exp_envi_hand->buff_read_idx = msgs_trial_hand_2_exp_envi_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

	msgs_exp_envi_dur_hand_2_exp_envi_hand->buff_read_idx = msgs_exp_envi_dur_hand_2_exp_envi_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	static_msgs_gui_2_exp_envi_hand->buff_read_idx = static_msgs_gui_2_exp_envi_hand->buff_write_idx;

        while (rt_exp_envi_handler_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(static_rt_tasks_data, EXP_ENVI_HANDLER_CPU_ID, EXP_ENVI_HANDLER_CPU_THREAD_ID, EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		curr_system_time = *sys_time_ptr;
		// routines
		if (! handle_gui_to_exp_envi_handler_msg(static_exp_envi_data, curr_system_time, static_msgs_gui_2_exp_envi_hand)) {
			print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! handle_gui_to_exp_envi_handler_msg()."); break; }
		if (! handle_trial_handler_to_exp_envi_handler_msg(static_exp_envi_data, curr_system_time, msgs_trial_hand_2_exp_envi_hand,  msgs_exp_envi_hand_2_exp_envi_dur_hand, static_exp_envi_paradigm, static_msgs_exp_envi_hand_2_gui))  {
			print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! handle_trial_handler_to_exp_envi_handler_msg()."); break; }
		if (! handle_exp_envi_rx_shm(static_exp_envi_data, curr_system_time, msgs_exp_envi_hand_2_exp_envi_dur_hand, msgs_exp_envi_hand_2_trial_hand, static_exp_envi_input_status_history)) {
			print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! handle_exp_envi_rx_shm()."); break; }
		if (! handle_exp_envi_dur_handler_to_exp_envi_handler_msg(static_exp_envi_data, curr_system_time, msgs_exp_envi_dur_hand_2_exp_envi_hand, msgs_exp_envi_hand_2_trial_hand, static_exp_envi_paradigm))  {
			print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! handle_exp_envi_dur_handler_to_exp_envi_handler_msg()."); break; }
		if (! handle_exp_envi_tx_shm(static_exp_envi_data, curr_system_time, static_exp_envi_output_status_history)) {
			print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "! handle_exp_envi_tx_shm()."); break; }
		// routines	
		evaluate_and_save_period_run_time(static_rt_tasks_data, EXP_ENVI_HANDLER_CPU_ID, EXP_ENVI_HANDLER_CPU_THREAD_ID, EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "rt_exp_envi_handler", "rt_task_delete().");	

        return 0; 
}

static bool connect_to_trial_hand(void )
{
	TrialHand2ExpEnviHandMsgItem msg_item;
	char str_trial_hand_2_exp_envi_hand_msg[TRIAL_HAND_2_EXP_ENVI_HAND_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_exp_envi_hand_msg_buffer_item(msgs_trial_hand_2_exp_envi_hand, &msg_item))
		{
			get_trial_hand_2_exp_envi_hand_msg_type_string(msg_item.msg_type, str_trial_hand_2_exp_envi_hand_msg);
			print_message(INFO_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", str_trial_hand_2_exp_envi_hand_msg);	
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_ARE_YOU_ALIVE:
					msgs_exp_envi_hand_2_trial_hand = allocate_shm_client_exp_envi_hand_2_trial_hand_msg_buffer(msgs_exp_envi_hand_2_trial_hand);
					usleep(1000);
					if (msgs_exp_envi_hand_2_trial_hand == NULL)
						return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", "msgs_exp_envi_hand_2_trial_hand == NULL.");	
					if (!write_to_exp_envi_hand_2_trial_hand_msg_buffer(msgs_exp_envi_hand_2_trial_hand, *sys_time_ptr, EXP_ENVI_HAND_2_TRIAL_HAND_MSG_I_AM_ALIVE, 0))
						return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", "write_to_exp_envi_hand_2_trial_hand_msg_buffer().");	
					print_message(INFO_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", "Connection to TRIAL_HANDLER is successful!!!");	
					return TRUE;		
				default:
					print_message(BUG_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", str_trial_hand_2_exp_envi_hand_msg);	
					return print_message(BUG_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", "default - switch.");
			}
		}
		print_message(INFO_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_trial_hand", "Waiting for TRIAL_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"ExpEnviHandler", "ExpEnviHandlerRtTask", "connect_to_exp_envi_interf", "Wrong hit in the code.");
}
