#include "DaqCard.h"


static int ni6259_comedi_configure(unsigned int card_number);
static void print_cmd(unsigned int card_number);

int open_daq_card(unsigned int card_number)
{
	char path_comedi[100], temp[10];	

	strcpy(path_comedi, "/dev/comedi");	
	sprintf(temp, "%d" , card_number);
	strcat(path_comedi, temp);
	ni6259_comedi_dev[card_number] = comedi_open(path_comedi);
	if (ni6259_comedi_dev[card_number] != NULL)
		return print_message(INFO_MSG ,"PCIe6259", "DaqCard", "open_daq_cards", "ni6259_comedi_dev[i] initialized."); 
	
	comedi_cancel(ni6259_comedi_dev[card_number], COMEDI_SUBDEVICE_AI);
	comedi_close(ni6259_comedi_dev[card_number]);			
	return print_message(ERROR_MSG ,"PCIe6259", "DaqCard", "open_daq_cards", "ni6259_comedi_dev[i] NOT initialized."); 		
}

int config_daq_card(unsigned int card_number)
{
	if (! ni6259_comedi_configure(card_number))
		return print_message(INFO_MSG ,"PCIe6259", "DaqCard", "config_daq_card", "ni6259_comedi_configure successful."); 	

	comedi_cancel(ni6259_comedi_dev[card_number], COMEDI_SUBDEVICE_AI);
	comedi_close(ni6259_comedi_dev[card_number]);			
	return print_message(ERROR_MSG ,"PCIe6259", "DaqCard", "config_daq_card", "ni6259_comedi_dev[i] NOT successful."); 			
}

static int ni6259_comedi_configure(unsigned int card_number)
{
	int i;
	memset(&ni6259_comedi_cmd[card_number], 0, sizeof(comedi_cmd));
	memset(&ni6259_comedi_chanlist[card_number], 0, sizeof(unsigned)*MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);

	ni6259_comedi_cmd[card_number].subdev = COMEDI_SUBDEVICE_AI;
	ni6259_comedi_cmd[card_number].flags = 0;
	ni6259_comedi_cmd[card_number].flags = TRIG_RT | TRIG_WAKE_EOS;   // to push data to DMA after every scan. 

	ni6259_comedi_cmd[card_number].start_src = TRIG_NOW;
	ni6259_comedi_cmd[card_number].start_arg = 0;

	ni6259_comedi_cmd[card_number].scan_begin_src = TRIG_TIMER;
	ni6259_comedi_cmd[card_number].scan_begin_arg = SAMPLING_INTERVAL*NUM_OF_SCAN;
	
	ni6259_comedi_cmd[card_number].convert_src = TRIG_TIMER;
	ni6259_comedi_cmd[card_number].convert_arg = SAMPLING_INTERVAL/MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;

	ni6259_comedi_cmd[card_number].scan_end_src = TRIG_COUNT;
	ni6259_comedi_cmd[card_number].scan_end_arg = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN;

	ni6259_comedi_cmd[card_number].stop_src = TRIG_NONE;
	ni6259_comedi_cmd[card_number].stop_arg = 0;

	ni6259_comedi_cmd[card_number].chanlist = ni6259_comedi_chanlist[card_number];
	ni6259_comedi_cmd[card_number].chanlist_len = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN;

	for (i = 0 ; i < MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN ; i++)
	{
		ni6259_comedi_chanlist[card_number][i] = CR_PACK(i % MAX_NUM_OF_CHANNEL_PER_DAQ_CARD, VOLTAGE_RANGE_6259, AREF_GROUND);
	}

	ni6259_comedi_cmd[card_number].data = NULL;
	ni6259_comedi_cmd[card_number].data_len = 0;

	print_cmd(card_number);
	printf("test 1: %i\n", comedi_command_test(ni6259_comedi_dev[card_number], &ni6259_comedi_cmd[card_number]));
	print_cmd(card_number);
	printf("test 2: %i\n", comedi_command_test(ni6259_comedi_dev[card_number],&ni6259_comedi_cmd[card_number]));
	print_cmd(card_number);

	return comedi_command(ni6259_comedi_dev[card_number], &ni6259_comedi_cmd[card_number]);
}


static void print_cmd(unsigned int card_number)
{
	printf("comedi_cmd[%d].subdev = %i\n", card_number, ni6259_comedi_cmd[card_number].subdev);
	printf("comedi_cmd[%d].flags = %i\n", card_number, ni6259_comedi_cmd[card_number].flags);

	printf("comedi_cmd[%d].start_src = %i\n", card_number, ni6259_comedi_cmd[card_number].start_src);
	printf("comedi_cmd[%d].start_arg = %i\n", card_number, ni6259_comedi_cmd[card_number].start_arg);

	printf("comedi_cmd[%d].scan_begin_src = %i\n", card_number, ni6259_comedi_cmd[card_number].scan_begin_src);
	printf("comedi_cmd[%d].scan_begin_arg = %i\n", card_number, ni6259_comedi_cmd[card_number].scan_begin_arg);
	
	printf("comedi_cmd[%d].convert_src = %i\n", card_number, ni6259_comedi_cmd[card_number].convert_src);
	printf("comedi_cmd[%d].convert_arg = %i\n", card_number, ni6259_comedi_cmd[card_number].convert_arg);

	printf("comedi_cmd[%d].scan_end_src = %i\n", card_number, ni6259_comedi_cmd[card_number].scan_end_src);
	printf("comedi_cmd[%d].scan_end_arg = %i\n", card_number, ni6259_comedi_cmd[card_number].scan_end_arg);

	printf("comedi_cmd[%d].stop_src = %i\n", card_number, ni6259_comedi_cmd[card_number].stop_src);
	printf("comedi_cmd[%d].stop_arg = %i\n", card_number, ni6259_comedi_cmd[card_number].stop_arg);

	printf("comedi_cmd[%d].chanlist_len = %i\n", card_number, ni6259_comedi_cmd[card_number].chanlist_len);
}


void close_daq_cards(unsigned int card_number)
{
	comedi_cancel(ni6259_comedi_dev[card_number], COMEDI_SUBDEVICE_AI);
	comedi_close(ni6259_comedi_dev[card_number]);
	print_message(INFO_MSG ,"PCIe6259", "DaqCard", "close_daq_cards", "comedi_close() successful."); 	
}
