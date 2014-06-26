#include "CubicInterpolation.h"


void cubic_interpolation(InterpolatedDataChan *interpolated_data_chan, RecordingDataSample *filtered_recording_data_chan_buff, unsigned int filtered_data_idx)
{
	double y0, y1, y2, y3;
	double a0, a1, a2, a3;

	InterpolatedDataSample	*interpolated_data_chan_buff;
	unsigned int			write_idx; 


	switch (filtered_data_idx)
	{
		case 0:
			y3 = filtered_recording_data_chan_buff[0];
			y2 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1];
			y1 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2];
			y0 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_3];
			break;
		case 1:
			y3 = filtered_recording_data_chan_buff[1];
			y2 = filtered_recording_data_chan_buff[0];
			y1 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1];
			y0 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_2];
			break;
		case 2:
			y3 = filtered_recording_data_chan_buff[2];
			y2 = filtered_recording_data_chan_buff[1];
			y1 = filtered_recording_data_chan_buff[0];
			y0 = filtered_recording_data_chan_buff[RECORDING_DATA_BUFF_SIZE_1];
			break;
		default:
			y3 = filtered_recording_data_chan_buff[filtered_data_idx];
			y2 = filtered_recording_data_chan_buff[filtered_data_idx-1];
			y1 = filtered_recording_data_chan_buff[filtered_data_idx-2];
			y0 = filtered_recording_data_chan_buff[filtered_data_idx-3];
			break;
	}

	a0 = (-0.5*y0) + (1.5*y1) - (1.5*y2) + (0.5*y3);
	a1 = y0 - (2.5*y1) + (2.0*y2) - (0.5*y3);
	a2 = (-0.5*y0) + (0.5*y2);
	a3 = y1;

	interpolated_data_chan_buff = interpolated_data_chan->data_buff;
	write_idx = interpolated_data_chan->buff_idx_write; 

	interpolated_data_chan_buff[write_idx] = (a0*0.125) + (a1*0.25) + (a2*0.5) + a3;	// mu = 0.5   // mid point between y1 and y2
	write_idx++;
	interpolated_data_chan_buff[write_idx] = y2;
	write_idx++;

	if (write_idx == INTERPOLATED_DATA_BUFF_SIZE)
 		write_idx = 0;

	interpolated_data_chan->buff_idx_write = write_idx;


	return;
}
