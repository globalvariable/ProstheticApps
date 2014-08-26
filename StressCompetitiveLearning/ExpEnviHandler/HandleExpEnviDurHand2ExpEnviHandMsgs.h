#ifndef HANDLE_EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSGS_H
#define HANDLE_EXP_ENVI_DUR_HAND_2_EXP_ENVI_HAND_MSGS_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Status/ExpEnviStatus.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.h"
#include "ConfigExpEnviComponentNums.h"
#include "HandleRS232Buffers.h"
#include "ExpEnviHandParadigm.h"

bool handle_exp_envi_dur_handler_to_exp_envi_handler_msg(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, ExpEnviHand2TrialHandMsg *msgs_exp_envi_hand_2_trial_hand, ExpEnviHandParadigmRobotReach *exp_envi_paradigm);



#endif
