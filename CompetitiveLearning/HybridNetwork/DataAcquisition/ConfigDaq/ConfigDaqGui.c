#include "ConfigDaqGui.h"


static GtkWidget *entry_daq_card_num;
static GtkWidget *entry_daq_card_chan_range_lower;
static GtkWidget *entry_daq_card_chan_range_upper;

static GtkWidget *entry_mwa_num;
static GtkWidget *entry_mwa_chan_range_lower;
static GtkWidget *entry_mwa_chan_range_upper;

static GtkWidget *btn_turn_daq_on_off;
static GtkWidget *btn_map_channels;
static GtkWidget *btn_interrogate_mapping;
static GtkWidget *btn_cancel_all_mapping;

static GtkWidget *btn_select_config_daq_file_to_load;
static GtkWidget *btn_load_config_daq_file; 

static GtkWidget *btn_select_folder_to_save_config_daq_file;
static GtkWidget *btn_save_config_daq_file; 

static GtkWidget *btn_exit; 


static bool set_directory_btn_select_directory_to_load(void);
static void set_directory_btn_select_directory_to_save(void);


static void turn_daq_on_off_button_func(void); 
static void map_channels_button_func(void); 
static void interrogate_mapping_button_func(void); 
static void cancel_all_mapping_button_func(void); 
static void load_config_daq_file_button_func(void); 
static void save_config_daq_file_button_func(void); 
static void exit_button_func(void); 

bool create_config_daq_gui(GtkWidget *tabs)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl, *vbox1, *hbox1;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     DaqCard     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  


 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(3,4,TRUE);
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);


///////////////////////////////////////////// FIRST COLUMN  ///////////////////////////////////////////////////////////////
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 0,1, 0, 3);  

  	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE,0);


    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);
	
    	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE,0);

	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	

	lbl = gtk_label_new("DAQ Card");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Card #			: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_num, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_num), "0");
  	gtk_widget_set_size_request(entry_daq_card_num, 30, 25) ;
  	  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Lower)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_chan_range_lower = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_chan_range_lower, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_chan_range_lower), "0");
  	gtk_widget_set_size_request(entry_daq_card_chan_range_lower, 30, 25) ;
  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Upper)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_chan_range_upper = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_chan_range_upper, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_chan_range_upper), "0");
  	gtk_widget_set_size_request(entry_daq_card_chan_range_upper, 30, 25) ;
  
      	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE,0);
	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,FALSE,FALSE, 0);	
        	
	lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);	
        
        lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);
        
        lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);
  	
    	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	

	lbl = gtk_label_new("MEA");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Array #			: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_num, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_num), "0");
  	gtk_widget_set_size_request(entry_mwa_num, 30, 25) ;
  		
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Lower)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_chan_range_lower = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_chan_range_lower, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_chan_range_lower), "0");
	gtk_widget_set_size_request(entry_mwa_chan_range_lower, 30, 25) ;
	  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Upper)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_chan_range_upper = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_chan_range_upper, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_chan_range_upper), "0");	
	gtk_widget_set_size_request(entry_mwa_chan_range_upper, 30, 25) ;
		
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,10);		
	
	btn_map_channels = gtk_button_new_with_label("Map Channels");
	gtk_box_pack_start (GTK_BOX (hbox), btn_map_channels, TRUE, FALSE, 0);
	
	btn_interrogate_mapping = gtk_button_new_with_label("Interrogate Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_mapping, TRUE, FALSE, 0);	
	
	btn_cancel_all_mapping = gtk_button_new_with_label("Cancel All Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_cancel_all_mapping, TRUE, FALSE, 0);		
	
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,10);		
	
	btn_turn_daq_on_off = gtk_button_new_with_label("DAQ Card : OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_turn_daq_on_off, TRUE, FALSE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	
	
	btn_select_config_daq_file_to_load = gtk_file_chooser_button_new ("Select Maps File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_config_daq_file_to_load, TRUE,TRUE,0);
	set_directory_btn_select_directory_to_load();

	btn_load_config_daq_file = gtk_button_new_with_label("Load ConfigDaq File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_config_daq_file,TRUE,TRUE, 0);		

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	

  	btn_select_folder_to_save_config_daq_file = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_save_config_daq_file, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();
	
	btn_save_config_daq_file = gtk_button_new_with_label("Save ConfigDaq File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_config_daq_file,TRUE,TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	

	btn_exit = gtk_button_new_with_label("EXIT");
	gtk_box_pack_start (GTK_BOX (hbox), btn_exit, TRUE, FALSE, 0);

	g_signal_connect(G_OBJECT(btn_turn_daq_on_off), "clicked", G_CALLBACK(turn_daq_on_off_button_func), NULL);       
        g_signal_connect(G_OBJECT(btn_map_channels), "clicked", G_CALLBACK(map_channels_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_interrogate_mapping), "clicked", G_CALLBACK(interrogate_mapping_button_func), NULL);      
        g_signal_connect(G_OBJECT(btn_cancel_all_mapping), "clicked", G_CALLBACK(cancel_all_mapping_button_func), NULL);              
        g_signal_connect(G_OBJECT(btn_load_config_daq_file), "clicked", G_CALLBACK(load_config_daq_file_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_save_config_daq_file), "clicked", G_CALLBACK(save_config_daq_file_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_exit), "clicked", G_CALLBACK(exit_button_func), NULL);

	config_daq_initialize_data_read_write_handlers();

	return TRUE;
}


void turn_daq_on_off_button_func(void)
{
	if (daq_cards_on)
	{		
		gtk_widget_set_sensitive(btn_map_channels, TRUE);
		gtk_widget_set_sensitive(btn_load_config_daq_file, TRUE);	
		gtk_widget_set_sensitive(btn_cancel_all_mapping, TRUE);
		gtk_widget_set_sensitive(btn_exit, TRUE);
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : OFF");
		daq_cards_on	= FALSE;	
	}
	else
	{
		gtk_widget_set_sensitive(btn_map_channels, FALSE);
		gtk_widget_set_sensitive(btn_load_config_daq_file, FALSE);		
		gtk_widget_set_sensitive(btn_cancel_all_mapping, FALSE);
		gtk_widget_set_sensitive(btn_exit, FALSE);
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : ON");
		daq_cards_on	= TRUE;	
		start_acquisition();										
	}		
}

void map_channels_button_func(void)
{
	int daq_num, daq_chan_start, daq_chan_end;
	int mwa_num, mwa_chan_start, mwa_chan_end;

	daq_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_num)));
	daq_chan_start = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_chan_range_lower)));	
	daq_chan_end = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_chan_range_upper)));	
	
	mwa_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_num)));
	mwa_chan_start = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_chan_range_lower)));	
	mwa_chan_end = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_chan_range_upper)));		
	
	if (! config_daq_mwa_map_data_range(daq_num, daq_chan_start, daq_chan_end, mwa_num, mwa_chan_start, mwa_chan_end))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "map_channels_button_func", "! config_daq_mwa_map_data_range()."); 		

	return;
}

void interrogate_mapping_button_func(void)
{
	printf("Interrogating mapping\n");
	interrogate_daq_mwa_map();
	printf("Interrogating mapping...complete\n");	
	return;
}

void cancel_all_mapping_button_func(void)
{
	cancel_daq_mwa_map_data();
}


static bool set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("./DataAcquisition/ConfigDaq/path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load),"/home");
		return FALSE;
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load),"/home");
			fclose(fp); 		
			return FALSE;
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load), gfile_path, NULL);
			g_object_unref(gfile_path);
			fclose(fp); 		
			return TRUE;
		}
	}  	 
}

static void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./DataAcquisition/ConfigDaq/path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),"/home");
		}
		else
		{
			line[strlen(line)-10] = 0;   /// to get the folder instead of ../../DaqConfig file
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),line);
		}
		fclose(fp); 		
	}  	 
}

static void load_config_daq_file_button_func(void)
{
	char *path = NULL;

	int version;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load));
	path = &(path[7]);                  ///     file:///path

	if (!config_daq_get_format_version(&version, path))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "! get_format_version()."); 
	
	if (!((*read_config_daq_data[version])(1, path)))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "! *read_config_daq_data[version]()."); 

	return (void)print_message(INFO_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "Succesuflly loaded ConfigDaq data file.");
}

static void save_config_daq_file_button_func(void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if (! (*write_config_daq_data[DATA_FORMAT_VERSION])(1, path))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "save_config_daq_file_button_func", "! *write_config_daq_data()."); 		

	return (void)print_message(INFO_MSG ,"ConfigDaq", "ConfigDaq", "save_config_daq_file_button_func", "Succesuflly saved ConfigDaq data file.");
}

static void exit_button_func(void)
{
	rt_periodic_task_stay_alive = 0;
	sleep(1);
	gtk_main_quit();
}
