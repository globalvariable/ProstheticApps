#include "DataFormat_v0.h"

#define META_DATA_FILE_PTR_ARR_IDX								0
#define INPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX			1
#define OUTPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX 			2
#define NUM_OF_DATA_FILE_PER_RECORDING							3


static FILE *file_ptr_arr[NUM_OF_DATA_FILE_PER_RECORDING];


static int create_main_meta_file(char *main_directory_path, ExpEnviHandParadigmRobotReach *paradigm, ExpEnviData *exp_envi_data);
static int create_data_files(TimeStamp rec_start, char *data_directory_path);
static int create_meta_data(TimeStamp rec_start, char *data_directory_path);
static int create_input_component_status_data(char *data_directory_path);
static int create_output_component_status_data(char *data_directory_path);
static int write_to_input_component_status_data(ExpEnviInputStatusHistory *exp_envi_input_status_history);
static int write_to_output_component_status_data(ExpEnviOutputStatusHistory *exp_envi_output_status_history);
static int close_meta_data(TimeStamp rec_end);
static int close_input_component_status_data(void);
static int close_output_component_status_data(void);
static int delete_data_files(char *data_directory_path);
static int delete_meta_data(char *data_directory_path);
static int delete_input_component_status_data(char *data_directory_path);
static int delete_output_component_status_data(char *data_directory_path);

int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
  	ExpEnviHandParadigmRobotReach *paradigm;
	ExpEnviData *exp_envi_data;
	DIR	*dir_main_folder;
	char main_directory_path[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	paradigm = va_arg ( arguments, ExpEnviHandParadigmRobotReach *); 
	exp_envi_data = va_arg ( arguments, ExpEnviData *); 
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/ExpEnviHandlerRecord");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("ExpEnviHandler: ERROR: path: %s already has ExpEnviHandlerRecord folder.\n", path_chooser);		
        	printf ("ExpEnviHandler: ERROR: Select another folder or delete ExpEnviHandlerRecord directory.\n\n");	
		closedir(dir_main_folder );	        		
                return 0;
        }
        closedir(dir_main_folder );
        
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("ExpEnviHandler: Created /ExpEnviHandlerRecord folder in: %s.\n", path_chooser);
        printf ("ExpEnviHandler: /ExpEnviHandlerRecord path is: %s.\n", main_directory_path); 
        
	if (!create_main_meta_file(main_directory_path, paradigm, exp_envi_data))
		return 0;

	if ((fp = fopen("./path_initial_directory", "w")) == NULL)  { printf ("ERROR: ExpEnviHandler: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
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
		printf("ExpEnviHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("ExpEnviHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/ExpEnviHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("ExpEnviHandler: ERROR: path: %s already has %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	  
      		        
	if ((ret = mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH)) != 0)
	{
		printf("Couldn't makedir: %s\n, Reason: %d\n", data_directory_path, ret);
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_directory_v0", "mkdir() != 0");
	}	

	if (! create_data_files(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_directory_v0", "! create_data_files.");


	return 1;
}
int fclose_all_data_files_v0(int num, ...)
{
	TimeStamp rec_end;

  	va_list arguments;
	va_start ( arguments, num );  
	rec_end = va_arg ( arguments, TimeStamp);  
	va_end ( arguments );	

	if (! close_meta_data(rec_end))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_directory_v0", "! close_meta_data");

	if (! close_input_component_status_data())
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_directory_v0", "! close_input_component_status_data");

	if (! close_output_component_status_data())
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_directory_v0", "! close_output_component_status_data");

	return 1;
}

int delete_data_directory_v0(int num, ...)
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
		printf("ExpEnviHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("ExpEnviHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/ExpEnviHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("ExpEnviHandler: ERROR: path: %s does not have %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	 

	if (! delete_data_files(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "delete_data_directory_v0", "! delete_data_files(data_directory_path)");

	if (rmdir(data_directory_path) != 0) 
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "delete_data_directory_v0", "! remove(data_directory_path)");

	return 1;
}

int write_to_data_files_v0(int num, ...)
{
	ExpEnviInputStatusHistory *exp_envi_input_status_history;
	ExpEnviOutputStatusHistory *exp_envi_output_status_history;

  	va_list arguments;
	va_start ( arguments, num );   
    	exp_envi_input_status_history = va_arg ( arguments, ExpEnviInputStatusHistory *); 
    	exp_envi_output_status_history = va_arg ( arguments, ExpEnviOutputStatusHistory *); 
	va_end ( arguments );

	if (! write_to_input_component_status_data(exp_envi_input_status_history))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_input_component_status_data()");

	if (! write_to_output_component_status_data(exp_envi_output_status_history))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_output_component_status_data()");

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

static int create_main_meta_file(char *main_directory_path, ExpEnviHandParadigmRobotReach *paradigm, ExpEnviData *exp_envi_data)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	unsigned int i;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: ExpEnviHandler: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------ExpEnviHandler - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	// already includes \n
	fprintf(fp,"NUM_OF_TARGET_LED_COMPONENTS\t%u\n", paradigm->num_of_target_led_components); 
	for (i = 0; i < paradigm->num_of_target_led_components; i++)
		fprintf(fp,"TARGET_LED_COMPONENT_INDEXES_LIST[%u]\t%u\n", i, paradigm->target_led_component_indexes_list[i]); 

	fprintf(fp,"EXP_ENVI_INPUT_COMPONENTS_TYPES------------------\n");
	fprintf(fp,"EXP_ENVI_INPUT_COMPONENT_NULL\t%u\n", EXP_ENVI_INPUT_COMPONENT_NULL	); 
	fprintf(fp,"EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE\t%u\n", EXP_ENVI_INPUT_COMPONENT_IR_BEAM_NOSE_POKE	); 
	fprintf(fp,"EXP_ENVI_INPUT_COMPONENT_LEFT_LEVER\t%u\n", EXP_ENVI_INPUT_COMPONENT_LEFT_LEVER	); 
	fprintf(fp,"EXP_ENVI_INPUT_COMPONENT_RIGHT_LEVER\t%u\n", EXP_ENVI_INPUT_COMPONENT_RIGHT_LEVER	);
 	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENTS_TYPES------------------\n");
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_VALVE_CENTER\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_VALVE_CENTER); 
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_BUZZER\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_BUZZER	); 
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_LEFT_TARGET_LED\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_LEFT_TARGET_LED);
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_RIGHT_TARGET_LED\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_RIGHT_TARGET_LED);
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_GUIDE_LED\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_GUIDE_LED);
	fprintf(fp,"EXP_ENVI_OUTPUT_COMPONENT_LEVER_SOLENOID\t%u\n", EXP_ENVI_OUTPUT_COMPONENT_LEVER_SOLENOID);
	fprintf(fp,"NUM_OF_INPUT_COMPONENTS\t%u\n", exp_envi_data->num_of_inp_comps); 
	for (i = 0; i < exp_envi_data->num_of_inp_comps; i++)
	{
		fprintf(fp,"INPUT_COMPONENT\t%u\n", i); 
		fprintf(fp,"component_type\t%u\n", exp_envi_data->inp_comp_types[i].type); 
		fprintf(fp,"required_status_to_initiate_switching\t%u\n", exp_envi_data->inp_comp_types[i].constraints.required_status_to_initiate_switching); 
		fprintf(fp,"max_high_status_duration\t%llu\n", exp_envi_data->inp_comp_types[i].constraints.max_high_status_duration);  
		fprintf(fp,"min_high_status_duration\t%llu\n", exp_envi_data->inp_comp_types[i].constraints.min_high_status_duration);  
		fprintf(fp,"max_low_status_duration\t%llu\n", exp_envi_data->inp_comp_types[i].constraints.max_low_status_duration);  
		fprintf(fp,"min_low_status_duration\t%llu\n", exp_envi_data->inp_comp_types[i].constraints.min_low_status_duration);  
		fprintf(fp,"num_of_low_2_high_switch\t%u\n", exp_envi_data->inp_comp_types[i].constraints.num_of_low_2_high_switch);  
		fprintf(fp,"num_of_high_2_low_switch\t%u\n", exp_envi_data->inp_comp_types[i].constraints.num_of_high_2_low_switch);  
	}
	fprintf(fp,"NUM_OF_OUTPUT_COMPONENTS\t%u\n", exp_envi_data->num_of_outp_comps); 
	for (i = 0; i < exp_envi_data->num_of_outp_comps; i++)
	{
		fprintf(fp,"OUTPUT_COMPONENT\t%u\n", i); 
		fprintf(fp,"component_type\t%u\n", exp_envi_data->outp_comp_types[i].type); 
		fprintf(fp,"high_status_duration\t%llu\n", exp_envi_data->outp_comp_types[i].high_status_duration);  
	}

	fprintf(fp,"EXP_ENVI_COMPONENTS_STATUS_TYPES------------------\n");
	fprintf(fp,"EXP_ENVI_COMP_STATUS_NULL\t%u\n", EXP_ENVI_COMP_STATUS_NULL); 
	fprintf(fp,"EXP_ENVI_COMP_STATUS_LOW\t%u\n", EXP_ENVI_COMP_STATUS_LOW); 
	fprintf(fp,"EXP_ENVI_COMP_STATUS_LOW_SUCCESS\t%u\n", EXP_ENVI_COMP_STATUS_LOW_SUCCESS); 
	fprintf(fp,"EXP_ENVI_COMP_STATUS_HIGH\t%u\n", EXP_ENVI_COMP_STATUS_HIGH); 
	fprintf(fp,"EXP_ENVI_COMP_STATUS_HIGH_SUCCESS\t%u\n", EXP_ENVI_COMP_STATUS_HIGH_SUCCESS); 
	fprintf(fp,"EXP_ENVI_COMP_STATUS_READY_FOR_SWITCHING\t%u\n", EXP_ENVI_COMP_STATUS_READY_FOR_SWITCHING); 

	fprintf(fp,"----------ExpEnviHandler - End of Main Meta File----------\n");
	fclose(fp);
	return 1;
}


static int create_data_files(TimeStamp rec_start, char *data_directory_path)
{
	if (!create_meta_data(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_files", "! create_meta_data");
	
	if (!create_input_component_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_files", "! create_input_component_status_data");

	if (!create_output_component_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "create_data_files", "! create_output_component_status_data");

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
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: ExpEnviHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------ExpEnviHandler - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"RECORDING START\t%llu\n", rec_start);		
	file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX] = fp;


	return 1;	
}

static int create_input_component_status_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/input_comp_status");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: ExpEnviHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------ExpEnviHandler - Input Component Status File----------\n");

	file_ptr_arr[INPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int create_output_component_status_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/output_comp_status");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: ExpEnviHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------ExpEnviHandler - Output Component Status File----------\n");

	file_ptr_arr[OUTPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int write_to_input_component_status_data(ExpEnviInputStatusHistory *exp_envi_input_status_history)
{
	ExpEnviInputStatusHistoryItem item;
	FILE *fp = file_ptr_arr[INPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX];

	while (get_next_exp_envi_input_status_history_item(exp_envi_input_status_history, &item))
	{
		fprintf(fp,"%llu\t%u\t%u\n", item.time, item.comp_num, item.new_status);	
	}

	return 1;	
}

static int write_to_output_component_status_data(ExpEnviOutputStatusHistory *exp_envi_output_status_history)
{
	ExpEnviOutputStatusHistoryItem item;
	FILE *fp = file_ptr_arr[OUTPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX];

	while (get_next_exp_envi_output_status_history_item(exp_envi_output_status_history, &item))
	{
		fprintf(fp,"%llu\t%u\t%u\n", item.time, item.comp_num, item.new_status);	
	}

	return 1;	
}

static int close_meta_data(TimeStamp rec_end)
{
	FILE *fp = file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX];
	fprintf(fp,"RECORDING END\t%llu\n", rec_end);	
	fprintf(fp,"----------ExpEnviHandler - End of Meta File----------\n");
	fclose(fp);
	return 1;
}
static int close_input_component_status_data(void)
{
	FILE *fp = file_ptr_arr[INPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX];

	fprintf(fp,"----------ExpEnviHandler - End of Input Component Status File----------\n");
	fclose(fp);
	return 1;
}
static int close_output_component_status_data(void)
{
	FILE *fp = file_ptr_arr[OUTPUT_COMPONENT_STATUS_DATA_FILE_PTR_ARR_IDX];

	fprintf(fp,"----------ExpEnviHandler - End of Input Component Status File----------\n");
	fclose(fp);
	return 1;
}

static int delete_data_files(char *data_directory_path)
{
	if (! delete_meta_data(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "delete_data_files", "! delete_meta_data(data_directory_path)");
	
	if (! delete_input_component_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "delete_data_files", "! delete_input_component_status_data(data_directory_path)");

	if (! delete_output_component_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "delete_data_files", "! delete_input_component_status_data(data_directory_path)");

	return 1;
}

static int delete_meta_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	
	if (remove(temp) != 0)  { printf ("ERROR: ExpEnviHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_input_component_status_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/input_comp_status");
	
	if (remove(temp) != 0)  { printf ("ERROR: ExpEnviHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_output_component_status_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/output_comp_status");
	
	if (remove(temp) != 0)  { printf ("ERROR: ExpEnviHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}
