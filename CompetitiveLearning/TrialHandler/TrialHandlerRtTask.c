#include "TrialHandlerRtTask.h"

static RtTasksData *static_rt_tasks_data = NULL;

static TrialStatus trial_status = TRIAL_STATUS_NULL;   // Only trial handler can change trial status. 

static TrialDurationStatus trial_duration_status = TRIAL_DUR_STATUS_NULL;   // Only trial duration handler can change trial status. 
static TimeStamp handling_end_time = 0;

static TrialHandParadigmRobotReach *static_paradigm = NULL;

static ClassifiedTrialHistory *classified_history = NULL;

static TrialStatusHistory *static_trial_status_history = NULL;

static Gui2TrialHandMsg *static_msgs_gui_2_trial_hand = NULL;  
static TrialHand2GuiMsg *static_msgs_trial_hand_2_gui = NULL;  

static ExpEnviHand2TrialHandMsg *msgs_exp_envi_hand_2_trial_hand = NULL;
static MovObjHand2TrialHandMsg *msgs_mov_obj_hand_2_trial_hand = NULL;

static TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand = NULL;
static TrialHand2MovObjHandMsg *msgs_trial_hand_2_mov_obj_hand = NULL;

static TrialDurHand2TrialHandMsg *msgs_trial_dur_hand_2_trial_hand = NULL;    // Trial Handler only read it, cannot write
static TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand = NULL;     // Trial Handler can write here, cannot read it, trial duration handler will read it

static TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;
static NeuralNet2TrialHandMsg *msgs_neural_net_2_trial_hand = NULL;


static int trial_handler_rt_thread = 0;
static bool rt_trial_handler_stay_alive = 1;

static void *rt_trial_handler(void *args);

static bool connect_to_exp_envi_hand(void);
static bool connect_to_mov_obj_hand(void);
static bool connect_to_neural_net(void);



bool create_trial_handler_rt_thread(RtTasksData *rt_tasks_data, Gui2TrialHandMsg *msgs_gui_2_trial_hand, TrialHandParadigmRobotReach *paradigm, ClassifiedTrialHistory* classified_trial_history, TrialHand2GuiMsg *msgs_trial_hand_2_gui, TrialStatusHistory *trial_status_history)
{
	static_rt_tasks_data = rt_tasks_data;

	trial_status = TRIAL_STATUS_TRIALS_DISABLED;

	trial_duration_status = TRIAL_DUR_STATUS_HANDLING_DISABLED;

	static_paradigm = paradigm;

	classified_history = classified_trial_history;

	static_trial_status_history = trial_status_history;

	static_msgs_gui_2_trial_hand = msgs_gui_2_trial_hand;	
	static_msgs_trial_hand_2_gui = msgs_trial_hand_2_gui;	

	msgs_exp_envi_hand_2_trial_hand = allocate_shm_server_exp_envi_hand_2_trial_hand_msg_buffer(msgs_exp_envi_hand_2_trial_hand);
	msgs_mov_obj_hand_2_trial_hand = allocate_shm_server_mov_obj_hand_2_trial_hand_msg_buffer(msgs_mov_obj_hand_2_trial_hand);
	msgs_neural_net_2_trial_hand = allocate_shm_server_neural_net_2_trial_hand_msg_buffer(msgs_neural_net_2_trial_hand);

	if (!connect_to_neural_net())
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "connect_to_neural_net().");	


	if (! connect_to_mov_obj_hand())		// connect to mov obj hand before exp envi so that mov obj hand initializes semaphore and shm shared with exp envi hand.
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "connect_to_exp_envi_hand().");	
	sleep(1);
	if (! connect_to_exp_envi_hand())
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "connect_to_exp_envi_hand().");	


	msgs_trial_dur_hand_2_trial_hand = allocate_trial_dur_hand_2_trial_hand_msg_buffer(msgs_trial_dur_hand_2_trial_hand);
	msgs_trial_hand_2_trial_dur_hand = allocate_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand);

/*	if(! create_trial_duration_handler_rt_thread(rt_tasks_data, msgs_trial_dur_hand_2_trial_hand, msgs_trial_hand_2_trial_dur_hand))
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "create_trial_handler_rt_thread().");	
*/
	if (trial_handler_rt_thread !=0)
		return print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "CANNOT create rt_thread again.");	
	trial_handler_rt_thread =  rt_thread_create(rt_trial_handler, NULL, 10000);
	if (trial_handler_rt_thread ==0)
		return print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "create_trial_handler_rt_thread", "Couldn' t create rt_thread.");	
	return TRUE;
}

bool kill_trial_handler_rt_thread(void)
{
	return TRUE;
}

static void *rt_trial_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	RTIME prev_time, curr_time;
	RTIME expected;
	RTIME trial_hand_time_ns;
	unsigned int timer_cpuid, task_cpuid;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;
	task_cpuid = (TRIAL_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_HANDLER_CPU_THREAD_ID;

	if (! check_rt_task_specs_to_init(static_rt_tasks_data, TRIAL_HANDLER_CPU_ID, TRIAL_HANDLER_CPU_THREAD_ID, TRIAL_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_HANDLER_TASK_NAME, TRIAL_HANDLER_TASK_PRIORITY, TRIAL_HANDLER_STACK_SIZE, TRIAL_HANDLER_MSG_SIZE,TRIAL_HANDLER_POLICY, 1 << task_cpuid))) {
		print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(static_rt_tasks_data, TRIAL_HANDLER_CPU_ID, TRIAL_HANDLER_CPU_THREAD_ID, TRIAL_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HANDLER_PERIOD, TRIAL_HANDLER_POSITIVE_JITTER_THRES, TRIAL_HANDLER_NEGATIVE_JITTER_THRES, "TrialHandler", FALSE))  {
		print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	curr_time = rt_get_time_cpuid(task_cpuid);	
	trial_hand_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

        period = nano2count(TRIAL_HANDLER_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;


        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	initialize_gui_to_trial_handler_msg_params(&trial_status, static_msgs_gui_2_trial_hand, msgs_trial_hand_2_trial_dur_hand, msgs_trial_hand_2_exp_envi_hand, msgs_trial_hand_2_mov_obj_hand, msgs_trial_hand_2_neural_net, static_paradigm, classified_history, static_msgs_trial_hand_2_gui, static_trial_status_history);
	initialize_trial_dur_handler_to_trial_handler_msg_params(&trial_status, msgs_trial_dur_hand_2_trial_hand, msgs_trial_hand_2_trial_dur_hand, msgs_trial_hand_2_exp_envi_hand, msgs_trial_hand_2_mov_obj_hand, msgs_trial_hand_2_neural_net, classified_history, static_paradigm, static_msgs_trial_hand_2_gui, static_trial_status_history);
	initialize_exp_envi_handler_to_trial_handler_msg_params(&trial_status, msgs_exp_envi_hand_2_trial_hand, msgs_trial_hand_2_trial_dur_hand, msgs_trial_hand_2_exp_envi_hand, msgs_trial_hand_2_mov_obj_hand, msgs_trial_hand_2_neural_net, static_paradigm, classified_history, static_msgs_trial_hand_2_gui, static_trial_status_history);
	initialize_mov_obj_handler_to_trial_handler_msg_params(&trial_status, msgs_mov_obj_hand_2_trial_hand, msgs_trial_hand_2_trial_dur_hand, msgs_trial_hand_2_exp_envi_hand, msgs_trial_hand_2_mov_obj_hand, msgs_trial_hand_2_neural_net, static_paradigm, classified_history, static_msgs_trial_hand_2_gui, static_trial_status_history);


	msgs_trial_hand_2_trial_dur_hand->buff_read_idx = msgs_trial_hand_2_trial_dur_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_trial_dur_hand_2_trial_hand->buff_read_idx = msgs_trial_dur_hand_2_trial_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_exp_envi_hand_2_trial_hand->buff_read_idx = msgs_exp_envi_hand_2_trial_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_mov_obj_hand_2_trial_hand->buff_read_idx = msgs_mov_obj_hand_2_trial_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,
	msgs_neural_net_2_trial_hand->buff_read_idx = msgs_neural_net_2_trial_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

	static_msgs_gui_2_trial_hand->buff_read_idx = static_msgs_gui_2_trial_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,


        while (rt_trial_handler_stay_alive) 
	{
        	rt_task_wait_period();

		curr_time = rt_get_time_cpuid(task_cpuid);	
		trial_hand_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

		expected += period;
		evaluate_and_save_jitter(static_rt_tasks_data, TRIAL_HANDLER_CPU_ID, TRIAL_HANDLER_CPU_THREAD_ID, TRIAL_HANDLER_CPU_THREAD_TASK_ID, curr_time, expected);
		prev_time = curr_time;

		// routines
		if (!handle_gui_to_trial_handler_msg(trial_hand_time_ns)) {
			print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! handle_gui_to_trial_handler_msg()."); break; }
		if (!handle_exp_envi_handler_to_trial_handler_msg(trial_hand_time_ns))  {
			print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! handle_exp_envi_handler_to_trial_handler_msg()."); break; }
		if (!handle_mov_obj_handler_to_trial_handler_msg(trial_hand_time_ns))  {
			print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! handle_mov_obj_handler_to_trial_handler_msg()."); break; }
		// handle_trial_handler_to_trial_dur_handler  --> handle_trial_handler_duration --> handle_trial_dur_handler_to_trial_handler_msg is important  === so that messages from exp envi and mov obj handler have priority over trial  duratiion handler.  e.g. nose poke event from exp envi handler extends refractory period (BEFORE trial_dur_handler_to_trial_handler makes trials available due to refractory timeout). 
		if (! handle_trial_handler_to_trial_dur_handler_msg(&trial_duration_status, trial_hand_time_ns, msgs_trial_hand_2_trial_dur_hand, &handling_end_time)) {
			print_message(ERROR_MSG ,"BMIExpController", "TrialHandlerRtTask", "rt_trial_duration_handler", "! handle_trial_handler_to_trial_duration_handler_msg()."); break; }
		if (! handle_trial_handler_duration(&trial_duration_status, trial_hand_time_ns, handling_end_time, msgs_trial_dur_hand_2_trial_hand)) {
			print_message(ERROR_MSG ,"BMIExpController", "TrialHandlerRtTask", "rt_trial_duration_handler", "! handle_trial_handler_to_trial_duration_handler_msg()."); break; }
		if (!handle_trial_dur_handler_to_trial_handler_msg(trial_hand_time_ns))  {
			print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "! handle_trial_dur_handler_to_trial_handler_msg()."); break; }
		// routines	
		evaluate_and_save_period_run_time(static_rt_tasks_data, TRIAL_HANDLER_CPU_ID, TRIAL_HANDLER_CPU_THREAD_ID, TRIAL_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_time_cpuid(timer_cpuid));		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "rt_trial_handler", "rt_task_delete().");	

        return 0; 
}


static bool connect_to_exp_envi_hand(void)
{
	ExpEnviHand2TrialHandMsgItem msg_item;
	char str_exp_envi_hand_2_trial_hand_msg[EXP_ENVI_HAND_2_TRIAL_HAND_MSG_STRING_LENGTH];

	msgs_trial_hand_2_exp_envi_hand = allocate_shm_client_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand);	
	if (msgs_trial_hand_2_exp_envi_hand == NULL)
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", "msgs_trial_hand_2_exp_envi_hand == NULL.");
	sleep(1);	
	if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, static_rt_tasks_data->current_system_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_ARE_YOU_ALIVE, 0))
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", "write_to_trial_hand_2_exp_envi_hand_msg_bufferr().");

	while(1)
	{
		while (get_next_exp_envi_hand_2_trial_hand_msg_buffer_item(msgs_exp_envi_hand_2_trial_hand, &msg_item))
		{
			get_exp_envi_hand_2_trial_hand_msg_type_string(msg_item.msg_type, str_exp_envi_hand_2_trial_hand_msg);
			print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", str_exp_envi_hand_2_trial_hand_msg);	
			switch (msg_item.msg_type)
			{
				case EXP_ENVI_HAND_2_TRIAL_HAND_MSG_I_AM_ALIVE:
					print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", "Connection to EXP_ENVI_HANDLER is successful!!!");	
					sleep(1);	
					return TRUE;			
				default:
					print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", str_exp_envi_hand_2_trial_hand_msg);	
					return (void*)print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", "default - switch.");
			}
		}
		sleep(1);
	}
	return print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_exp_envi_hand", "Wrong hit in the code.");
}

static bool connect_to_mov_obj_hand(void)
{
	MovObjHand2TrialHandMsgItem msg_item;
	char str_mov_obj_hand_2_trial_hand_msg[MOV_OBJ_HAND_2_TRIAL_HAND_MSG_STRING_LENGTH];
	TrialHand2MovObjHandMsgAdditional trial_hand_2_mov_obj_hand_add;

	msgs_trial_hand_2_mov_obj_hand = allocate_shm_client_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand);	
	if (msgs_trial_hand_2_mov_obj_hand == NULL)
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", "msgs_trial_hand_2_mov_obj_hand == NULL.");
	sleep(1);	

	trial_hand_2_mov_obj_hand_add.dummy = 0;
	if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, static_rt_tasks_data->current_system_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_ARE_YOU_ALIVE, trial_hand_2_mov_obj_hand_add))
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", "write_to_trial_hand_2_mov_obj_hand_msg_bufferr().");

	while(1)
	{
		while (get_next_mov_obj_hand_2_trial_hand_msg_buffer_item(msgs_mov_obj_hand_2_trial_hand, &msg_item))
		{
			get_mov_obj_hand_2_trial_hand_msg_type_string(msg_item.msg_type, str_mov_obj_hand_2_trial_hand_msg);
			print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", str_mov_obj_hand_2_trial_hand_msg);	
			switch (msg_item.msg_type)
			{
				case MOV_OBJ_HAND_2_TRIAL_HAND_MSG_I_AM_ALIVE:
					print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", "Connection to MOV_OBJ_HANDLER is successful!!!");
					sleep(1);		
					return TRUE;			
				default:
					print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", str_mov_obj_hand_2_trial_hand_msg);	
					return (void*)print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", "default - switch.");
			}
		}
		sleep(1);
	}
	return print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_mov_obj_hand", "Wrong hit in the code.");
}

static bool connect_to_neural_net(void)
{
	NeuralNet2TrialHandMsgItem msg_item;
	char str_neural_net_2_trial_hand_msg[NEURAL_NET_2_TRIAL_HAND_MSG_STRING_LENGTH];
	TrialHand2NeuralNetMsgAdditional trial_hand_to_neural_net_msg_add;
	msgs_trial_hand_2_neural_net = allocate_shm_client_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net);
	if (msgs_trial_hand_2_neural_net == NULL)
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", "msgs_trial_hand_2_neural_net == NULL.");
	trial_hand_to_neural_net_msg_add.dummy = 0;
	if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, static_rt_tasks_data->current_system_time, TRIAL_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE, trial_hand_to_neural_net_msg_add))
		return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", "write_to_trial_hand_2_neural_net_msg_buffer().");

	while (1) 
	{ 
		while (get_next_neural_net_2_trial_hand_msg_buffer_item(msgs_neural_net_2_trial_hand, &msg_item))
		{
			get_neural_net_2_trial_hand_msg_type_string(msg_item.msg_type, str_neural_net_2_trial_hand_msg);
			print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", str_neural_net_2_trial_hand_msg);	
			switch (msg_item.msg_type)
			{
				case NEURAL_NET_2_TRIAL_HAND_MSG_I_AM_ALIVE:
					print_message(INFO_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", "Connection to NEURAL_NET is successful!!!");
					trial_hand_to_neural_net_msg_add.dummy = 0;
					if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, static_rt_tasks_data->current_system_time, TRIAL_HAND_2_SPIKE_GEN_MSG_TRIAL_STATUS_CHANGED, trial_hand_to_neural_net_msg_add))
						return print_message(ERROR_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", "write_to_trial_hand_2_neural_net_msg_buffer().");	
					return TRUE;			
				default:
					return print_message(BUG_MSG ,"TrialHandler", "TrialHandlerRtTask", "connect_to_neural_net", str_neural_net_2_trial_hand_msg);	
			}
		}
		sleep(1); 
	}
	return print_message(BUG_MSG ,"TrialHandler", "MovObjHandlerRtTask", "connect_to_neural_net", "Wrong hit in the code.");
}


