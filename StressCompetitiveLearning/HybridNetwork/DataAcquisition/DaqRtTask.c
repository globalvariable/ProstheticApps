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
	RTIME curr_time, measured_time, prev_time, latency;
	RTIME current_daq_time;
        RTIME period;
        RTIME sync_step, diff, diff_thres, net_diff = 0;
	unsigned int daq_num;
	long int cb_val = 0, cb_retval = 0;
	lsampl_t daq_data[MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN];

	unsigned int timer_cpuid;

	comedi_insn insn;
	lsampl_t insn_data[1];

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;

	daq_num = *((unsigned int*)args);

	memset(&insn, 0, sizeof(comedi_insn));
	insn.insn = INSN_INTTRIG;
	insn.subdev = COMEDI_SUBDEVICE_AI;
	insn.data = insn_data;
	insn.n = 1;
	insn_data[0] = 0;

	period = nano2count(BLUESPIKE_DAQ_PERIOD);
	sync_step = nano2count(2000);	// 2 microseconds
	diff_thres = nano2count(500000);	// 500 microseconds

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

	sleep(1);


	prev_time = rt_get_time_cpuid(timer_cpuid);	

	comedi_do_insn(ni6259_comedi_dev[daq_num], &insn);
	rt_comedi_wait(&cb_val);

	measured_time = rt_get_time_cpuid(timer_cpuid);	

	printf ("%lld\n", count2nano(measured_time-prev_time));

	latency = measured_time - prev_time - nano2count(BLUESPIKE_DAQ_PERIOD);
	curr_time = measured_time;	// last sample's timestamp
	prev_time = measured_time;

	rt_comedi_command_data_read(ni6259_comedi_dev[daq_num], COMEDI_SUBDEVICE_AI, MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN, daq_data);	// discard this reading.

        while (daq_cards_on) 
	{
		cb_val = 0;
		cb_retval += rt_comedi_wait(&cb_val);

		measured_time = rt_get_time_cpuid(timer_cpuid);	

		curr_time += period;	

		evaluate_and_save_jitter(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, measured_time, curr_time);

		diff = curr_time-measured_time;
		if (diff > sync_step)
		{
			curr_time -= sync_step;  // sync
			net_diff -= sync_step; 
		}
		else if (diff < (-sync_step))
		{
			curr_time += sync_step;  // sync
			net_diff += sync_step; 
		}
		else
		{
			curr_time = measured_time;
			net_diff -= diff; 
		}
		if (diff > diff_thres) 
			printf("DaqRtTask: diff > diff_thres\n"); 
		if (diff < (-diff_thres)) 
			printf("DaqRtTask: diff < (-diff_thres)\n"); 		

//			printf ("%lld\n", count2nano(net_diff));

//		printf ("%lld\n", count2nano(measured_time-prev_time));

		prev_time = measured_time;

		current_daq_time = count2nano(curr_time - latency);

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
		evaluate_and_save_period_run_time(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, measured_time, rt_get_time_cpuid(timer_cpuid));		
        }
	close_daq_cards(daq_num);
	rt_make_soft_real_time();
        rt_task_delete(handler);
	if (! delete_rt_task_from_rt_tasks_data(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, TRUE)) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! delete_rt_task_from_rt_tasks_data()."); exit(1); }	
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "rt_task_delete().");	

        return 0; 
}

