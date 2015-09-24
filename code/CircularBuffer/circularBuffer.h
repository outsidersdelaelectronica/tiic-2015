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

#define BUFFER_SIZE 250

#include <msp430.h>
#include <stdint.h>

#include "ecgData.h"

typedef struct circularBuffer_t
{

	uint16_t index;
	uint16_t bufferSize;
	uint16_t fullReadIndex;
	ecgData_t ecgBuffer[BUFFER_SIZE];
}
volatile circularBuffer_t;

void	circularBuffer_setup(circularBuffer_t* buf);

//int		circularBuffer_isEmpty(circularBuffer_t* buf);
//int		circularBuffer_isFull(circularBuffer_t* buf);

int		circularBuffer_write(circularBuffer_t* buf, ecgData_t* value);
int		circularBuffer_read_last(circularBuffer_t* buf, ecgData_t* value);
int 	circularBuffer_read_full(circularBuffer_t* buf, ecgData_t* value);

#endif /* CIRCULARBUFFER_H_ */
