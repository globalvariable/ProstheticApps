#ifndef SPIKESORTER_GUI_H
#define  SPIKESORTER_GUI_H


#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>

#include <gtk/gtk.h>
#include <stdbool.h>

#include <meschach/matrix.h>
#include <meschach/matrix2.h>

#include <math.h>

#include "FileHandler.h"
#include "../../../../../BlueSpike/Library/Misc/Misc.h"
#include "../TemplateMatching.h"
#include "../../HybridNetworkData.h"
#include "../../../../../IzNeuronSimulators/Library/Network/Network.h"


#define SPIKE_MEM_TO_DISPLAY_UNIT 30
#define SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE 150

#define MIN_SPIKE_NUM_FOR_TEMPLATE 60


bool create_spike_sorter_gui(GtkWidget *tabs);





#endif
