#include "HybridNetworkRtTask.h"

static int rt_periodic_task_thread;

static void *rt_periodic_handler(void *args);



bool start_periodic_task(void)
{
	rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, sizeof(RtTasksData));
	if (rt_tasks_data == NULL) 
		return print_message(ERROR_MSG ,"PCIe6259", "RtTask", "start_periodic_task", "rt_tasks_data == NULL.");
	memset(rt_tasks_data, 0, sizeof(RtTasksData));

	rt_periodic_task_stay_alive = 1;
	rt_periodic_task_thread = rt_thread_create(rt_periodic_handler, NULL, 10000);

	return TRUE;
}


static void *rt_periodic_handler(void *args)
{
	RT_TASK *handler;
        RTIME period, expected;
	RTIME prev_time, curr_time;

	unsigned int timer_cpuid;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;

	rt_set_oneshot_mode();
//	rt_set_periodic_mode();

	if (! check_rt_task_specs_to_init(rt_tasks_data, SYSTIME_PERIODIC_CPU_ID, SYSTIME_PERIODIC_CPU_THREAD_ID, SYSTIME_PERIODIC_CPU_THREAD_TASK_ID, SYSTIME_PERIODIC_PERIOD, TRUE))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(SYSTIME_PERIODIC_TASK_NAME, SYSTIME_PERIODIC_TASK_PRIORITY, SYSTIME_PERIODIC_STACK_SIZE, SYSTIME_PERIODIC_MSG_SIZE,SYSTIME_PERIODIC_POLICY, 1 << ((SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, SYSTIME_PERIODIC_CPU_ID, SYSTIME_PERIODIC_CPU_THREAD_ID, SYSTIME_PERIODIC_CPU_THREAD_TASK_ID, SYSTIME_PERIODIC_PERIOD, SYSTIME_PERIODIC_POSITIVE_JITTER_THRES, SYSTIME_PERIODIC_NEGATIVE_JITTER_THRES, "SystemTime", TRUE) ) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	


	start_rt_timer(0);

	curr_time =  rt_get_time_cpuid(timer_cpuid);	
	rt_tasks_data->current_system_time = count2nano(curr_time);

        period = nano2count(SYSTIME_PERIODIC_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

        while (rt_periodic_task_stay_alive) 
	{
        	rt_task_wait_period();

		curr_time = rt_get_time_cpuid(timer_cpuid);	
		rt_tasks_data->current_system_time = count2nano(curr_time);


		expected += period;
		evaluate_and_save_jitter(rt_tasks_data, SYSTIME_PERIODIC_CPU_ID, SYSTIME_PERIODIC_CPU_THREAD_ID, SYSTIME_PERIODIC_CPU_THREAD_TASK_ID, curr_time, expected);
		prev_time = curr_time;
		//	routines are run below


		// no routines. evalute run time.

		// routines
		evaluate_and_save_period_run_time(rt_tasks_data, SYSTIME_PERIODIC_CPU_ID, SYSTIME_PERIODIC_CPU_THREAD_ID, SYSTIME_PERIODIC_CPU_THREAD_TASK_ID, curr_time, rt_get_time_cpuid(timer_cpuid));
	}
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_sem_delete().");	
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_task_delete().");	

        return 0; 
}
