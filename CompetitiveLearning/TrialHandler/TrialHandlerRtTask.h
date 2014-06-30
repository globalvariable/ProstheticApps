#ifndef TRIAL_HANDLER_RT_TASK_H
#define TRIAL_HANDLER_RT_TASK_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "TrialDurationHandlerRtTask.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"
#include "../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialDurHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.h"
#include "../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuRecHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuRecHand.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "HandleGui2TrialHandMsgs.h"
#include "HandleTrialDurHand2TrialHandMsgs.h"
#include "HandleExpEnviHand2TrialHandMsgs.h"
#include "HandleMovObjHand2TrialHandMsgs.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatusHistory.h"
#include "HandleTrialHand2TrialDurHandMsgs.h"
#include "HandleTrialHandDur.h"
#include "../TaskConfig.h"

TimeStamp	*sys_time_ptr;

bool create_trial_handler_rt_thread(RtTasksData *rt_tasks_data, Gui2TrialHandMsg *msgs_gui_2_trial_hand, TrialHandParadigmRobotReach *paradigm, ClassifiedTrialHistory* classified_trial_history, TrialHand2GuiMsg *msgs_trial_hand_2_gui, TrialStatusHistory *trial_status_history);
bool kill_trial_handler_rt_thread(void);









#endif
