#include "HandleMovObjDurHand2MovObjHandMsgs.h"


bool handle_mov_obj_dur_handler_to_mov_obj_handler_msg(ThreeDofRobot *robot_arm, TimeStamp current_time, MovObjStatus *mov_obj_status, MovObjDurHand2MovObjHandMsg *msgs_mov_obj_dur_hand_2_mov_obj_hand, MovObjHand2TrialHandMsg *msgs_mov_obj_hand_2_trial_hand, MovObjHand2MovObjDurHandMsg *msgs_mov_obj_hand_2_mov_obj_dur_hand, MovObjHand2NeuralNetMsg **msgs_mov_obj_hand_2_neural_net_multi_thread, SpikeData *scheduled_spike_data, MessageLogBuffer *message_log, MovObjHandParadigmRobotReach *mov_obj_paradigm, ThreeDofRobotPulseHistory *robot_pulse_history, ThreeDofRobotAngleHistory *robot_angle_history, MovObjStatusHistory* mov_obj_status_history)
{
	MovObjHand2MovObjDurHandMsgAdditional mov_obj_hand_2_mov_obj_dur_hand_additional_data;
	MovObjDurHand2MovObjHandMsgItem msg_item;
	char str_mov_obj_dur_msg[MOV_OBJ_DUR_HAND_2_MOV_OBJ_HAND_MSG_STRING_LENGTH];
	MovObjHand2NeuralNetMsgAdditional	mov_obj_hand_2_neural_net_msg_add;
	static double prev_distance_to_target = 0;
	double distance_to_target;
	CartesianCoordinates	*target_coordinates;
	ThreeDofRobotPosition	*tip_position;
	unsigned int i;
	while (get_next_mov_obj_dur_hand_2_mov_obj_hand_msg_buffer_item(msgs_mov_obj_dur_hand_2_mov_obj_hand, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case MOV_OBJ_DUR_HAND_2_MOV_OBJ_HAND_MSG_TIMEOUT:
				switch (msg_item.additional_data)
				{
					case MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION:
						get_mov_obj_dur_hand_2_mov_obj_hand_msg_type_string(msg_item.msg_type, str_mov_obj_dur_msg);
						if (! write_to_message_log_buffer(message_log, INFO_MSG, current_time, "MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", str_mov_obj_dur_msg))
							print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_message_log_buffer()");
						switch (*mov_obj_status)
						{
							case MOV_OBJ_STATUS_OUT_OF_TRIAL:
								return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION & *mov_obj_status - MOV_OBJ_STATUS_OUT_OF_TRIAL");									
							case MOV_OBJ_STATUS_STAYING_AT_START_POINT:
								*mov_obj_status = MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL;
								if (! write_to_mov_obj_hand_2_trial_hand_msg_buffer(msgs_mov_obj_hand_2_trial_hand, current_time,  MOV_OBJ_HAND_2_TRIAL_HAND_MSG_MOV_OBJ_CONTROL_ENABLED, 0))
									return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_hand_2_trial_hand_msg_buffer()");
								if (! write_to_mov_obj_status_history(mov_obj_status_history, current_time, MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL))
									return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_status_history()");
								break;
							case MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL:
								return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION & *mov_obj_status - MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL");									
							case MOV_OBJ_STATUS_RESETTING_TO_TARGET_POINT:
								return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION & *mov_obj_status - MOV_OBJ_STATUS_RESETTING_TO_TARGET_POINT");
							case MOV_OBJ_STATUS_REACHED_TARGET_POINT:
								*mov_obj_status = MOV_OBJ_STATUS_OUT_OF_TRIAL;
								if (! write_to_mov_obj_status_history(mov_obj_status_history, current_time, MOV_OBJ_STATUS_OUT_OF_TRIAL))
									return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_status_history()");
								break;
							case MOV_OBJ_STATUS_RESETTING_TO_START_POINT:
								return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION & *mov_obj_status - MOV_OBJ_STATUS_RESETTING_TO_START_POINT");
							default: 
								return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION & *mov_obj_status - switch- default");							
						}
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH:
						if (! handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command(current_time, robot_pulse_history, mov_obj_paradigm->max_servo_angle_change,  mov_obj_paradigm->spike_count_threshold ))
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command");
						// Schedule adc conversion results reading and pulse width sending again.
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.schedule = current_time + mov_obj_paradigm->send_pw_command_wait_period;
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.item_idx = MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH;
						if (! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand, current_time,  MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE, mov_obj_hand_2_mov_obj_dur_hand_additional_data)) 
							print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer().");
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.schedule = current_time + mov_obj_paradigm->receive_position_wait_period;
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.item_idx = MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION;
						if (! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand, current_time,  MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE, mov_obj_hand_2_mov_obj_dur_hand_additional_data)) 
							print_message(BUG_MSG ,"MovObjHandler", "MovObjHandlerRtTask", "rt_mov_obj_handler", "write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer().");
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION:
						if (! handle_exp_envi_tx_shm_and_send_rs232_adc_command(current_time))
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! handle_exp_envi_tx_shm_and_send_rs232_adc_command()"); 
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.schedule = current_time + mov_obj_paradigm->send_pw_command_wait_period;
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.item_idx = MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH;
						if (! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand, current_time,  MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE, mov_obj_hand_2_mov_obj_dur_hand_additional_data)) 
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer()"); 
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.schedule = current_time + mov_obj_paradigm->receive_position_wait_period;
						mov_obj_hand_2_mov_obj_dur_hand_additional_data.schedule.item_idx = MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION;
						if (! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer(msgs_mov_obj_hand_2_mov_obj_dur_hand, current_time,  MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE, mov_obj_hand_2_mov_obj_dur_hand_additional_data)) 
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer()"); 
						break;
					case MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION:
						if (! handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm())
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! ! handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm()");
						calculate_forward_kinematics_with_averaging(robot_arm);	
						if (! handle_robot_arm_position_threshold(robot_arm, mov_obj_paradigm, mov_obj_status, current_time, msgs_mov_obj_hand_2_mov_obj_dur_hand, msgs_mov_obj_hand_2_trial_hand, mov_obj_status_history, msgs_mov_obj_hand_2_neural_net_multi_thread))
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! handle_robot_arm_position_threshold()");
						if (! write_to_three_dof_robot_angle_history(robot_angle_history, current_time, robot_arm->servos[BASE_SERVO].current_angle, robot_arm->servos[SHOULDER_SERVO].current_angle, robot_arm->servos[ELBOW_SERVO].current_angle))
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_three_dof_robot_angle_history()");

						target_coordinates = &(mov_obj_paradigm->target_info.cart_coordinates[mov_obj_paradigm->target_info.selected_position_idx]);
						tip_position = &(robot_arm->tip_position);
						distance_to_target = distance_btwn_two_points(&(robot_arm->tip_position), &(mov_obj_paradigm->target_info.cart_coordinates[mov_obj_paradigm->target_info.selected_position_idx]));
						if ((*mov_obj_status) == MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL)  // to be faster using, if instead of switch.
						{
							mov_obj_hand_2_neural_net_msg_add.binary_reward_add.target_idx = mov_obj_paradigm->target_info.selected_position_idx;
							if (distance_to_target < prev_distance_to_target)
								mov_obj_hand_2_neural_net_msg_add.binary_reward_add.reward = 1;
							else if (distance_to_target > prev_distance_to_target)
								mov_obj_hand_2_neural_net_msg_add.binary_reward_add.reward = -1;
							else
								mov_obj_hand_2_neural_net_msg_add.binary_reward_add.reward = 0;
							for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
							{
								if (! write_to_mov_obj_hand_2_neural_net_msg_buffer(msgs_mov_obj_hand_2_neural_net_multi_thread[i], current_time, MOV_OBJ_HAND_2_NEURAL_NET_MSG_REINFORCEMENT, mov_obj_hand_2_neural_net_msg_add))
									return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjDurHand2MovObjHandMsgs", "handle_mov_obj_dur_handler_to_mov_obj_handler_msg", "! write_to_mov_obj_hand_2_neural_net_msg_buffer()");
							}
						}
						prev_distance_to_target = distance_to_target;
						break;	
					default:
						return FALSE;
				}
				break;
			default:
				return FALSE;
		}
	}

	return TRUE;
}
