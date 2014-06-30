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
        RTIME period;
	unsigned int prev_time, curr_time;
	
	if (! check_rt_task_specs_to_init(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, BLUESPIKE_PERIODIC_PERIOD, TRUE))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(BLUESPIKE_PERIODIC_TASK_NAME, BLUESPIKE_PERIODIC_TASK_PRIORITY, BLUESPIKE_PERIODIC_STACK_SIZE, BLUESPIKE_PERIODIC_MSG_SIZE,BLUESPIKE_PERIODIC_POLICY, 1 << ((BLUESPIKE_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUESPIKE_PERIODIC_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, BLUESPIKE_PERIODIC_PERIOD, BLUESPIKE_PERIODIC_POSITIVE_JITTER_THRES, BLUESPIKE_PERIODIC_NEGATIVE_JITTER_THRES, "BlueSpike", TRUE) ) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	// Initialize semaphore for system_time.
	if (sys_time_semaphore != NULL)
        	return (void*) print_message(ERROR_MSG ,"HybridNetwork", "HybridNetworkRtTask", "start_periodic_task", "sys_time_semaphore != NULL.");
	sys_time_semaphore = rt_get_adr(SEM_NUM_SYSTEM_TIME);
	if (sys_time_semaphore == NULL)  // semaphore had not been created before.
	{
		sys_time_semaphore = rt_typed_sem_init(SEM_NUM_SYSTEM_TIME, 1, BIN_SEM | FIFO_Q );
		print_message(INFO_MSG ,"HybridNetwork", "HybridNetworkRtTask", "start_periodic_task", "Initialized system_time.semaphore.");  
	}
	else // a semaphore created before, delete and init it again.
	{
		rt_sem_delete(sys_time_semaphore);
		sys_time_semaphore = rt_typed_sem_init(SEM_NUM_SYSTEM_TIME, 1, BIN_SEM | FIFO_Q );
		print_message(INFO_MSG ,"HybridNetwork", "HybridNetworkRtTask", "start_periodic_task", "Re-initialized system_time.semaphore.");  
	}

	start_rt_timer(nano2count(START_RT_TIMER_PERIOD));
        period = nano2count(BLUESPIKE_PERIODIC_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);


        rt_sem_wait(sys_time_semaphore);
	rt_tasks_data->current_cpu_time =  rt_get_cpu_time_ns();	
	rt_tasks_data->current_system_time = 0;
        rt_sem_signal(sys_time_semaphore);

	prev_time = rt_tasks_data->current_cpu_time ;

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

        while (rt_periodic_task_stay_alive) 
	{
        	rt_task_wait_period();

		rt_sem_wait(sys_time_semaphore);
		curr_time = rt_get_cpu_time_ns();
		rt_tasks_data->current_cpu_time= curr_time;
		rt_tasks_data->current_system_time += (curr_time-prev_time);
		rt_sem_signal(sys_time_semaphore);

		evaluate_and_save_jitter(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		//	routines are run below


		// no routines. evalute run time.

		// routines
		evaluate_and_save_period_run_time(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());
	}
	rt_sem_delete(sys_time_semaphore);
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_sem_delete().");	
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_task_delete().");	

        return 0; 
}
