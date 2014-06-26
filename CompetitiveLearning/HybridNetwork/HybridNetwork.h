#ifndef HYBRID_NETWORK_H
#define HYBRID_NETWORK_H



#define _GNU_SOURCE	// for sched_setaffinity(0, sizeof(mask), &mask))


#include <sched.h>
#include <stdlib.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "Gui.h"
#include "../TaskConfig.h"
#include "../CpuConfig.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "HybridNetworkRtTask.h"
#include "SNN/SnnData.h"






#endif

