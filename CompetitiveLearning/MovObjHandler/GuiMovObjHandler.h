#ifndef GUI_MOV_OBJ_HANDLER_H
#define GUI_MOV_OBJ_HANDLER_H


#include <stdlib.h>
#include "../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"
#include "MovObjHandParadigm.h"
#include "../../../BlueSpike/Library/Thresholding/Thresholding.h"
#include "DataHandling/DataHandling.h"
#include "../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.h"
#include "MovObjHandlerRtTask.h"

bool create_mov_obj_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *mov_obj_paradigm, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history);


#endif
