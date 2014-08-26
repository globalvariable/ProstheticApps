#ifndef CONFIG_DAQ_GUI_H
#define CONFIG_DAQ_GUI_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../MwaConfig.h"
#include "../DaqCardConfig.h"
#include "../../../../../BlueSpike/Library/Misc/Misc.h"
#include "FileHandler.h"
#include "../DaqRtTask.h"

bool create_config_daq_gui(GtkWidget *tabs);




#endif
