#include "SnnFunctions.h"




bool add_neurons_for_in_silico_network(void)
{
	double v;
	double a;
	double b;
	double c;
	double d;
	double k;
	double C;
	double v_resting;
	double v_threshold;
	double v_peak;
//	double I_inject;
	bool inhibitory;
	double E_excitatory;
	double E_inhibitory;
	double tau_excitatory;
	double tau_inhibitory;
	unsigned int randomize_params = 0;

	if (in_silico_network->num_of_neurons != 0)
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "Previously added neurons!! Restart program to clear network.().");	

	// BASE SERVO
	get_neuron_type_parameters(NRN_TYPE_MEDIUM_SPINY, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(in_silico_network, NUM_OF_OUTPUT_SPINY_PROJECTION_NEURONS, LAYER_BASE_SERVO_EXTENSOR_SPINY, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");	

	get_neuron_type_parameters(NRN_TYPE_MEDIUM_SPINY, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(in_silico_network, NUM_OF_OUTPUT_SPINY_PROJECTION_NEURONS, LAYER_BASE_SERVO_FLEXOR_SPINY, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");

	get_neuron_type_parameters(NRN_TYPE_MEDIUM_SPINY, &v, &a, &b, &c, &d, &k, &C, &v_resting, &v_threshold, &v_peak, &inhibitory, &E_excitatory, &E_inhibitory, &tau_excitatory, &tau_inhibitory);
	if (!add_iz_neurons_to_layer(in_silico_network, NUM_OF_SUPERFLUOUS_NEURONS, LAYER_SUPERFLUOUS, a, b, c, d, k, C, v_resting, v_threshold, v_peak, inhibitory, E_excitatory, E_inhibitory, tau_excitatory, tau_inhibitory, randomize_params))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "prepare_external_and_in_silico_network", "! add_iz_neurons_to_layer().");
	
	return TRUE;
}

bool submit_parker_sochacki_integration_precision(void)
{
	if (! parker_sochacki_set_order_tolerance(in_silico_network, PS_INTEGRATION_MAX_ORDER, PS_INTEGRATION_ERR_TOL))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "submit_parker_sochacki_integration_precision", "! parker_sochacki_set_order_tolerance().");
	return TRUE;
}

bool set_output_layers(void)
{
	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	set_layer_type_of_the_neurons_in_layer(in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, NEURON_LAYER_TYPE_OUTPUT);
	return TRUE;	
}

bool connect_external_to_in_silico_network(void)
{
	Neuron *nrn;
	unsigned int i, j, k, cntr = 0;
	
	bool connection_matrix[3][2] = {	{0,1},
								{1,0},
								{1,1}};

	for (i=0; i < blue_spike_network->layer_count; i++)
	{
		for (j=0; j < blue_spike_network->layers[i]->neuron_group_count; j++)
		{
			for (k=0; k < blue_spike_network->layers[i]->neuron_groups[j]->neuron_count; k++)
			{
				nrn = get_neuron_address(blue_spike_network, i, j, k);					
				if (! nrn->include)
					continue;
				if ((connection_matrix[cntr % 3][0]))
				{
					if (! connect_neurons(blue_spike_network, i, j, k, in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, 0, 0, BLUESPIKE_2_IN_SILICO_EXCITATORY_WEIGHT_MAX, BLUESPIKE_2_IN_SILICO_EXCITATORY_WEIGHT_MIN, BLUESPIKE_2_IN_SILICO_INHIBITORY_WEIGHT_MAX, BLUESPIKE_2_IN_SILICO_INHIBITORY_WEIGHT_MIN, BLUESPIKE_2_IN_SILICO_EXCITATORY_DELAY_MAX, BLUESPIKE_2_IN_SILICO_EXCITATORY_DELAY_MIN, BLUESPIKE_2_IN_SILICO_INHIBITORY_DELAY_MAX, BLUESPIKE_2_IN_SILICO_INHIBITORY_DELAY_MIN, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				if ((connection_matrix[cntr % 3][1]))
				{
					if (! connect_neurons(blue_spike_network, i, j, k, in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, 0, 0, BLUESPIKE_2_IN_SILICO_EXCITATORY_WEIGHT_MAX, BLUESPIKE_2_IN_SILICO_EXCITATORY_WEIGHT_MIN, BLUESPIKE_2_IN_SILICO_INHIBITORY_WEIGHT_MAX, BLUESPIKE_2_IN_SILICO_INHIBITORY_WEIGHT_MIN, BLUESPIKE_2_IN_SILICO_EXCITATORY_DELAY_MAX, BLUESPIKE_2_IN_SILICO_EXCITATORY_DELAY_MIN, BLUESPIKE_2_IN_SILICO_INHIBITORY_DELAY_MAX, BLUESPIKE_2_IN_SILICO_INHIBITORY_DELAY_MIN, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, TRUE, FALSE))
						return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_neurons().");
				}
				cntr++;
				if (cntr == 18)	// connect only 18 neurons to corticostriatal circuit model.
					goto EXIT;
			}
		}
	}
	if (cntr == 0)
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "No connections has been made. no included units in sorted_spike_time_stamp.");

	EXIT:
	for (i=0; i < blue_spike_network->layer_count; i++)
	{
		if (! connect_layers(blue_spike_network, i, in_silico_network, LAYER_SUPERFLUOUS, BLUESPIKE_2_SUPERFLUOUS_EXCITATORY_WEIGHT_MAX, BLUESPIKE_2_SUPERFLUOUS_EXCITATORY_WEIGHT_MIN, BLUESPIKE_2_SUPERFLUOUS_INHIBITORY_WEIGHT_MAX, BLUESPIKE_2_SUPERFLUOUS_INHIBITORY_WEIGHT_MIN, BLUESPIKE_2_SUPERFLUOUS_EXCITATORY_DELAY_MAX, BLUESPIKE_2_SUPERFLUOUS_EXCITATORY_DELAY_MIN, BLUESPIKE_2_SUPERFLUOUS_INHIBITORY_DELAY_MAX, BLUESPIKE_2_SUPERFLUOUS_INHIBITORY_DELAY_MIN, MAXIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_BLUE_SPIKE_TO_IN_SILICO_AXONAL_DELAY,  BLUESPIKE_2_SUPERFLUOUS_EXCITATORY_PROBABILITY, BLUESPIKE_2_SUPERFLUOUS_INHIBITORY_PROBABILITY, FALSE, FALSE))
			return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");
	}

	return TRUE;
}

bool connect_medium_spiny_neurons(void)
{
	if (! connect_layers(in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, MSN_2_MSN_EXCITATORY_WEIGHT_MAX, MSN_2_MSN_EXCITATORY_WEIGHT_MIN, MSN_2_MSN_INHIBITORY_WEIGHT_MAX, MSN_2_MSN_INHIBITORY_WEIGHT_MIN, MSN_2_MSN_EXCITATORY_DELAY_MAX, MSN_2_MSN_EXCITATORY_DELAY_MIN, MSN_2_MSN_INHIBITORY_DELAY_MAX, MSN_2_MSN_INHIBITORY_DELAY_MIN, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MSN_2_MSN_EXCITATORY_PROBABILITY, MSN_2_MSN_INHIBITORY_PROBABILITY, FALSE, FALSE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	if (! connect_layers(in_silico_network, LAYER_BASE_SERVO_FLEXOR_SPINY, in_silico_network, LAYER_BASE_SERVO_EXTENSOR_SPINY, MSN_2_MSN_EXCITATORY_WEIGHT_MAX, MSN_2_MSN_EXCITATORY_WEIGHT_MIN, MSN_2_MSN_INHIBITORY_WEIGHT_MAX, MSN_2_MSN_INHIBITORY_WEIGHT_MIN, MSN_2_MSN_EXCITATORY_DELAY_MAX, MSN_2_MSN_EXCITATORY_DELAY_MIN, MSN_2_MSN_INHIBITORY_DELAY_MAX, MSN_2_MSN_INHIBITORY_DELAY_MIN, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MSN_2_MSN_EXCITATORY_PROBABILITY, MSN_2_MSN_INHIBITORY_PROBABILITY, FALSE, FALSE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	return TRUE;
}

bool connect_superfluous_neurons(void)
{
	if (! connect_layers(in_silico_network, LAYER_SUPERFLUOUS, in_silico_network, LAYER_SUPERFLUOUS, SUPERFLUOUS_2_SUPERFLUOUS_EXCITATORY_WEIGHT_MAX, SUPERFLUOUS_2_SUPERFLUOUS_EXCITATORY_WEIGHT_MIN, SUPERFLUOUS_2_SUPERFLUOUS_INHIBITORY_WEIGHT_MAX, SUPERFLUOUS_2_SUPERFLUOUS_INHIBITORY_WEIGHT_MIN, SUPERFLUOUS_2_SUPERFLUOUS_EXCITATORY_DELAY_MAX, SUPERFLUOUS_2_SUPERFLUOUS_EXCITATORY_DELAY_MIN, SUPERFLUOUS_2_SUPERFLUOUS_INHIBITORY_DELAY_MAX, SUPERFLUOUS_2_SUPERFLUOUS_INHIBITORY_DELAY_MIN, MAXIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, MINIMUM_IN_SILICO_TO_IN_SILICO_AXONAL_DELAY, SUPERFLUOUS_2_SUPERFLUOUS_EXCITATORY_PROBABILITY, SUPERFLUOUS_2_SUPERFLUOUS_INHIBITORY_PROBABILITY, FALSE, FALSE))
		return print_message(ERROR_MSG ,"HybridNetRLBMI", "HybridNetRLBMIConfig", "connect_external_to_in_silico_network", "! connect_ext_network_layer_to_int_network_layer().");

	return TRUE;
}

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
		diff_weights = sum_weights - weight_total;

		for (j = 0; j < nrn->syn_list->num_of_synapses; j++)
		{
			if (! nrn->syn_list->synapses[j].plastic)
				continue;
			 nrn->syn_list->synapses[j].weight =  nrn->syn_list->synapses[j].weight - (( nrn->syn_list->synapses[j].weight/sum_weights)*diff_weights);
		}	
	}

}

void update_synaptic_weights_all_neurons_in_thread(Neuron **all_neurons, unsigned int num_of_all_neurons, unsigned int task_num, unsigned int num_of_dedicated_cpus, double reward, double total_synaptic_weights)
{
	unsigned int i, j;
	Neuron *nrn;
	Synapse			*synapses;
	SynapseIndex		num_of_synapses; 
	double	E, dw;  // eligibility
	double sum_weights, diff_weights;

	for (i = task_num; i < num_of_all_neurons; i+=num_of_dedicated_cpus)  // simulate the neurons for which this thread is responsible
	{
		nrn = all_neurons[i];
		if (nrn->iz_params == NULL)
			continue;
		synapses = nrn->syn_list->synapses;
		num_of_synapses = nrn->syn_list->num_of_synapses;

		for (j = 0; j < num_of_synapses; j++)
		{
			if (! synapses[j].plastic)
				continue;
			if (synapses[j].ps_eligibility->now > (exp(-1.0)*synapses[j].ps_eligibility->max_eligibility))
				E = 1;
			else
				E = 0;
			
			dw = reward * E * synapses[j].weight;

			synapses[j].weight+=dw;
		}
		sum_weights = 0;
		for (j = 0; j < num_of_synapses; j++)
		{
			if (! synapses[j].plastic)
				continue;
			sum_weights += synapses[j].weight;
		}
		diff_weights = sum_weights - total_synaptic_weights;
		for (j = 0; j < num_of_synapses; j++)
		{
			if (! synapses[j].plastic)
				continue;
			synapses[j].weight = synapses[j].weight - ((synapses[j].weight/sum_weights)*diff_weights);
		}		
	}
	return;
}
