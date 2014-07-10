#include "ExpEnviDurationHandlerRtTask.h"

static RtTasksData *static_rt_tasks_data = NULL;

static unsigned int num_of_input_components = 0;
static ExpEnviDurationStatus *exp_envi_inputs_min_duration_status = NULL;   // Only exp envi duration handler can change exp envi duration status. 
static ExpEnviDurationStatus *exp_envi_inputs_max_duration_status = NULL;   // Only exp envi duration handler can change exp envi duration status. 
static TimeStamp *inputs_handling_end_time_min = NULL;
static TimeStamp *inputs_handling_end_time_max = NULL;

static unsigned int num_of_output_components = 0;
static ExpEnviDurationStatus *exp_envi_outputs_duration_status = NULL;   // Only exp envi duration handler can change exp envi duration status. 
static TimeStamp *outputs_handling_end_time = NULL;



///  Define the guys above for outputs as well when necessary. 

static ExpEnviHand2ExpEnviDurHandMsg *static_msgs_exp_envi_hand_2_exp_envi_dur_hand = NULL;    
static ExpEnviDurHand2ExpEnviHandMsg *static_msgs_exp_envi_dur_hand_2_exp_envi_hand = NULL;    

static int exp_envi_duration_handler_rt_thread = 0;
static bool rt_exp_envi_duration_handler_stay_alive = 1;


static void *rt_exp_envi_duration_handler(void *args);

bool create_exp_envi_duration_handler_rt_thread(RtTasksData *rt_tasks_data, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, unsigned int num_of_inp_comps, unsigned int num_of_outp_comps)
{
	unsigned int i;

	static_rt_tasks_data = rt_tasks_data;

	static_msgs_exp_envi_dur_hand_2_exp_envi_hand = msgs_exp_envi_dur_hand_2_exp_envi_hand;
	static_msgs_exp_envi_hand_2_exp_envi_dur_hand = msgs_exp_envi_hand_2_exp_envi_dur_hand;

	num_of_input_components = num_of_inp_comps;
	exp_envi_inputs_min_duration_status = g_new0(ExpEnviDurationStatus, num_of_inp_comps);
	exp_envi_inputs_max_duration_status = g_new0(ExpEnviDurationStatus, num_of_inp_comps);
	for (i = 0; i < num_of_input_components; i++) 
		exp_envi_inputs_min_duration_status[i] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF;
	for (i = 0; i < num_of_input_components; i++) 
		exp_envi_inputs_max_duration_status[i] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF;
	inputs_handling_end_time_min = g_new0(TimeStamp, num_of_inp_comps);
	inputs_handling_end_time_max = g_new0(TimeStamp, num_of_inp_comps);

	num_of_output_components = num_of_outp_comps;
	exp_envi_outputs_duration_status = g_new0(ExpEnviDurationStatus, num_of_outp_comps);
	for (i = 0; i < num_of_output_components; i++) 
		exp_envi_outputs_duration_status[i] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
	outputs_handling_end_time = g_new0(TimeStamp, num_of_outp_comps);

	if (exp_envi_duration_handler_rt_thread !=0)
		return print_message(BUG_MSG ,"BMIExpController", "ExpEnviHandlerRtTask", "create_exp_envi_duration_handler_rt_thread", "CANNOT create rt_thread again.");	
	exp_envi_duration_handler_rt_thread =  rt_thread_create(rt_exp_envi_duration_handler, NULL, 10000);
	if (exp_envi_duration_handler_rt_thread ==0)
		return print_message(BUG_MSG ,"BMIExpController", "ExpEnviHandlerRtTask", "create_exp_envi_duration_handler_rt_thread", "Couldn' t create rt_thread.");	
	return TRUE;
}

bool kill_exp_envi_duration_handler_rt_thread(void)
{
	return TRUE;
}

static void *rt_exp_envi_duration_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	RTIME prev_time, curr_time;
	RTIME expected;
	RTIME exp_envi_dur_hand_time_ns;
	unsigned int timer_cpuid, task_cpuid;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;
	task_cpuid = (EXP_ENVI_DURATION_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID;

	if (! check_rt_task_specs_to_init(static_rt_tasks_data, EXP_ENVI_DURATION_HANDLER_CPU_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID, EXP_ENVI_DURATION_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(EXP_ENVI_DURATION_HANDLER_TASK_NAME, EXP_ENVI_DURATION_HANDLER_TASK_PRIORITY, EXP_ENVI_DURATION_HANDLER_STACK_SIZE, EXP_ENVI_DURATION_HANDLER_MSG_SIZE,EXP_ENVI_DURATION_HANDLER_POLICY, 1 << task_cpuid))) {
		print_message(ERROR_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(static_rt_tasks_data, EXP_ENVI_DURATION_HANDLER_CPU_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID, EXP_ENVI_DURATION_HANDLER_PERIOD, EXP_ENVI_DURATION_HANDLER_POSITIVE_JITTER_THRES, EXP_ENVI_DURATION_HANDLER_NEGATIVE_JITTER_THRES, "ExpEnviDurationHandler", FALSE))  {
		print_message(ERROR_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	curr_time = rt_get_time_cpuid(task_cpuid);	
	exp_envi_dur_hand_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

        period = nano2count(EXP_ENVI_DURATION_HANDLER_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;


        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	static_msgs_exp_envi_hand_2_exp_envi_dur_hand->buff_read_idx = static_msgs_exp_envi_hand_2_exp_envi_dur_hand->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        while (rt_exp_envi_duration_handler_stay_alive) 
	{
        	rt_task_wait_period();

		curr_time = rt_get_time_cpuid(task_cpuid);	
		exp_envi_dur_hand_time_ns = rt_get_time_ns_cpuid(timer_cpuid);	// use system time

		expected += period;
		evaluate_and_save_jitter(static_rt_tasks_data, EXP_ENVI_DURATION_HANDLER_CPU_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID, curr_time, expected);
		prev_time = curr_time;

		// routines
		if (! handle_exp_envi_handler_to_exp_envi_dur_handler_msg(exp_envi_inputs_min_duration_status, exp_envi_inputs_max_duration_status, exp_envi_dur_hand_time_ns, static_msgs_exp_envi_hand_2_exp_envi_dur_hand, inputs_handling_end_time_min, inputs_handling_end_time_max, exp_envi_outputs_duration_status, outputs_handling_end_time)) {
			print_message(ERROR_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "! handle_exp_envi_handler_to_exp_envi_dur_handler_msg()."); break; }
		if (! handle_exp_envi_handler_duration(exp_envi_inputs_min_duration_status, exp_envi_inputs_max_duration_status, exp_envi_dur_hand_time_ns, inputs_handling_end_time_min, inputs_handling_end_time_max, num_of_input_components, static_msgs_exp_envi_dur_hand_2_exp_envi_hand, num_of_output_components, exp_envi_outputs_duration_status, outputs_handling_end_time))  {
			print_message(ERROR_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "! handle_exp_envi_handler_duration()."); break; }
		// routines	
		evaluate_and_save_period_run_time(static_rt_tasks_data, EXP_ENVI_DURATION_HANDLER_CPU_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID, EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_time_cpuid(timer_cpuid));		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"BMIExpController", "ExpEnviDurationHandlerRtTask", "rt_exp_envi_duration_handler", "rt_task_delete().");	

        return 0; 
}
