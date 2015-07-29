/*
 * circularBuffer.h
 *
 * Double index (read/write) circular buffer definition
 *
 *  Created on: 29/7/2015
 *      Author: Smau
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#define BUFFER_SIZE 257				//Size is 256 + 1 unused slot to make full/empty distinction easy
									//Source: https://en.wikipedia.org/wiki/Circular_buffer#Always_keep_one_slot_open

#include <stdint.h>

typedef struct circularBuffer
{
	int bufferReadIndex;
	int bufferWriteIndex;
	int bufferSize;

	uint8_t buffer[BUFFER_SIZE];
}
circularBuffer;

void	circularBuffer_setup(circularBuffer* buf);

int		circularBuffer_isEmpty(circularBuffer* buf);
int		circularBuffer_isFull(circularBuffer* buf);

int		circularBuffer_write(circularBuffer* buf, uint8_t value);
int		circularBuffer_read(circularBuffer* buf);

#endif /* CIRCULARBUFFER_H_ */
