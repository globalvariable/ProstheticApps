#ifndef HANDLE_TRIAL_HAND_2_EXP_ENVI_HAND_MSGS_H
#define HANDLE_TRIAL_HAND_2_EXP_ENVI_HAND_MSGS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Status/ExpEnviStatus.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.h"
#include "ExpEnviHandParadigm.h"
#include "ConfigExpEnviComponentNums.h"
#include "HandleRS232Buffers.h"

bool handle_trial_handler_to_exp_envi_handler_msg(ExpEnviData *exp_envi_data, TimeStamp current_time, TrialHand2ExpEnviHandMsg *msgs_trial_hand_2_exp_envi_hand, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, ExpEnviHandParadigmRobotReach *exp_envi_paradigm, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui);












#endif
