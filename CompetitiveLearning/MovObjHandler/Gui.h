#ifndef GUI_H
#define GUI_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>
#include "GuiMovObjHandler.h"
#include "../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.h"

// Functions
void create_gui_handler(RtTasksData *rt_tasks_data, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *mov_obj_paradigm, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history);



#endif
