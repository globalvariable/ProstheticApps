#ifndef SNN_DATA_H
#define SNN_DATA_H

#include <gtk/gtk.h>
#include <stdbool.h>

#include "../HybridNetworkData.h"
#include "../../TaskConfig.h"
#include "../../InterProcessMsgConfig.h"
#include "../../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.h"
#include "../../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.h"
#include "../../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.h"
#include "../../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.h"
#include "../../../../IzNeuronSimulators/Library/NeuronDynamicsData/NeuronDynamicsData.h"
#include "../../../../IzNeuronSimulators/Library/STDPData/STDPData.h"
#include "../../../../IzNeuronSimulators/Library/EligibilityData/EligibilityData.h"
#include "../../../../IzNeuronSimulators/Library/SpikeData/SpikeData.h"


NeuralNet2GuiMsg				*msgs_neural_net_2_gui;


NeuronDynamicsBufferLimited	*neuron_dynamics_limited_buffer;
STDPBufferLimited			*stdp_limited_buffer;
EligibilityBufferLimited			*eligibility_limited_buffer;
SpikeData					**in_silico_spike_data_for_graph;  // for visualization  // each thread writes to one buffer.
SpikeData					**in_silico_spike_data_for_recording;  // for recording to hdd // each thread writes to one buffer.


TrialHand2NeuralNetMsg		*msgs_trial_hand_2_neural_net;
NeuralNet2TrialHandMsg		*msgs_neural_net_2_trial_hand;
MovObjHand2NeuralNetMsg		**msgs_mov_obj_hand_2_neural_net_multi_thread;
NeuralNet2MovObjHandMsg		**msgs_neural_net_2_mov_obj_hand_multi_thread;
NeuralNet2GuiMsg				*msgs_neural_net_2_gui;
TrialStatusEvents				*trial_status_events;   // to show status changed in graphs




double						total_synaptic_weights;

struct RewardData		
{
	double			learning_rate;	
	double			current_reward_prediction;		
} reward_data;



bool initialize_snn_data(void);
bool connect_to_mov_obj_hand(void );
bool connect_to_trial_hand(void );



#endif
