#ifndef FILE_HANDLER_V0_H
#define FILE_HANDLER_V0_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../../../BlueSpike/Library/Misc/Misc.h"
#include "../MwaConfig.h"
#include "../TemplateMatching.h"
#include "../../HybridNetworkData.h"
#include "../../../../../IzNeuronSimulators/Library/Network/Network.h"

int write_template_matching_data_v0(int num, ...);
int read_template_matching_data_v0(int num, ...);






#endif
