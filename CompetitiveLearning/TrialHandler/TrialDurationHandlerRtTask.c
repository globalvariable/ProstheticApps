#include "TrialDurationHandlerRtTask.h"

static RtTasksData *static_rt_tasks_data = NULL;

static TrialDurationStatus trial_duration_status = TRIAL_DUR_STATUS_NULL;   // Only trial duration handler can change trial status. 
static TimeStamp handling_end_time = 0;

static TrialHand2TrialDurHandMsg *static_msgs_trial_hand_2_trial_dur_hand;    
static TrialDurHand2TrialHandMsg *static_msgs_trial_dur_hand_2_trial_hand;    

static int trial_duration_handler_rt_thread = 0;
static bool rt_trial_duration_handler_stay_alive = 1;


static void *rt_trial_duration_handler(void *args);

bool create_trial_duration_handler_rt_thread(RtTasksData *rt_tasks_data, TrialDurHand2TrialHandMsg *msgs_trial_dur_hand_2_trial_hand, TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand)
{
	static_rt_tasks_data = rt_tasks_data;

	static_msgs_trial_dur_hand_2_trial_hand = msgs_trial_dur_hand_2_trial_hand;
	static_msgs_trial_hand_2_trial_dur_hand = msgs_trial_hand_2_trial_dur_hand;
	trial_duration_status = TRIAL_DUR_STATUS_HANDLING_DISABLED;
	if (trial_duration_handler_rt_thread !=0)
		return print_message(BUG_MSG ,"BMIExpController", "TrialHandlerRtTask", "create_trial_duration_handler_rt_thread", "CANNOT create rt_thread again.");	
	trial_duration_handler_rt_thread =  rt_thread_create(rt_trial_duration_handler, NULL, 10000);
	if (trial_duration_handler_rt_thread ==0)
		return print_message(BUG_MSG ,"BMIExpController", "TrialHandlerRtTask", "create_trial_duration_handler_rt_thread", "Couldn' t create rt_thread.");	
	return TRUE;
}

bool kill_trial_duration_handler_rt_thread(void)
{
	return TRUE;
}

static void *rt_trial_duration_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	TimeStamp curr_system_time;

	if (! check_rt_task_specs_to_init(static_rt_tasks_data, TRIAL_DURATION_HANDLER_CPU_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID, TRIAL_DURATION_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_DURATION_HANDLER_TASK_NAME, TRIAL_DURATION_HANDLER_TASK_PRIORITY, TRIAL_DURATION_HANDLER_STACK_SIZE, TRIAL_DURATION_HANDLER_MSG_SIZE,TRIAL_DURATION_HANDLER_POLICY, 1 << ((TRIAL_DURATION_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_DURATION_HANDLER_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(static_rt_tasks_data, TRIAL_DURATION_HANDLER_CPU_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID, TRIAL_DURATION_HANDLER_PERIOD, TRIAL_DURATION_HANDLER_POSITIVE_JITTER_THRES, TRIAL_DURATION_HANDLER_NEGATIVE_JITTER_THRES, "TrialDurationHandler", FALSE))  {
		print_message(ERROR_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	
        period = nano2count(TRIAL_HANDLER_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);
	prev_time = rt_get_cpu_time_ns();	


        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	static_msgs_trial_hand_2_trial_dur_hand->buff_read_idx = static_msgs_trial_hand_2_trial_dur_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        while (rt_trial_duration_handler_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(static_rt_tasks_data, TRIAL_DURATION_HANDLER_CPU_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		curr_system_time = *sys_time_ptr;
		// routines
		if (! handle_trial_handler_to_trial_dur_handler_msg(&trial_duration_status, curr_system_time, static_msgs_trial_hand_2_trial_dur_hand, &handling_end_time)) {
			print_message(ERROR_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "! handle_trial_handler_to_trial_duration_handler_msg()."); break; }
		if (! handle_trial_handler_duration(&trial_duration_status, curr_system_time, handling_end_time, static_msgs_trial_dur_hand_2_trial_hand)) {
			print_message(ERROR_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "! handle_trial_handler_to_trial_duration_handler_msg()."); break; }
		// routines	
		evaluate_and_save_period_run_time(static_rt_tasks_data, TRIAL_DURATION_HANDLER_CPU_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_ID, TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"BMIExpController", "TrialDurationHandlerRtTask", "rt_trial_duration_handler", "rt_task_delete().");	

        return 0; 
}
