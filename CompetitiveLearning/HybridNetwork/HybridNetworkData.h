#ifndef HYBRID_NETWORK_DATA_H
#define HYBRID_NETWORK_DATA_H


#include <rtai_sem.h>
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../IzNeuronSimulators/Library/Network/Network.h"


RtTasksData *rt_tasks_data;

SEM 	*sys_time_semaphore;

Network		*in_silico_network;
Network		*blue_spike_network;



#endif
