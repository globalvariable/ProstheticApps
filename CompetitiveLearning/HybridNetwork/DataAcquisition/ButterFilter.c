#include "ButterFilter.h"


void butter_bandpass_filter_31250hz_4th_400hz_8000hz(RecordingDataSample *recording_data_chan_buff, RecordingDataSample *filtered_recording_data_chan_buff, unsigned int idx)
{
	switch (idx)
	{
		case 0:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[0]) +
//												(FCOEF_A1 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
												(FCOEF_A2 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) + 
//												(FCOEF_A3 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) +
 												(FCOEF_A4 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) +
 //												(FCOEF_A5 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_7]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_8]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_7]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_8]); 	
			break;

		case 1:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[1]) +
//												(FCOEF_A1 * recording_data_chan_buff[0]) +
												(FCOEF_A2 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) + 
//												(FCOEF_A3 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) +
 												(FCOEF_A4 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) +
 //												(FCOEF_A5 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_7]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[0]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_7]); 	
			break;	

		case 2:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[2]) +
//												(FCOEF_A1 * recording_data_chan_buff[1]) +
												(FCOEF_A2 * recording_data_chan_buff[0]) + 
//												(FCOEF_A3 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
 												(FCOEF_A4 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) +
//												(FCOEF_A5 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[1]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[0]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_6]); 	
			break;	

		case 3:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[3]) +
//												(FCOEF_A1 * recording_data_chan_buff[2]) +
												(FCOEF_A2 * recording_data_chan_buff[1]) + 
//												(FCOEF_A3 * recording_data_chan_buff[0]) +
 												(FCOEF_A4 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
 //												(FCOEF_A5 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[2]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[1]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[0]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_5]); 	
			break;	

		case 4:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[4]) +
//												(FCOEF_A1 * recording_data_chan_buff[3]) +
												(FCOEF_A2 * recording_data_chan_buff[2]) + 
//												(FCOEF_A3 * recording_data_chan_buff[1]) +
 												(FCOEF_A4 * recording_data_chan_buff[0])  +
 //												(FCOEF_A5 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[3]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[2]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[1]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[0]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_4]); 	
			break;	

		case 5:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[5]) +
//												(FCOEF_A1 * recording_data_chan_buff[4]) +
												(FCOEF_A2 * recording_data_chan_buff[3]) + 
//												(FCOEF_A3 * recording_data_chan_buff[2]) +
 												(FCOEF_A4 * recording_data_chan_buff[1]) +
 //												(FCOEF_A5 * recording_data_chan_buff[0]) +
 												(FCOEF_A6 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[4]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[3]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[2]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[1]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[0]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3]); 	
			break;	

		case 6:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[6]) +
//												(FCOEF_A1 * recording_data_chan_buff[5]) +
												(FCOEF_A2 * recording_data_chan_buff[4]) + 
//												(FCOEF_A3 * recording_data_chan_buff[3]) +
 												(FCOEF_A4 * recording_data_chan_buff[2]) +
 //												(FCOEF_A5 * recording_data_chan_buff[1]) +
 												(FCOEF_A6 * recording_data_chan_buff[0]) +
 //												(FCOEF_A7 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[5]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[4]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[3]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[2]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[1]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[0]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2]); 	
			break;	

		case 7:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[7]) +
//												(FCOEF_A1 * recording_data_chan_buff[6]) +
												(FCOEF_A2 * recording_data_chan_buff[5]) + 
//												(FCOEF_A3 * recording_data_chan_buff[4]) +
 												(FCOEF_A4 * recording_data_chan_buff[3])  +
 //												(FCOEF_A5 * recording_data_chan_buff[2])  +
 												(FCOEF_A6 * recording_data_chan_buff[1])  +
 //												(FCOEF_A7 * recording_data_chan_buff[0])  +
 												(FCOEF_A8 * recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1])  -

												(FCOEF_B1 * filtered_recording_data_chan_buff[6]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[5]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[4]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[3]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[2]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[1]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[0]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1]); 	
			break;

		default:
			filtered_recording_data_chan_buff[idx] =	(FCOEF_A0 * recording_data_chan_buff[idx]) +
//												(FCOEF_A1 * recording_data_chan_buff[idx-1]) +
												(FCOEF_A2 * recording_data_chan_buff[idx-2]) + 
//												(FCOEF_A3 * recording_data_chan_buff[idx-3]) +
 												(FCOEF_A4 * recording_data_chan_buff[idx-4]) +
 //												(FCOEF_A5 * recording_data_chan_buff[idx-5]) +
 												(FCOEF_A6 * recording_data_chan_buff[idx-6]) +
 //												(FCOEF_A7 * recording_data_chan_buff[idx-7]) +
 												(FCOEF_A8 * recording_data_chan_buff[idx-8]) -

												(FCOEF_B1 * filtered_recording_data_chan_buff[idx-1]) -
												(FCOEF_B2 * filtered_recording_data_chan_buff[idx-2]) -
												(FCOEF_B3 * filtered_recording_data_chan_buff[idx-3]) -
												(FCOEF_B4 * filtered_recording_data_chan_buff[idx-4]) - 	
												(FCOEF_B5 * filtered_recording_data_chan_buff[idx-5]) - 	
												(FCOEF_B6 * filtered_recording_data_chan_buff[idx-6]) - 	
												(FCOEF_B7 * filtered_recording_data_chan_buff[idx-7]) - 	
												(FCOEF_B8 * filtered_recording_data_chan_buff[idx-8]); 	
			break;
	}
	return;

}
