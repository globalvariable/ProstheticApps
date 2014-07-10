#include "GuiExpEnviHandler.h"

static RtTasksData *static_rt_tasks_data = NULL;

static Gui2ExpEnviHandMsg *static_msgs_gui_2_exp_envi_hand = NULL;    
static ExpEnviHand2GuiMsg *static_msgs_exp_envi_hand_2_gui = NULL;  

static ExpEnviInputStatusHistory *static_exp_envi_input_status_history = NULL;
static ExpEnviOutputStatusHistory *static_exp_envi_output_status_history = NULL;

static ExpEnviHandParadigmRobotReach *static_exp_envi_paradigm = NULL;

static ExpEnviData *static_exp_envi_data = NULL;


static GtkWidget *entry_restrain_duration;
static GtkWidget *btn_submit_restrain_duration;

static GtkWidget *btn_restrain_on_off;

static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_create_recording_folder;

static void create_recording_folder_button_func (void);

static void set_directory_btn_select_directory_to_save(void);

static void restrain_on_off_button_func (void);
static void submit_restrain_duration_button_func (void);

static gboolean timeout_callback(gpointer user_data) ;

bool create_exp_envi_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2ExpEnviHandMsg *msgs_gui_2_exp_envi_hand, ExpEnviHand2GuiMsg *msgs_exp_envi_hand_2_gui, ExpEnviInputStatusHistory *exp_envi_input_status_history, ExpEnviOutputStatusHistory *exp_envi_output_status_history, ExpEnviData *exp_envi_data, ExpEnviHandParadigmRobotReach *exp_envi_paradigm)
{
	static_rt_tasks_data = rt_tasks_data;

	GtkWidget *frame, *frame_label, *hbox, *table, *vbox, *lbl;

	static_msgs_gui_2_exp_envi_hand = msgs_gui_2_exp_envi_hand;
	static_msgs_exp_envi_hand_2_gui = msgs_exp_envi_hand_2_gui;

	static_exp_envi_input_status_history = exp_envi_input_status_history;
	static_exp_envi_output_status_history = exp_envi_output_status_history;

	static_exp_envi_data = exp_envi_data;

	static_exp_envi_paradigm = exp_envi_paradigm;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Exp Envi Handler    ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(2 ,3,TRUE);   // 2 rows 3 columns
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 0,1, 0, 6);  // column 0-1, row 0-6

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	////////   LAST COLUMN
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 2,3, 0, 6);  // column 2-3, row 0-6

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_restrain_duration = gtk_button_new_with_label("Restrain Duration");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_restrain_duration, FALSE, FALSE, 0);

	entry_restrain_duration = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_restrain_duration, FALSE, FALSE, 0);
	gtk_widget_set_size_request(entry_restrain_duration, 50, 25);
	gtk_entry_set_text(GTK_ENTRY(entry_restrain_duration), "300");
	lbl = gtk_label_new("ms");	
     	gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_restrain_on_off = gtk_button_new_with_label("Restrain: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_restrain_on_off, FALSE, FALSE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      

  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_directory_to_save, TRUE,TRUE,0);
	set_directory_btn_select_directory_to_save();

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_create_recording_folder = gtk_button_new_with_label("Create Recording Folder");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_recording_folder, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_restrain_on_off), "clicked", G_CALLBACK(restrain_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_submit_restrain_duration), "clicked", G_CALLBACK(submit_restrain_duration_button_func), NULL);
	g_timeout_add(100, timeout_callback, NULL);		

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	char *path_temp, *path;
	ExpEnviHand2GuiMsgItem msg_item;
	static bool recording = FALSE;
	unsigned int recording_number;

	while (get_next_exp_envi_hand_2_gui_msg_buffer_item(static_msgs_exp_envi_hand_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case EXP_ENVI_HAND_2_GUI_MSG_START_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data;
				if (!(*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, static_rt_tasks_data->current_system_time, recording_number))	
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *create_data_directory().");		
					exit(1);
				}
				recording = TRUE;	
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, static_exp_envi_input_status_history, static_exp_envi_output_status_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}				
				break;
			case EXP_ENVI_HAND_2_GUI_MSG_STOP_RECORDING:
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, static_exp_envi_input_status_history, static_exp_envi_output_status_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}	
				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *fclose_all_data_file().");		
					exit(1);
				}
				recording = FALSE;	
				break;
			case EXP_ENVI_HAND_2_GUI_MSG_CANCEL_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....		

				static_exp_envi_input_status_history->buff_read_idx = static_exp_envi_input_status_history->buff_write_idx;
				static_exp_envi_output_status_history->buff_read_idx = static_exp_envi_output_status_history->buff_write_idx;

				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", "! *fclose_all_data_files().");
					exit(1);
				}
				if (! (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, recording_number))
				{
					print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *fdelete_all_data_files().");
					exit(1);
				}
				else
				recording = FALSE;	
				break;
			default:
				return print_message(ERROR_MSG ,"TrialHandler", "GuiExpEnviHandler", "timeout_callback", "switch (msg_item.msg_type) - default");
		}
	}
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, static_exp_envi_input_status_history, static_exp_envi_output_status_history))	// this function handles history buffers
		{
			print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "timeout_callback", " *write_to_data_directory().");		
			exit(1);
		}			
	}
	else
	{
		static_exp_envi_input_status_history->buff_read_idx = static_exp_envi_input_status_history->buff_write_idx;
		static_exp_envi_output_status_history->buff_read_idx = static_exp_envi_output_status_history->buff_write_idx;		
	}
	return TRUE;
}

static void create_recording_folder_button_func (void)
{
	unsigned int path_len;
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	path_len = strlen(path_temp);
	if (strcmp(&(path_temp[path_len-8]),"EXP_DATA") == 0)
		return (void)print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "create_recording_folder_button_func", "Selected folder is /EXP_DATA main folder. Select a folder inside this folder.");				
	if ((*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path,static_exp_envi_paradigm, static_exp_envi_data))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "create_recording_folder_button_func", " *create_main_directory().");			
}

static void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
		print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "set_directory_btn_select_directory_to_save", "Couldn't find file: ./path_initial_directory.");
		print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "set_directory_btn_select_directory_to_save", "Couldn' t read ./path_initial_directory.");
			print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
		}
		else
		{
			line[strlen(line)-16] = 0;   
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),line);
		}
		fclose(fp); 		
	}  	 
}

static void restrain_on_off_button_func (void)
{
	if (static_exp_envi_paradigm->restrain_on)
	{
		static_exp_envi_paradigm->restrain_on = FALSE;
		gtk_button_set_label (GTK_BUTTON (btn_restrain_on_off),"Restrain: OFF");		
	}
	else
	{
		static_exp_envi_paradigm->restrain_on = TRUE;
		gtk_button_set_label (GTK_BUTTON (btn_restrain_on_off),"Restrain: ON");		
	}	
}


static void submit_restrain_duration_button_func (void)
{
	TimeStamp restrain_duration;
	unsigned int idx;

	restrain_duration = (TimeStamp)(1000000.0 * atof(gtk_entry_get_text(GTK_ENTRY(entry_restrain_duration))));

	if (! get_output_component_type_idx_in_exp_envi_data(static_exp_envi_data, EXP_ENVI_OUTPUT_COMPONENT_LEVER_SOLENOID, &idx))
		return (void)print_message(ERROR_MSG ,"ExpEnviHandler", "GuiExpEnviHandler", "submit_restrain_duration_button_func", "! get_input_component_type_idx_in_exp_envi_data()");

	static_exp_envi_data->outp_comp_types[idx].high_status_duration = restrain_duration;

	return;
}
