/*
 * AFE_module.h
 *
 * AFE function definitions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#ifndef AFE_AFE_H_
#define AFE_AFE_H_

#include <msp430.h>
#include <stdint.h>

#include "AFE_serial.h"
#include "../CircularBuffer/circularBuffer.h"

void	AFE_setup();
void	AFE_initialize();

#endif /* AFE_AFE_H_ */
