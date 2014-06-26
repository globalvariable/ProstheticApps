#ifndef BLUESPIKETIMESTAMP_H
#define BLUESPIKETIMESTAMP_H

#include <stdbool.h>
#include "../../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "MwaConfig.h"
#include "SortedSpikes.h"

typedef struct __BlueSpikeTimeStampItem
{
	TimeStamp 	peak_time;	// spike peak time
	int 			unit;			// channel from which spike sorted
	int			interpolated_data_buff_idx;   // end of spike at interpolated data
} BlueSpikeTimeStampItem;

typedef BlueSpikeTimeStampItem	BlueSpikeTimeStampBuff[BLUESPIKE_SORTED_SPIKE_BUFF_SIZE];

typedef struct __BlueSpikeTimeStampChan
{
	BlueSpikeTimeStampBuff	buffer;
	unsigned int				buff_idx_write;
} BlueSpikeTimeStampChan;

typedef BlueSpikeTimeStampChan  BlueSpikeTimeStamp[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];


BlueSpikeTimeStamp blue_spike_time_stamp;


#endif
