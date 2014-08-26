#ifndef HYBRID_NETWORK_DATA_H
#define HYBRID_NETWORK_DATA_H


#include <pthread.h>
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../IzNeuronSimulators/Library/Network/Network.h"
#include "../../../IzNeuronSimulators/Library/SpikeData/SpikeData.h"

RtTasksData *rt_tasks_data;

Network		*in_silico_network;
Network		*blue_spike_network;

bool	sorted_spike_delivery_enabled ;	// tell that the snn simulation is started so that bluespike can send sorted spikes to snn.



#endif
