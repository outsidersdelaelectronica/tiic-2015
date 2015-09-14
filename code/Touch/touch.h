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

#include "touch_serial.h"
#include "touch_coordinate.h"

void	touch_setup();
void	touch_initialize();

#endif /* TOUCH_TOUCH_H_ */
