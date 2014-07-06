#include "SnnData.h"



bool initialize_snn_data(void)
{
	blue_spike_network = allocate_network(blue_spike_network);
	in_silico_network = allocate_network(in_silico_network);

	initialize_data_read_write_handlers();

	msgs_trial_hand_2_neural_net = allocate_shm_server_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net);

	msgs_neural_net_2_mov_obj_hand_multi_thread = g_new0(NeuralNet2MovObjHandMsg*, SNN_SIM_NUM_OF_DEDICATED_CPUS); 
	msgs_mov_obj_hand_2_neural_net_multi_thread = allocate_shm_server_mov_obj_hand_2_neural_net_multi_thread_msg_buffer(msgs_mov_obj_hand_2_neural_net_multi_thread, SNN_SIM_NUM_OF_DEDICATED_CPUS); 
	msgs_neural_net_2_gui = allocate_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui);
	trial_status_events = allocate_trial_status_events_buffer(trial_status_events, 100, 3000000);  //  3 ms latency


	in_silico_spike_data_for_graph = g_new0(SpikeData*, SNN_SIM_NUM_OF_DEDICATED_CPUS);	// allocate the array of buffers here. 
	in_silico_spike_data_for_recording = g_new0(SpikeData*, SNN_SIM_NUM_OF_DEDICATED_CPUS); // allocate the array of buffers here.  a recording message which is received before "ready_for_simulation" button can lead to segfault. since read_idx and write_idx are zero here, they will not try to access the buffers (refer to get_next_spike_data_item())


	return TRUE;
}


bool connect_to_mov_obj_hand(void )
{
	MovObjHand2NeuralNetMsgItem msg_item;
	char str_mov_obj_hand_2_neural_net_msg[MOV_OBJ_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];
	unsigned int i, num_of_alive_responses = 0;
	while (1) 
	{ 
		for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
		{
			while (get_next_mov_obj_hand_2_neural_net_msg_buffer_item(msgs_mov_obj_hand_2_neural_net_multi_thread[i], &msg_item))
			{
				get_mov_obj_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_mov_obj_hand_2_neural_net_msg);
				print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				switch (msg_item.msg_type)
				{
					case MOV_OBJ_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE:
						msgs_neural_net_2_mov_obj_hand_multi_thread[i] = allocate_shm_client_neural_net_2_mov_obj_hand_multi_thread_msg_buffer_item(msgs_neural_net_2_mov_obj_hand_multi_thread, i, MIN_NEURAL_NET_2_MOV_OBJ_HAND_EVENT_SCHEDULING_DELAY, MIN_NEURAL_NET_2_MOV_OBJ_HAND_EVENT_SCHEDULING_DELAY);
						usleep(1000);
						if (msgs_neural_net_2_mov_obj_hand_multi_thread[i] == NULL)
							return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "msgs_neural_net_2_mov_obj_hand == NULL.");	
						if (!write_to_neural_net_2_mov_obj_hand_msg_buffer(msgs_neural_net_2_mov_obj_hand_multi_thread[i], rt_tasks_data->current_system_time, NEURAL_NET_2_MOV_OBJ_HAND_MSG_I_AM_ALIVE, 0, 0, 0, 0))
							return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "write_to_neural_net_2_mov_obj_hand_msg_buffer().");	
						num_of_alive_responses++;
						if (num_of_alive_responses == SNN_SIM_NUM_OF_DEDICATED_CPUS)
						{
							goto EXIT;	// changed it from GET_JOINT_ANGLE_MINS to EXIT according to new design. no robot angle is required only binary reward signal is enough.
						}
						break;		
					default:
						return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", str_mov_obj_hand_2_neural_net_msg);	
				}
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Waiting for MOV_OBJ_HANDLER to connect.");	
		sleep(1);
	}

	EXIT:
	return print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_mov_obj_hand", "Connection to MOV_OBJ_HANDLER is successful!!!");
}

bool connect_to_trial_hand(void )
{
	TrialHand2NeuralNetMsgItem msg_item;
	char str_trial_hand_2_neural_net_msg[TRIAL_HAND_2_NEURAL_NET_MSG_STRING_LENGTH];

	while (1) 
	{ 
		while (get_next_trial_hand_2_neural_net_msg_buffer_item(msgs_trial_hand_2_neural_net, &msg_item))
		{
			get_trial_hand_2_neural_net_msg_type_string(msg_item.msg_type, str_trial_hand_2_neural_net_msg);
			print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", str_trial_hand_2_neural_net_msg);	
			switch (msg_item.msg_type)
			{
				case TRIAL_HAND_2_NEURAL_NET_MSG_ARE_YOU_ALIVE:
					msgs_neural_net_2_trial_hand = allocate_shm_client_neural_net_2_trial_hand_msg_buffer(msgs_neural_net_2_trial_hand);
					sleep(1);
					if (msgs_neural_net_2_trial_hand == NULL)
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "msgs_neural_net_2_trial_hand == NULL.");	
					if (!write_to_neural_net_2_trial_hand_msg_buffer(msgs_neural_net_2_trial_hand, rt_tasks_data->current_system_time, NEURAL_NET_2_TRIAL_HAND_MSG_I_AM_ALIVE, 0))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "write_to_neural_net_2_trial_hand_msg_buffer().");	
					print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Connection to TRIAL_HANDLER is successful!!!");	
					return TRUE;		
				default:
					return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", str_trial_hand_2_neural_net_msg);	
			}
		}
		print_message(INFO_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Waiting for TRIAL_HANDLER to connect.");	
		sleep(1);
	}
	return print_message(BUG_MSG ,"HybridNetRLBMI", "HybridNetRLBMI", "connect_to_trial_hand", "Wrong hit in the code.");
}

