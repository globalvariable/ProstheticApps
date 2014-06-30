#include "HandleExpEnviDurHand2ExpEnviHandMsgs.h"


bool handle_exp_envi_dur_handler_to_exp_envi_handler_msg(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, ExpEnviHand2TrialHandMsg *msgs_exp_envi_hand_2_trial_hand, ExpEnviHandParadigmRobotReach *exp_envi_paradigm)
{
	bool has_response;
	ExpEnviDurHand2ExpEnviHandMsgItem msg_item;
	char str_exp_envi_dur_msg[EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_STRING_LENGTH];
	bool timer_cancellation_required;
	while (get_next_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer_item(msgs_exp_envi_dur_hand_2_exp_envi_hand, &msg_item))
	{
//		get_exp_envi_dur_hand_2_exp_envi_hand_msg_type_string(msg_item.msg_type, str_exp_envi_dur_msg);
//		print_message(INFO_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", str_exp_envi_dur_msg);
		switch (msg_item.msg_type)
		{
			case EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_INPUT_TIMEOUT_FOR_MIN:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						if (! time_out_success_for_input_comp(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]), &has_response))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "time_out_success_for_input_comp().");
						if (has_response)
						{
							print_message(INFO_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "IR_BEAM_IDX_IN_EXP_ENVI_DATA RESPONSE");
							if (! write_to_exp_envi_hand_2_trial_hand_msg_buffer(msgs_exp_envi_hand_2_trial_hand, current_time, EXP_ENVI_HAND_2_TRIAL_HAND_MSG_START_TRIAL_REQUEST, 0))
								return print_message(ERROR_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_trial_hand_msg_buffer().");
						} 
						break;			
					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						if (! time_out_success_for_input_comp(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &has_response))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "min_time_out_for_input_comp().");
						if (has_response)
						{
							print_message(INFO_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA RESPONSE");
							if (! write_to_exp_envi_hand_2_trial_hand_msg_buffer(msgs_exp_envi_hand_2_trial_hand, current_time, EXP_ENVI_HAND_2_TRIAL_HAND_MSG_RESTRAIN_RELEASE_REQ, 0))
								return print_message(ERROR_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_trial_hand_msg_buffer().");

						} 
						 break;					
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "switch (msg_item.inp_comp_num)");	
				}
				break;
			case EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_INPUT_TIMEOUT_FOR_MAX:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						reset_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]), &timer_cancellation_required);
						if (timer_cancellation_required)
						{
							// not required to cancel timer here. it cancels itself when there is a timeout.	
						}
						break;			
					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						reset_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &timer_cancellation_required);
						{
							// not required to cancel timer here. it cancels itself when there is a timeout.	
						}
						break;						
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "switch (msg_item.inp_comp_num)");
				}
				break;
			case EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT:
				switch (msg_item.comp_num)
				{
					case VALVE_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[VALVE_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;			
					case BUZZER_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[BUZZER_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;	
					case LEFT_LED_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;	
					case RIGHT_LED_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;	
					case GUIDE_LED_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;	
					case LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA:
						switch(exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status)
						{
							case EXP_ENVI_COMP_STATUS_LOW:
								break;  // do nothing, it is already set low before (maybe by handle_trial_hand_2_exp_envi_hand)
							case EXP_ENVI_COMP_STATUS_HIGH:
								exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
								break;
							default:
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT & switch(exp_envi_data->outp_comps[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status) - default");
						}
						break;	
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", "switch (msg_item.inp_comp_num)");
				}
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviDurHand2ExpEnviHandMsgs", "handle_exp_envi_dur_handler_to_exp_envi_handler_msg", str_exp_envi_dur_msg);
		}
	}
	return TRUE;
}
