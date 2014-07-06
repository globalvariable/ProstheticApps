#include "HandleNeuralNet2MovObjHandMsgs.h"



bool handle_neural_net_to_mov_obj_handler_msg(ThreeDofRobot *robot_arm, TimeStamp current_time, NeuralNet2MovObjHandMsg **msgs_neural_net_2_mov_obj_hand_multi_thread, SpikeData *scheduled_spike_data)
{
	NeuralNet2MovObjHandMsgItem msg_item;
	char str_neural_net_msg[NEURAL_NET_2_MOV_OBJ_HAND_MSG_STRING_LENGTH];
	unsigned int i;

	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		while (get_next_neural_net_2_mov_obj_hand_msg_buffer_item(msgs_neural_net_2_mov_obj_hand_multi_thread[i], &msg_item))
		{
//			get_neural_net_2_mov_obj_hand_msg_type_string(msg_item.msg_type, str_neural_net_msg);
//			print_message(INFO_MSG ,"MovObjHandler", "HandleNeuralNet2MovObjHandMsgs", "handle_neural_net_to_mov_obj_handler_msg", str_neural_net_msg);
			switch (msg_item.msg_type)
			{
				case NEURAL_NET_2_MOV_OBJ_HAND_MSG_SPIKE_OUTPUT:	
					// to scheduled_spike_data to be handled by mov_obj_hand_spike_data_handler which should be run just after this function: 
					if (! write_to_spike_data_with_sorting_single_threaded(scheduled_spike_data, msg_item.layer_num, msg_item.nrn_grp_num, msg_item.neuron_num, msg_item.spike_time))  // msg_item includes already scheduled/delayed spike event while writing into msgs_neural_net_2_mov_obj_hand_multi_thread by Neural Net Simulator.
						return print_message(BUG_MSG ,"MovObjHandler", "HandleNeuralNet2MovObjHandMsgs", "handle_neural_net_to_mov_obj_handler_msg", "! write_to_spike_data_with_sorting()");
					break;
				default:
					return print_message(BUG_MSG ,"MovObjHandler", "HandleNeuralNet2MovObjHandMsgs", "handle_neural_net_to_mov_obj_handler_msg", str_neural_net_msg);
			}
		}
	}
	return TRUE;
}
