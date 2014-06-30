#include "HandleTrialHandDur.h"



bool handle_trial_handler_duration(TrialDurationStatus *trial_duration_status, TimeStamp current_time, TimeStamp handling_end_time, TrialDurHand2TrialHandMsg *msgs_trial_dur_hand_2_trial_hand)
{
	char str_dur_status[TRIAL_DUR_STATUS_MAX_STRING_LENGTH];
	switch (*trial_duration_status)
	{
		case TRIAL_DUR_STATUS_HANDLING_DISABLED:
			break;	// do nothing
		case TRIAL_DUR_STATUS_HANDLING_ENABLED:
			if (current_time >= handling_end_time)
			{
				*trial_duration_status = TRIAL_DUR_STATUS_HANDLING_DISABLED;	
				if (! write_to_trial_dur_hand_2_trial_hand_msg_buffer(msgs_trial_dur_hand_2_trial_hand, current_time, TRIAL_DUR_HAND_2_TRIAL_HAND_MSG_TIMEOUT, 0))
					return print_message(ERROR_MSG ,"TrialHandler", "HandleTrialHandDur", "handle_trial_handler_duration", "! write_to_trial_dur_hand_2_trial_hand_msg_buffer()."); 
			}	
			break;	// do nothing
		default: 
			get_trial_dur_status_type_string(*trial_duration_status, str_dur_status);  
			return print_message(BUG_MSG ,"TrialHandler", "HandleTrialHandDur", "handle_trial_handler_duration", str_dur_status);		
	}
	return TRUE;
}
