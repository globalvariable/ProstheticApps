#ifndef RECORDING_DATA_H
#define RECORDING_DATA_H

#include <stdbool.h>
#include <rtai_comedi.h>
#include <rtai_lxrt.h>

#include "MwaConfig.h"
#include "DaqCardConfig.h"
#include "ConfigDaq/DaqMwaMap.h"




#define RECORDING_DATA_BUFF_SIZE 		40000
#define RECORDING_DATA_BUFF_SIZE_1 	RECORDING_DATA_BUFF_SIZE-1
#define RECORDING_DATA_BUFF_SIZE_2 	RECORDING_DATA_BUFF_SIZE-2
#define RECORDING_DATA_BUFF_SIZE_3 	RECORDING_DATA_BUFF_SIZE-3
#define RECORDING_DATA_BUFF_SIZE_4 	RECORDING_DATA_BUFF_SIZE-4
#define RECORDING_DATA_BUFF_SIZE_5 	RECORDING_DATA_BUFF_SIZE-5
#define RECORDING_DATA_BUFF_SIZE_6 	RECORDING_DATA_BUFF_SIZE-6
#define RECORDING_DATA_BUFF_SIZE_7 	RECORDING_DATA_BUFF_SIZE-7
#define RECORDING_DATA_BUFF_SIZE_8 	RECORDING_DATA_BUFF_SIZE-8








typedef double RecordingDataSample;
typedef float RecordingDataSampleFloat;

typedef struct __RecordingDataChan
{
	RecordingDataSample	rec_data_buff[RECORDING_DATA_BUFF_SIZE];
	RecordingDataSample	filtered_recording_data_buff[RECORDING_DATA_BUFF_SIZE];
	unsigned int			buff_idx_write;      // Do not read data pointed by buff_idx_write. Read up to buff_idx_write by excluding buff_idx_write
} RecordingDataChan;

typedef RecordingDataChan  RecordingData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];


RecordingData recording_data;


bool handle_recording_data(unsigned int daq_num, lsampl_t *daq_data);





#endif
