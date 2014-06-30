#ifndef HAND_GUI_2_EXP_ENVI_HAND_MSGS_H
#define HAND_GUI_2_EXP_ENVI_HAND_MSGS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Status/ExpEnviStatus.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.h"

bool handle_gui_to_exp_envi_handler_msg(ExpEnviData *exp_envi_data, TimeStamp current_time, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand);









#endif
