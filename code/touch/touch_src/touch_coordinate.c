/*
 * touch_coordinate.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "touch_coordinate.h"

void touch_coordinate_set(touch_coordinate_t* tCoord, uint16_t touch_xValue, uint16_t touch_yValue)
{
//	uint16_t touch_xValue_flipped = 0x0FFF - touch_xValue;
	uint16_t tmp;
//	uint32_t x_tmp = ((uint32_t) touch_xValue_flipped * 320L) >> 12;	// * 320 / 4096
	tmp = ((0x0FFF - touch_xValue) >> 12) * 320;

//	tCoord->xPos = (uint16_t) tmp;
	tCoord->xPos = tmp;

//	uint32_t y_tmp = ((uint32_t) touch_yValue * 240L) >> 12;			// * 240 / 4096
	tmp = ((touch_yValue) >> 12) * 240;

//	tCoord->yPos = (uint16_t) y_tmp;
	tCoord->yPos = (uint8_t) tmp;
}
