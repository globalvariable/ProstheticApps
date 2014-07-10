#ifndef AXONAL_DELAY_CONFIG_H
#define AXONAL_DELAY_CONFIG_H


#include "../../TaskConfig.h"
#include "../DataAcquisition/DaqCardConfig.h"
#include "../DataAcquisition/SpikeSortingConfig.h"


/// determine values below according to the latency due to PARKER_SOCHACKI_INTEGRATION_STEP_SIZE. 
#define MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY SNN_SIM_PERIOD + 1500000    /// 1.5 ms for jitters
#define MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY + 10000000   // it can be increased but it will lead to increase in event buffer size of neurons

/// determine values below according to the latency due to BlueSpike spike detection - Ext to Internal network event scheduler and internal network simulation period. 
#define MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY BLUESPIKE_DAQ_PERIOD+(SPIKE_MIN_END_SAMP_NUM*SAMPLING_INTERVAL)+1500000	 /// 1.5 ms for jitters and latency after internal trigger
#define MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY + 10000000 // it can be increased but it will lead to increase in event buffer size of neurons





#endif

