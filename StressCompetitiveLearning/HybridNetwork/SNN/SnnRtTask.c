#include "SnnRtTask.h"



static int snn_rt_threads[SNN_SIM_NUM_OF_DEDICATED_CPUS];
static unsigned int snn_rt_thread_ids[SNN_SIM_NUM_OF_DEDICATED_CPUS];

static int trial_hand_2_neural_net_msgs_handler_rt_thread;

static void *snn_rt_handler(void *args);
static void *trial_hand_2_neural_net_msgs_handler(void *args); 

void create_snn_rt_threads(void)
{
	static bool first = TRUE;
	unsigned int i;
	if (in_silico_network->layer_count != NUM_OF_IN_SILICO_NETWORK_LAYERS)
		return (void)print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "in_silico_network->layer_count != NUM_OF_IN_SILICO_NETWORK_LAYERS.");		

	if (first)
	{

		for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
		{
			snn_rt_thread_ids[i] = i; 
			snn_rt_threads[i] =  rt_thread_create(snn_rt_handler, &(snn_rt_thread_ids[i]), 10000);
		}

		trial_hand_2_neural_net_msgs_handler_rt_thread =  rt_thread_create(trial_hand_2_neural_net_msgs_handler, NULL, 10000);

		sleep(1);	// sleep 1 second before enabling delivery of sorted spikes from spike sorter. thus, the snn_rt_handler tasks will be ensured to be active. 
		sorted_spike_delivery_enabled = TRUE;
		first = FALSE;
	}
	else
	{
		print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "hybrid_net_rl_bmi_create_rt_threads", "CANNOT create rt_thread again. Should restart the program. ");
		exit(1);	
	}

}


static void *snn_rt_handler(void *args)
{
	RT_TASK *handler;
        RTIME period, expected, execution_end;
	RTIME prev_time, curr_time;
	RTIME integration_start_time, integration_end_time, time_ns;
	RTIME current_snn_time;
	unsigned int task_num = *((unsigned int*)args);

	Neuron		**all_neurons = in_silico_network->all_neurons;
	unsigned int	num_of_all_neurons =  in_silico_network->num_of_neurons;
	Neuron 		*nrn;

	TimeStamp spike_time;
	bool spike_generated;

	unsigned int cpu_id, cpu_thread_id, cpu_thread_task_id;
	char task_name[10];
	char task_num_name[4];

	unsigned int i, timer_cpuid, task_cpuid;

	unsigned int run_time_cntr = 0;

	MovObjHand2NeuralNetMsg *msgs_mov_obj_hand_2_neural_net;
	MovObjHand2NeuralNetMsgItem msg_item;

	int squential_reward_val = 0;
	double reward;
	double sensory_reward;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;

	cpu_id =  SNN_SIM_CPU_ID + task_num ;
	cpu_thread_id = 0;
	cpu_thread_task_id = 0;

	task_cpuid = (cpu_id*MAX_NUM_OF_CPU_THREADS_PER_CPU)+cpu_thread_id;

	strcpy(task_name, SNN_SIM_TASK_NAME);
	sprintf(task_num_name, "%u", task_num );
	strcat(task_name, task_num_name);

	if (! check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, SNN_SIM_PERIOD, FALSE)) 
		return (void *)print_message(ERROR_MSG ,"CompetitiveLearning", "SnnRtTask", "snn_rt_handler", "! check_rt_task_specs_to_init().");
        if (! (handler = rt_task_init_schmod(nam2num(task_name), SNN_SIM_TASK_PRIORITY, SNN_SIM_STACK_SIZE, SNN_SIM_MSG_SIZE, SNN_SIM_POLICY, 1 << task_cpuid)))
		return (void *)print_message(ERROR_MSG ,"CompetitiveLearning", "SnnRtTask", "snn_rt_handler", "handler = rt_task_init_schmod()."); 
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, SNN_SIM_PERIOD, SNN_SIM_POSITIVE_JITTER_THRES, SNN_SIM_NEGATIVE_JITTER_THRES, SNN_SIM_RUN_TIME_THRES, "HybridNet", FALSE))
		return (void *)print_message(ERROR_MSG ,"CompetitiveLearning", "SnnRtTask", "snn_rt_handler", "! write_rt_task_specs_to_rt_tasks_data()."); 



	curr_time = rt_get_time_cpuid(task_cpuid);	
	current_snn_time =  rt_get_time_ns_cpuid(timer_cpuid);	// use system time

        period = nano2count(SNN_SIM_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;

	integration_start_time = (current_snn_time/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;

	reset_all_network_iz_neuron_dynamics (in_silico_network);	// clears neuron synaptic event buffers as well

	msgs_mov_obj_hand_2_neural_net = msgs_mov_obj_hand_2_neural_net_multi_thread[task_num];

	msgs_mov_obj_hand_2_neural_net->buff_read_idx = msgs_mov_obj_hand_2_neural_net->buff_write_idx;   // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

       while (1) 
	{
        	rt_task_wait_period();

		curr_time = rt_get_time_cpuid(task_cpuid);	
		current_snn_time = rt_get_time_ns_cpuid(timer_cpuid);	// use system time
		
		expected += period;
		evaluate_and_save_jitter(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, curr_time, expected);

		prev_time = curr_time;
		// routines
		while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item(msgs_mov_obj_hand_2_neural_net, &msg_item))		
		{
			switch (msg_item.msg_type)
			{
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_REINFORCEMENT:
//					printf("rf %d\n", msg_item.additional_data.binary_reward_add.reward);

					if (msg_item.additional_data.binary_reward_add.reward > 0)  // LTP
					{					
						squential_reward_val++;
						if (squential_reward_val >= 1)
							sensory_reward = 1;
						else
							sensory_reward = -1;
						reward = reward_data.learning_rate *(1-reward_data.current_reward_prediction) * sensory_reward;  

//						printf("rew = %.3f\n", reward);

						update_synaptic_weights_all_neurons_in_thread(all_neurons, num_of_all_neurons, task_num, SNN_SIM_NUM_OF_DEDICATED_CPUS, reward, total_synaptic_weights);

					}
					else	if (msg_item.additional_data.binary_reward_add.reward < 0)	// LTD
					{
						if (squential_reward_val > 0)
							squential_reward_val = 0;
						sensory_reward = -1;
						reward = reward_data.learning_rate *(1-reward_data.current_reward_prediction) * sensory_reward; 
//						printf("rew = %.3f\n", reward);
						update_synaptic_weights_all_neurons_in_thread(all_neurons, num_of_all_neurons, task_num, SNN_SIM_NUM_OF_DEDICATED_CPUS, reward, total_synaptic_weights);
					}		
					else
					{
						sensory_reward = -1;
						reward = reward_data.learning_rate *(1-reward_data.current_reward_prediction) * sensory_reward;  
//						printf("rew = %.3f\n", reward);
						update_synaptic_weights_all_neurons_in_thread(all_neurons, num_of_all_neurons, task_num, SNN_SIM_NUM_OF_DEDICATED_CPUS, reward, total_synaptic_weights);
					}	
					break;	
				case MOV_OBJ_HAND_2_NEURAL_NET_MSG_START_TRIAL:
					squential_reward_val = 0;
					sensory_reward = 0;
					break;	
				default:
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "mov_obj_hand_2_neural_net_msgs_handler", "Invalid message.");	
					goto EXIT;
			}
		}		

		integration_end_time =  (current_snn_time/PARKER_SOCHACKI_INTEGRATION_STEP_SIZE) *PARKER_SOCHACKI_INTEGRATION_STEP_SIZE;
		for (time_ns = integration_start_time; time_ns < integration_end_time; time_ns+= PARKER_SOCHACKI_INTEGRATION_STEP_SIZE)   // integrate remaining part in the next task period
		{
			for (i = task_num; i < num_of_all_neurons; i+=SNN_SIM_NUM_OF_DEDICATED_CPUS)  // simulate the neurons for which this thread is responsible
			{
				nrn = all_neurons[i];
				if (! evaluate_neuron_dyn_pre_post_w_resetting_stdp_elig(nrn, time_ns, time_ns+PARKER_SOCHACKI_INTEGRATION_STEP_SIZE, &spike_generated, &spike_time)) {
					print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "snn_rt_handler", "! evaluate_neuron_dyn_pre_post_w_resetting_stdp_elig()."); exit(1); }	
				if (spike_generated)
				{
					write_to_spike_data(in_silico_spike_data_for_graph[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
					write_to_spike_data(in_silico_spike_data_for_recording[task_num], nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time);
					if (nrn->layer_type == NEURON_LAYER_TYPE_OUTPUT)
					{
						if (! write_to_neural_net_2_mov_obj_hand_msg_buffer(msgs_neural_net_2_mov_obj_hand_multi_thread[task_num], integration_start_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_SPIKE_OUTPUT, nrn->layer, nrn->neuron_group, nrn->neuron_num, spike_time)) 
						{
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "snn_rt_handler", "! write_to_neural_net_2_mov_obj_hand_msg_buffer().");
							goto EXIT;
						}
					}
				}	
				push_neuron_dynamics_to_neuron_dynamics_buffer_limited(in_silico_network, neuron_dynamics_limited_buffer, time_ns, i);
				push_stdp_to_stdp_buffer_limited(in_silico_network, stdp_limited_buffer, time_ns, i);
				push_eligibility_to_eligibility_buffer_limited(in_silico_network, eligibility_limited_buffer, time_ns, i);
			}
		}
	
		integration_start_time = integration_end_time;
		// routines	
		execution_end = rt_get_time_cpuid(timer_cpuid);
		evaluate_and_save_period_run_time(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, curr_time, execution_end);	
		run_time_cntr++;
		if (run_time_cntr == NUM_OF_TASK_EXECUTIONS_4_PERFOMANCE_EVAL)
		{
			run_time_cntr = 0;
			write_run_time_to_averaging_buffer(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, curr_time, execution_end);
		}
	}
EXIT:
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMIRtTask", "snn_rt_handler", "rt_task_delete().");	

        return 0;
}

static void *trial_hand_2_neural_net_msgs_handler(void *args)
{
	RT_TASK *handler;
        RTIME period, expected;
	RTIME prev_time, curr_time;
	RTIME trial_2_nn_time_ns;
	char str_trial_hand_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	TrialHand2NeuralNetMsgItem msg_item;
	NeuralNet2GuiMsgAdditional neural_net_2_gui_msg_add;

	unsigned int timer_cpuid, task_cpuid;

	timer_cpuid = (SYSTIME_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+SYSTIME_PERIODIC_CPU_THREAD_ID;
	task_cpuid = (TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID;

	if (! check_rt_task_specs_to_init(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY, 1 << task_cpuid))) {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_RUN_TIME_THRES, "TrialHand2NeuralNet", FALSE))  {
		print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	


	curr_time = rt_get_time_cpuid(task_cpuid);	
	trial_2_nn_time_ns =  rt_get_time_ns_cpuid(timer_cpuid);	// use system time

        period = nano2count(TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD);
        rt_task_make_periodic(handler, curr_time + period, period);

	prev_time = curr_time ;
	expected = curr_time + period;


        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	msgs_trial_hand_2_neural_net->buff_read_idx = msgs_trial_hand_2_neural_net->buff_write_idx; // to reset message buffer. previously written messages and reading of them now might lead to inconvenience.,

        while (1) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_time_cpuid(task_cpuid);	
		trial_2_nn_time_ns =  rt_get_time_ns_cpuid(timer_cpuid);	// use system time // it is not used now. 

		expected += period;
		evaluate_and_save_jitter(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, curr_time, expected);
		prev_time = curr_time;
		// routines

		while (get_next_trial_hand_2_neural_net_msg_buffer_item(msgs_trial_hand_2_neural_net, &msg_item))
		{
			get_trial_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_trial_hand_msg);  
//			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED:   // current system time might be different from the one TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START. it can change during processing the message buffer but would not lead to problem.  TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED is used by the graphs.
					schedule_trial_status_event(trial_status_events, rt_tasks_data->current_system_time, msg_item.additional_data.trial_status_change_msg_add) ; 
					break;	
				case TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START:   // not implemeted yet, for RL it will be required.
					reward_data.current_reward_prediction = msg_item.additional_data.difficulty_reward_predict_add.reward_prediction;
					
//					printf("msg = %.3f\t%.3f\n", reward_data.current_reward_prediction, reward_data.learning_rate);

/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_START_EVENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}					
*/					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_REWARD_GIVEN:   
/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_REWARD)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					neural_net_2_gui_msg_add.reward = msg_item.additional_data.reward;


					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
*/					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_PUNISHMENT_GIVEN:   
/*					current_sys_time = rt_tasks_data->current_system_time;
					for (i = 0; i < num_of_neurons; i++)
					{
						if (all_neurons[i]->trial_event_buffer == NULL)   // maybe not an izhikevich neuron (or a neuron that can accept events, e.g. poisson neuron)
							continue;
						if (! write_to_neuron_trial_event_buffer(all_neurons[i], current_sys_time, NEURON_EVENT_TYPE_TRIAL_END_WITH_PUNISHMENT)) {
							print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neuron_trial_event_buffer()."); exit(1); }	
					}
					neural_net_2_gui_msg_add.reward = msg_item.additional_data.reward;	/// THIS MESSAGE SHOULD BE SENT BEFORE TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING which saves the synaptic weights.


					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
*/					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_END_TRIAL_WITH_NOTHING:   
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_UPDATE_SYNAPTIC_WEIGHTS_HISTORY, neural_net_2_gui_msg_add)) {
						print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "! write_to_neural_net_2_gui_msg_buffer()."); exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_START_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_START_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_STOP_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
					break;
				case TRIAL_HAND_2_NEURAL_NET_MSG_CANCEL_RECORDING:	
					neural_net_2_gui_msg_add.recording_number = msg_item.additional_data.recording_number;
					if (! write_to_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui, rt_tasks_data->current_system_time, NEURAL_NET_2_GUI_MSG_CANCEL_RECORDING, neural_net_2_gui_msg_add)) {
						print_message(BUG_MSG ,"NeuralNetler", "HandleTrialHand2NeuralNetMsgs", "write_to_neural_net_2_gui_msg_buffer", "! write_to_neural_net_2_gui_msg_buffer(().");exit(1); }	
					break;
				default: 
					print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", str_trial_hand_msg);
					rt_make_soft_real_time();
        				rt_task_delete(handler);
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "rt_task_delete().");	
				        exit(1); 
			}
		}
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID, TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID, curr_time, rt_get_time_cpuid(timer_cpuid));		
        }
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "trial_hand_2_neural_net_msgs_handler", "rt_task_delete().");	

        return 0;
}
