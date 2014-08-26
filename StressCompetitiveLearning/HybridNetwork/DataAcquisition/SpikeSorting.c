#include "SpikeSorting.h"


static bool is_index_between_indexes(int start_idx, int end_idx, int this_idx);
static bool handle_spike_end_handling_buffer(unsigned int mwa,  unsigned int mwa_chan, unsigned int daq_num);
static bool find_spike_end(unsigned int mwa,  unsigned int mwa_chan, TimeStamp previous_daq_time_ns, unsigned int daq_num);


bool spike_sorting(unsigned int daq_num, TimeStamp previous_daq_time_ns)
{
	unsigned int i;
	DaqMwaMapItem *map_item;

	
	for (i = 0; i < MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; i++)
	{
		map_item = &(daq_mwa_map[daq_num].map[i]);
		if (map_item->mwa == MAX_NUM_OF_MWA)   // this daq channel not mapped. skip this channel.
			continue;
		if (! handle_spike_end_handling_buffer(map_item->mwa, map_item->channel, daq_num))
			return print_message(ERROR_MSG ,"HybridNetwork", "SpikeSorting", "spike_sorting", "! handle_spike_end_handling_buffer()."); 
		if (! find_spike_end(map_item->mwa, map_item->channel, previous_daq_time_ns, daq_num))
			return print_message(ERROR_MSG ,"HybridNetwork", "SpikeSorting", "spike_sorting", "! find_spike_end()."); 
	}
	return TRUE;
}	

// Multiple Daq Card tasks can write into these arrays since each task writes for different MWA-Channels.
static unsigned int interpolated_data_buff_idx_prev[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] ={{0}};      // Do not read data pointed by buff_idx_write. Read up to buff_idx_write by excluding buff_idx_write
static bool in_spike_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};
static bool after_in_spike_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};
static int in_spike_sample_cntr_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};

static bool find_spike_end(unsigned int mwa,  unsigned int mwa_chan, TimeStamp previous_daq_time_ns, unsigned int daq_num)
{
	InterpolatedDataChan	*interpolated_data_chan;
	InterpolatedDataSample	*interpolated_data_chan_buff;
	int previous_acquisition_time_cntr =0; 
	int idx, end_idx, min_idx, i, spike_end_idx, handle_for_peak_idx;
	float amplitude_thres, min;
	bool in_spike;
	bool after_in_spike;
	int in_spike_sample_cntr;
	SpikeEndHandlingItem 	*spike_end_handling_buff;	
	TimeStamp peak_time;		
	SpikeEndHandlingChan	*spike_end_handling_chan;
	int write_idx;
	
	spike_end_handling_chan = &(spike_end_handling[mwa][mwa_chan]);

	interpolated_data_chan = &(interpolated_data[mwa][mwa_chan]);
	amplitude_thres = spike_thresholding.amplitude_thres[mwa][mwa_chan];
	if (amplitude_thres == 0.0)
	{
		interpolated_data_buff_idx_prev[mwa][mwa_chan] = interpolated_data_chan->buff_idx_write;
		spike_end_handling_chan->buff_start_idx = spike_end_handling_chan->buff_write_idx;
		return TRUE;
	}
	

	interpolated_data_chan_buff = interpolated_data_chan->data_buff;
	spike_end_handling_buff = spike_end_handling_chan->spike_end_handling_buff;
	
	idx = interpolated_data_buff_idx_prev[mwa][mwa_chan];
	end_idx = interpolated_data_chan->buff_idx_write;
	in_spike = in_spike_arr[mwa][mwa_chan];
	after_in_spike = after_in_spike_arr[mwa][mwa_chan];
	in_spike_sample_cntr = in_spike_sample_cntr_arr[mwa][mwa_chan];

	while (idx != end_idx)
	{	
		previous_acquisition_time_cntr++;


		if ( (interpolated_data_chan_buff[idx] < amplitude_thres) && (!after_in_spike) && (!in_spike))
		{
			in_spike = 1;
		}
		if (in_spike)
		{
			in_spike_sample_cntr++; 
			if (in_spike_sample_cntr > IN_SPIKE_SAMPLE_CNTR_MAX)	// not a spike if lasts longer than  ~320 us  
			{
				in_spike_sample_cntr = 0;	
				in_spike = 0;				// discard this noisy thing
				after_in_spike = 1;
			}
		}
		if (( interpolated_data_chan_buff[idx] > amplitude_thres) && after_in_spike)
		{
			after_in_spike = 0;
		}
		if ( (interpolated_data_chan_buff[idx] > amplitude_thres) && (in_spike))
		{
			in_spike = 0;
			in_spike_sample_cntr = 0;
			min = interpolated_data_chan_buff[idx];
			min_idx = idx;

			for (i=0; i< IN_SPIKE_SAMPLE_CNTR_MAX; i++)	
			{
				if (idx < i)   // if (idx - i <0) 
				{
					handle_for_peak_idx = idx-i+INTERPOLATED_DATA_BUFF_SIZE;
					if (interpolated_data_chan_buff[handle_for_peak_idx] < min)
					{
						min =  interpolated_data_chan_buff[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}	
				}
				else
				{
					handle_for_peak_idx = idx-i;
					if (interpolated_data_chan_buff[handle_for_peak_idx] < min)
					{
						min =  interpolated_data_chan_buff[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}		
				}
			}
			if ((min_idx+SPIKE_MIN_END_SAMP_NUM) >= INTERPOLATED_DATA_BUFF_SIZE)
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM - INTERPOLATED_DATA_BUFF_SIZE;
			else		
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM;
			
			peak_time = previous_daq_time_ns + (previous_acquisition_time_cntr * (SAMPLING_INTERVAL/2));	// SAMPLING_INTERVAL/2 due to interpolation
				
			if (is_index_between_indexes(interpolated_data_buff_idx_prev[mwa][mwa_chan], interpolated_data_chan->buff_idx_write, spike_end_idx))
			{
				if (! run_template_matching(mwa, mwa_chan, spike_end_idx, peak_time, daq_num))
					return print_message(ERROR_MSG ,"HybridNetwork", "SpikeSorting", "find_spike_end", "! run_template_matching()."); 
			}			
			else 	//   Write spike end into shared_memory->spike_end_handing
			{
				write_idx = spike_end_handling_chan->buff_write_idx;
				spike_end_handling_buff[write_idx].end_of_spike_in_interpolated_data_buff = spike_end_idx;     // push to the end off buffer to be handled later on
				spike_end_handling_buff[write_idx].peak_time = peak_time;
				if ((write_idx + 1) == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
					write_idx = 0;
				else
					write_idx++;	
				spike_end_handling_chan->buff_write_idx = write_idx;
			}
		}
		idx ++;
		if (idx ==	INTERPOLATED_DATA_BUFF_SIZE)
			idx = 0;
	}
	in_spike_arr[mwa][mwa_chan] = in_spike;
	after_in_spike_arr[mwa][mwa_chan] = after_in_spike;
	in_spike_sample_cntr_arr[mwa][mwa_chan] = in_spike_sample_cntr;
	interpolated_data_buff_idx_prev[mwa][mwa_chan] = interpolated_data_chan->buff_idx_write;

	return TRUE;
}


static bool handle_spike_end_handling_buffer(unsigned int mwa,  unsigned int mwa_chan, unsigned int daq_num)
{
	int interpolated_data_buff_idx;	
	TimeStamp	peak_time;
	int idx, end_idx;	
	int write_idx;
	SpikeEndHandlingItem 	*spike_end_handling_buff;	
	SpikeEndHandlingChan	*spike_end_handling_chan;

	
	spike_end_handling_chan = &(spike_end_handling[mwa][mwa_chan]);
	spike_end_handling_buff = spike_end_handling_chan->spike_end_handling_buff;

	idx = spike_end_handling_chan->buff_start_idx;
	end_idx = spike_end_handling_chan->buff_write_idx;
	
	while (idx != end_idx)
	{	
		interpolated_data_buff_idx = spike_end_handling_buff[idx].end_of_spike_in_interpolated_data_buff;
		peak_time = spike_end_handling_buff[idx].peak_time;
		
		if (is_index_between_indexes(interpolated_data_buff_idx_prev[mwa][mwa_chan], interpolated_data[mwa][mwa_chan].buff_idx_write, interpolated_data_buff_idx))
		{
			if (! run_template_matching(mwa, mwa_chan, interpolated_data_buff_idx, peak_time, daq_num))
				return print_message(ERROR_MSG ,"HybridNetwork", "SpikeSorting", "handle_spike_end_handling_buffer", "! run_template_matching()."); 
		}	
		else
		{
			write_idx = spike_end_handling_chan->buff_write_idx;
			spike_end_handling_buff[write_idx].end_of_spike_in_interpolated_data_buff = interpolated_data_buff_idx;;     // push to the end off buffer to be handled later on
			spike_end_handling_buff[write_idx].peak_time = peak_time;
			if ((write_idx + 1) == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				write_idx = 0;
			else
				write_idx++;	
			spike_end_handling_chan->buff_write_idx = write_idx;
		}
		idx ++;
		if (idx ==	SPIKE_END_HANDLING_DATA_BUFF_SIZE)
			idx = 0;
	}
	spike_end_handling_chan->buff_start_idx = end_idx;
	return TRUE;
}




static bool is_index_between_indexes(int start_idx, int end_idx, int this_idx)
{
	if (
		(
			(start_idx < end_idx) && (start_idx <= this_idx) && (this_idx < end_idx )
		)
		||
		(
			( start_idx > end_idx) &&
								(
									(start_idx <= this_idx ) || (this_idx < end_idx )
																		)
		)
	)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
