/*
 * touch_serial.h
 *
 *  Created on: 11/9/2015
 *      Author: slopez
 */

#ifndef TOUCH_TOUCH_SERIAL_H_
#define TOUCH_TOUCH_SERIAL_H_

#include <msp430.h>
#include <stdint.h>

#include "XPT2046_constants.h"

uint8_t	touch_serial_send(uint8_t data);

uint8_t	touch_send(uint8_t data);

#endif /* TOUCH_TOUCH_SERIAL_H_ */