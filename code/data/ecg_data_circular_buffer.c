/*
 * ecg_data_circular_buffer.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "ecg_data_circular_buffer.h"


void ecg_data_circular_buffer_setup(ecg_data_circular_buffer_t* buf)
{
	int i;

	buf->index = 0;
	buf->full_read_index = 0;
	buf->buffer_size = BUFFER_SIZE;

	// Clear all buffer on initialization is time consuming
	// and probably unnecesary, a test without clearing
	// should be done
	for(i = 0; i < buf->buffer_size; i++)
	{
		ecg_data_clear(&(buf->ecg_buffer[i]));			//Fill with zeros every ecgData value in ecgBuffer
	}
}

int	ecg_data_circular_buffer_write(ecg_data_circular_buffer_t* buf, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
{

	buf->index++;
	if (buf->index == buf->buffer_size)			//if reached the end, start again
	{
		buf->index = 0;
	}

	ecg_data_write(&(buf->ecg_buffer[buf->index]), msbyte, midbyte , lsbyte);

	return 1;											//Return true
}

int ecg_data_circular_buffer_read_last(ecg_data_circular_buffer_t* buf, ecg_data_t* value)
{
	ecg_data_clear(value);								//Fill with zeros

	*value = buf->ecg_buffer[buf->index];				//Returns the last writen data
	return 1;											//Return true
}

int ecg_data_circular_buffer_read_full(ecg_data_circular_buffer_t* buf, ecg_data_t* value)
{
	ecg_data_clear(value);								//Fill with zeros

	buf->full_read_index++;

	if (buf->full_read_index == buf->buffer_size)		//If reading out of the memory, return 0
	{
		buf->full_read_index = 0;
		return 0;
	}

	*value = buf->ecg_buffer[buf->full_read_index];		//Returns the last writen data
	return 1;											//Return true
}
