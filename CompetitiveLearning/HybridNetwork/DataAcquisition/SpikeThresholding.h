#ifndef SPIKETHRESHOLDING_H
#define SPIKETHRESHOLDING_H

#include <stdbool.h>
#include "MwaConfig.h"

typedef float SpikeThreshold;

typedef struct __SpikeThresholding
{
	SpikeThreshold 	amplitude_thres[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} SpikeThresholding;


SpikeThresholding spike_thresholding;


#endif

