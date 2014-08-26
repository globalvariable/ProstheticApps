#ifndef WEIGHT_HISTORY_VIEW_H
#define WEIGHT_HISTORY_VIEW_H

#define NUM_OF_WEIGHT_HISTORY_GRAPHS	4

#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.h"
#include "../../../../../IzNeuronSimulators/Library/Graphs/SynapticWeightHistGraph.h"
#include "../../HybridNetworkData.h"

bool create_synaptic_wegiht_history_view_gui(GtkWidget *tabs);


#endif
