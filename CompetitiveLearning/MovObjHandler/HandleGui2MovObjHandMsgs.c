#include "HandleGui2MovObjHandMsgs.h"



bool handle_gui_to_mov_obj_handler_msg(ThreeDofRobot *robot_arm, MovObjStatus *mov_obj_status, TimeStamp current_time, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand)
{
	Gui2MovObjHandMsgItem msg_item;
	char str_gui_msg[GUI_2_MOV_OBJ_HAND_MSG_STRING_LENGTH];

	while (get_next_gui_2_mov_obj_hand_msg_buffer_item(msgs_gui_2_mov_obj_hand, &msg_item))
	{
		get_gui_2_mov_obj_hand_msg_type_string(msg_item.msg_type, str_gui_msg);
		print_message(INFO_MSG ,"MovObjHandler", "HandleGui2MovObjHandMsgs", "handle_gui_to_mov_obj_handler_msg", str_gui_msg);
		switch (msg_item.msg_type)
		{
			case GUI_2_MOV_OBJ_HAND_MSG_SET_THRESHOLD:	
/*
				mov_obj_data->glo_constraints.current_threshold = msg_item.additional_data;
*/
				break;
			default:
				return print_message(BUG_MSG ,"MovObjHandler", "HandleGui2MovObjHandMsgs", "handle_gui_to_mov_obj_handler_msg", str_gui_msg);
		}
	}

	return TRUE;
}
