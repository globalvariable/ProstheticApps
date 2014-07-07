#include "HandleTrialHand2ExpEnviHandMsgs.h"



bool handle_trial_handler_to_exp_envi_handler_msg(ExpEnviData *exp_envi_data, TimeStamp current_time, TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, ExpEnviHandParadigmRobotReach *exp_envi_paradigm, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui)
{
	TrialHand2ExpEnviHandMsgItem msg_item;
	char str_trial_hand_msg[TRIAL_HAND_2_EXP_ENVI_HAND_MSG_STRING_LENGTH];
	unsigned int recording_number;
	bool cancellation_required_for_status_timers;
	bool timer_restart_reqiured_for_low_status_timers, timer_restart_reqiured_for_high_status_timers;
	while (get_next_trial_hand_2_exp_envi_hand_msg_buffer_item(msgs_trial_hand_2_exp_envi_hand, &msg_item))
	{
		get_trial_hand_2_exp_envi_hand_msg_type_string(msg_item.msg_type, str_trial_hand_msg);
//		print_message(INFO_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", str_trial_hand_msg);
		switch (msg_item.msg_type)
		{
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_TRIAL:	


				exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, GUIDE_LED_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");

				exp_envi_paradigm->selected_target_led_component_list_idx = msg_item.additional_data;
				switch (exp_envi_paradigm->target_led_component_indexes_list[exp_envi_paradigm->selected_target_led_component_list_idx])
				{
					case  LEFT_LED_IDX_IN_EXP_ENVI_DATA:

						exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
						if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, LEFT_LED_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");

						break;
					case RIGHT_LED_IDX_IN_EXP_ENVI_DATA:

						exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
						if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, RIGHT_LED_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");

						break;
					default:
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_TRIAL - switch (exp_envi_paradigm.target_led_comp_idx) -default");	
				}

				if (exp_envi_paradigm->restrain_on)
				{
					exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}

				if (! enable_exp_envi_input_with_no_status_reset(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &timer_restart_reqiured_for_low_status_timers, &timer_restart_reqiured_for_high_status_timers))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "! enable_exp_envi_input_with_no_status_reset().");
				if (timer_restart_reqiured_for_low_status_timers & timer_restart_reqiured_for_high_status_timers)
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "(timer_restart_reqiured_for_low_status_timers & timer_restart_reqiured_for_high_status_timers).");
				if (timer_restart_reqiured_for_low_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_low_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_low_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				}
				if (timer_restart_reqiured_for_high_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				}
				
				disable_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]),  &cancellation_required_for_status_timers) ;
				if (cancellation_required_for_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}

				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_END_TRIAL:	

				switch (exp_envi_paradigm->target_led_component_indexes_list[exp_envi_paradigm->selected_target_led_component_list_idx])
				{
					case  LEFT_LED_IDX_IN_EXP_ENVI_DATA:
						exp_envi_data->outp_comp_types[LEFT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
						if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER, LEFT_LED_IDX_IN_EXP_ENVI_DATA, 0))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
						break;
					case  RIGHT_LED_IDX_IN_EXP_ENVI_DATA:
						exp_envi_data->outp_comp_types[RIGHT_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
						if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER, RIGHT_LED_IDX_IN_EXP_ENVI_DATA, 0))
							return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
						break;
							default: 
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "TRIAL_HAND_2_EXP_ENVI_HAND_MSG_END_TRIAL - switch (exp_envi_paradigm.target_led_comp_idx) -default");	
				}	
						
				exp_envi_data->outp_comp_types[GUIDE_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER, GUIDE_LED_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");


				exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER, GET_READY_LED_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");

				exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_LOW;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_OUTPUT_TIMER, LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA, 0))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");

				disable_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]),  &cancellation_required_for_status_timers) ;
				if (cancellation_required_for_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}

				break;

			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_RELEASE_REWARD:	// trial handler should send reward before ending trial.
				exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, VALVE_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[VALVE_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_RELEASE_PUNISHMENT:	
				exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, BUZZER_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[BUZZER_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_START_RECORDING:
				recording_number = msg_item.additional_data;
				if (! write_to_exp_envi_hand_2_gui_msg_buffer(msgs_exp_envi_hand_2_gui, current_time,  EXP_ENVI_HAND_2_GUI_MSG_START_RECORDING, recording_number))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "! write_to_exp_envi_hand_2_gui_msg_buffer(().");
				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_STOP_RECORDING:	
				recording_number = msg_item.additional_data;
				if (! write_to_exp_envi_hand_2_gui_msg_buffer(msgs_exp_envi_hand_2_gui, current_time,  EXP_ENVI_HAND_2_GUI_MSG_STOP_RECORDING, recording_number))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "! write_to_exp_envi_hand_2_gui_msg_buffer(().");
				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_CANCEL_RECORDING:	
				recording_number = msg_item.additional_data;
				if (! write_to_exp_envi_hand_2_gui_msg_buffer(msgs_exp_envi_hand_2_gui, current_time,  EXP_ENVI_HAND_2_GUI_MSG_CANCEL_RECORDING, recording_number))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "! write_to_exp_envi_hand_2_gui_msg_buffer(().");
				break;

			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_GET_READY_TO_TRIAL_START:	
				exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
				if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, GET_READY_LED_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[GET_READY_LED_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");


				if (exp_envi_paradigm->restrain_on)
				{
					exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].status = EXP_ENVI_COMP_STATUS_HIGH;
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_OUTPUT_TIMER, LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA, exp_envi_data->outp_comp_types[LEVER_SOLENOID_IDX_IN_EXP_ENVI_DATA].high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}

				if (! enable_exp_envi_input_with_no_status_reset(&(exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA]), &timer_restart_reqiured_for_low_status_timers, &timer_restart_reqiured_for_high_status_timers))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "! enable_exp_envi_input_with_no_status_reset().");
				if (timer_restart_reqiured_for_low_status_timers & timer_restart_reqiured_for_high_status_timers)
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "write_to_exp_envi_hand_2_gui_msg_buffer", "(timer_restart_reqiured_for_low_status_timers & timer_restart_reqiured_for_high_status_timers).");
				if (timer_restart_reqiured_for_low_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_low_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_low_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				}
				if (timer_restart_reqiured_for_high_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MIN_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.min_high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_START_INPUT_MAX_TIMER, RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA, exp_envi_data->inp_comp_types[RESTRAIN_SWITCH_IDX_IN_EXP_ENVI_DATA].constraints.max_high_status_duration + current_time))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", "! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
				}
				
				disable_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]),  &cancellation_required_for_status_timers) ;
				if (cancellation_required_for_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}

				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_ENABLE_INP_COMPS_W_STATUS_RESET:

				if (! enable_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]), &cancellation_required_for_status_timers))
					return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "! enable_exp_envi_input_with_status_reset).");
				if (cancellation_required_for_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");	
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"BMIExpController", "HandleRS232Buffers", "handle_exp_envi_rx_shm", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");					
				}

				break;
			case TRIAL_HAND_2_EXP_ENVI_HAND_MSG_DISABLE_NOSE_POKE:
				disable_exp_envi_input_with_status_reset(&(exp_envi_data->inp_comp_types[IR_BEAM_IDX_IN_EXP_ENVI_DATA]),  &cancellation_required_for_status_timers) ;
				if (cancellation_required_for_status_timers)
				{
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MIN_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
					if (! write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer(msgs_exp_envi_hand_2_exp_envi_dur_hand, current_time,  EXP_ENVI_HAND_2_EXP_ENVI_DUR_HAND_MSG_CANCEL_INPUT_MAX_TIMER, IR_BEAM_IDX_IN_EXP_ENVI_DATA, 0))
						return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_exp_envi_interf_to_exp_envi_handler_msg", "write_to_exp_envi_hand_2_exp_envi_dur_hand_msg_buffer().");
				}
				break;
			default:
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleTrialHand2ExpEnviHandMsgs", "handle_trial_handler_to_exp_envi_handler_msg", str_trial_hand_msg);
		}
	}
	return TRUE;
}
