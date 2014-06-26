#include "FileHandler_v0.h"



int write_config_daq_data_v0(int num, ...)
{
	FILE *fp;	
	char  *path_folder;
	char  path[600];
	char message[600];
	int i, j;

  	va_list arguments;
	va_start ( arguments, num );   
    	path_folder = va_arg ( arguments, char*); 
	va_end ( arguments );

	strcpy(path, path_folder);
	strcat(path, "/ConfigDaq");

	if (!  interrogate_daq_mwa_map())
		return print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "write_config_daq_data_v0", "!  interrogate_daq_mwa_map().");
	 	
	if ((fp = fopen(path, "w")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "write_config_daq_data_v0", message); return 0; }
	
	fprintf(fp, "----------ConfigDaq Data----------\n");	
	fprintf(fp,"VERSION\t%u\n", 0);
	fprintf(fp,"MAX_NUM_OF_DAQ_CARD\t%u\n",MAX_NUM_OF_DAQ_CARD);
	fprintf(fp,"MAX_NUM_OF_CHANNEL_PER_DAQ_CARD\t%u\n",MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);		
	fprintf(fp,"MAX_NUM_OF_MWA\t%u\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%u\n",MAX_NUM_OF_CHAN_PER_MWA);
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			fprintf(fp, "DAQ:\t%u\tChannel:\t%u\t---->\tMWA:\t%u\tChannel:\t%u\t\n", i, j, daq_mwa_map[i].map[j].mwa, daq_mwa_map[i].map[j].channel);	
		}
	}
	fprintf(fp, "----------End of ConfigDaq Data----------\n");				
	fclose(fp);

	if ((fp = fopen("DataAcquisition/ConfigDaq/path_initial_directory", "r")) == NULL)  { print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "write_config_daq_data_v0", "Couldn' t read ConfigDaq/path_initial_directory."); return 0; }
	fclose(fp);
	if ((fp = fopen("DataAcquisition/ConfigDaq/path_initial_directory", "w")) == NULL)  { print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "write_config_daq_data_v0", "Couldn' t write to ConfigDaq/path_initial_directory."); return 0; }
	fprintf(fp, "%s", path); 
	fclose (fp);	
	print_message(INFO_MSG ,"ConfigDaq", "FileHandler", "write_config_daq_data_v0", "Succesuflly saved ConfigDaq data file.");	
	return 1;
}
int read_config_daq_data_v0(int num, ...)
{
	char *path;	
	char message[500];
	char line[300];
	char word[100];
	int i,j, line_cntr=0;
	int max_num_of_daq_card, max_num_of_channel_per_daq_card, max_num_of_mwa, max_num_of_channel_per_mwa;
	FILE *fp=NULL;
	int mwa, mwa_channel;

  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
	va_end ( arguments );

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------ConfigDaq Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_DAQ_CARD	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_daq_card = (int)atof(word);	
	if (MAX_NUM_OF_DAQ_CARD != max_num_of_daq_card )
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_DAQ_CARD = %d\n",max_num_of_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_DAQ_CARD);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_daq_card = (int)atof(word);
	if (MAX_NUM_OF_CHANNEL_PER_DAQ_CARD != max_num_of_channel_per_daq_card)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_DAQ_CARD = %d\n", max_num_of_channel_per_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
		fclose(fp);
		return 0; 
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: ConfigDaq file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}

	cancel_daq_mwa_map_data();

	for (i = 0; i<max_num_of_daq_card; i++)    /// Only configure the ones written in config file even though max_num_of_daq_card != MAX_NUM_OF_DAQ_CARD
	{	
		for (j = 0; j<max_num_of_channel_per_daq_card; j++)
		{	
			if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}  
			if (!(get_word_in_line('\t', 6, word, line, TRUE))) {	fclose(fp); return 0; }
			mwa = (int)atof(word);
			if ((mwa > MAX_NUM_OF_MWA) || (mwa < 0))  { printf("ERROR: Incompatible value at %d th line of maps file\n", line_cntr);  fclose(fp); return 0;} 
			if (!(get_word_in_line('\t', 8, word, line, TRUE))) {	fclose(fp); return 0; }
			mwa_channel = (int)atof(word);
			if ((mwa_channel > MAX_NUM_OF_CHAN_PER_MWA) || (mwa_channel < 0))  { printf("ERROR: Incompatible value at %d th line of maps file\n", line_cntr);  fclose(fp); return 0;} 

			config_daq_mwa_map_data(i, j, mwa, mwa_channel);		
		}
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
	if (strcmp(line, "----------End of ConfigDaq Data----------\n") != 0)
	{
		print_message(ERROR_MSG ,"ConfigDaq", "FileHandler_v0", "read_config_daq_data_v0", "strcmp(line, ----------End of ConfigDaq Data----------) != 0.");	
		fclose(fp);

		cancel_daq_mwa_map_data();

		return FALSE;
	}  		

	fclose(fp);

	print_message(INFO_MSG ,"ConfigDaq", "FileHandler", "read_config_daq_data_v0", "Succesuflly read ConfigDaq data file.");	
	return 1;
}





