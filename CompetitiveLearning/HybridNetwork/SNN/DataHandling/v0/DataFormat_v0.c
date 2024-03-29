#include "DataFormat_v0.h"


static unsigned int recording_data_buff_read_start_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
static unsigned int recording_data_buff_read_end_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
static FILE *recording_data_file_ptr_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];

static unsigned int blue_spike_spike_data_end_idx[MAX_NUM_OF_DAQ_CARD];
static unsigned int in_silico_spike_data_end_idx[SNN_SIM_NUM_OF_DEDICATED_CPUS];


static FILE *meta_data_file_ptr;
static FILE *blue_spike_spike_data_file_ptr;
static FILE *in_silico_spike_data_file_ptr;
static FILE *in_silico_network_data_file_ptr;

static int create_main_meta_file(char *main_directory_path);
static int create_blue_spike_meta_file(char *main_directory_path);
static int create_data_files(TimeStamp rec_start, char *data_directory_path);
static int create_meta_data(TimeStamp rec_start, char *data_directory_path);
static int create_blue_spike_spike_data(char *data_directory_path);
static int create_in_silico_spike_data(char *data_directory_path);
static int create_in_silico_network_data(char *data_directory_path);
static int create_recording_data(char *data_directory_path);

static int write_to_blue_spike_spike_data(void);
static int write_to_in_silico_spike_data(void);
static int write_to_recording_data(void);

static int close_meta_data(TimeStamp rec_end);
static int close_blue_spike_spike_data(void);
static int close_in_silico_spike_data(void);
static int close_in_silico_network_data(void);
static int close_recording_data(void);

static int delete_data_files(char *data_directory_path);
static int delete_meta_data(char *data_directory_path);
static int delete_blue_spike_spike_data(char *data_directory_path);
static int delete_in_silico_spike_data(char *data_directory_path);
static int delete_in_silico_network_data(char *data_directory_path);
static int delete_recording_data(char *data_directory_path);

static int load_main_meta_file(char *path_chooser, Network *in_silico_network, Network *blue_spike_network);
static int load_data_folder(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, unsigned int data_folder_num);


int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
	DIR	*dir_main_folder;
	char main_directory_path[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/HybridNetworkRecord");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("HybridNetwork: ERROR: path: %s already has HybridNetworkRecord folder.\n", path_chooser);		
        	printf ("HybridNetwork: ERROR: Select another folder or delete HybridNetworkRecord directory.\n\n");	
		closedir(dir_main_folder );	        		
                return 0;
        }
        closedir(dir_main_folder );
        
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("HybridNetwork: Created /HybridNetworkRecord folder in: %s.\n", path_chooser);
        printf ("HybridNetwork: /HybridNetworkRecord path is: %s.\n", main_directory_path); 
        
	if (!create_main_meta_file(main_directory_path))
		return 0;

	if (!create_blue_spike_meta_file(main_directory_path))
		return 0;

	if ((fp = fopen("./SNN/path_initial_directory", "w")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't find directory: %s\n\n", "./SNN/path_initial_directory"); return 0; }
	fprintf(fp, "%s", path_chooser);
	fclose (fp);	
	return 1;
}
int create_data_directory_v0(int num, ...)
{
	char *path_chooser;
	TimeStamp rec_start;
	unsigned int recording_number;

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	rec_start = va_arg ( arguments, TimeStamp); 
	recording_number = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	char data_directory_path[600];
	TimeStamp recording_start_time;
	unsigned int i, j, data_directory_cntr, ret;


	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		reset_spike_data_read_idx(blue_spike_spike_data_for_recording[i]);
	}	
	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		reset_spike_data_read_idx(in_silico_spike_data_for_recording[i]);
	}	

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_read_start_idx[i][j] = recording_data[i][j].buff_idx_write;
		}
	}

	recording_start_time = rec_start;		//  ACCORDING TO THIS DESIGN, RECORDING START TIME AND THE CORRESPONDING SPIKE DATA SAMPLE MIGHT NOT COINCODE TO SAME TIME BUT THEY WILL BE ALMOST EQUAL. 

	meta_data_file_ptr = NULL;
	blue_spike_spike_data_file_ptr = NULL;
	in_silico_spike_data_file_ptr = NULL;
	in_silico_network_data_file_ptr = NULL;

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_file_ptr_arr[i][j] = NULL;
		}
	}

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
		printf("HybridNetwork: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetwork: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/HybridNetworkRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("HybridNetwork: ERROR: path: %s already has %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	  
      		        
	if ((ret = mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH)) != 0)
	{
		printf("Couldn't makedir: %s\n, Reason: %d\n", data_directory_path, ret);
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "mkdir() != 0");
	}	

	if (! create_data_files(recording_start_time, data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! create_data_files.");

	return 1;
}
int fclose_all_data_files_v0(int num, ...)
{
	TimeStamp rec_end;

  	va_list arguments;
	va_start ( arguments, num );   
	rec_end = va_arg ( arguments, TimeStamp); 
	va_end ( arguments );

	TimeStamp recording_end_time;
	unsigned int i;

	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		blue_spike_spike_data_end_idx[i] = blue_spike_spike_data_for_recording[i]->buff_idx_write;
	}

	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		in_silico_spike_data_end_idx[i] = in_silico_spike_data_for_recording[i]->buff_idx_write;
	}	
	recording_end_time = rec_end;		//  ACCORDING TO THIS DESIGN, RECORDING END TIME AND THE CORRESPONDING SPIKE DATA SAMPLE MIGHT NOT COINCODE TO SAME TIME BUT THEY WILL BE ALMOST EQUAL. 


	if (! close_meta_data(recording_end_time))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! close_meta_data");

	if (! close_blue_spike_spike_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! close_blue_spike_spike_data");

	if (! close_in_silico_spike_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! close_in_silico_spike_data");

	if (! close_in_silico_network_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! close_in_silico_spike_data");

/*	if (! close_recording_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_directory_v0", "! close_recording_data");
*/
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
		printf("HybridNetwork: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetwork: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/HybridNetworkRecord/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("HybridNetwork: ERROR: path: %s does not have %s folder.\n", data_directory_path, data_directory_name);		
		closedir(dir_data_directory);
                return 0;
        }
	closedir(dir_data_directory);	 

	if (! delete_data_files(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_directory_v0", "! delete_data_files(data_directory_path)");

	if (rmdir(data_directory_path) != 0) 
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_directory_v0", "! remove(data_directory_path)");

	return 1;
}

int write_to_data_files_v0(int num, ...)
{
  	va_list arguments;
	va_start ( arguments, num );   
	va_end ( arguments );

	if (! write_to_blue_spike_spike_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "write_to_data_files_v0", "! write_to_blue_spike_spike_data");	

	if (! write_to_in_silico_spike_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "write_to_data_files_v0", "! write_to_in_silico_spike_data");		

/*	if (! write_to_recording_data())
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "write_to_data_files_v0", "! write_to_recording_data");		
*/
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

int load_data_directory_v0(int num, ...)
{

	char *path_chooser;
	unsigned int data_folder_num;

  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *); 
	data_folder_num = va_arg ( arguments, unsigned int); 
	va_end ( arguments );

	if (! load_main_meta_file(path_chooser, in_silico_network, blue_spike_network))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_data_directory_v0", "! load_main_meta_file");		

	if (! load_data_folder(path_chooser, in_silico_network, blue_spike_network, data_folder_num)) 
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_data_directory_v0", "! load_data_folder");

	return 1;	
}





static int create_main_meta_file(char *main_directory_path)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;

	unsigned int i, j, k, m;

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------HybridNetwork - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	

	fprintf(fp,"learning_rate\t%.15f\n", reward_data.learning_rate);

	fprintf(fp,"PARKER_SOCHACKI_ERROR_TOLERANCE\t%E\n", get_maximum_parker_sochacki_error_tolerance());
	fprintf(fp,"PARKER_SOCHACKI_MAX_ORDER\t%d\n", get_maximum_parker_sochacki_order());

	fprintf(fp,"............blue_spike_network.............\n");
	fprintf(fp,"blue_spike_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", blue_spike_network->num_of_neurons);
	fprintf(fp,"blue_spike_network->layer_count\t%u\n", blue_spike_network->layer_count);
	for(i = 0; i < blue_spike_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) blue_spike_network->layers[i]);
		fprintf(fp,"blue_spike_network->layers[%u]->num_of_connections\t%u\n", i, blue_spike_network->layers[i]->num_of_connections);
		for(j = 0; j < blue_spike_network->layers[i]->num_of_connections; j++)
		{
			fprintf(fp,"%u_th_connection_to_layer(address)\t%llu\n", j, (unsigned long long int) blue_spike_network->layers[i]->connected_to_layers[j]);
		}
		fprintf(fp,"neuron_group_count\t%u\n", blue_spike_network->layers[i]->neuron_group_count);	
		for(j = 0; j < blue_spike_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", blue_spike_network->layers[i]->neuron_groups[j]->neuron_count);		
			for (k = 0; k < blue_spike_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n", i, j, k, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].inhibitory);	
			}
		}			
	}


	fprintf(fp,"............in_silico_network.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) in_silico_network->layers[i]);
		fprintf(fp,"in_silico_network->layers[%u]->num_of_connections\t%u\n", i, in_silico_network->layers[i]->num_of_connections);
		for(j = 0; j < in_silico_network->layers[i]->num_of_connections; j++)
		{
			fprintf(fp,"%u_th_connection_to_layer(address)\t%llu\n", j, (unsigned long long int) in_silico_network->layers[i]->connected_to_layers[j]);
		}
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].inhibitory);	
				if (in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params != NULL)
				{
					fprintf(fp,"IzhikevichNeuron\n");	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->u);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->a);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->b);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->c);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->d);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->k);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_resting);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_threshold);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->v_peak);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->I_inject);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E_excitatory);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->E_inhibitory);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->decay_rate_excitatory);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->decay_rate_inhibitory);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->conductance_excitatory);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->conductance_inhibitory);	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].iz_params->k_v_threshold);	
				}
				if (in_silico_network->layers[i]->neuron_groups[j]->neurons[k].poisson_params != NULL)
				{
					fprintf(fp,"PoissonNeuron\n");	
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].poisson_params->firing_rate\t%.15f\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].poisson_params->firing_rate);		
				}
			}		
		}
	}

	fprintf(fp,"............blue_spike_network axon list.............\n");
	fprintf(fp,"blue_spike_network->layer_count\t%u\n", blue_spike_network->layer_count);
	for(i = 0; i < blue_spike_network->layer_count; i++)
	{
		fprintf(fp,"neuron_group_count\t%u\n", blue_spike_network->layers[i]->neuron_group_count);	
		for(j = 0; j < blue_spike_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", blue_spike_network->layers[i]->neuron_groups[j]->neuron_count);		
			for (k = 0; k < blue_spike_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_layer[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_group[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%u\n", i, j, k, m, (unsigned int)blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_num[m]);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].weight);
					fprintf(fp,"blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].plastic\t%u\n", i, j, k, m, blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[blue_spike_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].plastic);
				}
			}
		}
	}

	fprintf(fp,"............in_silico_network axon list.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"address\t%llu\n", (unsigned long long int) in_silico_network->layers[i]);
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			

				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons);
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->num_of_axons; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n", i, j, k, m, (unsigned long long int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->delay[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_layer[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_group[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%u\n", i, j, k, m, (unsigned int)in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->target_neuron_num[m]);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].weight);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].plastic\t%u\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m], in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->to[m]->syn_list->synapses[in_silico_network->layers[i]->neuron_groups[j]->neurons[k].axon_list->syn_idx[m]].plastic);
				}
			}		
		}
	}

	fprintf(fp,"............in_silico_network synapse list.............\n");
	fprintf(fp,"in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n", in_silico_network->num_of_neurons);
	fprintf(fp,"in_silico_network->layer_count\t%u\n", in_silico_network->layer_count);
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		fprintf(fp,"layer_num\t%u\n", i);
		fprintf(fp,"neuron_group_count\t%u\n", in_silico_network->layers[i]->neuron_group_count);	
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			fprintf(fp,"neuron_group_num\t%u\n", j);	
			fprintf(fp,"neuron_count\t%u\n", in_silico_network->layers[i]->neuron_groups[j]->neuron_count);
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				fprintf(fp,"neuron_num\t%u\n", k);			

				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list.............\n", i, j, k);	
				fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->num_of_synapses\t%u\n", i, j, k, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses);
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].weight);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].type\t%u\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].type);
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].plastic\t%u\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].plastic);
					if (in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].plastic)
					{
						fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_stdp_pre_post->change_amount\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].ps_stdp_pre_post->change_amount);
						fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_stdp_pre_post->decay_rate\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].ps_stdp_pre_post->decay_rate);
						fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->decay_rate\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].ps_eligibility->decay_rate);
						fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->max_eligibility\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].ps_eligibility->max_eligibility);
						fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->depol_eligibility\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].ps_eligibility->depol_eligibility);
					}
				}
			}		
		}
	}

	fprintf(fp,"............initial synaptic weights.............\n");
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			for (k = 0; k < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->num_of_synapses; m++)
				{
					fprintf(fp,"in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n", i, j, k, m, in_silico_network->layers[i]->neuron_groups[j]->neurons[k].syn_list->synapses[m].weight);
				}
			}		
		}
	}	

	fprintf(fp,"----------HybridNetwork - End of Main Meta File----------\n");
	fclose(fp);
	print_message(INFO_MSG ,"HybridNetwork", "DataFormat_v0", "create_main_meta_file", "created Main Meta File successfully");
	return 1;
}


static int create_blue_spike_meta_file(char *main_directory_path)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta_blue_spike");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------HybridNetwork - BlueSpike Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"MAX_NUM_OF_MWA\t%d\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%d\n",MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN\t%d\n",MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp,"NUM_OF_SAMP_PER_SPIKE\t%d\n",NUM_OF_SAMP_PER_SPIKE);
	fprintf(fp,"MAX_NUM_OF_DAQ_CARD\t%d\n",MAX_NUM_OF_DAQ_CARD);
	fprintf(fp,"MAX_NUM_OF_CHANNEL_PER_DAQ_CARD\t%d\n",MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
	fprintf(fp,"SAMPLING_INTERVAL\t%d\tNANOSECONDS\n", SAMPLING_INTERVAL);
	fprintf(fp,"LOWEST_VOLTAGE_MV\t%f\tMILLIVOLTS\n",LOWEST_VOLTAGE_MV);
	fprintf(fp,"HIGHEST_VOLTAGE_MV\t%f\tMILLIVOLTS\n",HIGHEST_VOLTAGE_MV);	
	fprintf(fp,"DAQ_0_MODEL\t%s\n",DAQ_0_MODEL);	
	fprintf(fp,"typedef float RecordingDataSample;\n");
	fprintf(fp,"sizeof(RecordingDataSample)\t%lu\n", sizeof(RecordingDataSample));	
	fprintf(fp,"struct __SpikeTimeStampItem\n");
	fprintf(fp,"{\n");
	fprintf(fp,"TimeStamp 		peak_time;\n");
	fprintf(fp,"unsigned int		layer;\n");
	fprintf(fp,"unsigned int		neuron_group;\n");
	fprintf(fp,"unsigned int		neuron;\n");
	fprintf(fp,"} SpikeTimeStampItem;\n");
	fprintf(fp,"sizeof(SpikeTimeStampItem)\t%lu\n", sizeof(SpikeTimeStampItem));
	fprintf(fp,"----------HybridNetwork - End of BlueSpike Main Meta File----------\n");
	fclose(fp);
	return 1;

}

static int create_data_files(TimeStamp rec_start, char *data_directory_path)
{

	if (!create_meta_data(rec_start, data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_files", "! create_meta_data");

	if (!create_blue_spike_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_files", "! create_blue_spike_spike_data");
	
	if (!create_in_silico_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_files", "! create_in_silico_spike_data");

	if (! create_in_silico_network_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_files", "! create_in_silico_network_data");

/*	if (!create_recording_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "create_data_files", "! create_recording_data");
*/
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
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------HybridNetwork - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s", asctime (timeinfo)); 	
	fprintf(fp,"RECORDING START\t%llu\n", rec_start);		
	meta_data_file_ptr = fp;

	return 1;	
}

static int create_blue_spike_spike_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/blue_spike_spikes");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp); return 0; }

	blue_spike_spike_data_file_ptr =  fp;

	return 1;	
}


static int create_in_silico_spike_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_spikes");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp); return 0; }

	in_silico_spike_data_file_ptr =  fp;

	return 1;	
}
static int create_in_silico_network_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;

	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_network");
	if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp); return 0; }

	in_silico_network_data_file_ptr =  fp;

	return 1;	
}

static int create_recording_data(char *data_directory_path)
{
	char temp[600];
	FILE *fp;
	char char_mwa_num[20], char_chan_num[20];
	unsigned int i,j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			strcpy(temp, data_directory_path);	
			if (i<10)
			{
				sprintf(char_mwa_num, "/rec_mwa_0%u" , i);
				strcat(temp, char_mwa_num);
			}
			else if (i<100)
			{
				sprintf(char_mwa_num, "/rec_mwa_%u" , i);
				strcat(temp, char_mwa_num);				
			}
			else
			{
				printf ("ERROR: HybridNetwork: MAX MWA NUM is larger than 99\n\n");
				printf ("ERROR: HybridNetwork: Couldn't create all recording files requested\n\n");
				return 0; 
			}
			strcat(temp, "_");
			if (j<10)
			{
				sprintf(char_chan_num, "chan_00%u" , j);
				strcat(temp, char_chan_num);
			}
			else if (j<100)
			{
				sprintf(char_chan_num, "chan_0%u" , j);
				strcat(temp, char_chan_num);				
			}
			else if (j<1000)
			{
				sprintf(char_chan_num, "chan_%u" , j);
				strcat(temp, char_chan_num);			
			}
			else
			{
				printf ("ERROR: HybridNetwork: MAX NUM OF CHAN PER MWA is larger than 999\n\n");
				printf ("ERROR: HybridNetwork: Couldn't create all recording files requested\n\n");
				return 0; 
			}			
			if ((fp = fopen(temp, "wb")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't create file: %s\n\n", temp); return 0; }
			recording_data_file_ptr_arr[i][j] =  fp;
		}
	}

	return 1;
}


static int write_to_blue_spike_spike_data(void)
{
	unsigned int i;
	SpikeTimeStampItem item;

	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		while (get_next_spike_data_item(blue_spike_spike_data_for_recording[i], &item))
		{
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, blue_spike_spike_data_file_ptr);
		}
	}	
	return 1;
}

static int write_to_in_silico_spike_data(void)
{
	unsigned int i;
	SpikeTimeStampItem item;

	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		while (get_next_spike_data_item(in_silico_spike_data_for_recording[i], &item))
		{
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, in_silico_spike_data_file_ptr);
		}
	}	
	return 1;
}

static int write_to_recording_data(void)
{
	RecordingDataSample	*recording_data_chan_buff;
	FILE *fp;
	unsigned int i,j, idx, end_idx;
	RecordingDataSampleFloat sample;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_chan_buff = recording_data[i][j].rec_data_buff;
			idx = recording_data_buff_read_start_idx[i][j];
			end_idx = recording_data[i][j].buff_idx_write;
			fp = recording_data_file_ptr_arr[i][j];
			while (idx != end_idx)
			{
				sample = (RecordingDataSampleFloat)(recording_data_chan_buff[idx]);	// lower the precision for faster writing
				fwrite(&sample, sizeof(RecordingDataSampleFloat), 1, fp);
				idx++;	
				if (idx ==	RECORDING_DATA_BUFF_SIZE)
					idx = 0;	
			}
			recording_data_buff_read_start_idx[i][j] = end_idx;
		}
	}		
	return 1;

}

static int close_meta_data(TimeStamp rec_end)
{
	fprintf(meta_data_file_ptr,"RECORDING END\t%llu\n", rec_end);	
	fprintf(meta_data_file_ptr,"----------HybridNetwork - End of Meta File----------\n");
	fclose(meta_data_file_ptr);

	return 1;
}
static int close_blue_spike_spike_data(void)
{
	unsigned int i, end_idx;
	SpikeTimeStampItem item;
	SpikeData *spike_data;
	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		spike_data = blue_spike_spike_data_for_recording[i];
		end_idx = blue_spike_spike_data_end_idx[i];
		while (get_next_spike_data_item(spike_data, &item))
		{
			if (spike_data->buff_idx_read == end_idx)
				break;
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, blue_spike_spike_data_file_ptr);
		}
		reset_spike_data_read_idx(spike_data);
	}
	fclose(blue_spike_spike_data_file_ptr);	
	return 1;
}

static int close_in_silico_spike_data(void)
{
	unsigned int i, end_idx;
	SpikeTimeStampItem item;
	SpikeData *spike_data;
	for (i = 0; i < SNN_SIM_NUM_OF_DEDICATED_CPUS; i++)
	{
		spike_data = in_silico_spike_data_for_recording[i];
		end_idx = in_silico_spike_data_end_idx[i];
		while (get_next_spike_data_item(spike_data, &item))
		{
			if (spike_data->buff_idx_read == end_idx)
				break;
			fwrite(&(item), sizeof(SpikeTimeStampItem), 1, in_silico_spike_data_file_ptr);
		}
		reset_spike_data_read_idx(spike_data);
	}
	fclose(in_silico_spike_data_file_ptr);	
	return 1;
}
static int close_in_silico_network_data(void)
{
	NeuronGroup	*neuron_group;
	Neuron *neuron;
	SynapseList *syn_list;	
	Synapse	*synapses;
	unsigned int i, j, k, m;

	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			neuron_group = in_silico_network->layers[i]->neuron_groups[j];
			for (k = 0; k < neuron_group->neuron_count; k++)
			{
				neuron = &(neuron_group->neurons[k]);
				syn_list = neuron->syn_list;
				synapses = syn_list->synapses;
				for (m = 0; m < syn_list->num_of_synapses; m++)
				{
					fwrite(&(synapses[m].weight), sizeof(SynapticWeight), 1, in_silico_network_data_file_ptr);
				}
			}		
		}
	}	
	fclose(in_silico_network_data_file_ptr);	
	return 1;
}

static int close_recording_data(void)
{
	RecordingDataSample	*recording_data_chan_buff;
	FILE *fp;
	unsigned int i,j, idx, end_idx;
	RecordingDataSampleFloat sample;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_chan_buff = recording_data[i][j].rec_data_buff;
			idx = recording_data_buff_read_start_idx[i][j];
			end_idx = recording_data_buff_read_end_idx[i][j];	// before closing the files, write data up to the write indexes of the buffers determined by fclose_all_data_files_v0()
			fp = recording_data_file_ptr_arr[i][j];
			while (idx != end_idx)
			{
				sample = (RecordingDataSampleFloat)(recording_data_chan_buff[idx]);	// lower the precision for faster writing
				fwrite(&sample, sizeof(RecordingDataSampleFloat), 1, fp);
				idx++;	
				if (idx ==	RECORDING_DATA_BUFF_SIZE)
					idx = 0;	
			}
			fclose(fp);
		}
	}	
	return 1;
}


static int delete_data_files(char *data_directory_path)
{
	if (! delete_meta_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_files", "! delete_meta_data(data_directory_path)");

	if (! delete_blue_spike_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_files", "! delete_blue_spike_spike_data(data_directory_path)");
	
	if (! delete_in_silico_spike_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_files", "! delete_in_silico_spike_data(data_directory_path)");

	if (! delete_in_silico_network_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_files", "!  delete_in_silico_network_data(data_directory_path)");

/*	if (! delete_recording_data(data_directory_path))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "delete_data_files", "!  delete_recording_data(data_directory_path)");
*/
	return 1;
}

static int delete_meta_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetwork: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_blue_spike_spike_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/blue_spike_spikes");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetwork: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_in_silico_spike_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_spikes");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetwork: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_in_silico_network_data(char *data_directory_path)
{
	char temp[600];
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/in_silico_network");
	
	if (remove(temp) != 0)  { printf ("ERROR: HybridNetwork: Couldn't delete file: %s\n\n", temp); return 0; }

	return 1;
}

static int delete_recording_data(char *data_directory_path)
{
	char temp[600];
	char char_mwa_num[20], char_chan_num[20];
	unsigned int i,j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			strcpy(temp, data_directory_path);	
			if (i<10)
			{
				sprintf(char_mwa_num, "/rec_mwa_0%u" , i);
				strcat(temp, char_mwa_num);
			}
			else if (i<100)
			{
				sprintf(char_mwa_num, "/rec_mwa_%u" , i);
				strcat(temp, char_mwa_num);				
			}
			else
			{
				printf ("ERROR: HybridNetwork: MAX MWA NUM is larger than 99\n\n");
				printf ("ERROR: HybridNetwork: Couldn't create all recording files requested\n\n");
				return 0; 
			}
			strcat(temp, "_");
			if (j<10)
			{
				sprintf(char_chan_num, "chan_00%u" , j);
				strcat(temp, char_chan_num);
			}
			else if (j<100)
			{
				sprintf(char_chan_num, "chan_0%u" , j);
				strcat(temp, char_chan_num);				
			}
			else if (j<1000)
			{
				sprintf(char_chan_num, "chan_%u" , j);
				strcat(temp, char_chan_num);			
			}
			else
			{
				printf ("ERROR: HybridNetwork: MAX NUM OF CHAN PER MWA is larger than 999\n\n");
				printf ("ERROR: HybridNetwork: Couldn't create all recording files requested\n\n");
				return 0; 
			}	
			if (remove(temp) != 0)  { printf ("ERROR: HybridNetwork: Couldn't delete file: %s\n\n", temp); return 0; }
		}
	}
	return 1;
}

static int load_main_meta_file(char *path_chooser, Network *in_silico_network, Network *blue_spike_network)
{
	char temp[1000];
	char line[1000];
	char word[100];
	char *end_ptr;	
	FILE *fp;
	unsigned int i, j, k_iter, m;
	double learning_rate;	

	double parker_sochacki_error_tolerance;
	double parker_sochacki_max_order;
	unsigned int 		layer_count, neuron_group_count, neuron_count, num_of_connections;
	bool inhibitory;
	double v, u, a, b, c, d, k, E, v_resting, v_threshold, v_peak, I_inject, E_excitatory, E_inhibitory, decay_rate_excitatory, decay_rate_inhibitory, conductance_excitatory, conductance_inhibitory, k_v_threshold;
	Neuron 			*to;
	AxonalDelay		delay;
	SynapseIndex		syn_idx;
	unsigned int		num_of_axons;	
	unsigned int		target_layer;		
	unsigned int		target_neuron_group;	
	unsigned int		target_neuron_num;	

	unsigned int		num_of_synapses;	
	double weight;
	bool plastic;
	bool syn_type;
	double change_stdp_pre_post, decay_rate_stdp_pre_post;
	double eligibility_decay_rate, max_eligibility, depol_eligibility;
	bool first;
	unsigned int randomize_params = 0;

	double firing_rate;

	Neuron *this_neuron, *target_neuron;

	strcpy(temp, path_chooser);
	strcat(temp, "/meta");

	if ((fp = fopen(temp, "r")) == NULL) 
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fopen() == NULL.");

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }
	if (strcmp(line, "----------HybridNetwork - Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a HybridNetwork - Main Meta File."); }

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // DATA_FORMAT_VERSION	
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // CREATION_DATE	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // learning_rate_scaler
	strcpy(word, line);
	word[20] = 0;
	if (strcmp(word, "learning_rate_scaler") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not learning_rate_scaler line."); }
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	learning_rate = atof(word);
	reward_data.learning_rate = learning_rate;
	printf("learning_rate = %.15f\n", reward_data.learning_rate);


	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // PARKER_SOCHACKI_ERROR_TOLERANCE	
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_error_tolerance = atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // PARKER_SOCHACKI_MAX_ORDER	
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	parker_sochacki_max_order = (int)atof(word);

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............blue_spike_network.............
	if (strcmp(line, "............blue_spike_network.............\n") != 0) 
	{ 
		fclose(fp); 
		print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", line); 
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a .............blue_spike_network............ line."); 
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //blue_spike_network->layers[%u]->num_of_connections\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		num_of_connections = strtoull(word, &end_ptr, 10);
		for(j = 0; j < num_of_connections; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // %u_th_connection_to_layer(address)\t%llu\n
		}
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);
			if (!add_neuron_nodes_to_layer(blue_spike_network, neuron_count ,i, FALSE)) {
				return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "main", "add_neuron_nodes_to_layer()."); return -1; }	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				inhibitory = strtoull(word, &end_ptr, 10);
			}
		}			
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "............in_silico_network.............\n
	if (strcmp(line, "............in_silico_network.............\n") != 0) 
	{ 
		fclose(fp); 
		print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", line); 
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a .............in_silico_network............ line."); 
	}
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //blue_spike_network->layers[%u]->num_of_connections\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		num_of_connections = strtoull(word, &end_ptr, 10);
		for(j = 0; j < num_of_connections; j++)
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // %u_th_connection_to_layer(address)\t%llu\n
		}
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);

		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
			inhibitory = strtoull(word, &end_ptr, 10);

			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // IzhikevichNeuron   /// Poisson Neuron.
			if (strcmp(line, "IzhikevichNeuron\n") == 0)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				u = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				a = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				b = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				c = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				d = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				k = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_resting = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_threshold = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				v_peak = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				I_inject = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				E_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				decay_rate_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				decay_rate_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				conductance_excitatory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				conductance_inhibitory = atof(word);

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				k_v_threshold = atof(word);

				if (!add_iz_neurons_to_layer(in_silico_network, neuron_count, i, a, b, c+v_resting, d, k, 1.0/E, v_resting, v_threshold+v_resting, v_peak+v_resting, inhibitory, E_excitatory+v_resting, E_inhibitory+v_resting, -1.0/decay_rate_excitatory, -1.0/decay_rate_inhibitory, randomize_params))
					return print_message(ERROR_MSG ,"HybridNetwork", "HybridNetworkConfig", "load_main_meta_file", "! add_iz_neurons_to_layer().");	
			}
			if (strcmp(line, "PoissonNeuron\n") == 0)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //  in_silico_network->layers[i]->neuron_groups[j]->neurons[k].poisson_params->firing_rate);		
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				firing_rate = atof(word);

				if (! add_poisson_neurons_to_layer(in_silico_network, neuron_count, i, inhibitory, firing_rate))	
					return print_message(ERROR_MSG ,"HybridNetwork", "HybridNetworkConfig", "load_main_meta_file", "! add_poisson_neurons_to_layer().");	
			}			

			first = TRUE;

			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (first)
				{
					first = FALSE;
					continue;
				}
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }   // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].inhibitory\t%u\n
				inhibitory = strtoull(word, &end_ptr, 10);
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // IzhikevichNeuron   /// Poisson Neuron.
				if (strcmp(line, "IzhikevichNeuron\n") == 0)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					v = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->u\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					u = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->a\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					a = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->b\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					b = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->c\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					c = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->d\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					d = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					k = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					E = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_resting\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					v_resting = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_threshold\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					v_threshold = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->v_peak\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					v_peak = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->I_inject\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					I_inject = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_excitatory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					E_excitatory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->E_inhibitory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					E_inhibitory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_excitatory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					decay_rate_excitatory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->decay_rate_inhibitory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					decay_rate_inhibitory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_excitatory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					conductance_excitatory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->conductance_inhibitory\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					conductance_inhibitory = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].iz_params->k_v_threshold\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					k_v_threshold = atof(word);
				
				}
				if (strcmp(line, "PoissonNeuron\n") == 0)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  //  in_silico_network->layers[i]->neuron_groups[j]->neurons[k].poisson_params->firing_rate);		
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					firing_rate = atof(word);
					
				}			


			}		
		}
	}

	if (! parker_sochacki_set_order_tolerance(in_silico_network, parker_sochacki_max_order, parker_sochacki_error_tolerance))
		return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "! parker_sochacki_set_order_tolerance().");

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............blue_spike_network axon list.............\n
	if (strcmp(line, "............blue_spike_network axon list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a ............blue_spike_network axon list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n	
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				this_neuron = get_neuron_address(blue_spike_network, i, j, k_iter);					
				num_of_axons = strtoull(word, &end_ptr, 10);	
				for (m = 0; m < num_of_axons; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					to = (Neuron*)strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					delay = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_idx = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_layer = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_group = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_num = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].plastic\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					plastic = (bool)atof(word);	

					target_neuron = get_neuron_address(in_silico_network, target_layer, target_neuron_group, target_neuron_num);	
					if (! create_axon_with_values(this_neuron, target_neuron, weight, delay, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, plastic))
 						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "! create_axon_with_values."); }
				}
			}
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............in_silico_network axon list.............\n
	if (strcmp(line, "............in_silico_network axon list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a ............in_silico_network axon list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // address\t%llu\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_count
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list.............\n
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->num_of_axons\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				num_of_axons = strtoull(word, &end_ptr, 10);
				this_neuron = get_neuron_address(in_silico_network, i, j, k_iter);					
				for (m = 0; m < num_of_axons; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					to = (Neuron*)strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->delay[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					delay = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->syn_idx[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_idx = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_layer[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_layer = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_group[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_group = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->target_neuron_num[%u]\t%llu\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					target_neuron_num = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // blue_spike_network->layers[%u]->neuron_groups[%u]->neurons[%u].axon_list->to[%u]->syn_list->synapses[%u].plastic\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					plastic = (bool)atof(word);	

					target_neuron = get_neuron_address(in_silico_network, target_layer, target_neuron_group, target_neuron_num);	
					if (! create_axon_with_values(this_neuron, target_neuron, weight, delay, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, plastic))
 						{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "! create_axon_with_values."); }
				}
			}		
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // ............in_silico_network synapse list.............\n
	if (strcmp(line, "............in_silico_network synapse list.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a ............in_silico_network synapse list............. line."); }
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->num_of_neurons(i.e.all_neurons_in_network)\t%u\n
	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layer_count\t%u\n
	if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
	layer_count = strtoull(word, &end_ptr, 10);	
	for(i = 0; i < layer_count; i++)
	{
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // layer_num\t%u\n
		if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->neuron_group_count\t%u\n
		if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
		neuron_group_count = strtoull(word, &end_ptr, 10);	
		for(j = 0; j < neuron_group_count; j++)	
		{
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_group_num\t%u\n
			if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->neuron_count\t%u\n
			if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
			neuron_count = strtoull(word, &end_ptr, 10);	
			for (k_iter = 0; k_iter < neuron_count; k_iter++)
			{
				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // neuron_num\t%u\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list.............\n

				if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->num_of_synapses\t%u\n
				if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
				num_of_synapses = strtoull(word, &end_ptr, 10);	
				this_neuron = get_neuron_address(in_silico_network, i, j, k_iter);					
				for (m = 0; m < num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					weight = atof(word);

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].type\t%u\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					syn_type = strtoull(word, &end_ptr, 10);	

					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].plastic\t%u\n
					if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
					plastic = strtoull(word, &end_ptr, 10);	

					if (plastic)
					{
						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_stdp_pre_post->change_amount\t%.15f\n
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
						change_stdp_pre_post = atof(word);		
	
						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_stdp_pre_post->decay_rate\t%.15f\n
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
						decay_rate_stdp_pre_post = atof(word);	

						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->decay_rate\t%.15f\n
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
						eligibility_decay_rate = atof(word);	

						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->max_eligibility\t%.15f\n
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
						max_eligibility = atof(word);	

						if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].ps_eligibility->depol_eligibility\t%.15f\n
						if(!get_word_in_line('\t', 1, word, line, TRUE)) { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "!get_word_in_line."); }
						depol_eligibility = atof(word);	

						if (! create_ps_pre_post_stdp_for_synapse(this_neuron , change_stdp_pre_post, change_stdp_pre_post, -1.0/decay_rate_stdp_pre_post, -1.0/decay_rate_stdp_pre_post, m))
 							{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "! create_ps_pre_post_stdp_for_synapse."); }

						if (! create_ps_eligibility_for_synapse(this_neuron, -1.0/eligibility_decay_rate, -1.0/eligibility_decay_rate, m, max_eligibility, max_eligibility, depol_eligibility, depol_eligibility))
 							{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "! create_ps_pre_post_stdp_for_synapse."); }
					}
				}
			}		
		}
	}

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // "............initial synaptic weights.............\n"
	if (strcmp(line, "............initial synaptic weights.............\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a ..............initial synaptic weights.............. line."); }
	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			for (k_iter = 0; k_iter < in_silico_network->layers[i]->neuron_groups[j]->neuron_count; k_iter++)
			{
				for (m = 0; m < in_silico_network->layers[i]->neuron_groups[j]->neurons[k_iter].syn_list->num_of_synapses; m++)
				{
					if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }  // in_silico_network->layers[%u]->neuron_groups[%u]->neurons[%u].syn_list->synapses[%u].weight\t%.15f\n
				}
			}		
		}
	}	

	if (fgets(line, sizeof line, fp ) == NULL)   { fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "fgets() == NULL."); }
	if (strcmp(line, "----------HybridNetwork - End of Main Meta File----------\n") != 0) 
		{ fclose(fp); return print_message(ERROR_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Not a HybridNetwork - Main Meta File. Not end of meta file."); }

	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);

	

	fclose(fp);

	print_message(INFO_MSG ,"HybridNetwork", "DataFormat_v0", "load_main_meta_file", "Successfully read main meta file.");

	return 1;
}
static int load_data_folder(char *path_chooser, Network *in_silico_network, Network *blue_spike_network, unsigned int data_folder_num)
{
	char data_directory_name[10];
	char data_directory_num[10];
	char data_directory_path[600];
	unsigned int data_directory_cntr, i, j, k, m, count_size = 0, size;
	FILE *fp;
	NeuronGroup	*neuron_group = NULL;
	Neuron			*neuron;	
	SynapseList		*syn_list;
	Synapse			*synapses;
	
	data_directory_cntr = data_folder_num;

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
		printf("HybridNetwork: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("HybridNetwork: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}

	strcpy(data_directory_path, path_chooser);	
	strcat(data_directory_path, "/");
	strcat(data_directory_path, data_directory_name);	
	strcat(data_directory_path, "/in_silico_network");	

	if ((fp = fopen(data_directory_path, "rb")) == NULL)  { printf ("ERROR: HybridNetwork: Couldn't read file: %s\n\n", data_directory_path); return 0; }

	fseek (fp , 0 , SEEK_END);
	size = ftell (fp);
	rewind (fp);

	for(i = 0; i < in_silico_network->layer_count; i++)
	{
		for(j = 0; j < in_silico_network->layers[i]->neuron_group_count; j++)	
		{
			neuron_group = in_silico_network->layers[i]->neuron_groups[j];
			for (k = 0; k < neuron_group->neuron_count; k++)
			{
				neuron = &(neuron_group->neurons[k]);
				syn_list = neuron->syn_list;
				synapses = syn_list->synapses;
				for (m = 0; m < syn_list->num_of_synapses; m++)
				{
					if (! fread(&(synapses[m].weight), sizeof(SynapticWeight), 1, fp))   { printf ("ERROR: HybridNetwork: Couldn't read weight info from file: %s\n\n", data_directory_path); fclose(fp); return 0; } 
					printf("layer: %u, neuron_grp: %u, neuron: %u, synapse:%u, weight:%f\n", i, j, k, m, synapses[m].weight);
					count_size += sizeof(SynapticWeight);
				}
			}		
		}
	}	

	if (count_size != size) { printf ("ERROR: HybridNetwork: Invalid formatted file: %s\n\n", data_directory_path); return 0; } 
		
	fclose(fp);


	return 1;
}
