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

typedef struct circularBuffer_t
{
	int bufferReadIndex;
	int bufferWriteIndex;
	int bufferSize;

	ecgData_t ecgBuffer[BUFFER_SIZE];
}
volatile circularBuffer_t;

void	circularBuffer_setup(circularBuffer_t* buf);

int		circularBuffer_isEmpty(circularBuffer_t* buf);
int		circularBuffer_isFull(circularBuffer_t* buf);

int		circularBuffer_write(circularBuffer_t* buf, ecgData_t* value);
int		circularBuffer_read(circularBuffer_t* buf, ecgData_t* value);

#endif /* CIRCULARBUFFER_H_ */
