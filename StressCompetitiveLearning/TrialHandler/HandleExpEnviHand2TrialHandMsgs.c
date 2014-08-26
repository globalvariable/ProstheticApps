#include "HandleExpEnviHand2TrialHandMsgs.h"


static TrialStatus *trial_status = NULL;
static ExpEnviHand2TrialHandMsg *msgs_exp_envi_hand_2_trial_hand = NULL;
static TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand = NULL;
static TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand = NULL;
static TrialHand2MovObjHandMsg *msgs_trial_hand_2_mov_obj_hand = NULL;
static TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;
static TrialHandParadigmRobotReach *paradigm = NULL;
static ClassifiedTrialHistory* classified_history = NULL;
static TrialHand2GuiMsg *msgs_trial_hand_2_gui = NULL;
static TrialStatusHistory *trial_status_history = NULL;


bool handle_exp_envi_handler_to_trial_handler_msg(TimeStamp current_time)
{
	ExpEnviHand2TrialHandMsgItem msg_item;
	char str_exp_envi_msg[EXP_ENVI_HAND_2_TRIAL_HAND_MSG_STRING_LENGTH];
	char str_status[TRIAL_STATUS_MAX_STRING_LENGTH];
	TrialHand2MovObjHandMsgAdditional trial_hand_2_mov_obj_hand_add;
	TimeStamp trial_length;
	TrialHand2NeuralNetMsgAdditional trial_hand_to_neural_net_msg_add;
	TrialData *prev_trial_w_type;

	while (get_next_exp_envi_hand_2_trial_hand_msg_buffer_item(msgs_exp_envi_hand_2_trial_hand, &msg_item))
	{
		get_exp_envi_hand_2_trial_hand_msg_type_string(msg_item.msg_type, str_exp_envi_msg);
//		print_message(INFO_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_exp_envi_msg);
		switch (msg_item.msg_type)
		{
			case EXP_ENVI_HAND_2_TRIAL_HAND_MSG_START_TRIAL_REQUEST:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						break;   // do nothing
					case TRIAL_STATUS_IN_TRIAL:
						break;   // do nothing
					case TRIAL_STATUS_IN_REFRACTORY:
						break;   // do nothing
					case TRIAL_STATUS_GET_READY_TO_START:	
						break;   // do nothing
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						*trial_status = TRIAL_STATUS_GET_READY_TO_START;
						paradigm->current_trial_data.trial_start_time = current_time;

						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING, current_time + paradigm->get_ready_to_trial_start_length))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_TRIAL, paradigm->current_trial_data.target_led_component_list_idx))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleTrialDurHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");


						prev_trial_w_type = get_previous_trial_history_data_ptr(classified_history->trial_types[paradigm->current_trial_data.robot_target_position_idx]);

						if (prev_trial_w_type->binary_reward)
						{
							paradigm->current_trial_data.reward_prediction = ((1.0-(1.0/REWARD_PREDICTION_WINDOW) ) * prev_trial_w_type->reward_prediction) + (1.0/REWARD_PREDICTION_WINDOW);
//							printf ("%f\t%f\t%f\n", paradigm->current_trial_data.reward_prediction, prev_trial_w_type->reward_prediction, averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) );
						}
						else
						{
//							UNCOMMENT THE FOLLOWING LINE FOR IN VIVO EXPERIMENTS
//							if (averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) < 0.5)  // if 3 out of last 4 trials failed
								paradigm->current_trial_data.reward_prediction = (1.0-(1.0/REWARD_PREDICTION_WINDOW) ) * prev_trial_w_type->reward_prediction;
//							printf ("%f\t%f\t%f\n", paradigm->current_trial_data.reward_prediction, prev_trial_w_type->reward_prediction, averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) );
						}

						trial_hand_to_neural_net_msg_add.difficulty_reward_predict_add.difficulty_level = paradigm->current_trial_data.difficulty_level;  // determined when trial ends according to robot start position
						trial_hand_to_neural_net_msg_add.difficulty_reward_predict_add.reward_prediction = paradigm->current_trial_data.reward_prediction;

						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_START, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						if (!write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_TRIAL_STATUS_CHANGE, TRIAL_STATUS_GET_READY_TO_START))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						if (! write_to_trial_status_history(trial_status_history, current_time, TRIAL_STATUS_GET_READY_TO_START))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_status_history()");


						break; 
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_exp_envi_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_status);
				}
				break;
			case EXP_ENVI_HAND_2_TRIAL_HAND_MSG_RESTRAIN_RELEASE_REQ:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						break;   // do nothing
					case TRIAL_STATUS_IN_TRIAL:
						*trial_status = TRIAL_STATUS_IN_REFRACTORY;

						trial_length = current_time - paradigm->current_trial_data.trial_start_time;
						paradigm->current_trial_data.trial_end_time = current_time;
						paradigm->current_trial_data.trial_length = trial_length;
						paradigm->current_trial_data.binary_reward = FALSE;
						paradigm->current_trial_data.trial_incomplete = TRUE;

						if (! write_trial_data_to_classified_trial_history(classified_history, &(paradigm->current_trial_data)))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_trial_data_to_classified_trial_history()");

						write_to_averaging_struct(paradigm->target_success_average[paradigm->current_trial_data.robot_target_position_idx], 0.0);
						write_to_averaging_struct(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx], 0.0);
						write_to_averaging_struct(paradigm->all_success_average, 0.0);
						
						paradigm->current_trial_data.trial_start_time = 0;
						paradigm->current_trial_data.trial_end_time = 0;
						paradigm->current_trial_data.trial_length = 0;

						if (paradigm->current_trial_data.auto_target_select_mode_on)
							paradigm->current_trial_data.robot_target_position_idx = (unsigned int)(paradigm->num_of_robot_target_positions * get_rand_number());   ///  Bunu trial bittiginde yap.
						else
							paradigm->current_trial_data.robot_target_position_idx = paradigm->current_trial_data.gui_selected_target_position_idx;

						paradigm->current_trial_data.target_led_component_list_idx = paradigm->current_trial_data.robot_target_position_idx;
					
						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_DISABLE_DURATION_HANDLING, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");

						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING, current_time + paradigm->trial_refractory))
								return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");

						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_END_TRIAL, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgss", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
/*						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_RELEASE_PUNISHMENT, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
*/
						if ((paradigm->current_trial_data.robot_target_position_idx) == 0)   /// LEFT TARGET
						{
							trial_hand_2_mov_obj_hand_add.robot_start_position_idx = paradigm->current_trial_data.robot_start_position_idx; 
						}
						else		// RIGHT TARGET
						{
							trial_hand_2_mov_obj_hand_add.robot_start_position_idx = paradigm->num_of_robot_start_positions - 1 - paradigm->current_trial_data.robot_start_position_idx;
						}

						paradigm->current_trial_data.difficulty_level = paradigm->current_trial_data.robot_start_position_idx;  ///   paradigm->current_trial_data.robot_start_position_idx is minimum 1

						if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_END_TRIAL, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleTrialDurHand2TrialHandMsgss", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_mov_obj_hand_msg_buffer()");
						if (!write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_TRIAL_STATUS_CHANGE, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						if (! write_to_trial_status_history(trial_status_history, current_time, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_status_history()");

						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_trial_status = TRIAL_STATUS_IN_REFRACTORY;
						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_robot_start_position_idx = paradigm->current_trial_data.robot_start_position_idx;
						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleMovObjHand2TrialHandMsgs", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						trial_hand_to_neural_net_msg_add.dummy = 0;
						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_END_TRIAL_WITH_NOTHING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleMovObjHand2TrialHandMsgs", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						break;  
					case TRIAL_STATUS_IN_REFRACTORY:
						break;   // do nothing
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						break;   // do nothing
					case TRIAL_STATUS_GET_READY_TO_START:	
						*trial_status = TRIAL_STATUS_IN_REFRACTORY;

						trial_length = current_time - paradigm->current_trial_data.trial_start_time;
						paradigm->current_trial_data.trial_end_time = current_time;
						paradigm->current_trial_data.trial_length = trial_length;
						paradigm->current_trial_data.binary_reward = FALSE;
						paradigm->current_trial_data.trial_incomplete = TRUE;

						if (! write_trial_data_to_classified_trial_history(classified_history, &(paradigm->current_trial_data)))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_trial_data_to_classified_trial_history()");

						write_to_averaging_struct(paradigm->target_success_average[paradigm->current_trial_data.robot_target_position_idx], 0.0);
						write_to_averaging_struct(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx], 0.0);
						write_to_averaging_struct(paradigm->all_success_average, 0.0);
						
						paradigm->current_trial_data.trial_start_time = 0;
						paradigm->current_trial_data.trial_end_time = 0;
						paradigm->current_trial_data.trial_length = 0;

					
						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_DISABLE_DURATION_HANDLING, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");

						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING, current_time + paradigm->trial_refractory))
								return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");

						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_END_TRIAL, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgss", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
/*						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_RELEASE_PUNISHMENT, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
*/
/*						trial_hand_to_mov_obj_hand_msg_add.robot_start_position_idx = paradigm->current_trial_data.robot_start_position_idx;
						if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_END_TRIAL, trial_hand_to_mov_obj_hand_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleTrialDurHand2TrialHandMsgss", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_mov_obj_hand_msg_buffer()");
*/						if (!write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_TRIAL_STATUS_CHANGE, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_bufferr()");
						if (! write_to_trial_status_history(trial_status_history, current_time, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_status_history()");

						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_trial_status = TRIAL_STATUS_IN_REFRACTORY;
						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_robot_start_position_idx = paradigm->current_trial_data.robot_start_position_idx;
						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleMovObjHand2TrialHandMsgs", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						trial_hand_to_neural_net_msg_add.dummy = 0;
						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_END_TRIAL_WITH_NOTHING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleMovObjHand2TrialHandMsgs", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						break;   // do nothing

					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_exp_envi_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_status);
				}
				break;


			default:
				return print_message(BUG_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", str_exp_envi_msg);	
		}
	}
	return TRUE;
}


void initialize_exp_envi_handler_to_trial_handler_msg_params(TrialStatus *arg_trial_status, ExpEnviHand2TrialHandMsg *arg_msgs_exp_envi_hand_2_trial_hand, TrialHand2TrialDurHandMsg *arg_msgs_trial_hand_2_trial_dur_hand, TrialHand2ExpEnviHandMsg *arg_msgs_trial_hand_2_exp_envi_hand, TrialHand2MovObjHandMsg *arg_msgs_trial_hand_2_mov_obj_hand, TrialHand2NeuralNetMsg *arg_msgs_trial_hand_2_neural_net, TrialHandParadigmRobotReach *arg_paradigm, ClassifiedTrialHistory *arg_classified_history, TrialHand2GuiMsg *arg_msgs_trial_hand_2_gui, TrialStatusHistory *arg_trial_status_history)
{
	trial_status = arg_trial_status;
	msgs_exp_envi_hand_2_trial_hand = arg_msgs_exp_envi_hand_2_trial_hand;
	msgs_trial_hand_2_trial_dur_hand = arg_msgs_trial_hand_2_trial_dur_hand ;
	msgs_trial_hand_2_exp_envi_hand = arg_msgs_trial_hand_2_exp_envi_hand;
	msgs_trial_hand_2_mov_obj_hand = arg_msgs_trial_hand_2_mov_obj_hand;
	msgs_trial_hand_2_neural_net = arg_msgs_trial_hand_2_neural_net ;
	paradigm = arg_paradigm;
	classified_history = arg_classified_history;
	msgs_trial_hand_2_gui = arg_msgs_trial_hand_2_gui;
	trial_status_history = arg_trial_status_history;
}

