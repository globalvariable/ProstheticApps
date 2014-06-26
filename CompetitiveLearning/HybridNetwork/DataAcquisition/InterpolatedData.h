#ifndef INTERPOLATED_DATA_H
#define INTERPOLATED_DATA_H




#include "MwaConfig.h"
#include "ConfigDaq/DaqMwaMap.h"
#include "RecordingData.h"


#define INTERPOLATED_DATA_BUFF_SIZE 		2 * RECORDING_DATA_BUFF_SIZE
#define INTERPOLATED_DATA_BUFF_SIZE_1 	INTERPOLATED_DATA_BUFF_SIZE-1
#define INTERPOLATED_DATA_BUFF_SIZE_2 	INTERPOLATED_DATA_BUFF_SIZE-2
#define INTERPOLATED_DATA_BUFF_SIZE_3 	INTERPOLATED_DATA_BUFF_SIZE-3









typedef RecordingDataSample InterpolatedDataSample;

typedef struct __InterpolatedDataChan
{
	InterpolatedDataSample	data_buff[INTERPOLATED_DATA_BUFF_SIZE];
	unsigned int			buff_idx_write;      // Do not read data pointed by buff_idx_write. Read up to buff_idx_write by excluding buff_idx_write
} InterpolatedDataChan;

typedef InterpolatedDataChan  InterpolatedData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];




InterpolatedData interpolated_data;



#endif
