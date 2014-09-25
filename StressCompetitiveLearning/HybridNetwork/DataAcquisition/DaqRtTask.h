#ifndef DAQ_RT_TASK_H
#define DAQ_RT_TASK_H



#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include <rtai_comedi.h>
#include <rtai_lxrt.h>
#include "DaqCardConfig.h"
#include "../../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../../BlueSpike/Library/Misc/Summing.h"
#include "../HybridNetworkData.h"
#include "../../TaskConfig.h"
#include "ConfigDaq/DaqCard.h"
#include "../HybridNetworkRtTask.h"
#include "ConfigDaq/DaqMwaMap.h"
#include "RecordingData.h"
#include "SpikeSorting.h"



bool daq_cards_on;

bool start_acquisition(void);


#endif

