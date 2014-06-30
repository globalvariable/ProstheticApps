#ifndef HANDLE_TRIAL_HAND_2_TRIAL_DUR_HAND_MSGS_H
#define HANDLE_TRIAL_HAND_2_TRIAL_DUR_HAND_MSGS_H




#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialDurStatus.h"


bool handle_trial_handler_to_trial_dur_handler_msg(TrialDurationStatus *trial_duration_status, TimeStamp current_time, TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand, TimeStamp *handling_end_time);




#endif
