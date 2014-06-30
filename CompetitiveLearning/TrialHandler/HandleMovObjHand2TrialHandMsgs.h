#ifndef HANDLE_MOV_OBJ_HAND_2_TRIAL_HAND_MSGS_H
#define HANDLE_MOV_OBJ_HAND_2_TRIAL_HAND_MSGS_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2TrialDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2Gui.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatus.h"
#include "TrialHandParadigm.h"
#include "../../../BlueSpike/Library/Misc/Randn.h"
#include "TrialHistory.h"
#include "../../../ExperimentHandlers/Library/Status/TrialStatusHistory.h"


void initialize_mov_obj_handler_to_trial_handler_msg_params(TrialStatus *arg_trial_status, MovObjHand2TrialHandMsg *arg_msgs_mov_obj_hand_2_trial_hand, TrialHand2TrialDurHandMsg *arg_msgs_trial_hand_2_trial_dur_hand, TrialHand2ExpEnviHandMsg *arg_msgs_trial_hand_2_exp_envi_hand, TrialHand2MovObjHandMsg *arg_msgs_trial_hand_2_mov_obj_hand, TrialHand2NeuralNetMsg *arg_msgs_trial_hand_2_neural_net, TrialHandParadigmRobotReach *arg_paradigm, ClassifiedTrialHistory *arg_classified_history, TrialHand2GuiMsg *arg_msgs_trial_hand_2_gui, TrialStatusHistory *arg_trial_status_history);

bool handle_mov_obj_handler_to_trial_handler_msg(TimeStamp current_time);





#endif
