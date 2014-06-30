#ifndef GUI_H
#define GUI_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "GuiExpEnviHandler.h"
#include "../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.h"

// Functions
void create_gui_handler(RtTasksData *rt_tasks_data, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui, ExpEnviInputStatusHistory *exp_envi_input_status_history, ExpEnviOutputStatusHistory *exp_envi_output_status_history, ExpEnviData *exp_envi_data, ExpEnviHandParadigmRobotReach *exp_envi_paradigm);



#endif
