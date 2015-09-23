/*
 * circularBuffer.c
 *
 * Double index (read/write) circular buffer
 *
 *  Created on: 29/7/2015
 *      Author: Smau
 */

#include "circularBuffer.h"

void circularBuffer_setup(circularBuffer_t* buf)
{
	buf->index = 0;
	buf->fullReadIndex = 0;
	buf->bufferSize = BUFFER_SIZE;

	int i;
	for(i = 0; i < buf->bufferSize; i++)
	{
		ecgData_write(&(buf->ecgBuffer[i]), 0x00, 0x00, 0x00);			//Fill with zeros every ecgData value in ecgBuffer
	}
}

int circularBuffer_write(circularBuffer_t* buf, ecgData_t* value)
{

	if (buf->index == buf->bufferSize - 1)			//if reached the end, start again
	{
		buf->index = 0;
	}
	buf->ecgBuffer[buf->index] = *value;				//If not, write the value in the buffer and
	buf->index++;

	return 1;											//Return true
}

int circularBuffer_read_last(circularBuffer_t* buf, ecgData_t* value)
{
	ecgData_clear(value);							//Fill with zeros

	*value = buf->ecgBuffer[buf->index];			//Returns the last writen data
	return 1;										//Return true
}

int circularBuffer_read_full(circularBuffer_t* buf, ecgData_t* value)
{
	ecgData_clear(value);								//Fill with zeros

	*value = buf->ecgBuffer[buf->fullReadIndex];		//Returns the last writen data
	if (buf->fullReadIndex == buf->bufferSize - 1)			//If reading out of the memory , return 0
	{
		buf->fullReadIndex = 0;
		return 0;
	}
	buf->fullReadIndex++;
	return 1;											//Return true
}
