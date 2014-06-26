#ifndef DAQ_CARD_CONFIG_H
#define DAQ_CARD_CONFIG_H

#define MAX_NUM_OF_DAQ_CARD					1 					// Increase if there is an additional card
#define MAX_NUM_OF_CHANNEL_PER_DAQ_CARD	32 

#define NUM_OF_SCAN							16    // num of scan to trigger interrupt for 	TRIG_RT | TRIG_WAKE_EOS	

#define NUM_OF_DAQ_CARD_NULL					MAX_NUM_OF_DAQ_CARD				
#define NUM_OF_CHANNEL_PER_DAQ_CARD_NULL	MAX_NUM_OF_CHANNEL_PER_DAQ_CARD

#define DAQ_0_MODEL 				"NI-PCIe6259"
#define COMEDI_SUBDEVICE_AI 		0
#define SAMPLING_INTERVAL 			32000     /// DO NOT CHANGE!!!
// #define VOLTAGE_RANGE_6259 		0	//	+10V,-10V
//#define VOLTAGE_RANGE_6259 		1	//	Notworking
//#define VOLTAGE_RANGE_6259 		2	//	Not working
//#define VOLTAGE_RANGE_6259 		3	//	Not working
#define VOLTAGE_RANGE_6259 		4	//	+0.5,-0.5V
//#define VOLTAGE_RANGE_6259 		5	//	+0.25,-0.25V
//#define VOLTAGE_RANGE_6259 		6	//	+0.1,-0.1V
//#define VOLTAGE_RANGE_6259 		7	//	+0.05,-0.05V

#define BASELINE_QUANT_6259 		32768.0
#define LOWEST_VOLTAGE_MV 		-500.0    // when VOLTAGE_RANGE_6259E = 4
#define HIGHEST_VOLTAGE_MV 		500.0
#define VOLTAGE_MULTIPLIER_MV_6259 	65.536     //   4096/1000.0    (0.5V - (- 0.5V) = 1000 mV)


#endif
