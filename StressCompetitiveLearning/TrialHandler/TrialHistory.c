#include "TrialHistory.h"



TrialHistory* allocate_trial_history(TrialHistory* hist, unsigned int buffer_size)
{
	if (hist != NULL)
	{
		hist = deallocate_trial_history(hist);
		hist = allocate_trial_history(hist, buffer_size);
		return hist;
	}  
	hist = g_new0(TrialHistory,1);
	hist->history = g_new0(TrialData, buffer_size);
	hist->buffer_size = buffer_size;	
	print_message(INFO_MSG ,"ExperimentHandler", "TrialData", "allocate_trials_history", "Created trials_history.");
	return hist;

} 

TrialHistory* deallocate_trial_history(TrialHistory* hist)
{
	if (hist == NULL)
		return (TrialHistory*)print_message(BUG_MSG ,"ExperimentHandler", "TrialData", "deallocate_trials_history", "trials_history == NULL.");    
	g_free(hist->history);
	g_free(hist);
	print_message(INFO_MSG ,"TrialControllers", "TrialsData", "deallocate_trials_data", "Destroyed trials_data.");
	return NULL;
}

ClassifiedTrialHistory* allocate_classified_trial_history(ClassifiedTrialHistory* classified_hist, unsigned int buffer_size, unsigned int num_of_target_positions)
{
	unsigned int i;
	if (classified_hist != NULL)
	{
		classified_hist = deallocate_classified_trial_history(classified_hist);
		classified_hist = allocate_classified_trial_history(classified_hist, buffer_size, num_of_target_positions);
		return classified_hist;
	}  
	classified_hist = g_new0(ClassifiedTrialHistory,1);

	classified_hist->all_trials = allocate_trial_history(classified_hist->all_trials, buffer_size);

	classified_hist->trial_types = g_new0(TrialHistory*, num_of_target_positions);
	for (i = 0; i < num_of_target_positions; i++)
	{
		classified_hist->trial_types[i] = allocate_trial_history(classified_hist->trial_types[i], buffer_size);
	}

	classified_hist->num_of_target_positions = num_of_target_positions;	

	print_message(INFO_MSG ,"ExperimentHandler", "TrialData", "allocate_classified_trial_history", "Created classified_trials_history.");
	return classified_hist;	

}
ClassifiedTrialHistory* deallocate_classified_trial_history(ClassifiedTrialHistory* classified_hist)
{
	unsigned int i;
	if (classified_hist == NULL)
		return (ClassifiedTrialHistory*)print_message(BUG_MSG ,"ExperimentHandler", "TrialData", "deallocate_classified_trial_history", "classified_trials_history == NULL.");    

	classified_hist->all_trials = deallocate_trial_history(classified_hist->all_trials);
	for (i = 0; i < classified_hist->num_of_target_positions; i++)
	{
		classified_hist->trial_types[i] = deallocate_trial_history(classified_hist->trial_types[i]);
	}
	g_free(classified_hist->trial_types);


	g_free(classified_hist);

	print_message(BUG_MSG ,"TrialControllers", "TrialsData", "deallocate_trials_data", "Destroyed trials_data.");
	return NULL;	
}

bool write_trial_data_to_classified_trial_history(ClassifiedTrialHistory* classified_history, TrialData *trial_data)
{
	TrialHistory *dest_history_data;
	TrialData *curr_trial_data;
	TrialData *prev_trial_data;

 	dest_history_data = classified_history->all_trials;
	curr_trial_data = &(dest_history_data->history[dest_history_data->buff_write_idx]);
	memcpy(curr_trial_data, trial_data, sizeof(TrialData));
	prev_trial_data = get_previous_trial_history_data_ptr(dest_history_data);
	curr_trial_data->num_of_trials = prev_trial_data->num_of_trials+1;
	curr_trial_data->success_ratio = (((double)prev_trial_data->num_of_trials * prev_trial_data->success_ratio) + (double)curr_trial_data->binary_reward) / (double)curr_trial_data->num_of_trials;
	if (curr_trial_data->trial_incomplete)
		curr_trial_data->num_of_incomplete_trials = prev_trial_data->num_of_incomplete_trials + 1;
	else
		curr_trial_data->num_of_incomplete_trials = prev_trial_data->num_of_incomplete_trials;
	if ((dest_history_data->buff_write_idx+1) == dest_history_data->buffer_size)
	{
		dest_history_data->buff_write_idx = 0;
		print_message(WARNING_MSG ,"ExperimentHandler", "TrialData", "write_trial_data_to_classified_trial_history", "classified_history->all_trials->buffer is FULL, circular buffer gets to the 0th idx to write.!!.");    	
	}
	else
	{
		dest_history_data->buff_write_idx++;
	}


 	dest_history_data = classified_history->trial_types[trial_data->robot_target_position_idx];
	curr_trial_data = &(dest_history_data->history[dest_history_data->buff_write_idx]);
	memcpy(curr_trial_data, trial_data, sizeof(TrialData));
	prev_trial_data = get_previous_trial_history_data_ptr(dest_history_data);
	curr_trial_data->num_of_trials = prev_trial_data->num_of_trials+1;
	curr_trial_data->success_ratio = (((double)prev_trial_data->num_of_trials * prev_trial_data->success_ratio) + (double)curr_trial_data->binary_reward) / (double)curr_trial_data->num_of_trials;
	if (curr_trial_data->trial_incomplete)
		curr_trial_data->num_of_incomplete_trials = prev_trial_data->num_of_incomplete_trials + 1;
	else
		curr_trial_data->num_of_incomplete_trials = prev_trial_data->num_of_incomplete_trials;
	if ((dest_history_data->buff_write_idx+1) == dest_history_data->buffer_size)
	{
		dest_history_data->buff_write_idx = 0;
		print_message(WARNING_MSG ,"ExperimentHandler", "TrialData", "write_trial_data_to_classified_trial_history", "classified_history->all_trials->buffer is FULL, circular buffer gets to the 0th idx to write.!!.");    	
	}
	else
	{
		dest_history_data->buff_write_idx++;
	}

	return TRUE;
}

TrialData* get_previous_trial_history_data_ptr(TrialHistory* hist)
{
	unsigned int idx = hist->buff_write_idx;
	if (idx == 0)
		idx = hist->buffer_size - 1;
	else
		idx --;
	return &(hist->history[idx]);
}

