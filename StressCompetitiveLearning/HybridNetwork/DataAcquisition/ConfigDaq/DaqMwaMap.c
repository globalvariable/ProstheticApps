#include "DaqMwaMap.h"



bool initialize_daq_mwa_map_data(void)
{
	unsigned int i, j;

	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		pthread_mutex_init(&(daq_mwa_map[i].mutex), NULL);	
		pthread_mutex_lock(&(daq_mwa_map[i].mutex));  
		for (j=0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{
			daq_mwa_map[i].map[j].mwa = NUM_OF_MWA_NULL;
			daq_mwa_map[i].map[j].channel = NUM_OF_CHAN_PER_MWA_NULL;
		}
		pthread_mutex_unlock(&(daq_mwa_map[i].mutex));  
	}
	
	return TRUE;
}

bool config_daq_mwa_map_data_range(unsigned int daq_num, unsigned int daq_chan_start, unsigned int daq_chan_end, unsigned int mwa_num, unsigned int mwa_chan_start, unsigned int mwa_chan_end)
{
	unsigned int i, j;

	if (daq_num >= MAX_NUM_OF_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_DAQ_CARD);
		return FALSE;
	}
	
	if (mwa_num >= MAX_NUM_OF_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_MWA);
		return FALSE;
	}	
	
	if (daq_chan_start >= MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: It should be less than %d\n",  MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
		return FALSE;
	}
	
	if (daq_chan_end >= MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
		return FALSE;
	}	
		
	if (mwa_chan_start >= MAX_NUM_OF_CHAN_PER_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: It should be less than %d\n",  MAX_NUM_OF_CHAN_PER_MWA);
		return FALSE;
	}
	
	if (mwa_chan_end >= MAX_NUM_OF_CHAN_PER_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_CHAN_PER_MWA);
		return FALSE;
	}	
	
	if (daq_chan_start > daq_chan_end)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: DAQ Card Channel Configuration  Start = %d\n", daq_chan_start);
		printf ("ERROR: DAQ Card Channel Configuration End = %d\n", daq_chan_end);	
		printf ("ERROR: Start cannot be greater than End\n");			
		return FALSE;
	}			
		
	if (mwa_chan_start > mwa_chan_end)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: Microwire Array Channel Configuration Start = %d\n", mwa_chan_start);
		printf ("ERROR: Microwire Array Channel Configuration End = %d\n", mwa_chan_end);	
		printf ("ERROR: Start cannot be greater than End\n");			
		return FALSE;
	}			
	
	if ((mwa_chan_end - mwa_chan_start) != (daq_chan_end - daq_chan_start))
	{
		printf ("ERROR: MWA Channel Range and DAQ Card Channel Range should be equal\n");
		printf ("ERROR: DAQ Card Channel Range = %d - %d\n", daq_chan_start, daq_chan_end);
		printf ("ERROR: Microwire Array Channel Range = %d - %d\n", mwa_chan_start, mwa_chan_end);	
		return FALSE;
	}

	if ((daq_num < 0) || (daq_chan_start < 0) || (daq_chan_end < 0) || (mwa_num < 0) || (mwa_chan_start < 0) || (mwa_chan_end < 0))
	{
		printf ("ERROR: No DAQ Card, Microwire Array or Channel Number can be smaller than zero\n");
		return FALSE;
	}


	j = mwa_chan_start;
	for (i = daq_chan_start; i<= daq_chan_end; i++)
	{	
		if ((daq_mwa_map[daq_num].map[i].mwa != NUM_OF_MWA_NULL) || (daq_mwa_map[daq_num].map[i].channel != NUM_OF_CHAN_PER_MWA_NULL))
		{
			printf ("WARNING: Daq Card %d, Channel %d has been configured previously\n", daq_num, i);
			printf ("WARNING: Configured as DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", daq_num, i, daq_mwa_map[daq_num].map[i].mwa, daq_mwa_map[daq_num].map[i].channel);
		}

		config_daq_mwa_map_data(daq_num, i, mwa_num, j);

		j++;  
	}

	printf ("DAQ Card -> Microwire Array mapping completed\n");
	printf ("Microwire Array -> DAQ Card mapping completed\n");

	return TRUE;
}


bool config_daq_mwa_map_data(unsigned int daq_num, unsigned int daq_chan, unsigned int mwa_num, unsigned int mwa_chan)
{


	pthread_mutex_lock(&(daq_mwa_map[daq_num].mutex)); 

	// delete previous mapping first to avoid multi task buffer handling problems.
	daq_mwa_map[daq_num].map[daq_chan].mwa = NUM_OF_MWA_NULL;
	daq_mwa_map[daq_num].map[daq_chan].channel = NUM_OF_MWA_NULL;

	daq_mwa_map[daq_num].map[daq_chan].mwa = mwa_num;
	daq_mwa_map[daq_num].map[daq_chan].channel = mwa_chan;

	pthread_mutex_unlock(&(daq_mwa_map[daq_num].mutex)); 

	return TRUE;
}

bool cancel_daq_mwa_map_data(void)
{
	unsigned int i, j;

	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		pthread_mutex_lock(&(daq_mwa_map[i].mutex)); 
		for (j=0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{
			daq_mwa_map[i].map[j].mwa = NUM_OF_MWA_NULL;
			daq_mwa_map[i].map[j].channel = NUM_OF_MWA_NULL;
		}
		pthread_mutex_unlock(&(daq_mwa_map[i].mutex)); 
	}
	
	return TRUE;
}

bool interrogate_daq_mwa_map(void)
{
	int i, j, check = 0;
	
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			if ((daq_mwa_map[i].map[j].mwa == MAX_NUM_OF_MWA) && (daq_mwa_map[i].map[j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: Idle\tChannel: Idle\n", i, j);
			}
			else if ((daq_mwa_map[i].map[j].mwa != MAX_NUM_OF_MWA) && (daq_mwa_map[i].map[j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: %d\tChannel: Idle\n", i, j, daq_mwa_map[i].map[j].mwa);			
				printf ("****************************************************************\n");			
				printf ("*************************BUG*********************************\n");
				printf ("BUG: MWA was not mapped but its channel was mapped\n");
				printf ("*************************BUG*********************************\n");
				printf ("****************************************************************\n");	
				return 0;					
			}
			else if ((daq_mwa_map[i].map[j].mwa == MAX_NUM_OF_MWA) && (daq_mwa_map[i].map[j].channel != MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: Idle\tChannel: %d\n", i, j, daq_mwa_map[i].map[j].channel);			
				printf ("*******************************************************************\n");			
				printf ("*************************BUG************************************\n");
				printf ("BUG: MWA was mapped but its channel was not mapped\n");
				printf ("*************************BUG************************************\n");
				printf ("******************************************************************\n");	
				return 0;					
			}			
			else
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: %d\tChannel: %d\n", i, j, daq_mwa_map[i].map[j].mwa, daq_mwa_map[i].map[j].channel);
				check =1;
			}
		}
	}

	if (check == 1)
	{
		return 1;
	}
	else
	{
		printf("ERROR: None of the channels of any DAQ Card was mapped\n");
		return 0;
	}	
}

