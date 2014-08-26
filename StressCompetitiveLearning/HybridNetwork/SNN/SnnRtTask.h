#ifndef SNN_RT_TASK_H
#define SNN_RT_TASK_H


#include "../../NetworkConfig.h"
#include "../HybridNetworkData.h"
#include "SnnData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../../IzNeuronSimulators/Library/ParkerSochacki/PSPrePostResetSTDPElig.h"
#include "SnnFunctions/SnnFunctions.h"

void create_snn_rt_threads(void);


#endif
