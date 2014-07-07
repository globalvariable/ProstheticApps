#include "HandleExpEnviHand2ExpEnviDurHandMsgs.h"

bool handle_exp_envi_handler_to_exp_envi_dur_handler_msg(ExpEnviDurationStatus *exp_envi_inputs_min_dur_status, ExpEnviDurationStatus *exp_envi_inputs_max_dur_status, TimeStamp current_time, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, TimeStamp *inputs_handling_end_time_min, TimeStamp *inputs_handling_end_time_max, ExpEnviDurationStatus *exp_envi_outputs_dur_status, TimeStamp *outputs_handling_end_time)
{
	ExpEnviHand2ExpEnviDurHandMsgItem msg_item;
	char str_exp_envi_hand_msg[EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_STRING_LENGTH];
	char str_exp_envi_dur_status[EXP_ENVI_DUR_STATUS_MAX_STRING_LENGTH];
	while (get_next_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer_item(msgs_exp_envi_hand_2_exp_envi_dur_hand, &msg_item))
	{
		get_exp_envi_hand_2_exp_envi_dur_hand_msg_type_string(msg_item.msg_type, str_exp_envi_hand_msg);
//		print_message(INFO_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_hand_msg);
		switch (msg_item.msg_type)
		{
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "IR_BEAM_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);   
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF:
								inputs_handling_end_time_min[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			
					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);   
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF:
								inputs_handling_end_time_min[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	

					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON:
								exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF:
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			
					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON:
								exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF:
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
		
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "IR_BEAM_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);   
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF:
								inputs_handling_end_time_max[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			

					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);   
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF:
								inputs_handling_end_time_max[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;				
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER:	
				switch (msg_item.comp_num)
				{
					case IR_BEAM_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON:
								exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF:
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[IR_BEAM_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			

					case RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON:
								exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF:
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;		
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER:	 // "send current time" to cancel out the timer which is set before 
				switch (msg_item.comp_num)
				{
					case VALVE_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "VALVE_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[VALVE_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								printf("VALVE_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			
					case BUZZER_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "BUZZER_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[BUZZER_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								printf("BUZZER_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case LEFT_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "LEFT_LED_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[LEFT_LED_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								printf("LEFT_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case RIGHT_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "RIGHT_LED_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[RIGHT_LED_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								printf("RIGHT_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case GUIDE_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "GUIDE_LED_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[GUIDE_LED_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								printf("GUIDE_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			
					case LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case GET_READY_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "GET_READY_LED_IDX_IN_EXP_ENVI_DATA");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
								outputs_handling_end_time[GET_READY_LED_IDX_IN_EXP_ENVI_DATA] = msg_item.additional_data;
								exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON;
								break;			
							default:
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status); 
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			case EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER:	
				switch (msg_item.comp_num)
				{
					case VALVE_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for VALVE_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								printf("VALVE_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[VALVE_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;			
					case BUZZER_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for BUZZER_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								printf("BUZZER_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[BUZZER_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
					case LEFT_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for LEFT_LED_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								printf("LEFT_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEFT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;		
					case RIGHT_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for RIGHT_LED_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								printf("RIGHT_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[RIGHT_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;		
					case GUIDE_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for GUIDE_LED_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								printf("GUIDE_LED_IDX_IN_EXP_ENVI_DATA\n");
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GUIDE_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for LEVER_SOLENOID_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					case GET_READY_LED_IDX_IN_EXP_ENVI_DATA:
						switch (exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA])
						{
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
								exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;
								break;			
							case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
//								print_message(WARNING_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "Unnecessary Cancel Output Timer for GET_READY_LED_IDX_IN_EXP_ENVI_DATA");
								break; // do nothing
							default:
								get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[GET_READY_LED_IDX_IN_EXP_ENVI_DATA], str_exp_envi_dur_status);  
								return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_dur_status);
						}
						 break;	
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", "msg_item.inp_comp_num");	
				}
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHand2ExpEnviDurHandMsgs", "handle_exp_envi_handler_to_exp_envi_dur_handler_msg", str_exp_envi_hand_msg);
		}
	}
	return TRUE;
}
