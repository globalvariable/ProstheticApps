#ifndef EXP_ENVI_HAND_PARADIGM_H
#define EXP_ENVI_HAND_PARADIGM_H



typedef struct 
{
	unsigned int				selected_target_led_component_list_idx;
	unsigned int				*target_led_component_indexes_list;
	unsigned int				num_of_target_led_components;
	bool					restrain_on;
} ExpEnviHandParadigmRobotReach;


#endif
