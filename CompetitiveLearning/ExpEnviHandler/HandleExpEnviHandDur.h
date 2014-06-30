#ifndef EXP_ENVI_HAND_DUR_H
#define EXP_ENVI_HAND_DUR_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include "../../../ExperimentHandlers/Library/Messages/ExpEnviDurHand2ExpEnviHand.h"
#include "../../../ExperimentHandlers/Library/Status/ExpEnviDurStatus.h"

bool handle_exp_envi_handler_duration(ExpEnviDurationStatus *exp_envi_inputs_min_dur_status, ExpEnviDurationStatus *exp_envi_inputs_max_dur_status, TimeStamp current_time, TimeStamp *inputs_handling_end_time_min, TimeStamp *inputs_handling_end_time_max, unsigned int num_of_input_components, ExpEnviDurHand2ExpEnviHandMsg *msgs_exp_envi_dur_hand_2_exp_envi_hand, unsigned int num_of_output_components, ExpEnviDurationStatus *exp_envi_outputs_dur_status, TimeStamp *outputs_handling_end_time);



#endif
