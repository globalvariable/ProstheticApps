#ifndef BLUE_SPIKE_SPIKE_PATTERN_VIEW_H
#define BLUE_SPIKE_SPIKE_PATTERN_VIEW_H

#include "../../../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.h"
#include "../../DataAcquisition/BlueSpikeData.h"
#include "../BufferViewHandler/BufferViewHandler.h"

bool create_blue_spike_spike_pattern_view_gui(GtkWidget *tabs);
NetworkSpikePatternGraphScroll* get_blue_spike_spike_pattern_graph_ptr(void);










#endif
