#ifndef GUI_TRIAL_HANDLER_H
#define GUI_TRIAL_HANDLER_H

#include "../../../ExperimentHandlers/Library/Messages/Gui2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "TrialHandlerRtTask.h"
#include "TrialHistory.h"
#include "DataHandling/DataHandling.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatusHistory.h"

bool create_trial_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2TrialHandMsg *msgs_gui_2_trial_hand, TrialHandParadigmRobotReach *trial_hand_paradigm, ClassifiedTrialHistory* classified_trial_history, TrialHand2GuiMsg *msgs_trial_hand_2_gui, TrialStatusHistory *trial_status_history);


#endif
