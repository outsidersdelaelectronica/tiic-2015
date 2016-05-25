/*
 * touch.h
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "gpio.h"
#include "spi.h"

#include "touch_constants.h"
#include "touch_position.h"

void touch_init();

void touch_get_position(touch_pos_t *position);

#endif /* TOUCH_TOUCH_H_ */
