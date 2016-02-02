/*
 * touch_coordinate.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "touch_coordinate.h"

void touch_coordinate_set(touch_coordinate_t* tCoord, uint16_t touch_xValue, uint16_t touch_yValue)
{
	uint16_t touch_xValue_flipped = 0x0FFF - touch_xValue;

	uint32_t x_tmp = (multiplication(touch_xValue_flipped , 320 )) >> 12;	// * 320 / 4096
	tCoord->xPos = (uint16_t) x_tmp;

	uint32_t y_tmp = (multiplication(touch_yValue, 240)) >> 12;			// * 240 / 4096
	tCoord->yPos = (uint16_t) y_tmp;
}
