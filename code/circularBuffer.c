/*
 * circularBuffer.c
 *
 * Double index (read/write) circular buffer
 *
 *  Created on: 29/7/2015
 *      Author: Smau
 */

#include "circularBuffer.h"
#include <stdint.h>

void circularBuffer_setup(circularBuffer* buf)
{
	buf->bufferReadIndex = 0;
	buf->bufferWriteIndex = 0;
	buf->bufferSize = BUFFER_SIZE;

	int i;
	for(i = 0; i < buf->bufferSize; i++)
	{
		buf->buffer[i] = 0;
	}
}

int circularBuffer_isEmpty(circularBuffer* buf)
{
	return buf->bufferReadIndex == buf->bufferWriteIndex;				//If both pointers are pointing to the same slot, the buffer is empty
}

int circularBuffer_isFull(circularBuffer* buf)
{
	if (buf->bufferReadIndex - 1 >= 0)
	{
		return buf->bufferReadIndex - 1 == buf->bufferWriteIndex;		//If the write pointer refers to the slot preceding the one referred to by the read pointer, the buffer is full
	}
	else
	{
		return buf->bufferSize - 1 == buf->bufferWriteIndex;			//Special case: when the read pointer is 0, the preceding one is 256
	}
}


int circularBuffer_write(circularBuffer* buf, uint8_t value)
{
	if (circularBuffer_isFull(buf))							//If the buffer is full, return a null value
	{
		return 0;
	}
	else
	{
		buf->buffer[buf->bufferWriteIndex] = value;			//If not, write the value in the buffer and
		if (buf->bufferWriteIndex == buf->bufferSize - 1)		//update write index
		{
			buf->bufferWriteIndex = 0;
		}
		else
		{
			buf->bufferWriteIndex++;
		}
		return 1;											//Return true
	}
}


int circularBuffer_read(circularBuffer* buf)
{
	if (circularBuffer_isEmpty(buf))						//If the buffer is empty, do nothing
	{
		return 0;
	}
	else
	{
		uint8_t value = buf->buffer[buf->bufferReadIndex];	//If not, read the value in the buffer and
		if (buf->bufferReadIndex == buf->bufferSize - 1)		//update read index
		{
			buf->bufferReadIndex = 0;
		}
		else
		{
			buf->bufferReadIndex++;
		}
		return value;										//Return read value
	}
}
