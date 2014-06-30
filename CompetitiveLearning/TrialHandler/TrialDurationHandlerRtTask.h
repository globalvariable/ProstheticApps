#ifndef TRIAL_DURATION_HANDLER_RT_TASK_H
#define TRIAL_DURATION_HANDLER_RT_TASK_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.h"
#include "../../../ExperimentHandlers/Library/Status/TrialDurStatus.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "HandleTrialHand2TrialDurHandMsgs.h"
#include "HandleTrialHandDur.h"
#include "TrialHandlerRtTask.h"

bool create_trial_duration_handler_rt_thread(RtTasksData *rt_tasks_data, TrialDurHand2TrialHandMsg *msgs_trial_dur_hand_2_trial_hand, TrialHand2TrialDurHandMsg *msgs_trial_hand_2_trial_dur_hand);
bool kill_trial_duration_handler_rt_thread(void);









#endif
