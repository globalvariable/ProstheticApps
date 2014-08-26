#ifndef HANDLE_RS232_BUFFERS_H
#define HANDLE_RS232_BUFFERS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "../../../ExperimentHandlers/Library/RS232/RS232Handler.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobot.h"
#include "../../../ExperimentHandlers/Library/Status/MovObjStatusHistory.h"
#include "../../../ExperimentHandlers/Library/Robot/ThreeDofRobotHistory.h"

bool init_rs232_buffers(ThreeDofRobot *robot_arm, SEM **exp_envi_rx_buff_sem, SEM **exp_envi_tx_buff_sem, ExpEnviRxShm **exp_envi_rx_buff_shm, ExpEnviTxShm **exp_envi_tx_buff_shm, TimeStamp current_time);
bool handle_exp_envi_tx_shm_and_send_rs232_adc_command(TimeStamp current_time);
bool handle_exp_envi_tx_shm_and_send_rs232_pulse_width_command(TimeStamp current_time, ThreeDofRobotPulseHistory *robot_pulse_history, double max_servo_angle_change, double spike_count_threshold);
bool handle_rs232_rx_buffer_and_write_to_exp_envi_rx_shm(void);









#endif
