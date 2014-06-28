#ifndef HYBRID_NETWORK_DATA_H
#define HYBRID_NETWORK_DATA_H


#include <rtai_sem.h>
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../IzNeuronSimulators/Library/Network/Network.h"
#include "DataAcquisition/SortedSpikes.h"
#include "../../../IzNeuronSimulators/Library/SpikeData/SpikeData.h"

RtTasksData *rt_tasks_data;

SEM 	*sys_time_semaphore;

Network		*in_silico_network;
Network		*blue_spike_network;

bool	sorted_spike_delivery_enabled ;

SortedSpikes 			sorted_spikes;
SpikeData			**blue_spike_spike_data_for_graph;   // for visualization

#endif
