/*
 * AFE_serial.h
 *
 *  Created on: 9/9/2015
 *      Author: Smau
 */

#ifndef AFE_AFE_SERIAL_H_
#define AFE_AFE_SERIAL_H_

#include <msp430.h>
#include <stdint.h>

#include "../ecgData.h"
#include "../CircularBuffer/circularBuffer.h"

uint8_t	AFE_send(uint8_t data);

#endif /* AFE_AFE_SERIAL_H_ */
