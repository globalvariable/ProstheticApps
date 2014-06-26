#include "SnnFunctions.h"





void normalize_plastic_synaptic_weights(Network *in_silico_network, double weight_total)
{
	Neuron	**all_neurons;
	unsigned int num_of_all_neurons, i, j;
	double sum_weights, diff_weights;	
	Neuron *nrn;

	all_neurons = in_silico_network->all_neurons;

	num_of_all_neurons = in_silico_network->num_of_neurons;

	for (i = 0; i < num_of_all_neurons; i++) 
	{
		nrn = all_neurons[i];
		sum_weights = 0;
		for (j = 0; j < nrn->syn_list->num_of_synapses; j++)
		{
			if (! nrn->syn_list->synapses[j].plastic)
				continue;
			sum_weights += nrn->syn_list->synapses[j].weight;		
		}
		diff_weights = sum_weights - get_hybrid_net_rl_bmi_data()->total_synaptic_weights;

		for (j = 0; j < nrn->syn_list->num_of_synapses; j++)
		{
			if (! nrn->syn_list->synapses[j].plastic)
				continue;
			 nrn->syn_list->synapses[j].weight =  nrn->syn_list->synapses[j].weight - (( nrn->syn_list->synapses[j].weight/sum_weights)*diff_weights);
		}	
	}

}
