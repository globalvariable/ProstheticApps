#ifndef HANDLE_MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSGS_H
#define HANDLE_MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSGS_H

#include "../../../ExperimentHandlers/Library/Status/MovObjDurStatus.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjHand2MovObjDurHand.h"
#include "../../../ExperimentHandlers/Library/Messages/MovObjDurHand2MovObjHand.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"

void init_mov_obj_duration_handler(void);
bool handle_mov_obj_handler_duration(TimeStamp current_time, MovObjDurHand2MovObjHandMsg *msgs_mov_obj_dur_hand_2_mov_obj_hand);
bool handle_mov_obj_handler_to_mov_obj_dur_handler_msg(TimeStamp current_time, MovObjHand2MovObjDurHandMsg *msgs_mov_obj_hand_2_mov_obj_dur_hand);











#endif
