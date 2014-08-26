#include "HandleExpEnviHandDur.h"


bool handle_exp_envi_handler_duration(ExpEnviDurationStatus *exp_envi_inputs_min_dur_status, ExpEnviDurationStatus *exp_envi_inputs_max_dur_status, TimeStamp current_time, TimeStamp *inputs_handling_end_time_min, TimeStamp *inputs_handling_end_time_max, unsigned int num_of_input_components, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, unsigned int num_of_output_components, ExpEnviDurationStatus *exp_envi_outputs_dur_status, TimeStamp *outputs_handling_end_time)
{
	char str_dur_status[EXP_ENVI_DUR_STATUS_MAX_STRING_LENGTH];
	unsigned int i;
	for (i = 0; i < num_of_input_components; i ++)
	{
		switch (exp_envi_inputs_min_dur_status[i])
		{
			case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF:
				break;	// do nothing
			case EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_ON:
				if (current_time >= inputs_handling_end_time_min[i])
				{
					exp_envi_inputs_min_dur_status[i] = EXP_ENVI_INPUTS_MIN_DUR_STATUS_TIMER_OFF;	
					if (! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer(msgs_exp_envi_dur_hand_2_exp_envi_hand, current_time, EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_INPUT_TIMEOUT_FOR_MIN, i, 0))
						return print_message(ERROR_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", "! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer()."); 
				}	
				break;	// do nothing
			default: 
				get_exp_envi_dur_status_type_string(exp_envi_inputs_min_dur_status[i], str_dur_status);  
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", str_dur_status);		
		}
		switch (exp_envi_inputs_max_dur_status[i])
		{
			case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF:
				break;	// do nothing
			case EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_ON:
				if (current_time >= inputs_handling_end_time_max[i])
				{
					exp_envi_inputs_max_dur_status[i] = EXP_ENVI_INPUTS_MAX_DUR_STATUS_TIMER_OFF;	
					if (! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer(msgs_exp_envi_dur_hand_2_exp_envi_hand, current_time, EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_INPUT_TIMEOUT_FOR_MAX, i, 0))
						return print_message(ERROR_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", "! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer()."); 
				}	
				break;	// do nothing
			default: 
				get_exp_envi_dur_status_type_string(exp_envi_inputs_max_dur_status[i], str_dur_status);  
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", str_dur_status);
		}
	}

	for (i = 0; i < num_of_output_components; i ++)
	{
		switch (exp_envi_outputs_dur_status[i])
		{
			case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF:
				break;	// do nothing
			case EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_ON:
				if (current_time >= outputs_handling_end_time[i])
				{
					exp_envi_outputs_dur_status[i] = EXP_ENVI_OUTPUTS_DUR_STATUS_TIMER_OFF;	
					if (! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer(msgs_exp_envi_dur_hand_2_exp_envi_hand, current_time, EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSG_OUTPUT_TIMEOUT, i, 0))
						return print_message(ERROR_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", "! write_to_exp_envi_dur_hand_2_exp_envi_hand_msg_buffer()."); 
				}	
				break;	// do nothing
			default: 
				get_exp_envi_dur_status_type_string(exp_envi_outputs_dur_status[i], str_dur_status);  
				return print_message(BUG_MSG ,"ExpEnviHandler", "HandleExpEnviHandDur", "handle_exp_envi_handler_duration", str_dur_status);		
		}
	}

	return TRUE;
}
