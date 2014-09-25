#ifndef TRIAL_HAND_PARADIGM_H
#define TRIAL_HAND_PARADIGM_H

#include "../../../BlueSpike/System/TimeStamp/TimeStamp.h"
#include "../../../ExperimentHandlers/Library/Coordinate/Cartesian.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"
#include "../../../BlueSpike/Library/Thresholding/Thresholding.h"
#include "TrialData.h"
#include "../../../BlueSpike/Library/Misc/Averaging.h"

#define REWARD_PREDICTION_WINDOW	4.0


typedef struct 
{
	unsigned int				num_of_robot_start_positions;
	unsigned int				num_of_robot_target_positions;
	unsigned int				*target_led_component_indexes_list;
	unsigned int				num_of_target_led_components;
	unsigned int				num_of_difficulty_levels;
	TimeStamp				*max_trial_length;			// num of difficulty levels
	TimeStamp				trial_refractory;	
	EllipsoidThreshold			min_target_reach_threshold;
	TimeStamp				get_ready_to_trial_start_length;	
//	Selected values for a trial:
	TrialData					current_trial_data;
	AveragingStruct			**target_success_average; // num_of_targets;
	AveragingStruct			**target_success_average_small; // num_of_targets;
	AveragingStruct			*all_success_average; // num_of_targets;
} TrialHandParadigmRobotReach;


#endif
