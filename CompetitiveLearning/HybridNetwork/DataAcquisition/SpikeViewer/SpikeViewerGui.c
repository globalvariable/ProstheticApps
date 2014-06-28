#include "SpikeViewerGui.h"

static GtkWidget *databox_signal;
static GtkWidget *databox_spike_shape;

static GtkWidget *box_signal;
static GtkWidget *box_spike_shape;

static GtkWidget *combo_mwa;
static GtkWidget *combo_chan;

static GtkWidget *clear_button;
static GtkWidget *pause_button;
static GtkWidget *threshold_button;
static GtkWidget *btn_filter_on_off;
static GtkWidget *btn_print_spike_end_buff;

static GtkWidget *btn_select_spike_thresholds_file_to_load;
static GtkWidget *btn_load_spike_thresholds_file; 

static GtkWidget *btn_select_folder_to_save_spike_thresholds_file;
static GtkWidget *btn_save_spike_thresholds_file; 
static GtkWidget *btn_select_folder_to_record_data;
static GtkWidget *btn_create_recording_folder;
static GtkWidget *btn_record_data;

static GtkWidget *entryThreshold;

static GdkColor color_bg_signal;
static GdkColor color_signal;
static GdkColor color_grid_signal;

static GdkColor color_bg_spike_shape;
static GdkColor color_spike_shape;

static float *X_raw;
static float *Y_raw;

static float *X_spike;
static float *Y_spike;

static int Y_spikes_idx;
static GPtrArray* Y_spikes_ptr;

static int display_mwa = 0;
static int display_mwa_chan = 0;

static int previous_start_idx_to_plot = 0;

static int disp_paused = 0;
static int print_spike_end_buff = 0;

static int blue_spike_time_stamp_buff_read_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};

static bool filter_on = FALSE;

static bool recording = FALSE;
static unsigned int recording_number = 0;

static bool set_directory_btn_select_directory_to_load(void);
static void set_directory_btn_select_directory_to_save(void);

static void clear_spike_screen(void);
static void clear_raw_data_screen(void);

static gboolean timeout_callback(gpointer user_data);
static gboolean recording_timeout_callback(gpointer user_data) ;

static gboolean combo_mwa_func (GtkDatabox * box);
static gboolean combo_chan_func (GtkDatabox * box);

static gboolean filter_on_off_button_func (GtkDatabox * box);
static gboolean threshold_but_func (GtkDatabox * box);
static gboolean load_spike_thresholds_file_button_func (GtkDatabox * box);
static gboolean save_spike_thresholds_file_button_func (GtkDatabox * box);
static gboolean pause_button_func (GtkDatabox * box);
static gboolean clear_screen_but_func (GtkDatabox * box);

static gboolean print_spike_end_buff_button_func (GtkDatabox * box);

static gboolean create_recording_folder_button_func (GtkDatabox * box);

static gboolean record_data_button_func (GtkDatabox * box);



bool create_spike_viewer_gui(GtkWidget *tabs)
{
	GtkWidget *frame, *frame_label, *table, *vbox, *hbox, *lbl;
	int i,j;

        frame = gtk_frame_new ("");
        frame_label = gtk_label_new ("     SpikeViewer     ");      
   
        gtk_notebook_append_page (GTK_NOTEBOOK (tabs), frame, frame_label);  


 	hbox = gtk_hbox_new(TRUE, 0);
        gtk_container_add (GTK_CONTAINER (frame), hbox);

	table = gtk_table_new(9,17,TRUE);
	gtk_box_pack_start(GTK_BOX(hbox),table, TRUE,TRUE,0);


	//   Buttons and Combos
	
	vbox = gtk_vbox_new(FALSE, 0);
   	gtk_table_attach_defaults(GTK_TABLE(table), vbox, 13,16, 6, 9);      // Put adjustments the rightmost

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);

 	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

        lbl = gtk_label_new("Microwire Array : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE, 0);

 	combo_mwa = gtk_combo_box_new_text();
 	char temp[5];	
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_mwa), temp);
	} 		
 	gtk_box_pack_start (GTK_BOX (hbox), combo_mwa, TRUE, TRUE, 0);
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_mwa), 0);
 	
 	combo_chan = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_CHAN_PER_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_chan), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_chan, TRUE, TRUE, 0);
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_chan), 0);
 	 	
        lbl = gtk_label_new(": Channel");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE, 0);
 
 
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 3);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
  	
        lbl = gtk_label_new("Threshold (mV): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);

        entryThreshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entryThreshold, FALSE,FALSE,0);
	gtk_widget_set_size_request(entryThreshold, 80, 25);

	gtk_entry_set_text (GTK_ENTRY(entryThreshold), "0.00");


	threshold_button = gtk_button_new_with_label("Submit");
	gtk_box_pack_start (GTK_BOX (hbox), threshold_button, TRUE, TRUE, 0);	

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	
	
	btn_select_spike_thresholds_file_to_load = gtk_file_chooser_button_new ("Select Spike Thresholds File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_spike_thresholds_file_to_load, TRUE,TRUE,0);
 	gtk_widget_set_size_request(btn_select_spike_thresholds_file_to_load, 120, 25);       
	set_directory_btn_select_directory_to_load();
	        
        btn_load_spike_thresholds_file = gtk_button_new_with_label("Load Thresholds");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_spike_thresholds_file,TRUE,TRUE, 0);	

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

  	btn_select_folder_to_save_spike_thresholds_file = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_save_spike_thresholds_file, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();

	btn_save_spike_thresholds_file = gtk_button_new_with_label("Save Thresholds File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_spike_thresholds_file,TRUE,TRUE, 0);
          	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 3);
  	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
        
	pause_button = gtk_button_new_with_label("Pause Display");
	gtk_box_pack_start (GTK_BOX (hbox), pause_button, TRUE, TRUE, 0);

	clear_button = gtk_button_new_with_label("Clear Spike Screen");
	gtk_box_pack_start (GTK_BOX (hbox), clear_button, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0); 
  	
 	btn_print_spike_end_buff = gtk_button_new_with_label("Enable Spikes Log Printing");
	gtk_box_pack_start (GTK_BOX (hbox), btn_print_spike_end_buff, TRUE, TRUE, 0); 		

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 3);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  		
	btn_filter_on_off = gtk_button_new_with_label("FILTER : OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_on_off, TRUE, TRUE, 0);	
	

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 3);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

  	btn_select_folder_to_record_data = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_record_data, FALSE,FALSE,0);

	btn_create_recording_folder = gtk_button_new_with_label("Create Folder");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_recording_folder, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

	btn_record_data = gtk_button_new_with_label("Record");
        gtk_box_pack_start(GTK_BOX(hbox),btn_record_data, TRUE,TRUE, 0);
  	
	// Raw Data Plot 

	X_raw = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	Y_raw = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	for (i=0; i<NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
	{
		X_raw[i] = ((float)i)*(RAW_DATA_DISP_DURATION_MS/NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	}	
	
	color_bg_signal.red = 65535;
	color_bg_signal.green = 65535;
	color_bg_signal.blue = 65535;

	color_signal.red = 0;
	color_signal.green = 0;
	color_signal.blue = 0;

	color_grid_signal.red = 50000;
	color_grid_signal.green = 50000;
	color_grid_signal.blue = 50000;



  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_signal, &databox_signal,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_signal, GTK_STATE_NORMAL, &color_bg_signal);	
   	gtk_databox_graph_add (GTK_DATABOX (box_signal), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X_raw, Y_raw, &color_signal, 0));  	
   	gtk_table_attach_defaults(GTK_TABLE(table), databox_signal, 0,12,0,9);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);	

	// Spike Shape Plot
	

	X_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_spike[i] = (float)i;
	}
	Y_spikes_idx = 0;
	Y_spikes_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_ptr, Y_spike);
	}

	color_bg_spike_shape.red = 0;
	color_bg_spike_shape.green = 0;
	color_bg_spike_shape.blue = 0;

	color_spike_shape.red = 65535;
	color_spike_shape.green = 65535;
	color_spike_shape.blue = 65535;

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_shape, &databox_spike_shape,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_spike_shape, GTK_STATE_NORMAL, &color_bg_spike_shape);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_ptr_array_index(Y_spikes_ptr,i);
		gtk_databox_graph_add (GTK_DATABOX (box_spike_shape), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_spike, Y_spike, &color_spike_shape, 0));
	}
   	gtk_table_attach_defaults(GTK_TABLE(table), databox_spike_shape, 12,17,0,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	


	g_signal_connect(G_OBJECT(combo_mwa), "changed", G_CALLBACK(combo_mwa_func), NULL);
	g_signal_connect(G_OBJECT(combo_chan), "changed", G_CALLBACK(combo_chan_func), NULL);

	g_signal_connect(G_OBJECT(btn_filter_on_off), "clicked", G_CALLBACK(filter_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(threshold_button), "clicked", G_CALLBACK(threshold_but_func), NULL);
	g_signal_connect(G_OBJECT(btn_load_spike_thresholds_file), "clicked", G_CALLBACK(load_spike_thresholds_file_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_save_spike_thresholds_file), "clicked", G_CALLBACK(save_spike_thresholds_file_button_func), NULL);	

	g_signal_connect(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), NULL);
	g_signal_connect(G_OBJECT(clear_button), "clicked", G_CALLBACK(clear_screen_but_func), NULL);

	g_signal_connect(G_OBJECT(btn_print_spike_end_buff), "clicked", G_CALLBACK(print_spike_end_buff_button_func), NULL);	

	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);	

	g_signal_connect(G_OBJECT(btn_record_data), "clicked", G_CALLBACK(record_data_button_func), NULL);	


	spike_viewer_initialize_data_read_write_handlers();

	for (i = 0; i< MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j< MAX_NUM_OF_MWA; j++)		
		{
			blue_spike_time_stamp_buff_read_idx[i][j] = blue_spike_time_stamp[i][j].buff_idx_write;
		}		
	}


	g_timeout_add(50, timeout_callback, box_signal);
	g_timeout_add(50, recording_timeout_callback, box_signal);

	return TRUE;

}


static gboolean timeout_callback(gpointer user_data) 
{
	int start_idx, i;
	int blue_spike_time_stamp_buff_end_idx, idx;
	int blue_spike_time_stamp_buff_interpolated_data_idx; 
	int spike_idx;
	float *Y_temp_spike;

	RecordingDataSample	*handling_data_chan_buff;
	RecordingDataChan	*recording_data_chan;

	InterpolatedDataSample	*interpolated_data_chan_buff;

	recording_data_chan = &(recording_data[display_mwa][display_mwa_chan]);

	if (filter_on)
	{
		handling_data_chan_buff = recording_data_chan->filtered_recording_data_buff;
	}
	else
	{
		handling_data_chan_buff = recording_data_chan->rec_data_buff;
	}

	start_idx = recording_data_chan->buff_idx_write / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY

	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section
			
	if ((start_idx != previous_start_idx_to_plot) && (!disp_paused))  // Do not plot the same section if it is the same as the previous one due to high refresh rate (high timeout_callback frequency)
	{	
		previous_start_idx_to_plot = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y_raw[i] = handling_data_chan_buff[i+start_idx];
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}
	
	idx = blue_spike_time_stamp_buff_read_idx[display_mwa][display_mwa_chan];
	blue_spike_time_stamp_buff_end_idx = blue_spike_time_stamp[display_mwa][display_mwa_chan].buff_idx_write;
	if (!disp_paused)
	{
		interpolated_data_chan_buff = interpolated_data[display_mwa][display_mwa_chan].data_buff;

		while (idx != blue_spike_time_stamp_buff_end_idx)
		{
			blue_spike_time_stamp_buff_interpolated_data_idx = blue_spike_time_stamp[display_mwa][display_mwa_chan].buffer[idx].interpolated_data_buff_idx;
			spike_idx = blue_spike_time_stamp_buff_interpolated_data_idx;

			Y_temp_spike = g_ptr_array_index(Y_spikes_ptr,Y_spikes_idx);
			Y_spikes_idx ++;
			if (Y_spikes_idx == SPIKE_MEM_TO_DISPLAY)
				Y_spikes_idx = 0;
			for (i = NUM_OF_SAMP_PER_SPIKE -1; i >= 0; i--)
			{
				Y_temp_spike[i] = interpolated_data_chan_buff[spike_idx];
				spike_idx -= 2;		// -= 2 due to interpolation
				if (spike_idx < 0)
					spike_idx	= INTERPOLATED_DATA_BUFF_SIZE + spike_idx;
			}

			if (print_spike_end_buff)
				printf ("%d %d %llu %d\n", display_mwa, display_mwa_chan, blue_spike_time_stamp[display_mwa][display_mwa_chan].buffer[idx].peak_time, blue_spike_time_stamp_buff_interpolated_data_idx);	
			idx++;	
			if (idx == BLUESPIKE_SORTED_SPIKE_BUFF_SIZE)
				idx = 0;	
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
	}
	blue_spike_time_stamp_buff_read_idx[display_mwa][display_mwa_chan] = blue_spike_time_stamp_buff_end_idx;
	return TRUE;  

}



static bool set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("DataAcquisition/SpikeViewer/path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeViewer: Couldn't find file: DataAcquisition/SpikeViewer/path_initial_directory\n"); 
       		printf ("ERROR: SpikeViewer: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load),"/home");
		return FALSE;
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeViewer: Couldn' t read DataAcquisition/SpikeViewer/path_initial_directory\n"); 
       			printf ("ERROR: SpikeViewer: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load),"/home");
			fclose(fp); 		
			return FALSE;
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load), gfile_path, NULL);
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
       	if ((fp = fopen("DataAcquisition/SpikeViewer/path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeViewer: Couldn't find file: DataAcquisition/SpikeViewer/path_initial_directory\n"); 
       		printf ("ERROR: SpikeViewer: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeViewer: Couldn' t read DataAcquisition/SpikeViewer/path_initial_directory\n"); 
       			printf ("ERROR: SpikeViewer: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),"/home");
		}
		else
		{
			line[strlen(line)-16] = 0;   /// to get the folder instead of ../../DaqConfig file
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),line);
		}
		fclose(fp); 		
	}  	 
}

static gboolean combo_mwa_func (GtkDatabox * box)
{
	int idx;
	char thres[20];
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_mwa));
	if ((idx < 0) || (idx >= MAX_NUM_OF_MWA))
	{
		printf("SpikeViewer:\n");	
		printf ("BUG: Selected combobox index from MWA Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array selected automatically\n");
		idx = 0;
	}
	display_mwa = idx;
	blue_spike_time_stamp_buff_read_idx[display_mwa][display_mwa_chan] = blue_spike_time_stamp[display_mwa][display_mwa_chan].buff_idx_write;

	sprintf(thres, "%.2f" , spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);	
	clear_spike_screen();
	clear_raw_data_screen();		
	return TRUE;	
}
static gboolean combo_chan_func (GtkDatabox * box)
{
	int idx;
	char thres[20];	
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_chan));
	if ((idx < 0) || (idx >= MAX_NUM_OF_CHAN_PER_MWA))
	{
		printf("SpikeViewer:\n");	
		printf ("BUG: Selected combobox index from MWA Channel Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array selected automatically\n");
		idx = 0;
	}
	display_mwa_chan = idx;	
	blue_spike_time_stamp_buff_read_idx[display_mwa][display_mwa_chan] = blue_spike_time_stamp[display_mwa][display_mwa_chan].buff_idx_write;

	sprintf(thres, "%.2f" , spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);	
	clear_spike_screen();
	clear_raw_data_screen();		
	return TRUE;	
}

static void clear_spike_screen(void)
{
	float *Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
}

static void clear_raw_data_screen(void)
{
	int i;

	for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
	{
		Y_raw[i] = 0;
	}		
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
}


static gboolean filter_on_off_button_func (GtkDatabox * box)
{
	if (filter_on)
	{
		filter_on = FALSE;
		gtk_button_set_label (GTK_BUTTON(btn_filter_on_off),"FILTER : OFF");
	}
	else
	{
		filter_on = TRUE;
		gtk_button_set_label (GTK_BUTTON(btn_filter_on_off),"FILTER : ON");
	}
	return TRUE;	
}


static gboolean threshold_but_func (GtkDatabox * box)
{
	float threshold = atof(gtk_entry_get_text(GTK_ENTRY(entryThreshold)));



	if (threshold <= 0.0)
	{
		if (threshold == 0.0)
		{
			printf("Spike detection is disable for this channel by applying 0.0 Volts as threshold\n");		
		}
		spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan] = threshold;
	}
	else
	{
		printf("WARNING: Threshold cannot be greater than 0.0\n");
		printf("WARNING: Submit Threshold cancelled\n");		
	}
	return TRUE;

}


static gboolean load_spike_thresholds_file_button_func (GtkDatabox * box)
{

	char *path = NULL;

	int version;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load));
	path = &(path[7]);                  ///     file:///path

	if (!spike_viewer_get_format_version(&version, path))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "! get_format_version()."); 
	
	if (!((*read_spike_thresholds_data[version])(1, path)))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "! *read_spike_thresholds_data[version]()."); 

	return print_message(INFO_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "Succesuflly loaded SpikeThresholds data file.");

}



static gboolean save_spike_thresholds_file_button_func (GtkDatabox * box)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if (! (*write_spike_thresholds_data[DATA_FORMAT_VERSION])(1, path))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "save_spike_thresholds_file_button_func", "! *write_spike_thresholds_data()."); 		
	return print_message(INFO_MSG ,"SpikeViewer", "SpikeViewer", "save_spike_thresholds_file_button_func", "Succesuflly saved SpikeThresholds data file.");
}


static gboolean pause_button_func (GtkDatabox * box)
{
	if (disp_paused)
	{
		blue_spike_time_stamp_buff_read_idx[display_mwa][display_mwa_chan] = blue_spike_time_stamp[display_mwa][display_mwa_chan].buff_idx_write;
		disp_paused = 0;
		gtk_button_set_label (GTK_BUTTON(pause_button),"Pause");
	}
	else
	{
		disp_paused = 1;
		gtk_button_set_label (GTK_BUTTON(pause_button),"Resume");
	}	
	return TRUE;	
}

static gboolean clear_screen_but_func (GtkDatabox * box)
{
	clear_spike_screen();
	clear_raw_data_screen();
	return TRUE;	
}

static gboolean print_spike_end_buff_button_func (GtkDatabox * box)
{
	if (print_spike_end_buff)
	{
		print_spike_end_buff = 0;
		gtk_button_set_label (GTK_BUTTON(btn_print_spike_end_buff),"Enable Spikes Log Printing");
		printf("SpikeViewer: Spike End Buffer log printing disabled\n");
	}
	else
	{
		print_spike_end_buff = 1;
		gtk_button_set_label (GTK_BUTTON(btn_print_spike_end_buff),"Disable Spikes Log Printing");
		printf("SpikeViewer: Spike End Buffer log printing enabled\n");
		printf ("Data Format:  [mwa] [channel] [spike_peak_time] [spike_end_index_in_recording_data_buffer]");	
	}	
	return TRUE;	
}

static gboolean create_recording_folder_button_func (GtkDatabox * box)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_record_data));
	path = &path_temp[7];   // since     uri returns file:///home/....	
		
	if ((*create_main_directory[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(1, path))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"SpikeViewer", "Gui", "create_recording_folder_button_func", " *create_main_directory().");
	return TRUE;
}

static gboolean record_data_button_func (GtkDatabox * box)
{
	char *path_temp, *path; 
	path_temp = NULL; path = NULL;
	if (recording)
	{
		if (! (*fclose_all_data_files[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(3, &(rt_tasks_data->current_system_time), recording_data, sorted_spikes))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "timeout_callback", " *fclose_all_data_file().");		
			exit(1);
		}
		recording = FALSE;	

		gtk_button_set_label (GTK_BUTTON(btn_record_data),"Record");
	}
	else
	{
		path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_record_data));
		path = &path_temp[7];   // since     uri returns file:///home/....	
		if (!(*create_data_directory[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(5, path, &(rt_tasks_data->current_system_time), recording_number, recording_data, sorted_spikes))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "timeout_callback", " *create_data_directory().");		
			exit(1);
		}
		recording = TRUE;	
		recording_number++;
		gtk_button_set_label (GTK_BUTTON(btn_record_data),"Stop");
	}
	return TRUE;
}


static gboolean recording_timeout_callback(gpointer user_data) 
{
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(0))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "recording_timeout_callback", " *write_to_data_files().");		
			exit(1);
		}	
	}
	return TRUE;
}
