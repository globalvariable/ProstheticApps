#include "HandleMovObjHand2MovObjDurHandMsgs.h"

static MovObjSchedule *mov_obj_schedule = NULL;

void init_mov_obj_duration_handler(void)
{
	mov_obj_schedule = g_new0(MovObjSchedule,1);
}

bool handle_mov_obj_handler_duration(TimeStamp current_time, MovObjDurHand2MovObjHandMsg *msgs_mov_obj_dur_hand_2_mov_obj_hand)
{
	unsigned int i;

	for (i = 0; i < MOV_OBJ_DUR_STATUS_NUM_OF_ITEMS; i++)
	{
		if (mov_obj_schedule->items[i].active)
		{
			if (current_time >= mov_obj_schedule->items[i].schedule)
			{
				mov_obj_schedule->items[i].active = FALSE;	
				if (! write_to_mov_obj_dur_hand_2_mov_obj_hand_msg_buffer(msgs_mov_obj_dur_hand_2_mov_obj_hand, current_time, MOV_OBJ_DUR_HAND_2_MOV_OBJ_HAND_MSG_TIMEOUT, i))
					return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjHandDur", "handle_mov_obj_handler_duration", "! write_to_mov_obj_dur_hand_2_mov_obj_hand_msg_buffer()."); 
			}	
		}
	}
	return TRUE;
}


bool handle_mov_obj_handler_to_mov_obj_dur_handler_msg(TimeStamp current_time, MovObjHand2MovObjDurHandMsg *msgs_mov_obj_hand_2_mov_obj_dur_hand)
{
	MovObjHand2MovObjDurHandMsgItem msg_item;
	while (get_next_mov_obj_hand_2_mov_obj_dur_hand_msg_buffer_item(msgs_mov_obj_hand_2_mov_obj_dur_hand, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE:	
				switch (msg_item.additional_data.schedule.item_idx)
				{
					case MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION:
						if (mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION].active)
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE & MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION");
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION].schedule = msg_item.additional_data.schedule.schedule;
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION].active = TRUE;
						break;
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH:
						if (mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH].active)
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE &MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH");
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH].schedule = msg_item.additional_data.schedule.schedule;
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH].active = TRUE;
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION:
						if (mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION].active)
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE &MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION");
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION].schedule = msg_item.additional_data.schedule.schedule;
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION].active = TRUE;
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION:
						if (mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION].active)
							return print_message(ERROR_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE & MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION");
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION].schedule = msg_item.additional_data.schedule.schedule;
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION].active = TRUE;
						break;	
					default:
						return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_SET_SCHEDULE - switch (msg_item.additional_data.schedule.schedule_item) - default");
				}
				break;	
			case MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_CANCEL_SCHEDULE:	
				switch (msg_item.additional_data.schedule.item_idx)
				{
					case MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION:
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_STAY_AT_CURRENT_POSITION].active = FALSE;
						break;
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH:
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_PULSE_WIDTH].active = FALSE;
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION:
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_SEND_AD_CONVERSION].active = FALSE;
						break;	
					case MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION:
						mov_obj_schedule->items[MOV_OBJ_DUR_STATUS_ITEM_READ_POSITION].active = FALSE;
						break;	
					default:
						return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "MOV_OBJ_HAND_2_MOV_OBJ_DUR_HAND_MSG_CANCEL_SCHEDULE - switch (msg_item.additional_data.schedule.schedule_item) - default");
				}
				break;	
			default:
				return print_message(BUG_MSG ,"MovObjHandler", "HandleMovObjHand2MovObjDurHandMsgs", "handle_mov_obj_handler_to_mov_obj_dur_handler_msg", "switch(msg_item.msg_type) - default");
		}
	}
	return TRUE;
}
