#include "GuiMovObjHandler.h"

static RtTasksData *static_rt_tasks_data = NULL;

static Gui2MovObjHandMsg *static_msgs_gui_2_mov_obj_hand;    
static MovObjHand2GuiMsg *static_msgs_mov_obj_hand_2_gui;

static ThreeDofRobot *static_robot_arm = NULL;
static MovObjHandParadigmRobotReach *static_mov_obj_paradigm = NULL;

static MovObjStatusHistory* static_mov_obj_status_history = NULL;
static ThreeDofRobotAngleHistory *static_robot_angle_history = NULL;
static ThreeDofRobotPulseHistory *static_robot_pulse_history = NULL;

static GtkWidget *entry_spike_multiplier;
static GtkWidget *btn_submit_spike_multiplier;

static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_create_recording_folder;

static void submit_spike_multiplier_button_func (void);

static void create_recording_folder_button_func (void);

static void set_directory_btn_select_directory_to_save(void);



static gboolean timeout_callback(gpointer graph);

bool create_mov_obj_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2MovObjHandMsg *msgs_gui_2_mov_obj_hand, MovObjHand2GuiMsg *msgs_mov_obj_hand_2_gui, ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *mov_obj_paradigm, MovObjStatusHistory* mov_obj_status_history, ThreeDofRobotAngleHistory *robot_angle_history, ThreeDofRobotPulseHistory *robot_pulse_history)
{
	GtkWidget *frame, *frame_label, *hbox, *table, *vbox;

	static_rt_tasks_data = rt_tasks_data;

	static_robot_arm = robot_arm;
	static_mov_obj_paradigm = mov_obj_paradigm;

	static_msgs_gui_2_mov_obj_hand = msgs_gui_2_mov_obj_hand;
	static_msgs_mov_obj_hand_2_gui = msgs_mov_obj_hand_2_gui;

	static_mov_obj_status_history = mov_obj_status_history;
	static_robot_angle_history = robot_angle_history;
	static_robot_pulse_history = robot_pulse_history;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Mov Obj Handler    ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  

 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(2 ,3,TRUE);   // 2 rows 3 columns
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 0,1, 0, 3);  // column 0-1, row 0-3

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_spike_multiplier = gtk_button_new_with_label("Left x Coeff");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_spike_multiplier, FALSE, FALSE, 0);

	entry_spike_multiplier = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_spike_multiplier, FALSE, FALSE, 0);
	gtk_widget_set_size_request(entry_spike_multiplier, 50, 25);
	gtk_entry_set_text(GTK_ENTRY(entry_spike_multiplier), "1.0");


	////////   LAST COLUMN
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 2,3, 0, 6);  // column 2-3, row 0-6

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

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


	g_signal_connect(G_OBJECT(btn_submit_spike_multiplier), "clicked", G_CALLBACK(submit_spike_multiplier_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);

	g_timeout_add(100, timeout_callback, NULL);		

	return TRUE;
}

static gboolean timeout_callback(gpointer graph)
{
	char *path_temp, *path;
	MovObjHand2GuiMsgItem msg_item;
	static bool recording = FALSE;
	unsigned int recording_number;

	while (get_next_mov_obj_hand_2_gui_msg_buffer_item(static_msgs_mov_obj_hand_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case MOV_OBJ_HAND_2_GUI_MSG_START_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data;
				if (!(*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, static_rt_tasks_data->current_system_time, recording_number))	
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *create_data_directory().");		
					exit(1);
				}
				recording = TRUE;	
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, static_mov_obj_status_history, static_robot_angle_history, static_robot_pulse_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}				
				break;
			case MOV_OBJ_HAND_2_GUI_MSG_STOP_RECORDING:
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, static_mov_obj_status_history, static_robot_angle_history, static_robot_pulse_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}	
				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *fclose_all_data_file().");		
					exit(1);
				}
				recording = FALSE;	
				break;
			case MOV_OBJ_HAND_2_GUI_MSG_CANCEL_RECORDING:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....		

				static_mov_obj_status_history->buff_read_idx = static_mov_obj_status_history->buff_write_idx;
				static_robot_angle_history->buff_read_idx = static_robot_angle_history->buff_write_idx;
				static_robot_pulse_history->buff_read_idx = static_robot_pulse_history->buff_write_idx;

				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_rt_tasks_data->current_system_time))	
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", "! *fclose_all_data_files().");
					exit(1);
				}
				if (! (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, recording_number))
				{
					print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *fdelete_all_data_files().");
					exit(1);
				}
				recording = FALSE;	
				break;
			default:
				return print_message(ERROR_MSG ,"TrialHandler", "GuiMovObjHandler", "timeout_callback", "switch (msg_item.msg_type) - default");
		}
	}
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, static_mov_obj_status_history, static_robot_angle_history, static_robot_pulse_history))	// this function handles history buffers
		{
			print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "timeout_callback", " *write_to_data_files().");		
			exit(1);
		}			
	}
	else
	{
		static_mov_obj_status_history->buff_read_idx = static_mov_obj_status_history->buff_write_idx;
		static_robot_angle_history->buff_read_idx = static_robot_angle_history->buff_write_idx;
		static_robot_pulse_history->buff_read_idx = static_robot_pulse_history->buff_write_idx;	
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
		return (void)print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "create_recording_folder_button_func", "Selected folder is /EXP_DATA main folder. Select a folder inside this folder.");				
	if ((*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, static_mov_obj_paradigm, static_robot_arm))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "create_recording_folder_button_func", " *create_main_directory().");			
}

static void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
		print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "set_directory_btn_select_directory_to_save", "Couldn't find file: ./path_initial_directory.");
		print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "set_directory_btn_select_directory_to_save", "Couldn' t read ./path_initial_directory.");
			print_message(ERROR_MSG ,"MovObjHandler", "GuiMovObjHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
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

static void submit_spike_multiplier_button_func (void)
{
	static_mov_obj_paradigm->left_spike_multiplier = atof(gtk_entry_get_text(GTK_ENTRY(entry_spike_multiplier)));

}
