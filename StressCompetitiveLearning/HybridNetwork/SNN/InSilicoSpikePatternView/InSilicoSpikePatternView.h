#ifndef IN_SILICO_SPIKE_PATTERN_VIEW_H
#define IN_SILICO_SPIKE_PATTERN_VIEW_H

#include "../../HybridNetworkData.h"
#include "../../../../../IzNeuronSimulators/Library/Graphs/SpikePatternGraph.h"
#include "../BufferViewHandler/BufferViewHandler.h"

bool create_in_silico_spike_pattern_view_gui(GtkWidget *tabs);
NetworkSpikePatternGraphScroll* get_in_silico_spike_pattern_graph_ptr(void);





#endif
