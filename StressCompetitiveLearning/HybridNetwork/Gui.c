#include "Gui.h"


void create_gui(void)
{

	GtkWidget *window, *vbox, *tabs;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1020);
  	gtk_window_set_deletable(GTK_WINDOW(window), FALSE);
  	gtk_window_set_title(GTK_WINDOW(window), "HybridNetwork");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	tabs = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (tabs), GTK_POS_TOP);
        gtk_box_pack_start(GTK_BOX(vbox),tabs, TRUE, TRUE, 0);

 	if (! create_config_daq_gui(tabs))
		return (void)print_message(ERROR_MSG ,"HybridNetwork", "Gui", "create_gui", "create_daq_config_gui().");

 	if (! create_spike_viewer_gui(tabs))
		return (void)print_message(ERROR_MSG ,"HybridNetwork", "Gui", "create_gui", "create_spike_viewer_gui().");

 	if (! create_spike_sorter_gui(tabs))  // to increase number of guis for spike_sorter (or others), one should define ptr arrays for button functions, buttons and variables controlling display, pause etc.
								// OR, pass the 'gui number' to the button_functions to control specific graphs, buttons belonging to specific gui.
								// OR, define static variables, buttons which produces copies for each function call.
		return (void)print_message(ERROR_MSG ,"HybridNetwork", "Gui", "create_gui", "create_spike_sorter_gui().");

 	if (! create_network_view_gui(tabs))  
		return (void)print_message(ERROR_MSG ,"HybridNetwork", "Gui", "create_gui", "create_network_view_gui().");

	gtk_widget_show_all(window);


}
