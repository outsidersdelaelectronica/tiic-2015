/*
 * display_functions.h
 *
 *  Created on: 14/9/2015
 *      Author: Smau
 */

#ifndef DISPLAY_DISPLAY_FUNCTIONS_H_
#define DISPLAY_DISPLAY_FUNCTIONS_H_

#include <msp430.h>
#include <stdint.h>

#include "display_IO.h"

#include "font.h"
#include "../utils.h"

void	display_sleep();
void	display_wake_up();

void 	display_write_pixel(uint8_t red, uint8_t green, uint8_t blue, uint16_t x, uint16_t y);

void	display_write_char(char character, uint8_t red, uint8_t green, uint8_t blue, uint16_t posH, uint16_t posV);
void	display_write_string(char* string, uint8_t red, uint8_t green, uint8_t blue, uint16_t posH, uint16_t posV);

#endif /* DISPLAY_DISPLAY_FUNCTIONS_H_ */
