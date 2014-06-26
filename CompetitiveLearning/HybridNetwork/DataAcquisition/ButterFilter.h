#ifndef BANDPASS_4TH_31250HZ_400HZ_8000HZ_H
#define BANDPASS_4TH_31250HZ_400HZ_8000HZ_H

#include "RecordingData.h"

#define FCOEF_A0	0.086183491031273
#define FCOEF_A1	0
#define FCOEF_A2	-0.344733964125094
#define FCOEF_A3	0
#define FCOEF_A4	0.517100946187641
#define FCOEF_A5	0
#define FCOEF_A6	-0.344733964125094
#define FCOEF_A7	0
#define FCOEF_A8	0.086183491031273

#define FCOEF_B0	1
#define FCOEF_B1	-3.776863265435255
#define FCOEF_B2	5.866106488868384
#define FCOEF_B3	-5.333679774535625
#define FCOEF_B4	3.643628956955165
#define FCOEF_B5	-1.914639199376246
#define FCOEF_B6	0.583957623156287
#define FCOEF_B7	-0.086326032585339
#define FCOEF_B8	0.017882918126610


void butter_bandpass_filter_31250hz_4th_400hz_8000hz(RecordingDataSample *recording_data_chan_buff, RecordingDataSample *filtered_recording_data_chan_buff, unsigned int idx);

#endif
