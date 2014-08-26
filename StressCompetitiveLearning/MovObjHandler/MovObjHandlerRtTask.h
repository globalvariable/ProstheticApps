#ifndef MOV_OBJ_HANDLER_RT_TASK_H
#define MOV_OBJ__HANDLER_RT_TASK_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../ExperimentHandlers/Library/Messages/Gui2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2Gui.h"
#include "../../../ExperimentHandlers/Library/Messages/TrialHand2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2TrialHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"
#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../../ExperimentHandlers/Library/MovObjData/MovObjData.h"
#include "MovObjHandParadigm.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"
#include "../../../BlueSpike/Library/Thresholding/Thresholding.h"
#include "../../../BlueSpike/Library/LogBuffer/MessageLogBuffer.h"
#include "../../../ExperimentHandlers/Library/RS232/RS232Handler.h"
#include "HandleGui2MovObjHandMsgs.h"
#include "HandleMovObjDurHand2MovObjHandMsgs.h"
#include "HandleMovObjHand2MovObjDurHandMsgs.h"
#include "HandleTrialHand2MovObjHandMsgs.h"
#include "HandleNeuralNet2MovObjHandMsgs.h"
#include "HandleSpikeDataBuff.h"
#include "../TaskConfig.h"
#include "../InterProcessMsgConfig.h"

bool create_mov_obj_handler_rt_thread(RtTasksData *rt_tasks_data, ThreeDofRobot *robot_arm, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, MovObjHandParadigmRobotReach *mov_obj_paradigm, MessageLogBuffer *message_log, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history);
bool kill_mov_obj_handler_rt_thread(void);



#endif
