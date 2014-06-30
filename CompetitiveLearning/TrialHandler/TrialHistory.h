#ifndef TRIAL_HISTORY_H
#define TRIAL_HISTORY_H


typedef struct __TrialHistory TrialHistory;
typedef struct __ClassifiedTrialHistory ClassifiedTrialHistory;

#include <math.h>
#include "TrialData.h"

struct __TrialHistory    
{
	TrialData				*history;
	unsigned int 			buff_write_idx;    
	unsigned int 			buffer_size;	
};

struct __ClassifiedTrialHistory
{
	TrialHistory			*all_trials;
	TrialHistory			**trial_types;	
	unsigned int num_of_target_positions;
};


TrialHistory* allocate_trial_history(TrialHistory* hist, unsigned int buffer_size); 
TrialHistory* deallocate_trial_history(TrialHistory* hist); 

ClassifiedTrialHistory* allocate_classified_trial_history(ClassifiedTrialHistory* classified_hist, unsigned int buffer_size, unsigned int num_of_target_positions);
ClassifiedTrialHistory* deallocate_classified_trial_history(ClassifiedTrialHistory* classified_hist);
bool write_trial_data_to_classified_trial_history(ClassifiedTrialHistory* classified_history, TrialData *trial_data);

TrialData* get_previous_trial_history_data_ptr(TrialHistory* hist);
#endif
