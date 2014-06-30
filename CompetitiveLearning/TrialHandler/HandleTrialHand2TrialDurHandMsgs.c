#include "HandleTrialHand2TrialDurHandMsgs.h"


bool handle_trial_handler_to_trial_dur_handler_msg(TrialDurationStatus *trial_duration_status, TimeStamp current_time, TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand, TimeStamp *handling_end_time)
{
	TrialHand2TrialDurHandMsgItem msg_item;
	char str_trial_hand_msg[TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_STRING_LENGTH];
	char str_dur_status[TRIAL_DUR_STATUS_MAX_STRING_LENGTH];
	while (get_next_trial_hand_2_trial_dur_hand_msg_buffer_item(msgs_trial_hand_2_trial_dur_hand, &msg_item))
	{
		get_trial_hand_2_trial_dur_hand_msg_type_string(msg_item.msg_type, str_trial_hand_msg);
		print_message(INFO_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_trial_hand_msg);
		switch (msg_item.msg_type)
		{
			case TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_ENABLE_DURATION_HANDLING:	
				switch (*trial_duration_status)
				{
					case TRIAL_DUR_STATUS_HANDLING_ENABLED:
						print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_trial_hand_msg);
						get_trial_dur_status_type_string(*trial_duration_status, str_dur_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_dur_status);
					case TRIAL_DUR_STATUS_HANDLING_DISABLED:
						*trial_duration_status = TRIAL_DUR_STATUS_HANDLING_ENABLED;
						*handling_end_time = msg_item.additional_data;
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_trial_hand_msg);
						get_trial_dur_status_type_string(*trial_duration_status, str_dur_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_dur_status);
				}
				break;
			case TRIAL_HAND_2_TRIAL_DUR_HAND_MSG_DISABLE_DURATION_HANDLING:	
				switch (*trial_duration_status)
				{
					case TRIAL_DUR_STATUS_HANDLING_ENABLED:
						*trial_duration_status = TRIAL_DUR_STATUS_HANDLING_DISABLED;
						break;
					case TRIAL_DUR_STATUS_HANDLING_DISABLED:
						break;
					default:
						print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_trial_hand_msg);
						get_trial_dur_status_type_string(*trial_duration_status, str_dur_status);   
						return print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_dur_status);
				}
				break;
			default:
				return print_message(BUG_MSG ,"TrialHandler", "HandleTrialHand2TrialDurHandMsgs", "handle_trial_handler_to_trial_dur_handler_msg", str_trial_hand_msg);
		}
	}
	return TRUE;
}
