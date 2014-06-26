#ifndef DAQ_CARD_H
#define DAQ_CARD_H

#include <stdlib.h>
#include <stdbool.h>
#include <rtai_comedi.h>
#include <rtai_lxrt.h>

#include "../DaqCardConfig.h"
#include "../../../../../BlueSpike/Library/Misc/Misc.h"

comedi_t* ni6259_comedi_dev[MAX_NUM_OF_DAQ_CARD];
comedi_cmd ni6259_comedi_cmd[MAX_NUM_OF_DAQ_CARD];
unsigned ni6259_comedi_chanlist[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN];
unsigned int ni6259_comedi_dev_ids[MAX_NUM_OF_DAQ_CARD];


int open_daq_card(unsigned int card_number);
int config_daq_card(unsigned int card_number);
void close_daq_cards(unsigned int card_number);









#endif
