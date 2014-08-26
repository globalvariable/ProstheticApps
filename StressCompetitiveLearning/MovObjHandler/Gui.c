#include "Gui.h"

void create_gui_handler(RtTasksData *rt_tasks_data, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *mov_obj_paradigm, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history)
{
	GtkWidget *window, *vbox, *tabs;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 1000);
  	gtk_window_set_title(GTK_WINDOW(window), "BMIExpController - MovObjHandler");
  	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
  	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	tabs = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (tabs), GTK_POS_TOP);
        gtk_box_pack_start(GTK_BOX(vbox),tabs, TRUE, TRUE, 0);

	if (!create_mov_obj_handler_tab(tabs, rt_tasks_data, msgs_gui_2_mov_obj_hand, msgs_mov_obj_hand_2_gui, robot_arm, mov_obj_paradigm, mov_obj_status_history, robot_angle_history, robot_pulse_history))
		print_message(ERROR_MSG ,"BMIExpController", "Gui", "create_gui", "create_mov_obj_handler_gui().");

	gtk_widget_show_all(window);
}
