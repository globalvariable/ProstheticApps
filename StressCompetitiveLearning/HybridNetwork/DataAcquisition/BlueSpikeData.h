#ifndef BLUESPIKE_DATA_H
#define BLUESPIKE_DATA_H

#include "DaqCardConfig.h"
#include "MwaConfig.h"
#include "../HybridNetworkData.h"
#include "../../../../IzNeuronSimulators/Library/Network/Network.h"
#include "../../../IzNeuronSimulators/Library/SpikeData/SpikeData.h"


SpikeData			**blue_spike_spike_data_for_graph;   // for visualization
SpikeData			**blue_spike_spike_data_for_recording;   // for recording


bool initialize_blue_spike_data(void);







#endif
