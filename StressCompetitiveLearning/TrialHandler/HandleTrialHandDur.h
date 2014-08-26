#ifndef HANDLE_TRIAL_HAND_DUR_H
#define HANDLE_TRIAL_HAND_DUR_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialDurStatus.h"

bool handle_trial_handler_duration(TrialDurationStatus *trial_duration_status, TimeStamp current_time, TimeStamp handling_end_time, TrialDurHand2TrialHandMsg *msgs_trial_dur_hand_2_trial_hand);









#endif
