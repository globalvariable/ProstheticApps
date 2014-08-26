#include "FileHandler.h"




int config_daq_initialize_data_read_write_handlers(void)
{
	write_config_daq_data[0] = &write_config_daq_data_v0;
	read_config_daq_data[0] = &read_config_daq_data_v0;
	
	return 1;
}

int is_config_daq_data(char *path)
{
	char message[600];
	char line[200];
	FILE *fp;
	int line_cntr = 0;

	if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "is_config_daq_data", message); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "is_config_daq_data", message);  fclose(fp); return 0; } else {line_cntr++;}   
	fclose(fp);   
	if (strcmp(line, "----------ConfigDaq Data----------\n" ) == 0)
		return 1;
	else
		return 0;	
}

int config_daq_get_format_version(int *version, char *path)
{
	char line[200];
	char word[10];
	char message[600];
	int line_cntr = 0;
	FILE *fp;
	if (is_config_daq_data(path))
	{
		if ((fp = fopen(path, "r")) == NULL)  { sprintf(message, "Couldn' t read %s.", path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "get_format_version", message); return 0; }
		if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "is_config_daq_data", message);  fclose(fp); return 0; } else {line_cntr++;}   //  ----------ConfigDaq Data----------
		if (fgets(line, sizeof line, fp ) == NULL)   { sprintf(message, "Couldn' t read %d th line of %s.", line_cntr, path); print_message(ERROR_MSG ,"ConfigDaq", "FileHandler", "is_config_daq_data", message);  fclose(fp); return 0; } else {line_cntr++;}   //  VERSION	XX
		if (!(get_word_in_line('\t', 1, word, line, TRUE)))
		{
			fclose(fp);
			return 0;
		}
		fclose(fp);   	
		*version = (int)atof(word);
		return 1;
	}
	return 0;
}
