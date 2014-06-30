#ifndef EXP_ENVI_HANDLER_RT_TASK_H
#define EXP_ENVI_HANDLER_RT_TASK_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../ExperimentHandlers/Library/Messages/Gui2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2Gui.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviHand2ExpEnviDurHand.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviStatus.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "ExpEnviDurationHandlerRtTask.h"
#include "../../../ExperimentHandlers/Library/ExpEnviData/ExpEnviData.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviComponentStatusHistory.h"
#include "HandleGui2ExpEnviHandMsgs.h"
#include "HandleExpEnviDurHand2ExpEnviHandMsgs.h"
#include "HandleTrialHand2ExpEnviHandMsgs.h"
#include "HandleRS232Buffers.h"
#include "../TaskConfig.h"


TimeStamp	*sys_time_ptr;		

bool create_exp_envi_handler_rt_thread(RtTasksData *rt_tasks_data, ExpEnviData *exp_envi_data, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui, ExpEnviInputStatusHistory *exp_envi_input_status_history, ExpEnviOutputStatusHistory *exp_envi_output_status_history, ExpEnviHandParadigmRobotReach *exp_envi_paradigm);
bool kill_exp_envi_handler_rt_thread(void);



#endif
