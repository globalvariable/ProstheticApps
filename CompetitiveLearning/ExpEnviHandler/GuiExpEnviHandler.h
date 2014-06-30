#ifndef GUI_EXP_ENVI_HANDLER_H
#define GUI_EXP_ENVI_HANDLER_H

#include "../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "DataHandling/DataHandling.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "ExpEnviHandParadigm.h"
#include "ExpEnviHandlerRtTask.h"

bool create_exp_envi_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui, ExpEnviInputStatusHistory *exp_envi_input_status_history, ExpEnviOutputStatusHistory *exp_envi_output_status_history, ExpEnviData *exp_envi_data, ExpEnviHandParadigmRobotReach *exp_envi_paradigm);


#endif
