#ifndef SPIKE_SORTING_CONFIG_H
#define SPIKE_SORTING_CONFIG_H

#include "TemplateMatchingConfig.h"

#define SPIKE_END_HANDLING_DATA_BUFF_SIZE 	1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

#define IN_SPIKE_SAMPLE_CNTR_MAX	(320000/(SAMPLING_INTERVAL/2))		// not a spike if lasts longer than  ~320 us  	(SAMPLING_INTERVAL/2) due to interpolation

#define SPIKE_MIN_END_SAMP_NUM   ((NUM_OF_SAMP_PER_SPIKE *3)/ 5) * 2   //   *2 due to interpolation




#endif
