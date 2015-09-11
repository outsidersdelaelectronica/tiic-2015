/*
 * font.h
 *
 *  Created on: 11/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_FONT_H_
#define DISPLAY_FONT_H_

#include <msp430.h>
#include <stdint.h>

#define FONT_HEIGHT		16
#define FONT_WIDTH		16
#define STARTING_CHAR	0x20
#define CHAR_NUM		95

#define BYTES_PER_CHAR		FONT_HEIGHT * (FONT_WIDTH/8)
#define FONT_ARRAY_SIZE		BYTES_PER_CHAR * CHAR_NUM

typedef const struct font
{
	const uint8_t fontHeight;
	const uint8_t fontWidth;
	const uint8_t startingCharacter;
	const uint8_t characterNum;

	const uint8_t bytesPerCharacter;
	const uint8_t fontArray[FONT_ARRAY_SIZE];
} font;

uint8_t*	font_get_char(const font* currentFont, char character);

#endif /* DISPLAY_FONT_H_ */
