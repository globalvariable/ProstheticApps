#include "SnnData.h"



bool initialize_snn_data(void)
{
	blue_spike_network = allocate_network(blue_spike_network);
	in_silico_network = allocate_network(in_silico_network);

	num_of_dedicated_cpus = IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS;


	msgs_trial_hand_2_neural_net = allocate_shm_server_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net);
	msgs_neural_net_2_mov_obj_hand_multi_thread = g_new0(NeuralNet2MovObjHandMsg*, num_of_dedicated_cpus); 
	msgs_mov_obj_hand_2_neural_net_multi_thread = allocate_shm_server_mov_obj_hand_2_neural_net_multi_thread_msg_buffer(msgs_mov_obj_hand_2_neural_net_multi_thread, num_of_dedicated_cpus); 
	msgs_neural_net_2_gui = allocate_neural_net_2_gui_msg_buffer(msgs_neural_net_2_gui);
	trial_status_events = allocate_trial_status_events_buffer(trial_status_events, 100, 3000000);  //  3 ms latency

	in_silico_spike_data_for_graph = g_new0(SpikeData*, num_of_dedicated_cpus);	// allocate the array of buffers here. 
	in_silico_spike_data_for_recording = g_new0(SpikeData*, num_of_dedicated_cpus); // allocate the array of buffers here.  a recording message which is received before "ready_for_simulation" button can lead to segfault. since read_idx and write_idx are zero here, they will not try to access the buffers (refer to get_next_spike_data_item())


	return TRUE;
}

