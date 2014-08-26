#ifndef HANDLE_ROBOT_POSITION_THRESHOLD_H
#define HANDLE_ROBOT_POSITION_THRESHOLD_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Status/MovObjStatus.h"
#include "../../../ExperimentHandlers/Library/Status/MovObjDurStatus.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"
#include "MovObjHandParadigm.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.h"
#include "ConfigMovObjHandler.h"
#include "../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"

bool handle_robot_arm_position_threshold(ThreeDofRobot *robot, MovObjHandParadigmRobotReach *paradigm, MovObjStatus *mov_obj_status, TimeStamp current_time, MovObjHand2MovObjDurHandMsg *msgs_mov_obj_hand_2_mov_obj_dur_hand, MovObjHand2TrialHandMsg *msgs_mov_obj_hand_2_trial_hand, MovObjStatusHistory* mov_obj_status_history, MovObjHand2NeuralNetMsg **msgs_mov_obj_hand_2_neural_net_multi_thread);






#endif

