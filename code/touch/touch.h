/*
 * touch_module.h
 *
 * Resistive touch surface controller definitions
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include <msp430.h>
#include <stdint.h>

#include "touch_src/touch_coordinate.h"
#include "touch_src/touch_serial.h"

typedef const struct touch_t
{
	touch_coordinate_t touch_last_position;
}
touch_t;

void	touch_setup(const touch_t* touch);
void	touch_initialize(const touch_t* touch);

void	touch_request_position(const touch_t* touch);

#endif /* TOUCH_TOUCH_H_ */
