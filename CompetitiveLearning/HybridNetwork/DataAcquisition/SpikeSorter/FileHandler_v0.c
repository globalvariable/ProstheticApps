#include "FileHandler_v0.h"



int write_template_matching_data_v0(int num, ...)
{
	FILE *fp;	
	char  *path_folder;
	char  path[600];
	char message[600];

  	va_list arguments;
	va_start ( arguments, num );   
    	path_folder = va_arg ( arguments, char*); 
	va_end ( arguments );

	strcpy(path, path_folder);
	strcat(path, "/template_matching");

	if ((fp = fopen(path, "w")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", message); return 0; }
	
	fprintf(fp, "----------TemplateMatching Data----------\n");	
	fprintf(fp,"VERSION\t%u\n", 0);
	fprintf(fp,"MAX_NUM_OF_MWA\t%d\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%d\n",MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN\t%d\n",MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp,"NUM_OF_SAMP_PER_SPIKE\t%d\n",NUM_OF_SAMP_PER_SPIKE);	
	fprintf(fp,"-------------------------------------------\n");	
	fprintf(fp,"Template_Matching_Data_Struct\n");	
	fprintf(fp,"typedef double TemplateProbabilityThres\n");	
	fprintf(fp,"typedef struct __TemplateMatchingUnitData\n");	
	fprintf(fp,"	double 					template[NUM_OF_SAMP_PER_SPIKE];   //mean\n");	
	fprintf(fp,"	double 					inv_S[NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];  // inverted covariance\n");	
	fprintf(fp,"	double 					sqrt_det_S;\n");	
	fprintf(fp,"	double 					log_det_S;\n");	
	fprintf(fp,"	TemplateProbabilityThres 	probability_thres;\n");	
	fprintf(fp,"	bool 					sorting_on;\n");
	fprintf(fp,"	bool 					include_unit;\n");
	fprintf(fp,"} TemplateMatchingUnitData;\n");
	fprintf(fp,"TemplateMatchingUnitData TemplateMatchingData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][MAX_NUM_OF_UNIT_PER_CHAN+1]; // +1 for not sorted\n");

	fclose (fp);	

	strcpy(path, path_folder);
	strcat(path, "/template_matching.bin");
	if ((fp = fopen(path, "wb")) == NULL)  { sprintf(message, "Couldn' t create %s.", path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", message); return 0; }


	printf ("sizeof(TemplateMatchingData) = %lu\n", sizeof(TemplateMatchingData));
	fwrite(&template_matching_data, sizeof(TemplateMatchingData), 1, fp);

	fclose (fp);	

	if ((fp = fopen("SpikeSorter/path_initial_directory", "r")) == NULL)  { print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Couldn' t read SpikeSorter/path_initial_directory."); return 0; }
	fclose(fp);
	if ((fp = fopen("SpikeSorter/path_initial_directory", "w")) == NULL)  { print_message(ERROR_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Couldn' t write to SpikeSorter/path_initial_directory."); return 0; }

	strcpy(path, path_folder);
	strcat(path, "/template_matching");
	fprintf(fp, "%s", path); 
	fclose (fp);	
	print_message(INFO_MSG ,"SpikeSorter", "FileHandler", "write_template_matching_data_v0", "Succesuflly saved TemplateMatching data file.");	
	return 1;
}
int read_template_matching_data_v0(int num, ...)
{
	char *path;	
	char path_bin[600];	
	char message[500];
	char line[300];
	char word[100];
	int i, j, k, m, n, line_cntr = 0;
	int max_num_of_mwa, max_num_of_channel_per_mwa, max_num_of_unit_per_chan, num_of_samp_per_spike;
	FILE *fp=NULL;
	TemplateMatchingData	local_template_matching_data;
	Neuron *nrn;	

  	va_list arguments;
	va_start ( arguments, num );   
    	path = va_arg ( arguments, char*); 
	va_end ( arguments );

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------TemplateMatching Data----------
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message);  fclose(fp); return 0; } else {line_cntr++;}   //  MAX_NUM_OF_MWA	XX
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_mwa = (int)atof(word);
	if (MAX_NUM_OF_MWA != max_num_of_mwa)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_channel_per_mwa = (int)atof(word);	
	if (MAX_NUM_OF_CHAN_PER_MWA != max_num_of_channel_per_mwa)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	max_num_of_unit_per_chan = (int)atof(word);	
	if (MAX_NUM_OF_UNIT_PER_CHAN != max_num_of_unit_per_chan)
	{
		printf("ERROR: TemplateMatching file was saved when MAX_NUM_OF_UNIT_PER_CHAN = %d\n", max_num_of_unit_per_chan);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_UNIT_PER_CHAN);	
		fclose(fp);
		return 0;
	}
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	if (!(get_word_in_line('\t', 1, word, line, TRUE))) {	fclose(fp); return 0; }
	num_of_samp_per_spike = (int)atof(word);	
	if (NUM_OF_SAMP_PER_SPIKE != num_of_samp_per_spike)
	{
		printf("ERROR: TemplateMatching file was saved when NUM_OF_SAMP_PER_SPIKE = %d\n", num_of_samp_per_spike);
		printf("ERROR: Now it is NUM_OF_SAMP_PER_SPIKE = %d\n", NUM_OF_SAMP_PER_SPIKE);	
		fclose(fp);
		return 0;
	}
	fclose(fp);


	strcpy(path_bin, path);
	strcat(path_bin, ".bin");


	if ((fp = fopen(path_bin, "rb")) == NULL)  { sprintf(message, "Couldn' t read %s.", path_bin); print_message(ERROR_MSG ,"SpikeSorter", "FileHandler_v0", "read_template_matching_data_v0", message); return 0; }	

	rewind (fp);
	if (fread (&local_template_matching_data, sizeof(TemplateMatchingData), 1, fp) != 1)  { fclose(fp);
		return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "read_template_matching_data_v0", "! fread()."); }

	fclose(fp);

	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				for (m=0; m<NUM_OF_SAMP_PER_SPIKE; m++)
				{
					template_matching_data[i][j][k].template[m] = local_template_matching_data[i][j][k].template[m];
				}
			}
		}
	}
	for (i=0; i<max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			for (k=0; k<max_num_of_unit_per_chan; k++)
			{
				for (m=0; m < num_of_samp_per_spike; m++)
				{
					for (n=0; n<num_of_samp_per_spike; n++)
					{
						template_matching_data[i][j][k].inv_S[m][n] = local_template_matching_data[i][j][k].inv_S[m][n];
					}
				}
			}
		}
	}
	for (i=0; i<max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			for (k=0; k<max_num_of_unit_per_chan; k++)
			{
				template_matching_data[i][j][k].sqrt_det_S = local_template_matching_data[i][j][k].sqrt_det_S;
				template_matching_data[i][j][k].log_det_S = local_template_matching_data[i][j][k].log_det_S;
				template_matching_data[i][j][k].probability_thres = local_template_matching_data[i][j][k].probability_thres;
				template_matching_data[i][j][k].alarm_thres = local_template_matching_data[i][j][k].alarm_thres;
			}
		}
	}

	for (i=0; i<max_num_of_mwa; i++)
	{
		for (j=0; j<max_num_of_channel_per_mwa; j++)
		{
			for (k=0; k<max_num_of_unit_per_chan; k++)
			{
				if (local_template_matching_data[i][j][k].sorting_on)
				{
					template_matching_data[i][j][k].sorting_on = TRUE;	
				}
				else
				{
					template_matching_data[i][j][k].sorting_on = FALSE;		
				}

				if (local_template_matching_data[i][j][k].include_unit)
				{
					template_matching_data[i][j][k].include_unit = TRUE;	
					sorted_spikes[i][j].included_units[k] = TRUE;	
					nrn = get_neuron_address(blue_spike_network, i, j, k);	
					nrn->include = TRUE;
				}
				else
				{
					template_matching_data[i][j][k].include_unit = FALSE;	
					sorted_spikes[i][j].included_units[k] = FALSE;		
					nrn = get_neuron_address(blue_spike_network, i, j, k);	
					nrn->include = FALSE;		
				}

				if (local_template_matching_data[i][j][k].alarm_on)
				{
					template_matching_data[i][j][k].alarm_on = TRUE;	
				}
				else
				{
					template_matching_data[i][j][k].alarm_on = FALSE;		
				}
			}
		}
	}

	print_message(INFO_MSG ,"SpikeSorter", "FileHandler", "read_template_matching_data_v0", "Succesuflly read TemplateMatching data file.");	
	return 1;
}





