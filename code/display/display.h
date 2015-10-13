/*
 * display.h
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#include <msp430.h>
#include <stdint.h>

#include "display_src/display_hal.h"
#include "display_src/display_interface.h"

#include "display_timer.h"

typedef const struct display_t
{
	//Display interface
		const display_interface_t display_interface;
}
display_t;

void	display_setup(const display_t* display);
void	display_init(const display_t* display);


void	display_write_pixel(const display_t* display, color_t color, uint16_t x, uint16_t y);
void	display_write_char(const display_t* display, char character, color_t color, color_t bg_color,
															   uint16_t x, uint16_t y);
void	display_write_string(const display_t* display, char* string, color_t color, color_t bg_color,
													 	 	   uint16_t x, uint16_t y);
void	display_write_signal(const display_t* display, ecg_data_t* signal_data, color_t color);


void	display_sleep(const display_t* display);
void	display_wake_up(const display_t* display);

void	display_screen_on(const display_t* display);
void	display_screen_off(const display_t* display);

#endif /* DISPLAY_DISPLAY_H_ */
