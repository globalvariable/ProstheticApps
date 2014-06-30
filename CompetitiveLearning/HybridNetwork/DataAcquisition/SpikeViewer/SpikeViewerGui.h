#ifndef SPIKEVIEWER_GUI_H
#define  SPIKEVIEWER_GUI_H

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>

#include <gtk/gtk.h>
#include <stdbool.h>

#include "../../../../../BlueSpike/Library/Misc/Misc.h"
#include "../MwaConfig.h"
#include "../ConfigDaq/DaqCard.h"
#include "../TemplateMatching.h"
#include "../BlueSpikeTimeStamp.h"
#include "../RecordingData.h"
#include "../SpikeThresholding.h"
#include "../../HybridNetworkData.h"

#include "FileHandler.h"



#define NUM_OF_RAW_SAMPLE_TO_DISPLAY 3125		// Adjusted for 31.25KHz sampling rate 
#define RAW_DATA_DISP_DURATION_MS 100.0	

#define SPIKE_MEM_TO_DISPLAY 30 

bool create_spike_viewer_gui(GtkWidget *tabs);





#endif
