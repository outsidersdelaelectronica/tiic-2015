/*
 * ecg_data_circular_buffer.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef DATA_ECG_DATA_CIRCULAR_BUFFER_H_
#define DATA_ECG_DATA_CIRCULAR_BUFFER_H_

#include <msp430.h>
#include <stdint.h>

#include "ecg_data.h"

#define BUFFER_SIZE 250

typedef struct ecg_data_circular_buffer_t
{
	uint16_t index;
	uint16_t buffer_size;
	uint16_t full_read_index;

	ecg_data_t ecg_buffer[BUFFER_SIZE];
}
ecg_data_circular_buffer_t;

void	ecg_data_circular_buffer_setup(ecg_data_circular_buffer_t* buf);

int		ecg_data_circular_buffer_write(ecg_data_circular_buffer_t* buf, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte);
int		ecg_data_circular_buffer_read_last(ecg_data_circular_buffer_t* buf, ecg_data_t* value);
int 	ecg_data_circular_buffer_read_full(ecg_data_circular_buffer_t* buf, ecg_data_t* value);

#endif /* DATA_ECG_DATA_CIRCULAR_BUFFER_H_ */
