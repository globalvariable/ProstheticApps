#ifndef HANDLE_RS232_BUFFERS_H
#define HANDLE_RS232_BUFFERS_H



#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include "ConfigExpEnviComponentNums.h"
#include "../../../ExperimentHandlers/Library/RS232/RS232Handler.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviEvaluate.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.h"
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.h"

bool init_rs232_buffers(SEM **exp_envi_rx_buff_sem, SEM **exp_envi_tx_buff_sem, ExpEnviRxShm **exp_envi_rx_buff_shm, ExpEnviTxShm **exp_envi_tx_buff_shm);
bool handle_exp_envi_tx_shm(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviOutputStatusHistory *exp_envi_output_status_history);
bool handle_exp_envi_rx_shm(ExpEnviData *exp_envi_data, TimeStamp current_time, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, ExpEnviHand2TrialHandMsg* msgs_exp_envi_hand_2_trial_hand, ExpEnviInputStatusHistory *exp_envi_input_status_history);






#endif
