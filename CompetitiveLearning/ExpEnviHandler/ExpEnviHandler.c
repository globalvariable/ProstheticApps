#include "ExpEnviHandler.h"



int main( int argc, char *argv[])
{
	RtTasksData *rt_tasks_data = NULL;
	Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand = NULL;  
  	ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui = NULL;  
	ExpEnviData *exp_envi_data = NULL;
	ExpEnviHandParadigmRobotReach *exp_envi_paradigm = NULL;
	ExpEnviInputStatusHistory *exp_envi_input_status_history = NULL;
	ExpEnviOutputStatusHistory *exp_envi_output_status_history = NULL;

   	rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, 0);
	if (rt_tasks_data == NULL) 
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "rt_tasks_data == NULL.");

	sys_time_ptr = &(rt_tasks_data->current_system_time);

	if (*sys_time_ptr == 0)
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "rt_tasks_data->current_daq_system_time.");



	exp_envi_data = allocate_exp_envi_data(exp_envi_data);

	exp_envi_paradigm = g_new0(ExpEnviHandParadigmRobotReach, 1);
	exp_envi_paradigm->target_led_component_indexes_list = g_new0(unsigned int, 2);
	exp_envi_paradigm->target_led_component_indexes_list[0] = LEFT_LED_IDX_IN_EXP_ENVI_DATA;
	exp_envi_paradigm->target_led_component_indexes_list[1] = RIGHT_LED_IDX_IN_EXP_ENVI_DATA;


//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_input_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE, 10000000, 1500000000, 10000000, 1500000000, 1, 1, EXP_ENVI_COMP_STATUS_HIGH, FALSE)) /// 1 nose poke   // initially the nose of the animal gets into ir_beam. this means status low (pic sends it this way). Then the animal retracts nose and status becomes high. and gets it into ir_beam and the status is low. So the repsonse criterion is reached. 
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_input_component_type_to_exp_envi_data().");  
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_input_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_INPUT_COMPONENT_RESTRAIN_SWITCH, 10000000, 1500000000, 10000000, 1500000000, 0, 1, EXP_ENVI_COMP_STATUS_HIGH, FALSE)) /// 1 nose poke   // initially the nose of the animal gets into ir_beam. this means status low (pic sends it this way). Then the animal retracts nose and status becomes high. and gets it into ir_beam and the status is low. So the repsonse criterion is reached. 
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_input_component_type_to_exp_envi_data().");

//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_VALVE_CENTER, 40000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_BUZZER, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_LEFT_TARGET_LED, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_RIGHT_TARGET_LED, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_GUIDE_LED, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_LEVER_SOLENOID, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");
//     Order of addition of components is important. Add them according to ConfigExpEnviComponentNums.h
	if (! add_output_component_type_to_exp_envi_data(exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_GET_READY_LED, 15000000000))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "! add_output_component_type_to_exp_envi_data().");

	msgs_gui_2_exp_envi_hand = allocate_gui_2_exp_envi_hand_msg_buffer(msgs_gui_2_exp_envi_hand);
	msgs_exp_envi_hand_2_gui = allocate_exp_envi_hand_2_gui_msg_buffer(msgs_exp_envi_hand_2_gui);

	exp_envi_input_status_history = allocate_exp_envi_input_status_history(exp_envi_input_status_history, 100);
	exp_envi_output_status_history = allocate_exp_envi_output_status_history(exp_envi_output_status_history, 100);

	initialize_data_read_write_handlers();

	if(! create_exp_envi_handler_rt_thread(rt_tasks_data, exp_envi_data, msgs_gui_2_exp_envi_hand, msgs_exp_envi_hand_2_gui, exp_envi_input_status_history, exp_envi_output_status_history, exp_envi_paradigm))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "ExpEnviHandler", "main", "create_exp_envi_handler_rt_thread().");
	gtk_init(&argc, &argv);
	create_gui_handler(rt_tasks_data, msgs_gui_2_exp_envi_hand, msgs_exp_envi_hand_2_gui, exp_envi_input_status_history, exp_envi_output_status_history, exp_envi_data, exp_envi_paradigm);
	gtk_main();
	return 0;
}	
