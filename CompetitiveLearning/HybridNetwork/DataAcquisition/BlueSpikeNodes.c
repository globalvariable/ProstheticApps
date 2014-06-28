#include "BlueSpikeNodes.h"




bool create_blue_spike_neuron_nodes(void)
{
	unsigned int i, j;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_neuron_nodes_to_layer(blue_spike_network, MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE ,i, FALSE)) {
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "add_neuron_nodes_to_layer()."); return -1; }				
		}
	}

	return TRUE;
}
