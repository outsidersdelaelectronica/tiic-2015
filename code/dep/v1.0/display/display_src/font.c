/*
 * font.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "font.h"

uint8_t* font_get_char(const font_t* current_font, char character)
{
	uint8_t character_number;
	uint16_t character_position;
	const uint8_t* character_pointer;

	//Get character number (substract ASCII offset)
		character_number = ((uint8_t) character) - current_font->starting_character;
	//Get character starting position in font array
		character_position = ((uint16_t) character_number) * current_font->bytes_per_character;

	//Check if requested char exists in array
		if (character_number < current_font->character_num)
		{
			//If character exists, return its position in the array
			character_pointer = &(current_font->font_array[character_position]);
		}
		else
		{
			//If it doesn't, return '?'
			character_pointer = &(current_font->font_array[31*current_font->bytes_per_character]);
		}

	return (uint8_t*) character_pointer;
}

const font_t font =
{
	.font_height =			FONT_HEIGHT,
	.font_width =			FONT_WIDTH,
	.starting_character =	STARTING_CHAR,
	.character_num =			CHAR_NUM,
	.bytes_per_character = 	BYTES_PER_CHAR,
	.font_array = {
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // <space>
		0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,  // !
		0x03,0x30,0x03,0x30,0x03,0x30,0x03,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // "
		0x00,0x00,0x00,0x00,0x0C,0x30,0x0C,0x30,0x3F,0xFC,0x3F,0xFC,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x3F,0xFC,0x3F,0xFC,0x0C,0x30,0x0C,0x30,0x00,0x00,0x00,0x00,  // #
		0x03,0x00,0x03,0x00,0x3F,0xF0,0x3F,0xF0,0x33,0x00,0x33,0x00,0x3F,0xF0,0x3F,0xF0,0x03,0x30,0x03,0x30,0x3F,0xF0,0x3F,0xF0,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // $
		0x00,0x00,0x00,0x00,0x3C,0x0C,0x3C,0x0C,0x3C,0x30,0x3C,0x30,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x3C,0x0C,0x3C,0x30,0x3C,0x30,0x3C,0x00,0x00,0x00,0x00,  // %
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x0C,0xC0,0x0C,0xC0,0x03,0x00,0x03,0x00,0x0C,0xCC,0x0C,0xCC,0x30,0x30,0x30,0x30,0x0F,0xCC,0x0F,0xCC,0x00,0x00,0x00,0x00,  // &
		0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // '
		0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,  // (
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // )
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0xC0,0x0C,0xC0,0x03,0x00,0x03,0x00,0x3F,0xF0,0x3F,0xF0,0x03,0x00,0x03,0x00,0x0C,0xC0,0x0C,0xC0,0x00,0x00,0x00,0x00,  // *
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x3F,0xF0,0x3F,0xF0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // +
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,  // ,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x03,0xC0,0x00,0x00,0x00,0x00,  // .
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,  // /

		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0xF0,0xC0,0xF0,0xC3,0x30,0xC3,0x30,0xCC,0x30,0xCC,0x30,0xF0,0x30,0xF0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 0
		0x00,0x00,0x00,0x00,0x3C,0x00,0x3C,0x00,0xCC,0x00,0xCC,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0xFF,0xC0,0xFF,0xC0,0x00,0x00,0x00,0x00,  // 1
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0x00,0x30,0x00,0x30,0x3F,0xC0,0x3F,0xC0,0xC0,0x00,0xC0,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0x00,0x00,0x00,  // 2
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0x03,0xC0,0x03,0xC0,0x00,0x30,0x00,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 3
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x0F,0x00,0x0F,0x00,0x33,0x00,0x33,0x00,0xC3,0x00,0xC3,0x00,0xFF,0xF0,0xFF,0xF0,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // 4
		0x00,0x00,0x00,0x00,0xFF,0xF0,0xFF,0xF0,0xC0,0x00,0xC0,0x00,0xFF,0xC0,0xFF,0xC0,0x00,0x30,0x00,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 5
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x00,0xC0,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 6
		0x00,0x00,0x00,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0x30,0x00,0x30,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,  // 7
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 8
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xF0,0x3F,0xF0,0x00,0x30,0x00,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // 9
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x00,0x00,0x00,0x00,  // :
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,  // ;
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x03,0x00,0x03,0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,  // <
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0x00,0x00,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // =
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x00,0x03,0x00,0x03,0x00,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,  // >
		0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x30,0x0C,0x30,0x0C,0x00,0x30,0x00,0x30,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,  // ?

		0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x30,0xCC,0x30,0xCC,0x33,0x3C,0x33,0x3C,0x33,0xFC,0x33,0xFC,0x30,0x00,0x30,0x00,0x0F,0xF0,0x0F,0xF0,0x00,0x00,0x00,0x00,  // @
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFF,0xF0,0xFF,0xF0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // A
		0x00,0x00,0x00,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x30,0xC0,0x30,0xFF,0xC0,0xFF,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFF,0xC0,0xFF,0xC0,0x00,0x00,0x00,0x00,  // B
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // C
		0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0xC0,0xC0,0xC0,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,  // D
		0x00,0x00,0x00,0x00,0xFF,0xF0,0xFF,0xF0,0xC0,0x00,0xC0,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0x00,0x00,0x00,  // E
		0x00,0x00,0x00,0x00,0xFF,0xF0,0xFF,0xF0,0xC0,0x00,0xC0,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  // F
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x00,0xC0,0x00,0xC3,0xF0,0xC3,0xF0,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // G
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFF,0xF0,0xFF,0xF0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // H
		0x00,0x00,0x00,0x00,0x3F,0xF0,0x3F,0xF0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0x00,0x00,0x00,  // I
		0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // J
		0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0xC3,0x00,0xC3,0x00,0xFC,0x00,0xFC,0x00,0xC3,0x00,0xC3,0x00,0xC0,0xC0,0xC0,0xC0,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // K
		0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0x00,0x00,0x00,  // L
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xF0,0xF0,0xF0,0xF0,0xCF,0x30,0xCF,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // M
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xF0,0x30,0xF0,0x30,0xCC,0x30,0xCC,0x30,0xC3,0x30,0xC3,0x30,0xC0,0xF0,0xC0,0xF0,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // N
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // O

		0x00,0x00,0x00,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFF,0xC0,0xFF,0xC0,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  // P
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xCC,0x30,0xCC,0x30,0xC3,0x30,0xC3,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // Q
		0x00,0x00,0x00,0x00,0xFF,0xC0,0xFF,0xC0,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xFF,0xC0,0xFF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // R
		0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0xC0,0x00,0xC0,0x00,0x3F,0xC0,0x3F,0xC0,0x00,0x30,0x00,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // S
		0x00,0x00,0x00,0x00,0xFF,0xFC,0xFF,0xFC,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // T
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // U
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0x30,0xC0,0x30,0xC0,0x0F,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,  // V
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x30,0xCF,0x30,0xCF,0x30,0x30,0xC0,0x30,0xC0,0x00,0x00,0x00,0x00,  // W
		0x00,0x00,0x00,0x00,0xC0,0x30,0xC0,0x30,0x30,0xC0,0x30,0xC0,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x30,0xC0,0x30,0xC0,0xC0,0x30,0xC0,0x30,0x00,0x00,0x00,0x00,  // X
		0x00,0x00,0x00,0x00,0xC0,0x0C,0xC0,0x0C,0x30,0x30,0x30,0x30,0x0C,0xC0,0x0C,0xC0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // Y
		0x00,0x00,0x00,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x30,0x00,0x30,0x00,0xFF,0xF0,0xFF,0xF0,0x00,0x00,0x00,0x00,  // Z
		0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0F,0xC0,0x0F,0xC0,0x00,0x00,0x00,0x00,  // [
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x0C,0x00,0x0C,0x00,0x03,0x00,0x03,0x00,0x00,0xC0,0x00,0xC0,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,  // <backslash>
		0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x0F,0xC0,0x0F,0xC0,0x00,0x00,0x00,0x00,  // ]
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x0F,0xC0,0x0F,0xC0,0x33,0x30,0x33,0x30,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // ^
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFC,0xFF,0xFC,  // _

		0x0F,0xF0,0x0F,0xF0,0x30,0x0C,0x30,0x0C,0xC3,0xC3,0xC3,0xC3,0xCC,0x03,0xCC,0x03,0xCC,0x03,0xCC,0x03,0xC3,0xC3,0xC3,0xC3,0x30,0x0C,0x30,0x0C,0x0F,0xF0,0x0F,0xF0,  // `
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x00,0x30,0x00,0x30,0x0F,0xF0,0x0F,0xF0,0x30,0x30,0x30,0x30,0x0F,0xF0,0x0F,0xF0,0x00,0x00,0x00,0x00,  // a
		0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x3F,0xC0,0x3F,0xC0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // b
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x03,0xF0,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x03,0xF0,0x03,0xF0,0x00,0x00,0x00,0x00,  // c
		0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x0F,0xF0,0x0F,0xF0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0F,0xF0,0x0F,0xF0,0x00,0x00,0x00,0x00,  // d
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x30,0x30,0x30,0x30,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x30,0x00,0x0F,0xF0,0x0F,0xF0,0x00,0x00,0x00,0x00,  // e
		0x00,0x00,0x00,0x00,0x00,0xF0,0x00,0xF0,0x03,0x00,0x03,0x00,0x03,0xC0,0x03,0xC0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // f
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFC,0x0F,0xFC,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x30,0x0C,0x0F,0xFC,0x0F,0xFC,0x00,0x0C,0x00,0x0C,0x0F,0xF0,0x0F,0xF0,  // g
		0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x3F,0xC0,0x3F,0xC0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,  // h
		0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,0x03,0xC0,0x03,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x03,0xF0,0x03,0xF0,0x00,0x00,0x00,0x00,  // i
		0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x0C,0x30,0x0C,0x30,0x03,0xC0,0x03,0xC0,  // j
		0x00,0x00,0x00,0x00,0x18,0x00,0x18,0x00,0x19,0x80,0x19,0x80,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x1E,0x00,0x19,0x80,0x19,0x80,0x18,0x60,0x18,0x60,0x00,0x00,0x00,0x00,  // k
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,  // l
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0xC0,0x3C,0xC0,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x00,0x00,0x00,0x00,  // m
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,  // n
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0F,0xC0,0x0F,0xC0,0x00,0x00,0x00,0x00,  // o

		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,  // p
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0F,0xF0,0x0F,0xF0,0x00,0x30,0x00,0x30,0x00,0x3C,0x00,0x3C,  // q
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x03,0xF0,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,  // r
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xC0,0x0F,0xC0,0x30,0x00,0x30,0x00,0x0F,0xC0,0x0F,0xC0,0x00,0x30,0x00,0x30,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,  // s
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x0F,0xC0,0x0F,0xC0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,  // t
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0F,0xC0,0x0F,0xC0,0x00,0x00,0x00,0x00,  // u
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0C,0xC0,0x0C,0xC0,0x0C,0xC0,0x0C,0xC0,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // v
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x33,0x30,0x0C,0xC0,0x0C,0xC0,0x00,0x00,0x00,0x00,  // w
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x0C,0xC0,0x0C,0xC0,0x03,0x00,0x03,0x00,0x0C,0xC0,0x0C,0xC0,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,  // x
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x0F,0xF0,0x0F,0xF0,0x00,0x30,0x00,0x30,0x0F,0xC0,0x0F,0xC0,  // y
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0xC0,0x00,0xC0,0x03,0x00,0x03,0x00,0x0C,0x00,0x0C,0x00,0x3F,0xF0,0x3F,0xF0,0x00,0x00,0x00,0x00,  // z
		0x00,0x00,0x00,0x00,0x03,0xF0,0x03,0xF0,0x03,0x00,0x03,0x00,0x3C,0x00,0x3C,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0xF0,0x03,0xF0,0x00,0x00,0x00,0x00,  // {
		0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,  // |
		0x00,0x00,0x00,0x00,0x3F,0x00,0x3F,0x00,0x03,0x00,0x03,0x00,0x00,0xF0,0x00,0xF0,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x3F,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,  // }
		0x00,0x00,0x00,0x00,0x03,0x30,0x03,0x30,0x0C,0xC0,0x0C,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00   // ~
	}
};
