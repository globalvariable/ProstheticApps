#include "GuiTrialHandler.h"

static RtTasksData *static_rt_tasks_data = NULL;

static Gui2TrialHandMsg *static_msgs_gui_2_trial_hand = NULL;  
static TrialHand2GuiMsg *static_msgs_trial_hand_2_gui = NULL;  

static TrialHandParadigmRobotReach *paradigm = NULL;

static ClassifiedTrialHistory* classified_history = NULL;

static TrialStatusHistory *static_trial_status_history = NULL;

static bool stop_continuous_recording_request = FALSE;

static GtkWidget *btn_reset_connections;
static GtkWidget *btn_enable_trials;
static GtkWidget *btn_disable_trials;
static GtkWidget *btn_quit_trials;

static GtkWidget *btn_select_target;
static GtkWidget *entry_select_target;

static GtkWidget *btn_auto_target_select_mode_on;
static GtkWidget *btn_auto_target_select_mode_off;

static GtkWidget *lbl_difficulty_level;

static GtkWidget *btn_increase_robot_start_idx;
static GtkWidget *btn_decrease_robot_start_idx;

static GtkWidget *btn_release_reward;

static GtkWidget *btn_start_trial;

//Stats
static GtkWidget *entry_trial_number;
static GtkWidget *lbl_trial_length;
static GtkWidget *lbl_robot_start_position_idx;
static GtkWidget *lbl_robot_target_position_idx;
static GtkWidget *lbl_reward_amount;
static GtkWidget *lbl_interrogated_difficulty_level;
static GtkWidget *btn_submit_trial_number;

static GtkWidget *lbl_num_of_trials;
static GtkWidget *lbl_success_ratio_all;
static GtkWidget *lbl_num_of_left_trials;
static GtkWidget *lbl_success_ratio_left_small;
static GtkWidget *lbl_success_ratio_left_large;
static GtkWidget *lbl_num_of_right_trials;
static GtkWidget *lbl_success_ratio_right_small;
static GtkWidget *lbl_success_ratio_right_large;

static GtkWidget *btn_select_directory_to_save;
static GtkWidget *btn_create_recording_folder;

static GtkWidget *lbl_trial_status;

static GtkWidget *btn_start_recording;
static GtkWidget *btn_stop_recording;
static GtkWidget *btn_cancel_recording;

static GtkWidget *lbl_recording_status;

static void reset_connections_button_func (void);
static void enable_trials_button_func (void);
static void disable_trials_button_func (void);
static void quit_trials_button_func (void);

static void auto_target_select_mode_on_button_func(void);
static void auto_target_select_mode_off_button_func(void);

static void select_target_button_func (void);

static void increase_robot_start_idx_button_func (void);
static void decrease_robot_start_idx_button_func (void);

static void release_reward_button_func (void);

static void start_trial_button_func (void);

static void submit_trial_number_button_func (void);

static void create_recording_folder_button_func (void);

static void start_recording_button_func (void);
static void stop_recording_button_func (void);
static void cancel_recording_button_func (void);

static void set_directory_btn_select_directory_to_save(void);

static gboolean timeout_callback(gpointer user_data) ;

bool create_trial_handler_tab(GtkWidget *tabs, RtTasksData *rt_tasks_data, Gui2TrialHandMsg *msgs_gui_2_trial_hand, TrialHandParadigmRobotReach *trial_hand_paradigm, ClassifiedTrialHistory* classified_trial_history, TrialHand2GuiMsg *msgs_trial_hand_2_gui, TrialStatusHistory *trial_status_history)
{
	GtkWidget *frame, *frame_label, *hbox, *lbl, *table, *vbox;

	static_rt_tasks_data = rt_tasks_data;

	static_msgs_gui_2_trial_hand = msgs_gui_2_trial_hand;
	static_msgs_trial_hand_2_gui = msgs_trial_hand_2_gui;

	paradigm = trial_hand_paradigm;

	classified_history = classified_trial_history;

	static_trial_status_history = trial_status_history;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     Trials Handler    ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(2 ,3,TRUE);   // 2 rows 3 columns
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 0,1, 0, 6);  // column 0-1, row 0-6

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_reset_connections = gtk_button_new_with_label("Reset Connections");
	gtk_box_pack_start (GTK_BOX (hbox), btn_reset_connections, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Trials: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	btn_enable_trials = gtk_button_new_with_label("Enable");
	gtk_box_pack_start (GTK_BOX (hbox), btn_enable_trials, TRUE, TRUE, 0);

	btn_disable_trials = gtk_button_new_with_label("Disable");
	gtk_box_pack_start (GTK_BOX (hbox), btn_disable_trials, TRUE, TRUE, 0);

	btn_quit_trials = gtk_button_new_with_label("Quit");
	gtk_box_pack_start (GTK_BOX (hbox), btn_quit_trials, TRUE, TRUE, 0);

       gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_auto_target_select_mode_on = gtk_button_new_with_label("Auto Target Selection ON");
	gtk_box_pack_start (GTK_BOX (hbox), btn_auto_target_select_mode_on , TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_auto_target_select_mode_off = gtk_button_new_with_label("Auto Target Selection OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_auto_target_select_mode_off , TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_select_target = gtk_button_new_with_label("Select Target");
	gtk_box_pack_start (GTK_BOX (hbox), btn_select_target , TRUE, TRUE, 0);

	lbl = gtk_label_new("Idx: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	entry_select_target = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_select_target, FALSE, FALSE, 0);
	gtk_widget_set_size_request(entry_select_target, 50, 25);
	gtk_entry_set_text(GTK_ENTRY(entry_select_target), "0");

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_increase_robot_start_idx = gtk_button_new_with_label("Increase Robot Start Idx");
	gtk_box_pack_start (GTK_BOX (hbox), btn_increase_robot_start_idx, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_decrease_robot_start_idx = gtk_button_new_with_label("Decrease Robot Start Idx");
	gtk_box_pack_start (GTK_BOX (hbox), btn_decrease_robot_start_idx, TRUE, TRUE, 0);

       gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_release_reward = gtk_button_new_with_label("Reward");
	gtk_box_pack_start (GTK_BOX (hbox), btn_release_reward , TRUE, TRUE, 0);

       gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 20);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_start_trial = gtk_button_new_with_label("Start Trial");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_trial , TRUE, TRUE, 0);

       gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("Difficulty Level: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);

	lbl_difficulty_level = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox),lbl_difficulty_level, FALSE,FALSE,0);

	////////   SECOND COLUMN
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 1,2, 0, 6);  // column 5-6, row 0-6

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Num of Trials: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_num_of_trials = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_num_of_trials, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Success All: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_success_ratio_all = gtk_label_new("0.00");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_success_ratio_all, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), TRUE,TRUE, 5);


	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Num of LEFT: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_num_of_left_trials = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_num_of_left_trials, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Success Small: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_success_ratio_left_small = gtk_label_new("0.00");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_success_ratio_left_small, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Success Large: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_success_ratio_left_large = gtk_label_new("0.00");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_success_ratio_left_large, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), TRUE,TRUE, 5);


	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Num of RIGHT: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_num_of_right_trials = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_num_of_right_trials, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Success Small: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_success_ratio_right_small = gtk_label_new("0.00");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_success_ratio_right_small, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Success Large: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_success_ratio_right_large = gtk_label_new("0.00");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_success_ratio_right_large, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), TRUE,TRUE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,5);

	lbl = gtk_label_new("---- Interrogate Trial ----");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Trial #: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	entry_trial_number = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_trial_number, FALSE, FALSE, 0);
	gtk_widget_set_size_request(entry_trial_number, 50, 25);
	gtk_entry_set_text(GTK_ENTRY(entry_trial_number), "0");

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Length: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_trial_length = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_trial_length, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Robot Start Position Idx: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_robot_start_position_idx = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_robot_start_position_idx, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Robot Target Position Idx: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_robot_target_position_idx = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_robot_target_position_idx, FALSE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("Reward: ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 0);
	lbl_reward_amount = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox), lbl_reward_amount, FALSE, FALSE, 0);

 	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);
	lbl = gtk_label_new("difficulty level:	");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE,0);
	lbl_interrogated_difficulty_level = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox),lbl_interrogated_difficulty_level, FALSE,FALSE,0);


	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	btn_submit_trial_number = gtk_button_new_with_label("Submit Trial Number");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_trial_number, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), TRUE,TRUE, 5);


	////////   LAST COLUMN
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 2, 3, 0, 6);  // column 5-6, row 0-6

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	lbl_trial_status = gtk_label_new("TRIAL_STATUS_NOT_KNOWN");
        gtk_box_pack_start(GTK_BOX(hbox),lbl_trial_status, TRUE, TRUE, 0);
	gtk_widget_set_size_request(lbl_trial_status, 300, 30);	

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 10);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      

  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_directory_to_save, TRUE,TRUE,0);
	set_directory_btn_select_directory_to_save();


   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_create_recording_folder = gtk_button_new_with_label("Create Recording Folders");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_recording_folder, TRUE, TRUE, 0);

        gtk_box_pack_start(GTK_BOX(vbox),gtk_hseparator_new(), FALSE,FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);

	lbl = gtk_label_new("DATA RECORDING CONTROL");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE, TRUE, 5);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_start_recording = gtk_button_new_with_label("START");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_recording, TRUE, TRUE, 0);
	gtk_widget_set_size_request(btn_start_recording, 20, 80);	

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_stop_recording = gtk_button_new_with_label("STOP");
	gtk_box_pack_start (GTK_BOX (hbox), btn_stop_recording, TRUE, TRUE, 0);
	gtk_widget_set_size_request(btn_stop_recording, 20, 80);	

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	btn_cancel_recording = gtk_button_new_with_label("CANCEL");
	gtk_box_pack_start (GTK_BOX (hbox), btn_cancel_recording, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	     

	lbl_recording_status = gtk_label_new("RECORDING IDLE");
        gtk_box_pack_start(GTK_BOX(hbox),lbl_recording_status, TRUE, TRUE, 0);
	gtk_widget_set_size_request(lbl_recording_status, 300, 30);	

	g_signal_connect(G_OBJECT(btn_reset_connections), "clicked", G_CALLBACK(reset_connections_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_enable_trials), "clicked", G_CALLBACK(enable_trials_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_disable_trials), "clicked", G_CALLBACK(disable_trials_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_quit_trials), "clicked", G_CALLBACK(quit_trials_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_auto_target_select_mode_on), "clicked", G_CALLBACK(auto_target_select_mode_on_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_auto_target_select_mode_off), "clicked", G_CALLBACK(auto_target_select_mode_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_select_target), "clicked", G_CALLBACK(select_target_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_increase_robot_start_idx), "clicked", G_CALLBACK(increase_robot_start_idx_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_decrease_robot_start_idx), "clicked", G_CALLBACK(decrease_robot_start_idx_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_release_reward), "clicked", G_CALLBACK(release_reward_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_start_trial), "clicked", G_CALLBACK(start_trial_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_submit_trial_number), "clicked", G_CALLBACK(submit_trial_number_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);

	g_signal_connect(G_OBJECT(btn_start_recording), "clicked", G_CALLBACK(start_recording_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_stop_recording), "clicked", G_CALLBACK(stop_recording_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_cancel_recording), "clicked", G_CALLBACK(cancel_recording_button_func), NULL);

	gtk_widget_set_sensitive(btn_disable_trials, FALSE);	
	gtk_widget_set_sensitive(btn_stop_recording, FALSE);	
	gtk_widget_set_sensitive(btn_cancel_recording, FALSE);	

	g_timeout_add(100, timeout_callback, NULL);		

	return TRUE;
}

static gboolean timeout_callback(gpointer user_data) 
{
	char *path_temp, *path;
	char temp[TRIAL_HAND_2_GUI_MSG_STRING_LENGTH];
	TrialHand2GuiMsgItem msg_item;
	static bool recording = FALSE, first_start_recording_msg = TRUE;
	static bool continuous_recording = FALSE;
	TrialStatus trial_status;
	unsigned int recording_number;

	sprintf (temp, "%u", paradigm->current_trial_data.difficulty_level);
	gtk_label_set_text (GTK_LABEL (lbl_difficulty_level), temp);
	
	while (get_next_trial_hand_2_gui_msg_buffer_item(static_msgs_trial_hand_2_gui, &msg_item))
	{
		switch (msg_item.msg_type)
		{
			case TRIAL_HAND_2_GUI_MSG_BROADCAST_START_RECORDING_MSG_ACK:
				if (first_start_recording_msg)
				{
					first_start_recording_msg = FALSE;
					continuous_recording = TRUE;
					gtk_widget_set_sensitive(btn_start_recording, FALSE);
					gtk_widget_set_sensitive(btn_stop_recording, TRUE);
				}
				recording = TRUE;	
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	
				recording_number = msg_item.additional_data;
				if (! (*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(3, path, *sys_time_ptr, recording_number))	
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *create_data_directory().");	
					exit(1);
				}
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_trial_status_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}	
				sprintf (temp, "RECORDING DAT%u", recording_number);
				gtk_label_set_text (GTK_LABEL (lbl_recording_status), temp);
				gtk_widget_set_sensitive(btn_cancel_recording, TRUE);
				break;
			case TRIAL_HAND_2_GUI_MSG_BROADCAST_STOP_RECORDING_MSG_ACK:
				if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_trial_status_history))	// this function handles history buffers
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *write_to_data_files().");		
					exit(1);
				}	
				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, *sys_time_ptr, &(classified_history->all_trials->history[recording_number])))	
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *fclose_all_data_files().");
					exit(1);
				}
				sprintf (temp, "FINISHED RECORDING DAT%u", recording_number);
				gtk_label_set_text (GTK_LABEL (lbl_recording_status), temp);
				recording = FALSE;	
				gtk_widget_set_sensitive(btn_cancel_recording, FALSE);
				if (stop_continuous_recording_request)
				{
					first_start_recording_msg = TRUE;
					continuous_recording = FALSE;
					stop_continuous_recording_request = FALSE;
					gtk_widget_set_sensitive(btn_stop_recording, FALSE);
					gtk_widget_set_sensitive(btn_start_recording, TRUE);
				}
				else	// it comes here when there is continous recording.
				{
					if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_BROADCAST_START_RECORDING, 0))
						return print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "enable_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");			
				}
				break;
			case TRIAL_HAND_2_GUI_MSG_BROADCAST_CANCEL_RECORDING_MSG_ACK:
				path_temp = NULL; path = NULL;
				path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
				path = &path_temp[7];   // since     uri returns file:///home/....	

				static_trial_status_history->buff_read_idx = static_trial_status_history->buff_write_idx;	

				recording_number = msg_item.additional_data;
				if (! (*fclose_all_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, *sys_time_ptr, &(classified_history->all_trials->history[recording_number])))	
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", "! *fclose_all_data_files().");
					exit(1);
				}
				if (! (*delete_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, recording_number))
				{
					print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *fdelete_all_data_files().");
					exit(1);
				}
				sprintf (temp, "DELETED DAT%u", recording_number);
				gtk_label_set_text (GTK_LABEL (lbl_recording_status), temp);
				recording = FALSE;	
				stop_continuous_recording_request = FALSE;
				continuous_recording = FALSE;
				first_start_recording_msg = TRUE;
				gtk_widget_set_sensitive(btn_start_recording, TRUE);
				gtk_widget_set_sensitive(btn_stop_recording, FALSE);
				gtk_widget_set_sensitive(btn_cancel_recording, FALSE);
				break;
			case TRIAL_HAND_2_GUI_MSG_TRIAL_STATUS_CHANGE:
				trial_status = msg_item.additional_data;
				get_trial_status_type_string(trial_status, temp);  
				gtk_label_set_text(GTK_LABEL (lbl_trial_status ), temp);
				switch (trial_status)
				{
					case TRIAL_STATUS_TRIALS_DISABLED:
						break;
					case TRIAL_STATUS_IN_TRIAL:
						gtk_widget_set_sensitive(btn_start_recording, FALSE);
						gtk_widget_set_sensitive(btn_cancel_recording, FALSE);					
						break;
					case TRIAL_STATUS_IN_REFRACTORY:
						sprintf (temp, "%u", classified_history->all_trials->buff_write_idx);
						gtk_label_set_text (GTK_LABEL (lbl_num_of_trials), temp);
						sprintf (temp, "%.2f", averaging_struct_get_mean(paradigm->all_success_average));
						gtk_label_set_text (GTK_LABEL (lbl_success_ratio_all), temp);

						sprintf (temp, "%u", classified_history->trial_types[0]->buff_write_idx);
						gtk_label_set_text (GTK_LABEL (lbl_num_of_left_trials), temp);
						sprintf (temp, "%.2f", averaging_struct_get_mean(paradigm->target_success_average[0]));
						gtk_label_set_text (GTK_LABEL (lbl_success_ratio_left_large), temp);

						sprintf (temp, "%u", classified_history->trial_types[1]->buff_write_idx);
						gtk_label_set_text (GTK_LABEL (lbl_num_of_right_trials), temp);
						sprintf (temp, "%.2f", averaging_struct_get_mean(paradigm->target_success_average[1]));
						gtk_label_set_text (GTK_LABEL (lbl_success_ratio_right_large), temp);
						if ( continuous_recording)
						{
							usleep(100000);
							if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_BROADCAST_STOP_RECORDING, 0))
								return print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "enable_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");	
						}
						else
						{
							gtk_widget_set_sensitive(btn_start_recording, TRUE);
						}
						break;
					case TRIAL_STATUS_START_TRIAL_AVAILABLE:	
						if (! continuous_recording)
						{
							gtk_widget_set_sensitive(btn_start_recording, TRUE);
						}
						break;
					case TRIAL_STATUS_GET_READY_TO_START:
						break;
					default:
						return print_message(BUG_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", "switch (trial_status) - default");
				}
				break;
			default:
				return print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", "switch (msg_item.msg_type) - default");
		}
	}
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, static_trial_status_history))	// this function handles history buffers
		{
			print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "timeout_callback", " *write_to_data_files().");		
			exit(1);
		}			
	}
	else
	{
		static_trial_status_history->buff_read_idx = static_trial_status_history->buff_write_idx;	
	}
	return TRUE;
}

static void reset_connections_button_func (void)
{
	// order via "gui 2 exp envi handler" & "gui 2 mov obj handler"
}


static void enable_trials_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_ENABLE_TRIAL_HANDLING, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "enable_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");		
	gtk_widget_set_sensitive(btn_disable_trials, TRUE);	
}

static void disable_trials_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_DISABLE_TRIAL_HANDLING, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "disable_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");		
}

static void quit_trials_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_QUIT, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "quit_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");		
}

static void auto_target_select_mode_on_button_func(void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_AUTO_TARGET_SELECTION_ON, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "auto_target_select_mode_on_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");	
}

static void auto_target_select_mode_off_button_func(void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_AUTO_TARGET_SELECTION_OFF, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "auto_target_select_mode_off_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");	
}

static void select_target_button_func (void)
{
	unsigned int target_idx;
	target_idx = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_select_target)));
	if (target_idx >= paradigm->num_of_robot_target_positions)
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "select_target_button_func", "target_idx >= static_trial_hand_paradigm->num_of_robot_target_positions.");	
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_SELECT_TARGET, target_idx))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "select_target_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");	
}


static void increase_robot_start_idx_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_INCREASE_ROBOT_START_POSITION_IDX, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "increase_robot_start_idx_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");		
}

static void decrease_robot_start_idx_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_DECREASE_ROBOT_START_POSITION_IDX, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "decrease_robot_start_idx_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");		
}

static void submit_trial_number_button_func (void)
{
	unsigned int trial_number;
	char temp[50];
	double trial_length = 0;

	trial_number = (unsigned int)atof(gtk_entry_get_text(GTK_ENTRY(entry_trial_number)));
	
	if (trial_number >= classified_history->all_trials->buffer_size)
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "disable_trials_button_func", "Invalid Trial Number.");	
	trial_length = classified_history->all_trials->history[trial_number].trial_end_time - classified_history->all_trials->history[trial_number].trial_start_time;
	sprintf (temp, "%.2f", trial_length/1000000000.0);
	gtk_label_set_text (GTK_LABEL (lbl_trial_length), temp);

	sprintf (temp, "%u",  classified_history->all_trials->history[trial_number].robot_start_position_idx);
	gtk_label_set_text (GTK_LABEL (lbl_robot_start_position_idx), temp);

	sprintf (temp, "%u", classified_history->all_trials->history[trial_number].robot_target_position_idx);
	gtk_label_set_text (GTK_LABEL (lbl_robot_target_position_idx), temp);

	sprintf (temp, "%u", classified_history->all_trials->history[trial_number].difficulty_level);
	gtk_label_set_text (GTK_LABEL (lbl_interrogated_difficulty_level), temp);

}

static void create_recording_folder_button_func (void)
{
	unsigned int path_len;
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	path_len = strlen(path_temp);
	if (strcmp(&(path_temp[path_len-8]),"EXP_DATA") == 0)
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "create_recording_folder_button_func", "Selected folder is /EXP_DATA main folder. Select a folder inside this folder.");				
	if ((*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(2, path, paradigm))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "create_recording_folder_button_func", " *create_main_directory().");				
}


static void start_recording_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_BROADCAST_START_RECORDING, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "start_recording_button_func ", "! write_to_gui_2_trial_hand_msg_buffer().");
}
static void stop_recording_button_func (void)
{
	stop_continuous_recording_request = TRUE;
	gtk_widget_set_sensitive(btn_stop_recording, FALSE);
}
static void cancel_recording_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_BROADCAST_CANCEL_RECORDING, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "enable_trials_button_func", "! write_to_gui_2_trial_hand_msg_buffer().");	
}

static void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
		print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "set_directory_btn_select_directory_to_save", "Couldn't find file: ./path_initial_directory.");
		print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "set_directory_btn_select_directory_to_save", "Couldn' t read ./path_initial_directory.");
			print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "set_directory_btn_select_directory_to_save", "/home is loaded as initial direcoty to create data folder.");
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

static void release_reward_button_func (void)
{
	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_RELEASE_REWARD, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "start_recording_button_func ", "! write_to_gui_2_trial_hand_msg_buffer().");	
}


static void start_trial_button_func (void)
{
#ifdef	SIMULATION_MODE

	if (!write_to_gui_2_trial_hand_msg_buffer(static_msgs_gui_2_trial_hand, *sys_time_ptr, GUI_2_TRIAL_HAND_MSG_START_TRIAL_REQUEST, 0))
		return (void)print_message(ERROR_MSG ,"TrialHandler", "GuiTrialHandler", "start_recording_button_func ", "! write_to_gui_2_trial_hand_msg_buffer().");	

#endif
}
