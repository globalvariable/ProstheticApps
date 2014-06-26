#ifndef SORTED_SPIKES_H
#define SORTED_SPIKES_H

#include "../../../../BlueSpike/System/TimeStamp/TimeStamp.h"


#define BLUESPIKE_SORTED_SPIKE_BUFF_SIZE 1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

typedef struct __SortedSpikeItem
{
	TimeStamp 		peak_time;				// spike peak time
	unsigned int		unit;				// channel from which spike sorted
} SortedSpikeItem;

typedef SortedSpikeItem	SortedSpikeBuff[BLUESPIKE_SORTED_SPIKE_BUFF_SIZE];

typedef struct __SortedSpikeChan
{
	SortedSpikeBuff			buffer;
	unsigned int				buff_idx_write;
	bool					included_units[MAX_NUM_OF_UNIT_PER_CHAN];
} SortedSpikeChan;

typedef SortedSpikeChan  SortedSpikes[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];


SortedSpikes sorted_spikes;

#endif
