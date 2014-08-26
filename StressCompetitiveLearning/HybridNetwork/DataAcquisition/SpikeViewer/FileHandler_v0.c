#include "FileHandler_v0.h"



int write_spike_thresholds_data_v0(int num, ...)
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
	strcat(path, "/SpikeThresholds");

	if ((fp = fopen(path, "w")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"SpikeViewer", "FileHandler", "write_spike_thresholds_data_v0", message); return 0; }
	
	fprintf(fp, "----------SpikeThresholds Data----------\n");	
	fprintf(fp,"VERSION\t%u\n", 0);
	fprintf(fp,"MAX_NUM_OF_MWA\t%u\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%u\n",MAX_NUM_OF_CHAN_PER_MWA);
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			fprintf(fp, "%.2f\n", spike_thresholding.amplitude_thres[i][j]);
		}
	}
	fprintf(fp, "----------End of SpikeThresholds Data----------\n");				
	fclose(fp);

	if ((fp = fopen("SpikeViewer/path_initial_directory", "r")) == NULL)  { print_message(ERROR_MSG ,"SpikeViewer", "FileHandler", "write_spike_thresholds_data_v0", "Couldn' t read SpikeViewer/path_initial_directory."); return 0; }
	fclose(fp);
	if ((fp = fopen("SpikeViewer/path_initial_directory", "w")) == NULL)  { print_message(ERROR_MSG ,"SpikeViewer", "FileHandler", "write_spike_thresholds_data_v0", "Couldn' t write to SpikeViewer/path_initial_directory."); return 0; }
	fprintf(fp, "%s", path); 
	fclose (fp);	
	print_message(INFO_MSG ,"SpikeViewer", "FileHandler", "write_spike_thresholds_data_v0", "Succesuflly saved SpikeThresholds data file.");	
	return 1;
}
int read_spike_thresholds_data_v0(int num, ...)
{
	char *path;	
	char message[500];
	char line[300];
	char word[100];
	int i,j, line_cntr=0;
	int max_num_of_mwa, max_num_of_channel_per_mwa;
	FILE *fp=NULL;
	SpikeThreshold threshold;

  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
	va_end ( arguments );

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"SpikeViewer", "FileHandler_v0", "read_spike_thresholds_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeViewer", "FileHandler_v0", "read_spike_thresholds_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------SpikeThresholds Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeViewer", "FileHandler_v0", "read_spike_thresholds_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeViewer", "FileHandler_v0", "read_spike_thresholds_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_MWA	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: SpikeThresholds file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}

	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: SpikeThresholds file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}
	for (i = 0; i < max_num_of_mwa; i++)    /// Only configure the ones written in config file even though max_num_of_daq_card != MAX_NUM_OF_DAQ_CARD
	{
		for (j = 0; j < max_num_of_channel_per_mwa; j++)
		{	
			if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}  	
			if (!(get_word_in_line('\t', 0, word, line, TRUE))) {	fclose(fp); return 0; }
			threshold = atof(word);
			if (threshold > 0) {  printf("ERROR: Threshold cannot be larger than zero: Thres: %f\n", threshold) ;  fclose(fp); return 0; } 
			spike_thresholding.amplitude_thres[i][j] = threshold;		
		}
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;} 
	if (strcmp(line, "----------End of SpikeThresholds Data----------\n") != 0)
	{
		fclose(fp);
		return print_message(ERROR_MSG ,"SpikeViewer", "FileHandler_v0", "read_spike_thresholds_data_v0", "strcmp(line, ----------End of SpikeThresholds Data----------) != 0.");	
	}  		

	fclose(fp);

	print_message(INFO_MSG ,"SpikeViewer", "FileHandler", "read_spike_thresholds_data_v0", "Succesuflly read SpikeThresholds data file.");	
	return 1;
}





