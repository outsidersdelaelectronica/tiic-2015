/*
 * touch_coordinate.c
 *
 *  Created on: 14/9/2015
 *      Author: slopez
 */

#include "touch_coordinate.h"

void touch_coordinate_set(touch_coordinate_t* tCoord, uint16_t touch_xValue, uint16_t touch_yValue)
{
	uint32_t x_tmp = ((uint32_t) touch_xValue * 320L) >> 12;		// * 320 / 4096
	tCoord->xPos = (uint16_t) x_tmp;

	uint32_t y_tmp = ((uint32_t) touch_yValue * 240L) >> 12;		// * 240 / 4096
	tCoord->yPos = (uint16_t) y_tmp;
}
