#ifndef SPIKE_SORTING_H
#define SPIKE_SORTING_H


#include <stdbool.h>
#include "../../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "MwaConfig.h"
#include "ConfigDaq/DaqMwaMap.h"
#include "InterpolatedData.h"
#include "SpikeThresholding.h"
#include "TemplateMatching.h"
#include "SpikeSortingConfig.h"

typedef struct __SpikeEndHandlingItem
{
	int 			end_of_spike_in_interpolated_data_buff;    // index of SpikeEndData in filtered recording data buffer 
	TimeStamp	peak_time;
} SpikeEndHandlingItem;

typedef SpikeEndHandlingItem	SpikeEndHandlingBuff[SPIKE_END_HANDLING_DATA_BUFF_SIZE];

typedef struct __SpikeEndHandling
{
	SpikeEndHandlingBuff		spike_end_handling_buff;
	int						buff_start_idx;					// circular buffer, start reading from here. it discards the previous items
	int						buff_write_idx;
} SpikeEndHandlingChan;

typedef SpikeEndHandlingChan	SpikeEndHandling[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];

SpikeEndHandling	spike_end_handling;

void spike_sorting(unsigned int daq_num, TimeStamp previous_daq_time_ns);



#endif
