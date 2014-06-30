#ifndef EXP_ENVI_DURATION_HANDLER_RT_TASK_H
#define EXP_ENVI_DURATION_HANDLER_RT_TASK_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "HandleExpEnviHand2ExpEnviDurHandMsgs.h"
#include "HandleExpEnviHandDur.h"
#include "ExpEnviHandlerRtTask.h"

bool create_exp_envi_duration_handler_rt_thread(RtTasksData *rt_tasks_data, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, ExpEnviHand2ExpEnviDurHandMsg *msgs_exp_envi_hand_2_exp_envi_dur_hand, unsigned int num_of_inp_comps, unsigned int num_of_outp_comps);
bool kill_exp_envi_duration_handler_rt_thread(void);









#endif
