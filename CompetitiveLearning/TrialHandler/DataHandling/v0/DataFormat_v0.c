#include "DataFormat_v0.h"


#define META_DATA_FILE_PTR_ARR_IDX			0
#define TRIAL_DATA_FILE_PTR_ARR_IDX			1
#define TRIAL_STATUS_DATA_FILE_PTR_ARR_IDX 	2
#define NUM_OF_DATA_FILE_PER_RECORDING		3


static FILE *file_ptr_arr[NUM_OF_DATA_FILE_PER_RECORDING];

static int create_main_meta_file(char *main_directory_path, TrialHandParadigmRobotReach *paradigm);
static int create_data_files(TimeStamp rec_start, char *data_directory_path);
static int create_meta_data(TimeStamp rec_start, char *data_directory_path);
static int create_trial_data(char *data_directory_path);
static int create_trial_status_data(char *data_directory_path);
static int write_to_trial_status_data(TrialStatusHistory *trial_status_history);
static int close_meta_data(TimeStamp rec_end);
static int close_trial_data(TrialData *trial_data);
static int close_trial_status_data(void);
static int delete_data_files(char *data_directory_path);
static int delete_meta_data(char *data_directory_path);
static int delete_trial_data(char *data_directory_path);
static int delete_trial_status_data(char *data_directory_path);

int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
  	TrialHandParadigmRobotReach *paradigm;
	DIR	*dir_main_folder;
	char main_directory_path[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	paradigm = va_arg ( arguments, TrialHandParadigmRobotReach *); 
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/TrialHandlerRecord");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("TrialHandler: ERROR: path: %s already has TrialHandlerRecord folder.\n", path_chooser);		
        	printf ("TrialHandler: ERROR: Select another folder or delete TrialHandlerRecord directory.\n\n");	
		closedir(dir_main_folder );	        		
                return 0;
        }
        closedir(dir_main_folder );
        
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("TrialHandler: Created /TrialHandlerRecord folder in: %s.\n", path_chooser);
        printf ("TrialHandler: /TrialHandlerRecord path is: %s.\n", main_directory_path); 
        
	if (!create_main_meta_file(main_directory_path, paradigm))
		return 0;

	if ((fp = fopen("./path_initial_directory", "w")) == NULL)  { printf ("ERROR: TrialHandler: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", path_chooser);
	fclose (fp);		

	return 1;
}

int create_data_directory_v0(int num, ...)
{
	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	char *path_chooser;
	char data_directory_path[600];
	TimeStamp rec_start;

	unsigned int i, data_directory_cntr, ret, recording_number;
 
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	rec_start = va_arg ( arguments, TimeStamp); 
	recording_number = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	for (i = 0; i < NUM_OF_DATA_FILE_PER_RECORDING; i++)
		file_ptr_arr[i] = NULL;

	data_directory_cntr = recording_number;
	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("TrialHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("TrialHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/TrialHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("TrialHandler: ERROR: path: %s already has %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	  
      		        
	if ((ret = mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH)) != 0)
	{
		printf("Couldn't makedir: %s\n, Reason: %d\n", data_directory_path, ret);
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "create_data_directory_v0", "mkdir() != 0");
	}	

	if (! create_data_files(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "create_data_directory_v0", "! create_data_files.");

	return 1;
}

int fclose_all_data_files_v0(int num, ...)
{
	TrialData *trial_data;
	TimeStamp rec_end;

  	va_list arguments;
	va_start ( arguments, num );  
	rec_end = va_arg ( arguments, TimeStamp);  
	trial_data = va_arg ( arguments, TrialData*); 
	va_end ( arguments );	

	if (! close_meta_data(rec_end))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", " fclose_all_data_files_v0", "! close_meta_data");

	if (! close_trial_data(trial_data))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", " fclose_all_data_files_v0", "! close_meta_data");
	if (! close_trial_status_data())
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", " fclose_all_data_files_v0", "! close_trial_status_data");

	return 1;
}

int delete_data_directory_v0(int num, ...)   // call it after fclose_all_data_files_v0
{
	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	char *path_chooser;
	char data_directory_path[600];

	unsigned int data_directory_cntr, recording_number;
 
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	recording_number = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	data_directory_cntr = recording_number;
	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("TrialHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("TrialHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/TrialHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("TrialHandler: ERROR: path: %s does not have %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	 

	if (! delete_data_files(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "delete_data_directory_v0", "! delete_data_files(data_directory_path)");

	if (rmdir(data_directory_path) != 0) 
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "delete_data_directory_v0", "! remove(data_directory_path)");

	return 1;
}

int write_to_data_files_v0(int num, ...)
{
	TrialStatusHistory *trial_status_history;
  	va_list arguments;
	va_start ( arguments, num );   
    	trial_status_history = va_arg ( arguments, TrialStatusHistory *); 
	va_end ( arguments );

	if (! write_to_trial_status_data(trial_status_history))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_trial_status_data()");

	return 1;
}

int write_notes_to_files_v0(int num, ...)
{
	return 1;
}
int write_additional_notes_to_files_v0(int num, ...)
{
	return 1;
}



static int create_main_meta_file(char *main_directory_path, TrialHandParadigmRobotReach *paradigm)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	unsigned int i;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: TrialHandler: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------TrialHandler - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	// already includes \n
	fprintf(fp,"NUM_OF_DIFFICULTY_LEVELS\t%u\n", paradigm->num_of_difficulty_levels);
	for (i = 0; i < paradigm->num_of_difficulty_levels; i++)
		fprintf(fp,"MAX_TRIAL_LENGTH[%u]\t%llu\n", i, paradigm->max_trial_length[i]); 	

	fprintf(fp,"TRIAL_REFRACTORY\t%llu\n", paradigm->trial_refractory); 	
	fprintf(fp,"NUM_OF_ROBOT_START_POSITIONS\t%u\n", paradigm->num_of_robot_start_positions); 
	fprintf(fp,"NUM_OF_ROBOT_TARGET_POSITIONS\t%u\n", paradigm->num_of_robot_target_positions); 
	fprintf(fp,"NUM_OF_TARGET_LED_COMPONENTS\t%u\n", paradigm->num_of_target_led_components); 
	for (i = 0; i < paradigm->num_of_target_led_components; i++)
		fprintf(fp,"TARGET_LED_COMPONENT_INDEXES_LIST[%u]\t%u\n", i, paradigm->target_led_component_indexes_list[i]); 

	fprintf(fp,"TRIAL_STATUS_TYPES------------------\n");
	fprintf(fp,"TRIAL_STATUS_NULL\t%u\n", TRIAL_STATUS_NULL); 
	fprintf(fp,"TRIAL_STATUS_TRIALS_DISABLED\t%u\n", TRIAL_STATUS_TRIALS_DISABLED); 
	fprintf(fp,"TRIAL_STATUS_IN_TRIAL\t%u\n", TRIAL_STATUS_IN_TRIAL); 
	fprintf(fp,"TRIAL_STATUS_IN_REFRACTORY\t%u\n", TRIAL_STATUS_IN_REFRACTORY); 
	fprintf(fp,"TRIAL_STATUS_START_TRIAL_AVAILABLE\t%u\n", TRIAL_STATUS_START_TRIAL_AVAILABLE); 
	fprintf(fp,"TRIAL_STATUS_GET_READY_TO_START\t%u\n", TRIAL_STATUS_GET_READY_TO_START); 
	fprintf(fp,"----------TrialHandler - End of Main Meta File----------\n");
	fclose(fp);
	return 1;
}

static int create_data_files(TimeStamp rec_start, char *data_directory_path)
{
	if (!create_meta_data(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "create_data_files", "! create_meta_data");
	
	if (!create_trial_data(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "create_data_files", "! create_trial_data");

	if (!create_trial_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "create_data_files", "! create_trial_data");
	
	return 1;
}

static int create_meta_data(TimeStamp rec_start, char *data_directory_path)
{
	char temp[600];
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: TrialHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------TrialHandler - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"RECORDING START\t%llu\n", rec_start);		
	file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX] = fp;


	return 1;	
}

static int create_trial_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/trial_data");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: TrialHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------TrialHandler - TrialData File----------\n");

	file_ptr_arr[TRIAL_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int create_trial_status_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/trial_status");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: TrialHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------TrialHandler - TrialStatus File----------\n");

	file_ptr_arr[TRIAL_STATUS_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int write_to_trial_status_data(TrialStatusHistory *trial_status_history)
{
	TrialStatusHistoryItem item;
	FILE *fp = file_ptr_arr[TRIAL_STATUS_DATA_FILE_PTR_ARR_IDX];

	while (get_next_trial_status_history_item(trial_status_history, &item))
	{
		fprintf(fp,"%llu\t%u\n", item.time, item.new_status);	
	}

	return 1;	
}


static int close_meta_data(TimeStamp rec_end)
{
	FILE *fp = file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX];
	fprintf(fp,"RECORDING END\t%llu\n", rec_end);	
	fprintf(fp,"----------TrialHandler - End of Meta File----------\n");
	fclose(fp);
	return 1;
}

static int close_trial_data(TrialData *trial_data)
{
	FILE *fp = file_ptr_arr[TRIAL_DATA_FILE_PTR_ARR_IDX];

	fprintf(fp,"trial_start_time\t%llu\n", trial_data->trial_start_time);
	fprintf(fp,"trial_end_time\t%llu\n", trial_data->trial_end_time);
	fprintf(fp,"trial_length\t%llu\n", trial_data->trial_length);
	fprintf(fp,"target_led_component_list_idx\t%u\n", trial_data->target_led_component_list_idx);
	fprintf(fp,"robot_start_position_idx\t%u\n", trial_data->robot_start_position_idx);
	fprintf(fp,"robot_target_position_idx\t%u\n", trial_data->robot_target_position_idx);
	fprintf(fp,"binary_reward\t%u\n", trial_data->binary_reward);
	fprintf(fp,"trial_incomplete\t%u\n", trial_data->trial_incomplete);
	fprintf(fp,"rewarding_threshold.r_x\t%.15f\n", trial_data->rewarding_threshold.r_x);
	fprintf(fp,"rewarding_threshold.r_y\t%.15f\n", trial_data->rewarding_threshold.r_y);
	fprintf(fp,"rewarding_threshold.r_z\t%.15f\n", trial_data->rewarding_threshold.r_z);
	fprintf(fp,"gui_selected_target_position_idx\t%u\n", trial_data->gui_selected_target_position_idx);
	fprintf(fp,"auto_target_select_mode_on\t%u\n", trial_data->auto_target_select_mode_on);

	fprintf(fp,"----------TrialHandler - End of TrialData File----------\n");
	fclose(fp);
	return 1;
}

static int close_trial_status_data(void)
{
	FILE *fp = file_ptr_arr[TRIAL_STATUS_DATA_FILE_PTR_ARR_IDX];

	fprintf(fp,"----------TrialHandler - End of TrialStatus File----------\n");
	fclose(fp);
	return 1;
}


static int delete_data_files(char *data_directory_path)
{
	if (! delete_meta_data(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_meta_data(data_directory_path)");
	
	if (! delete_trial_data(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_trial_data(data_directory_path)");

	if (! delete_trial_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"TrialHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_trial_status_data(data_directory_path)");

	return 1;
}

static int delete_meta_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	
	if (remove(temp) != 0)  { printf ("ERROR: TrialHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_trial_data(char *data_directory_path)
{
	char temp[600];

	strcpy(temp, data_directory_path);
	strcat(temp, "/trial_data");	
	if (remove(temp) != 0)  { printf ("ERROR: TrialHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_trial_status_data(char *data_directory_path)
{
	char temp[600];

	strcpy(temp, data_directory_path);
	strcat(temp, "/trial_status");	
	if (remove(temp) != 0)  { printf ("ERROR: TrialHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

