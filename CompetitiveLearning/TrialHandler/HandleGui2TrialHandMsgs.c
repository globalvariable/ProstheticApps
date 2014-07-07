#include "HandleGui2TrialHandMsgs.h"


static TrialStatus *trial_status = NULL;
static Gui2TrialHandMsg *msgs_gui_2_trial_hand = NULL;
static TrialHand2TrialDurHandMsg* msgs_trial_hand_2_trial_dur_hand = NULL;
static TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand = NULL;
static TrialHand2MovObjHandMsg *msgs_trial_hand_2_mov_obj_hand = NULL;
static TrialHand2NeuralNetMsg *msgs_trial_hand_2_neural_net = NULL;

static TrialHandParadigmRobotReach *paradigm = NULL;
static ClassifiedTrialHistory* classified_history = NULL;
static TrialHand2GuiMsg *msgs_trial_hand_2_gui = NULL;
static TrialStatusHistory *trial_status_history = NULL;



bool handle_gui_to_trial_handler_msg(TimeStamp current_time)
{
	Gui2TrialHandMsgItem msg_item;
	char str_gui_msg[GUI_2_TRIAL_HAND_MSG_STRING_LENGTH];
	char str_status[TRIAL_STATUS_MAX_STRING_LENGTH];
	TrialHand2NeuralNetMsgAdditional trial_hand_to_neural_net_msg_add;

	TrialHand2MovObjHandMsgAdditional trial_hand_2_mov_obj_hand_add;
	unsigned int recording_number;
	TrialData *prev_trial_w_type;

	while (get_next_gui_2_trial_hand_msg_buffer_item(msgs_gui_2_trial_hand, &msg_item))
	{
		get_gui_2_trial_hand_msg_type_string(msg_item.msg_type, str_gui_msg);
//		print_message(INFO_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
		switch (msg_item.msg_type)
		{
			case GUI_2_TRIAL_HAND_MSG_ENABLE_TRIAL_HANDLING:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						*trial_status = TRIAL_STATUS_IN_REFRACTORY;
						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING, current_time + paradigm->trial_refractory))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");

						if (paradigm->current_trial_data.auto_target_select_mode_on)
							paradigm->current_trial_data.robot_target_position_idx = (unsigned int)(paradigm->num_of_robot_target_positions * get_rand_number());   ///  Bunu trial bittiginde yap.
						else
							paradigm->current_trial_data.robot_target_position_idx = paradigm->current_trial_data.gui_selected_target_position_idx;

						paradigm->current_trial_data.target_led_component_list_idx = paradigm->current_trial_data.robot_target_position_idx;

						if ((paradigm->current_trial_data.robot_target_position_idx) == 0)   /// LEFT TARGET
						{
							trial_hand_2_mov_obj_hand_add.robot_start_position_idx = paradigm->current_trial_data.robot_start_position_idx; 
						}
						else		// RIGHT TARGET
						{
							trial_hand_2_mov_obj_hand_add.robot_start_position_idx = paradigm->num_of_robot_start_positions - 1 - paradigm->current_trial_data.robot_start_position_idx;
						}

						if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_TRIALS_ENABLED, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");
						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_trial_status = TRIAL_STATUS_IN_REFRACTORY;
						trial_hand_to_neural_net_msg_add.trial_status_change_msg_add.new_robot_target_position_idx = 0;
						if (!write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_TRIAL_STATUS_CHANGED, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");

						if (!write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_TRIAL_STATUS_CHANGE, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						if (! write_to_trial_status_history(trial_status_history, current_time, TRIAL_STATUS_IN_REFRACTORY))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_status_history()");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Enable is not possible when TRIAL_STATUS_IN_TRIAL. It must be Enabled already.");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Enable is not possible when TRIAL_STATUS_IN_REFRACTORY. It must be Enabled already.");
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Enable is not possible when TRIAL_STATUS_START_TRIAL_AVAILABLE. It must be Enabled already.");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Enable is not possible when TRIAL_STATUS_GET_READY_TO_START. It must be Enabled already.");
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_DISABLE_TRIAL_HANDLING:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Disable is not possible when TRIAL_STATUS_TRIALS_DISABLED.");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Disable is not possible when TRIAL_STATUS_IN_TRIAL.");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						*trial_status = TRIAL_STATUS_TRIALS_DISABLED;
						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_DISABLE_DURATION_HANDLING, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.dummy = 0;
						if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_TRIALS_DISABLED, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");
						print_message(INFO_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Successfully disabled trials.");
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						*trial_status = TRIAL_STATUS_TRIALS_DISABLED;
						trial_hand_2_mov_obj_hand_add.dummy = 0;
						if (!write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_TRIALS_DISABLED, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_neural_net_msg_buffer()");
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_DISABLE_NOSE_POKE, 0))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						print_message(INFO_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Successfully disabled trials.");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Trial Disable is not possible when TRIAL_STATUS_GET_READY_TO_START.");
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_QUIT:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						break;
					case TRIAL_STATUS_IN_TRIAL:
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_INCREASE_ROBOT_START_POSITION_IDX:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						if ((paradigm->current_trial_data.robot_start_position_idx +1 ) > (paradigm->num_of_robot_start_positions/2))
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->num_of_robot_start_positions number. Cannot be increase  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx++;
						}
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Increasing threshold cannot be done during trial");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Increasing threshold cannot be done during TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						if ((paradigm->current_trial_data.robot_start_position_idx +1 ) > (paradigm->num_of_robot_start_positions/2))
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->num_of_robot_start_positions number. Cannot be increase  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx++;
						}
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						if ((paradigm->current_trial_data.robot_start_position_idx +1 ) > (paradigm->num_of_robot_start_positions/2))
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->num_of_robot_start_positions number. Cannot be increase  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx++;
						}
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_DECREASE_ROBOT_START_POSITION_IDX:	
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						if (paradigm->current_trial_data.robot_start_position_idx == 0 )
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->current_trial_data.robot_start_position_idx == 1. Cannot  decrease  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx--;
						}
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Decreasing threshold cannot be done during trial");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Decreasing threshold cannot be done during TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						if (paradigm->current_trial_data.robot_start_position_idx == 0 )
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->current_trial_data.robot_start_position_idx == 1. Cannot  decrease  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx--;
						}
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						if (paradigm->current_trial_data.robot_start_position_idx == 0 )
						{
//							print_message(WARNING_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Reached paradigm->current_trial_data.robot_start_position_idx == 1. Cannot  decrease  more.");
						}
						else
						{
							paradigm->current_trial_data.robot_start_position_idx--;
						}
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_SELECT_TARGET:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						paradigm->current_trial_data.gui_selected_target_position_idx = msg_item.additional_data;
						break;
					case TRIAL_STATUS_IN_TRIAL:
						paradigm->current_trial_data.gui_selected_target_position_idx = msg_item.additional_data;
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						paradigm->current_trial_data.gui_selected_target_position_idx = msg_item.additional_data;
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						paradigm->current_trial_data.gui_selected_target_position_idx = msg_item.additional_data;
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						paradigm->current_trial_data.gui_selected_target_position_idx = msg_item.additional_data;
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_AUTO_TARGET_SELECTION_OFF:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						paradigm->current_trial_data.auto_target_select_mode_on = FALSE;
						break;
					case TRIAL_STATUS_IN_TRIAL:
						paradigm->current_trial_data.auto_target_select_mode_on = FALSE;
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						paradigm->current_trial_data.auto_target_select_mode_on = FALSE;
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						paradigm->current_trial_data.auto_target_select_mode_on = FALSE;
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						paradigm->current_trial_data.auto_target_select_mode_on = FALSE;
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_AUTO_TARGET_SELECTION_ON:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						paradigm->current_trial_data.auto_target_select_mode_on = TRUE;
						break;
					case TRIAL_STATUS_IN_TRIAL:
						paradigm->current_trial_data.auto_target_select_mode_on = TRUE;
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						paradigm->current_trial_data.auto_target_select_mode_on = TRUE;
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						paradigm->current_trial_data.auto_target_select_mode_on = TRUE;
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						paradigm->current_trial_data.auto_target_select_mode_on = TRUE;
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_BROADCAST_START_RECORDING:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot start recording when TRIAL_STATUS_TRIALS_DISABLED");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot start recording when TRIAL_STATUS_IN_TRIAL");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot start recording when TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						recording_number = classified_history->all_trials->buff_write_idx;  // delete previous 
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_START_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_START_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_START_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						recording_number = classified_history->all_trials->buff_write_idx; 
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_START_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_START_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_START_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_BROADCAST_STOP_RECORDING:		
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot stop recording when TRIAL_STATUS_TRIALS_DISABLED");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot stop recording when TRIAL_STATUS_IN_TRIAL");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot stop recording when TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						if (classified_history->all_trials->buff_write_idx == 0)
						{
							print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "No trial finished to be saved");
							break;
						}
						recording_number = classified_history->all_trials->buff_write_idx-1;  // delete previous 
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_STOP_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_STOP_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_STOP_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;				
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						if (classified_history->all_trials->buff_write_idx == 0)
						{
							print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "No trial finished to be saved");
							break;
						}
						recording_number = classified_history->all_trials->buff_write_idx-1;  // delete previous 
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_STOP_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_STOP_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_STOP_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_STOP_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;			
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_BROADCAST_CANCEL_RECORDING:
				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot cancel recording when TRIAL_STATUS_TRIALS_DISABLED");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot cancel recording when TRIAL_STATUS_IN_TRIAL");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot cancel recording when TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						recording_number = classified_history->all_trials->buff_write_idx;  // delete the recording for current trial   //  history buffer write idx is inceremented when trial ends by TrialHandler rt thread funcitons.
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_CANCEL_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_CANCEL_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_CANCEL_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_CANCEL_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;				
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						recording_number = classified_history->all_trials->buff_write_idx;  // delete the recording for current trial   //  history buffer write idx is inceremented when trial ends by TrialHandler rt thread funcitons.
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_CANCEL_RECORDING, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_2_mov_obj_hand_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_mov_obj_hand_msg_buffer(msgs_trial_hand_2_mov_obj_hand, current_time, TRIAL_HAND_2_MOV_OBJ_HAND_MSG_CANCEL_RECORDING, trial_hand_2_mov_obj_hand_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
						trial_hand_to_neural_net_msg_add.recording_number = recording_number;
						if (! write_to_trial_hand_2_neural_net_msg_buffer(msgs_trial_hand_2_neural_net, current_time, TRIAL_HAND_2_NEURAL_NET_MSG_CANCEL_RECORDING, trial_hand_to_neural_net_msg_add))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						if (! write_to_trial_hand_2_gui_msg_buffer(msgs_trial_hand_2_gui, current_time, TRIAL_HAND_2_GUI_MSG_BROADCAST_CANCEL_RECORDING_MSG_ACK, recording_number))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_gui_msg_buffer()");
						break;			
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}
				break;
			case GUI_2_TRIAL_HAND_MSG_RELEASE_REWARD:
				if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_RELEASE_REWARD, 0))
					return print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");
				break;

			case GUI_2_TRIAL_HAND_MSG_START_TRIAL_REQUEST:    // used for simulaitons.

				switch (*trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot START_TRIAL_REQUEST when TRIAL_STATUS_TRIALS_DISABLED");
						break;
					case TRIAL_STATUS_IN_TRIAL:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot START_TRIAL_REQUEST when TRIAL_STATUS_IN_TRIAL");
						break;
					case TRIAL_STATUS_GET_READY_TO_START:	
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot START_TRIAL_REQUEST when TRIAL_STATUS_GET_READY_TO_START");
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						print_message(ERROR_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", "Cannot START_TRIAL_REQUEST when TRIAL_STATUS_IN_REFRACTORY");
						break;		
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						*trial_status = TRIAL_STATUS_GET_READY_TO_START;
						paradigm->current_trial_data.trial_start_time = current_time;

						if (!write_to_trial_hand_2_trial_dur_hand_msg_buffer(msgs_trial_hand_2_trial_dur_hand, current_time, TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING, current_time + paradigm->get_ready_to_trial_start_length))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleExpEnviHand2TrialHandMsgs", "handle_exp_envi_handler_to_trial_handler_msg", "write_to_trial_hand_2_trial_dur_hand_msg_buffer()");
						if (!write_to_trial_hand_2_exp_envi_hand_msg_buffer(msgs_trial_hand_2_exp_envi_hand, current_time, TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_TRIAL, paradigm->current_trial_data.target_led_component_list_idx))
							return print_message(ERROR_MSG ,"TrialHandler", "HandleTrialDurHand2TrialHandMsgs", "handle_trial_dur_handler_to_trial_handler_msg", "write_to_trial_hand_2_exp_envi_hand_msg_buffer()");

						prev_trial_w_type = get_previous_trial_history_data_ptr(classified_history->trial_types[paradigm->current_trial_data.robot_target_position_idx]);

						if (prev_trial_w_type->binary_reward)
						{
							paradigm->current_trial_data.reward_prediction = ((1.0-(1.0/REWARD_PREDICTION_WINDOW) ) * prev_trial_w_type->reward_prediction) + (1.0/REWARD_PREDICTION_WINDOW);
							printf ("%f\t%f\t%f\n", paradigm->current_trial_data.reward_prediction, prev_trial_w_type->reward_prediction, averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) );
						}
						else
						{
							if (averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) < 0.5)  // if 3 out of last 4 trials failed
								paradigm->current_trial_data.reward_prediction = (1.0-(1.0/REWARD_PREDICTION_WINDOW) ) * prev_trial_w_type->reward_prediction;
							printf ("%f\t%f\t%f\n", paradigm->current_trial_data.reward_prediction, prev_trial_w_type->reward_prediction, averaging_struct_get_mean(paradigm->target_success_average_small[paradigm->current_trial_data.robot_target_position_idx]) );
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
						print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);
						get_trial_status_type_string(*trial_status, str_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_status);
				}

				break;

			default:
				return print_message(BUG_MSG ,"TrialHandler", "HandleGui2TrialHandMsgs", "handle_gui_to_trial_handler_msg", str_gui_msg);	
		}
	}
	return TRUE;
}

void initialize_gui_to_trial_handler_msg_params(TrialStatus *arg_trial_status, Gui2TrialHandMsg *arg_msgs_gui_2_trial_hand, TrialHand2TrialDurHandMsg* arg_msgs_trial_hand_2_trial_dur_hand, TrialHand2ExpEnviHandMsg *arg_msgs_trial_hand_2_exp_envi_hand, TrialHand2MovObjHandMsg *arg_msgs_trial_hand_2_mov_obj_hand, TrialHand2NeuralNetMsg *arg_msgs_trial_hand_2_neural_net, TrialHandParadigmRobotReach *arg_paradigm, ClassifiedTrialHistory* arg_classified_history, TrialHand2GuiMsg *arg_msgs_trial_hand_2_gui, TrialStatusHistory *arg_trial_status_history)

{
	trial_status = arg_trial_status;
	msgs_gui_2_trial_hand = arg_msgs_gui_2_trial_hand;
	msgs_trial_hand_2_trial_dur_hand = arg_msgs_trial_hand_2_trial_dur_hand;
	msgs_trial_hand_2_exp_envi_hand = arg_msgs_trial_hand_2_exp_envi_hand;
	msgs_trial_hand_2_mov_obj_hand = arg_msgs_trial_hand_2_mov_obj_hand;
	msgs_trial_hand_2_neural_net = arg_msgs_trial_hand_2_neural_net;

	paradigm = arg_paradigm;
	classified_history = arg_classified_history;
	msgs_trial_hand_2_gui = arg_msgs_trial_hand_2_gui;
	trial_status_history = arg_trial_status_history;
}
