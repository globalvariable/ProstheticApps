#ifndef SNN_FUNCTIONS_H
#define SNN_FUNCTIONS_H

#include "../../../NetworkConfig.h"
#include "../../HybridNetworkData.h"
#include "../IntegrationConfig.h"
#include "../AxonalDelayConfig.h"
#include "../../../../../IzNeuronSimulators/Library/Network/Network.h"
#include "../../../../../IzNeuronSimulators/Library/Neuron/NeuronTypes.h"

bool add_neurons_for_in_silico_network(void);
bool submit_parker_sochacki_integration_precision(void);
bool set_output_layers(void);
bool connect_external_to_in_silico_network(void);
bool connect_medium_spiny_neurons(void);
bool connect_babling_2_medium_spiny_neurons(void);
void normalize_plastic_synaptic_weights(Network *in_silico_network, double weight_total);




#endif
