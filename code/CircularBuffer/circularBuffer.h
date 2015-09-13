/*
 * circularBuffer.h
 *
 * Double index (read/write) circular buffer definition
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#define BUFFER_SIZE 257				//Size is 256 + 1 unused slot to make full/empty distinction easy
									//Source: https://en.wikipedia.org/wiki/Circular_buffer#Always_keep_one_slot_open

#include <msp430.h>
#include <stdint.h>

#include "ecgData.h"

typedef struct circularBuffer
{
	int bufferReadIndex;
	int bufferWriteIndex;
	int bufferSize;

	ecgData ecgBuffer[BUFFER_SIZE];
}
volatile circularBuffer;

void	circularBuffer_setup(circularBuffer* buf);

int		circularBuffer_isEmpty(circularBuffer* buf);
int		circularBuffer_isFull(circularBuffer* buf);

int		circularBuffer_write(circularBuffer* buf, ecgData value);
ecgData	circularBuffer_read(circularBuffer* buf);

#endif /* CIRCULARBUFFER_H_ */
