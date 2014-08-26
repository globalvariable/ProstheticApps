#ifndef HAND_GUI_2_MOV_OBJ_HAND_MSGS_H
#define HAND_GUI_2_MOV_OBJ_HAND_MSGS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Status/MovObjStatus.h"
#include "../../../ExperimentHandlers/Library/MovObjData/MovObjData.h"
#include "../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"

bool handle_gui_to_mov_obj_handler_msg(ThreeDofRobot *robot_arm, MovObjStatus *mov_obj_status, TimeStamp current_time, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand);









#endif
