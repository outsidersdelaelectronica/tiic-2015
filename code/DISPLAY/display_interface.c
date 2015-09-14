/*
 * display_interface.c
 *
 *  Created on: 14/9/2015
 *      Author: Smau
 */

#include "display_interface.h"

uint8_t signal_background_color[3]  = {0x00, 0x00, 0x10};
uint8_t menubar_background_color[3] = {0x20, 0x20, 0x60};

void display_interface_setup()
{
	display_interface_set_window(signal_background_color, 0, 176);				//Set signal window
	display_interface_set_window(menubar_background_color, 176, 240);			//Set menubar window

	uint16_t i;
	for (i = 0; i < 320; i++)													//White line
	{
		display_write_pixel(0xFF, 0xFF, 0xFF, i, 176);
	}

}

void display_interface_set_window(uint8_t* color, uint16_t line_origin, uint16_t line_end)
{
	//Set origin line
		display_IO_set_GRAM_address(0, line_origin);

	long i;
	long last_pixel = 320L * (long)(line_end - line_origin);

	for (i = 0; i < last_pixel; i++)
	{
		display_IO_write_GRAM(color[0], color[1], color[2]);
	}

}

void display_write_signal(ecgData* signalDataPoint)
{

}
