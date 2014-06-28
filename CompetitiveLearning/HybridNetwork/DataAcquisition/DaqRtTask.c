#include "DaqRtTask.h"


static int rt_daq_threads[MAX_NUM_OF_DAQ_CARD];




static void *rt_daq_handler(void *args);




bool start_acquisition(void)
{
	unsigned int i;
	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		ni6259_comedi_dev_ids[i] = i;
		rt_daq_threads[i] = rt_thread_create(rt_daq_handler, &(ni6259_comedi_dev_ids[i]), 10000);	// support to handle daq cards on multiple threads.  To handle on multiple CPUs assign daq handling tasks to specific CPUs via rt_task_init_schmod()

	}

	return TRUE;
}


static void *rt_daq_handler(void *args)
{
	RT_TASK *handler;
	unsigned int prev_time, curr_time;
	TimeStamp current_daq_time, now;
	unsigned int daq_num;
	long int cb_val = 0, cb_retval = 0;
	lsampl_t daq_data[MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN];

	unsigned int daq_sync_cntr = 0;

	daq_num = *((unsigned int*)args);

	if (! check_rt_task_specs_to_init(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, BLUESPIKE_DAQ_PERIOD, TRUE))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! check_rt_task_specs_to_init()."); exit(1); }
        if (! (handler = rt_task_init_schmod(BLUESPIKE_DAQ_TASK_NAME, BLUESPIKE_DAQ_TASK_PRIORITY, BLUESPIKE_DAQ_STACK_SIZE, BLUESPIKE_DAQ_MSG_SIZE,BLUESPIKE_DAQ_POLICY, 1 << ((BLUESPIKE_DAQ_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUESPIKE_DAQ_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, BLUESPIKE_DAQ_PERIOD, BLUESPIKE_DAQ_POSITIVE_JITTER_THRES, BLUESPIKE_DAQ_NEGATIVE_JITTER_THRES, "BlueSpike", TRUE) ) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	


        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	if (! open_daq_card(daq_num))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! open_daq_cards()."); exit(1); }		

	rt_comedi_register_callback(ni6259_comedi_dev[daq_num], COMEDI_SUBDEVICE_AI, COMEDI_CB_EOS, NULL, handler);

	if (! config_daq_card(daq_num))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! config_daq_cards()."); exit(1); }		

	rt_sem_wait(sys_time_semaphore);
	curr_time = rt_get_cpu_time_ns();	
	current_daq_time =  (curr_time - rt_tasks_data->current_cpu_time) + rt_tasks_data->current_system_time;
	rt_sem_signal(sys_time_semaphore);

	prev_time = curr_time;

        while (daq_cards_on) 
	{
		cb_val = 0;
		cb_retval += rt_comedi_wait(&cb_val);

		rt_sem_wait(sys_time_semaphore);
		curr_time = rt_get_cpu_time_ns();
		now = (curr_time - rt_tasks_data->current_cpu_time) + rt_tasks_data->current_system_time;
		rt_sem_signal(sys_time_semaphore);

		evaluate_and_save_jitter(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, prev_time, curr_time);

		prev_time = curr_time;


		daq_sync_cntr++;
		if (daq_sync_cntr == 4)		// her 4 rt_comedi_wait'te bir kere yüksek jitter oluyor. Muhtemelen onboard memory'nin sonuna geliyor ve kart daha hızlı interrupt gönderiyor. 
		{	
			daq_sync_cntr = 0;
			current_daq_time = now;   // sync daq time according to cpu clock, cpu clock is the reference. 
		}
		else
		{
			current_daq_time += (SAMPLING_INTERVAL*NUM_OF_SCAN);		
		}		

		// routines
		if (!(cb_val & COMEDI_CB_EOS))
		{
			print_message(WARNING_MSG ,"HybridNetwork", "DaqRtTask", "rt_daq_handler", "! (cb_val & COMEDI_CB_EOS)."); 
		}

		rt_comedi_command_data_read(ni6259_comedi_dev[daq_num], COMEDI_SUBDEVICE_AI, MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN, daq_data);

		pthread_mutex_lock(&(daq_mwa_map[daq_num].mutex));   // do not allow mapping change by configdaqgui during processing retrieved data. 	


		handle_recording_data(daq_num, daq_data);
		if (! spike_sorting(daq_num, current_daq_time - (SAMPLING_INTERVAL*NUM_OF_SCAN)))
		{
			print_message(ERROR_MSG ,"HybridNetwork", "DaqRtTask", "rt_daq_handler", "! spike_sorting())."); 
			break;
		}

		pthread_mutex_unlock(&(daq_mwa_map[daq_num].mutex)); 
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, curr_time, rt_get_cpu_time_ns());		
        }
	close_daq_cards(daq_num);
	rt_make_soft_real_time();
        rt_task_delete(handler);
	if (! delete_rt_task_from_rt_tasks_data(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, TRUE)) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! delete_rt_task_from_rt_tasks_data()."); exit(1); }	
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "rt_task_delete().");	

        return 0; 
}

