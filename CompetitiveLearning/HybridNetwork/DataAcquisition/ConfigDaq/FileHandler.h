#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "FileHandler_v0.h"

// Includes handling of DaqConfig Data Format versions starting from v0. The following versions shoudl be v1, v2, v3 and so on. 

#define MAX_NUMBER_OF_DATA_FORMAT_VER	1	// increment this for every new data format handling to be implemented. 

#define DATA_FORMAT_VERSION	0	// Change this according to the intended data format/   Upper limit is MAX_NUMBER_OF_DATA_FORMAT_VER-1

int (*write_config_daq_data[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );
int (*read_config_daq_data[MAX_NUMBER_OF_DATA_FORMAT_VER]) (int num, ... );


int config_daq_initialize_data_read_write_handlers(void);
int is_config_daq_data(char *path);
int config_daq_get_format_version(int *version, char *path);

#endif
