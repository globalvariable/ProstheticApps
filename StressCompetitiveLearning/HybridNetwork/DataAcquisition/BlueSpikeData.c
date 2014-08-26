#include "BlueSpikeData.h"


static bool create_blue_spike_neuron_nodes(void);


bool initialize_blue_spike_data(void)
{
	unsigned int i;

	if (! create_blue_spike_neuron_nodes())
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "! create_blue_spike_neuron_nodes()."); 

	blue_spike_spike_data_for_graph = g_new0(SpikeData*, MAX_NUM_OF_DAQ_CARD);
	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		blue_spike_spike_data_for_graph[i] = allocate_spike_data(blue_spike_spike_data_for_graph[i], (get_num_of_neurons_in_network(blue_spike_network)*3*1000)/MAX_NUM_OF_DAQ_CARD ); // 3 seconds buffer assuming a neuron firing rate cannot be more than 1000 Hz 
	}
	blue_spike_spike_data_for_recording = g_new0(SpikeData*, MAX_NUM_OF_DAQ_CARD);	
	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		blue_spike_spike_data_for_recording[i] = allocate_spike_data(blue_spike_spike_data_for_recording[i], (get_num_of_neurons_in_network(blue_spike_network)*3*1000)/MAX_NUM_OF_UNIT_PER_CHAN); // 3 seconds buffer assuming a neuron firing rate cannot be more than 1000 Hz 
	}

	return TRUE;
}


static bool create_blue_spike_neuron_nodes(void)
{
	unsigned int i, j;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j = 0; j < MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			if (!add_neuron_nodes_to_layer(blue_spike_network, MAX_NUM_OF_UNIT_PER_CHAN_TO_HANDLE ,i, FALSE)) 
				return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "main", "add_neuron_nodes_to_layer()."); 	
		}
	}

	return TRUE;
}
