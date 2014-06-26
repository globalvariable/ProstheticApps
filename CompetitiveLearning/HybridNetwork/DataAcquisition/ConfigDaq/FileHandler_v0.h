#ifndef FILE_HANDLER_V0_H
#define FILE_HANDLER_V0_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../DaqCardConfig.h"
#include "../MwaConfig.h"
#include "DaqMwaMap.h"
#include "../../../../../BlueSpike/Library/Misc/Misc.h"

int write_config_daq_data_v0(int num, ...);
int read_config_daq_data_v0(int num, ...);






#endif
