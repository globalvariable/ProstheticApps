#include "DataFormat_v0.h"

#define META_DATA_FILE_PTR_ARR_IDX						0
#define MOV_OBJ_STATUS_DATA_FILE_PTR_ARR_IDX			1
#define JOINT_ANGLE_DATA_FILE_PTR_ARR_IDX 				2
#define ROBOT_PULSE_DATA_FILE_PTR_ARR_IDX 				3
#define NUM_OF_DATA_FILE_PER_RECORDING					4


static FILE *file_ptr_arr[NUM_OF_DATA_FILE_PER_RECORDING];

static int create_main_meta_file(char *main_directory_path, MovObjHandParadigmRobotReach *paradigm, ThreeDofRobot *robot_arm);
static int create_data_files(TimeStamp rec_start, char *data_directory_path);
static int create_meta_data(TimeStamp rec_start, char *data_directory_path);
static int create_mov_obj_status_data(char *data_directory_path);
static int create_joint_angle_data(char *data_directory_path);
static int create_robot_pulse_data(char *data_directory_path);
static int close_meta_data(TimeStamp rec_end);
static int close_mov_obj_status_data(void);
static int close_joint_angle_data(void);
static int close_robot_pulse_data(void);
static int delete_data_files(char *data_directory_path);
static int delete_meta_data(char *data_directory_path);
static int delete_mov_obj_status_data(char *data_directory_path);
static int delete_joint_angle_data(char *data_directory_path);
static int delete_robot_pulse_data(char *data_directory_path);
static int write_to_mov_obj_status_data(MovObjStatusHistory* mov_obj_status_history);
static int write_to_joint_angle_data(ThreeDofRobotAngleHistory *robot_angle_history);
static int write_to_robot_pulse_data(ThreeDofRobotPulseHistory *robot_pulse_history);

int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
  	MovObjHandParadigmRobotReach *paradigm;
	ThreeDofRobot *robot_arm;
	DIR	*dir_main_folder;
	char main_directory_path[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	paradigm = va_arg ( arguments, MovObjHandParadigmRobotReach *); 
	robot_arm = va_arg ( arguments, ThreeDofRobot *); 
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/MovObjHandlerRecord");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("MovObjHandler: ERROR: path: %s already has MovObjHandlerRecord folder.\n", path_chooser);		
        	printf ("MovObjHandler: ERROR: Select another folder or delete MovObjHandlerRecord directory.\n\n");	
		closedir(dir_main_folder );	        		
                return 0;
        }
        closedir(dir_main_folder );
        
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("MovObjHandler: Created /MovObjHandlerRecord folder in: %s.\n", path_chooser);
        printf ("MovObjHandler: /MovObjHandlerRecord path is: %s.\n", main_directory_path); 
        
	if (!create_main_meta_file(main_directory_path, paradigm, robot_arm))
		return 0;

	if ((fp = fopen("./path_initial_directory", "w")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
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
		printf("MovObjHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("MovObjHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/MovObjHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("MovObjHandler: ERROR: path: %s already has %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	  
      		        
	if ((ret = mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH)) != 0)
	{
		printf("Couldn't makedir: %s\n, Reason: %d\n", data_directory_path, ret);
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "mkdir() != 0");
	}	

	if (! create_data_files(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "! create_data_files.");

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
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "! close_meta_data");

	if (! close_mov_obj_status_data())
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "! close_mov_obj_status_data");

	if (! close_joint_angle_data())
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "! close_joint_angle_data");

	if (! close_robot_pulse_data())
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_directory_v0", "! close_robot_pulse_data");

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
		printf("MovObjHandler: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("MovObjHandler: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/MovObjHandlerRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("MovObjHandler: ERROR: path: %s does not have %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	 

	if (! delete_data_files(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_directory_v0", "! delete_data_files(data_directory_path)");

	if (rmdir(data_directory_path) != 0) 
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_directory_v0", "! remove(data_directory_path)");

	return 1;
}

int write_to_data_files_v0(int num, ...)
{
	MovObjStatusHistory* mov_obj_status_history;
	ThreeDofRobotAngleHistory *robot_angle_history;
	ThreeDofRobotPulseHistory *robot_pulse_history;

  	va_list arguments;
	va_start ( arguments, num );   
    	mov_obj_status_history = va_arg ( arguments, MovObjStatusHistory*); 
    	robot_angle_history = va_arg ( arguments, ThreeDofRobotAngleHistory *); 
    	robot_pulse_history = va_arg ( arguments, ThreeDofRobotPulseHistory *); 
	va_end ( arguments );

	if (! write_to_mov_obj_status_data(mov_obj_status_history))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_mov_obj_status_data()");

	if (! write_to_joint_angle_data(robot_angle_history ))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_joint_angle_data()");

	if (! write_to_robot_pulse_data(robot_pulse_history))
		return print_message(ERROR_MSG ,"ExpEnviHandler", "DataFormat_v0", "write_to_data_files_v0", "! write_to_robot_pulse_data()");

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


static int create_main_meta_file(char *main_directory_path, MovObjHandParadigmRobotReach *paradigm, ThreeDofRobot *robot_arm)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	unsigned int i;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------MovObjHandler - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	// already includes \n
	fprintf(fp,"IT_IS_A_THREE_DOF_ROBOT->THREE_DOF_ROBOT_NUM_OF_SERVOS\t%u\n", THREE_DOF_ROBOT_NUM_OF_SERVOS); 	// already includes \n
	fprintf(fp,"BASE_SERVO\t%u\n", BASE_SERVO); 	
	fprintf(fp,"SHOULDER_SERVO\t%u\n", SHOULDER_SERVO); 	
	fprintf(fp,"ELBOW_SERVO\t%u\n", ELBOW_SERVO); 	
	fprintf(fp,"ThreeDofRobotSize.length_humerus\t%f\n", robot_arm->size.length_humerus);
	fprintf(fp,"ThreeDofRobotSize.length_ulna\t%f\n", robot_arm->size.length_ulna);
	fprintf(fp,"ThreeDofRobotSize.height_ulna\t%f\n", robot_arm->size.height_ulna);

	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].range.pulse_width_0_degree\t%u\n", robot_arm->servos[BASE_SERVO].range.pulse_width_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].range.pulse_width_90_degree\t%u\n", robot_arm->servos[BASE_SERVO].range.pulse_width_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].range.position_0_degree\t%u\n", robot_arm->servos[BASE_SERVO].range.position_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].range.position_90_degree\t%u\n", robot_arm->servos[BASE_SERVO].range.position_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].range.radian_per_pos_quanta\t%.15f\n", robot_arm->servos[BASE_SERVO].range.radian_per_pos_quanta);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].range.pulse_width_0_degree\t%u\n", robot_arm->servos[SHOULDER_SERVO].range.pulse_width_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].range.pulse_width_90_degree\t%u\n", robot_arm->servos[SHOULDER_SERVO].range.pulse_width_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].range.position_0_degree\t%u\n", robot_arm->servos[SHOULDER_SERVO].range.position_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].range.position_90_degree\t%u\n", robot_arm->servos[SHOULDER_SERVO].range.position_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].range.radian_per_pos_quanta\t%.15f\n", robot_arm->servos[SHOULDER_SERVO].range.radian_per_pos_quanta);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].range.pulse_width_0_degree\t%u\n", robot_arm->servos[ELBOW_SERVO].range.pulse_width_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].range.pulse_width_90_degree\t%u\n", robot_arm->servos[ELBOW_SERVO].range.pulse_width_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].range.position_0_degree\t%u\n", robot_arm->servos[ELBOW_SERVO].range.position_0_degree);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].range.position_90_degree\t%u\n", robot_arm->servos[ELBOW_SERVO].range.position_90_degree);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].range.radian_per_pos_quanta\t%.15f\n", robot_arm->servos[ELBOW_SERVO].range.radian_per_pos_quanta);

	fprintf(fp,"ThreeDofRobotCartesianLimit.depth_min\t%f\n", robot_arm->cartesian_security_limits.depth_min);
	fprintf(fp,"ThreeDofRobotCartesianLimit.depth_max\t%f\n", robot_arm->cartesian_security_limits.depth_max);
	fprintf(fp,"ThreeDofRobotCartesianLimit.lateral_min\t%f\n", robot_arm->cartesian_security_limits.lateral_min);
	fprintf(fp,"ThreeDofRobotCartesianLimit.lateral_max\t%f\n", robot_arm->cartesian_security_limits.lateral_max);
	fprintf(fp,"ThreeDofRobotCartesianLimit.height_min\t%f\n", robot_arm->cartesian_security_limits.height_min);
	fprintf(fp,"ThreeDofRobotCartesianLimit.height_max\t%f\n", robot_arm->cartesian_security_limits.height_max);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].angular_security_limit.min\t%.15f\n", robot_arm->servos[BASE_SERVO].angular_security_limit.min);
	fprintf(fp,"ThreeDofRobot.servos[BASE_SERVO].angular_security_limit.max\t%.15f\n", robot_arm->servos[BASE_SERVO].angular_security_limit.max);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].angular_security_limit.min\t%.15f\n", robot_arm->servos[SHOULDER_SERVO].angular_security_limit.min);
	fprintf(fp,"ThreeDofRobot.servos[SHOULDER_SERVO].angular_security_limit.max\t%.15f\n", robot_arm->servos[SHOULDER_SERVO].angular_security_limit.max);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].angular_security_limit.min\t%.15f\n", robot_arm->servos[ELBOW_SERVO].angular_security_limit.min);
	fprintf(fp,"ThreeDofRobot.servos[ELBOW_SERVO].angular_security_limit.max\t%.15f\n", robot_arm->servos[ELBOW_SERVO].angular_security_limit.max);



	fprintf(fp,"MovObjHandParadigmRobotReach.stay_at_target_duration\t%llu\n", paradigm->stay_at_target_duration);
	fprintf(fp,"MovObjHandParadigmRobotReach.send_pw_command_wait_period\t%llu\n", paradigm->send_pw_command_wait_period);
	fprintf(fp,"MovObjHandParadigmRobotReach.send_pw_command_wait_period\t%llu\n", paradigm->send_pw_command_wait_period);
	fprintf(fp,"MovObjHandParadigmRobotReach.spike_2_servo_degree_multiplier\t%.15f\n", paradigm->spike_2_servo_degree_multiplier);
	fprintf(fp,"MovObjHandParadigmRobotReach.max_servo_angle_change\t%.15f\n", paradigm->max_servo_angle_change);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.point_reach_threshold.r_x\t%.15f\n", paradigm->threshold.point_reach_threshold.r_x);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.point_reach_threshold.r_y\t%.15f\n", paradigm->threshold.point_reach_threshold.r_y);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.point_reach_threshold.r_z\t%.15f\n", paradigm->threshold.point_reach_threshold.r_z);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.outer_threshold.r_x\t%.15f\n", paradigm->threshold.outer_threshold.r_x);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.outer_threshold.r_y\t%.15f\n", paradigm->threshold.outer_threshold.r_y);
	fprintf(fp,"MovObjHandParadigmRobotReach.threshold.outer_threshold.r_z\t%.15f\n", paradigm->threshold.outer_threshold.r_z);

	fprintf(fp,"MovObjHandParadigmRobotReach.start_info.num_of_positions\t%u\n", paradigm->start_info.num_of_positions);
	for (i = 0; i < paradigm->start_info.num_of_positions; i++)
	{
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.cart_coordinates[%u].height\t%.15f\n", i, paradigm->start_info.cart_coordinates[i].height);
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.cart_coordinates[%u].lateral\t%.15f\n", i, paradigm->start_info.cart_coordinates[i].lateral);
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.cart_coordinates[%u].depth\t%.15f\n", i, paradigm->start_info.cart_coordinates[i].depth);
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.robot_pulse_widths[%u].pulse[BASE_SERVO]\t%u\n",  i, paradigm->start_info.robot_pulse_widths[i].pulse[BASE_SERVO]);
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.robot_pulse_widths[%u].pulse[SHOULDER_SERVO]\t%u\n",  i, paradigm->start_info.robot_pulse_widths[i].pulse[SHOULDER_SERVO]);
		fprintf(fp,"MovObjHandParadigmRobotReach.start_info.robot_pulse_widths[%u].pulse[ELBOW_SERVO]\t%u\n",  i, paradigm->start_info.robot_pulse_widths[i].pulse[ELBOW_SERVO]);
	}
	fprintf(fp,"MovObjHandParadigmRobotReach.target_info.num_of_positions\t%u\n", paradigm->target_info.num_of_positions);
	for (i = 0; i < paradigm->target_info.num_of_positions; i++)
	{
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.cart_coordinates[%u].height\t%.15f\n", i, paradigm->target_info.cart_coordinates[i].height);
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.cart_coordinates[%u].lateral\t%.15f\n", i, paradigm->target_info.cart_coordinates[i].lateral);
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.cart_coordinates[%u].depth\t%.15f\n", i, paradigm->target_info.cart_coordinates[i].depth);
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.robot_pulse_widths[%u].pulse[BASE_SERVO]\t%u\n",  i, paradigm->target_info.robot_pulse_widths[i].pulse[BASE_SERVO]);
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.robot_pulse_widths[%u].pulse[SHOULDER_SERVO]\t%u\n",  i, paradigm->target_info.robot_pulse_widths[i].pulse[SHOULDER_SERVO]);
		fprintf(fp,"MovObjHandParadigmRobotReach.target_info.robot_pulse_widths[%u].pulse[ELBOW_SERVO]\t%u\n",  i, paradigm->target_info.robot_pulse_widths[i].pulse[ELBOW_SERVO]);
	}
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.depth_min\t%.15f\n",  paradigm->cartesian_space_limits.depth_min);
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.depth_max\t%.15f\n",  paradigm->cartesian_space_limits.depth_max);
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.lateral_min\t%.15f\n",  paradigm->cartesian_space_limits.lateral_min);
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.lateral_max\t%.15f\n",  paradigm->cartesian_space_limits.lateral_max);
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.height_min\t%.15f\n",  paradigm->cartesian_space_limits.height_min);
	fprintf(fp,"MovObjHandParadigmRobotReach.cartesian_space_limits.height_max\t%.15f\n",  paradigm->cartesian_space_limits.height_max);

	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[BASE_SERVO].min\t%.15f\n",  paradigm->polar_space_limits[BASE_SERVO].min);
	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[BASE_SERVO].max\t%.15f\n",  paradigm->polar_space_limits[BASE_SERVO].max);
	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[SHOULDER_SERVO].min\t%.15f\n",  paradigm->polar_space_limits[SHOULDER_SERVO].min);
	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[SHOULDER_SERVO].max\t%.15f\n",  paradigm->polar_space_limits[SHOULDER_SERVO].max);
	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[ELBOW_SERVO].min\t%.15f\n",  paradigm->polar_space_limits[ELBOW_SERVO].min);
	fprintf(fp,"MovObjHandParadigmRobotReach.polar_space_limits[ELBOW_SERVO].max\t%.15f\n",  paradigm->polar_space_limits[ELBOW_SERVO].max);

	fprintf(fp,"MOV_OBJ_STATUS_TYPES------------------\n");
	fprintf(fp,"MOV_OBJ_STATUS_NULL\t%u\n", MOV_OBJ_STATUS_NULL); 
	fprintf(fp,"MOV_OBJ_STATUS_OUT_OF_TRIAL\t%u\n", MOV_OBJ_STATUS_OUT_OF_TRIAL); 
	fprintf(fp,"MOV_OBJ_STATUS_STAYING_AT_START_POINT\t%u\n", MOV_OBJ_STATUS_STAYING_AT_START_POINT); 
	fprintf(fp,"MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL\t%u\n", MOV_OBJ_STATUS_AVAILABLE_TO_CONTROL); 
	fprintf(fp,"MOV_OBJ_STATUS_RESETTING_TO_TARGET_POINT\t%u\n", MOV_OBJ_STATUS_RESETTING_TO_TARGET_POINT); 
	fprintf(fp,"MOV_OBJ_STATUS_REACHED_TARGET_POINT\t%u\n", MOV_OBJ_STATUS_REACHED_TARGET_POINT); 
	fprintf(fp,"MOV_OBJ_STATUS_RESETTING_TO_START_POINT\t%u\n", MOV_OBJ_STATUS_RESETTING_TO_START_POINT); 
	fprintf(fp,"MOV_OBJ_STATUS_DISABLED	\t%u\n", MOV_OBJ_STATUS_DISABLED); 

	fprintf(fp,"struct __ThreeDofRobotAngleHistoryItem\n");
	fprintf(fp,"{\n");
	fprintf(fp,"\tTimeStamp 		time;\n");
	fprintf(fp,"\tServoAngle		servo_angles[THREE_DOF_ROBOT_NUM_OF_SERVOS];\n");
	fprintf(fp,"};\n");
	fprintf(fp,"sizeof(ThreeDofRobotAngleHistoryItem)\t%lu\n", sizeof(ThreeDofRobotAngleHistoryItem));	
	fprintf(fp,"typedef double ServoAngle;\n");	

	fprintf(fp,"struct __ThreeDofRobotPulseHistoryItem\n");
	fprintf(fp,"{\n");
	fprintf(fp,"\tTimeStamp 		time;\n");
	fprintf(fp,"\tServoPulse		servo_pulses[THREE_DOF_ROBOT_NUM_OF_SERVOS];\n");
	fprintf(fp,"};\n");
	fprintf(fp,"sizeof(ThreeDofRobotPulseHistoryItem)\t%lu\n", sizeof(ThreeDofRobotPulseHistoryItem));	
	fprintf(fp,"typedef unsigned short int ServoPulse;\n");	

	fprintf(fp,"----------MovObjHandler - End of Main Meta File----------\n");
	fclose(fp);
	return 1;
}

static int create_data_files(TimeStamp rec_start, char *data_directory_path)
{
	if (!create_meta_data(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_files", "! create_meta_data");
	
	if (!create_mov_obj_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_files", "! create_mov_obj_status_data");

	if (!create_joint_angle_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_files", "! create_joint_angle_data");

	if (!create_robot_pulse_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "create_data_files", "! create_robot_pulse_data");

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
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------MovObjHandler - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"RECORDING START\t%llu\n", rec_start);		
	file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX] = fp;

	return 1;	
}

static int create_mov_obj_status_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/mov_obj_status");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------MovObjHandler - Mov Obj Status File----------\n");

	file_ptr_arr[MOV_OBJ_STATUS_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int create_joint_angle_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/joint_angle.bin");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't create file: %s\n\n", temp); return 0; }

	file_ptr_arr[JOINT_ANGLE_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int create_robot_pulse_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/robot_pulse.bin");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: MovObjHandler: Couldn't create file: %s\n\n", temp); return 0; }

	file_ptr_arr[ROBOT_PULSE_DATA_FILE_PTR_ARR_IDX] =  fp;

	return 1;	
}

static int close_meta_data(TimeStamp rec_end)
{
	FILE *fp = file_ptr_arr[META_DATA_FILE_PTR_ARR_IDX];
	fprintf(fp,"RECORDING END\t%llu\n", rec_end);	
	fprintf(fp,"----------MovObjHandler - End of Meta File----------\n");
	fclose(fp);
	return 1;
}

static int close_mov_obj_status_data(void)
{
	FILE *fp = file_ptr_arr[MOV_OBJ_STATUS_DATA_FILE_PTR_ARR_IDX];

	fprintf(fp,"----------MovObjHandler - End of Mov Obj Status File----------\n");
	fclose(fp);
	return 1;
}

static int close_joint_angle_data(void)
{
	FILE *fp = file_ptr_arr[JOINT_ANGLE_DATA_FILE_PTR_ARR_IDX ];

	fclose(fp);
	return 1;
}

static int close_robot_pulse_data(void)
{
	FILE *fp = file_ptr_arr[ROBOT_PULSE_DATA_FILE_PTR_ARR_IDX];

	fclose(fp);
	return 1;
}

static int delete_data_files(char *data_directory_path)
{
	if (! delete_meta_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_meta_data(data_directory_path)");
	
	if (! delete_mov_obj_status_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_mov_obj_status_data(data_directory_path)");

	if (! delete_joint_angle_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_joint_angle_data(data_directory_path)");

	if (! delete_robot_pulse_data(data_directory_path))
		return print_message(ERROR_MSG ,"MovObjHandler", "DataFormat_v0", "delete_data_files_v0", "! delete_robot_pulse_data(data_directory_path)");

	return 1;
}

static int delete_meta_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	
	if (remove(temp) != 0)  { printf ("ERROR: MovObjHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_mov_obj_status_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/mov_obj_status");
	
	if (remove(temp) != 0)  { printf ("ERROR: MovObjHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_joint_angle_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/joint_angle.bin");
	
	if (remove(temp) != 0)  { printf ("ERROR: MovObjHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int delete_robot_pulse_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/robot_pulse.bin");
	
	if (remove(temp) != 0)  { printf ("ERROR: MovObjHandler: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;	
}

static int write_to_mov_obj_status_data(MovObjStatusHistory* mov_obj_status_history)
{
	MovObjStatusHistoryItem item;
	FILE *fp = file_ptr_arr[MOV_OBJ_STATUS_DATA_FILE_PTR_ARR_IDX];

	while (get_next_mov_obj_status_history_item(mov_obj_status_history, &item))
	{
		fprintf(fp,"%llu\t%u\n", item.time, item.new_status);	
	}

	return 1;
}

static int write_to_joint_angle_data(ThreeDofRobotAngleHistory *robot_angle_history)
{
	ThreeDofRobotAngleHistoryItem item;
	FILE *fp = file_ptr_arr[JOINT_ANGLE_DATA_FILE_PTR_ARR_IDX];

	while (get_next_three_dof_robot_angle_history_item(robot_angle_history, &item))
	{
		fwrite(&item, sizeof(ThreeDofRobotAngleHistoryItem), 1, fp);
	}

	return 1;
}

static int write_to_robot_pulse_data(ThreeDofRobotPulseHistory *robot_pulse_history)
{
	ThreeDofRobotPulseHistoryItem item;
	FILE *fp = file_ptr_arr[ROBOT_PULSE_DATA_FILE_PTR_ARR_IDX ];

	while (get_next_three_dof_robot_pulse_history_item(robot_pulse_history, &item))
	{
		fwrite(&item, sizeof(ThreeDofRobotPulseHistoryItem), 1, fp);
	}

	return 1;
}

