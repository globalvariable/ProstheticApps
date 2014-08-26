#ifndef NETWORK_VIEW_H
#define NETWORK_VIEW_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../NetworkConfig.h"
#include "../../HybridNetworkData.h"
#include "../SnnData.h"
#include "../SnnRtTask.h"
#include "../SnnFunctions/SnnFunctions.h"
#include "../AxonalDelayConfig.h"
#include "../NeuronDynamicsView/NeuronDynamicsView.h"
#include "../DataHandling/DataHandling.h"
#include "../../../../../IzNeuronSimulators/Library/Neuron/Neuron.h"
#include "../../../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.h"
#include "../../../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNeuronCombo.h"
#include "../../../../../IzNeuronSimulators/Library/Combos/NeuronDynamicsCombo.h"
#include "../../../../../IzNeuronSimulators/Library/Graphs/NeuronDynamicsGraph.h"
#include "../../../../../IzNeuronSimulators/Library/SpikeData/SpikeData.h"
#include "../../../../../IzNeuronSimulators/Library/Combos/LayerNrnGrpNrnSynapseCombo.h"


bool create_network_view_gui(GtkWidget *tabs);










#endif
