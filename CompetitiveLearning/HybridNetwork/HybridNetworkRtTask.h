#ifndef HYBRID_NETWORK_RT_TASK_H
#define HYBRID_NETWORK_RT_TASK_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "HybridNetworkData.h"
#include "HybridNetworkData.h"
#include "../TaskConfig.h"




bool rt_periodic_task_stay_alive;

bool start_periodic_task(void);




#endif
