/*
 * display_interface.c
 *
 *  Created on: 14/9/2015
 *      Author: Smau
 */

#include "display_interface.h"

static uint16_t display_interface_get_index(display_interface_t* interface, uint16_t offset)
{
	uint16_t index;
	index = (interface->currentIndex + offset) % SIGNAL_COLUMNS;

	return index;
}

static void display_interface_set_index(display_interface_t* interface, uint16_t index)
{
	interface->currentIndex = interface->currentIndex + index;
	interface->currentIndex = interface->currentIndex % SIGNAL_COLUMNS;
}

static void display_interface_inc_index(display_interface_t* interface)
{
	interface->currentIndex++;
	interface->currentIndex = interface->currentIndex % SIGNAL_COLUMNS;
}

static void display_interface_set_window(uint8_t* color, uint16_t line_origin, uint16_t line_end)
{
	//Set origin line
		display_IO_set_GRAM_address(0, line_origin);

	long i;
	long last_pixel = DISPLAY_COLUMNS * (long)(line_end - line_origin);

	for (i = 0; i < last_pixel; i++)
	{
		display_IO_write_GRAM(color[0], color[1], color[2]);
	}

}

void display_interface_setup(display_interface_t* interface)
{
	//Set signal index
		display_interface_set_index(interface, 0);

	//Store window colors
		interface->signal_background_color[0] = 0x00;
		interface->signal_background_color[1] = 0x00;
		interface->signal_background_color[2] = 0x10;

		interface->menubar_background_color[0] = 0x10;
		interface->menubar_background_color[1] = 0x20;
		interface->menubar_background_color[2] = 0x40;

	//Write them to GRAM
		display_interface_set_window(interface->signal_background_color, 0, SIGNAL_LINES);					//Set signal window
		display_interface_set_window(interface->menubar_background_color, SIGNAL_LINES, DISPLAY_LINES);		//Set menubar window

}

void display_write_signal(display_interface_t* interface, ecgData_t* signalDataPoint)
{
	//Clear screen
		uint16_t currentIndex, clearIndex;
		int i;

		currentIndex = display_interface_get_index(interface, 0);
		clearIndex = display_interface_get_index(interface, CLEAR_DISTANCE);

		for (i = 0; i < SIGNAL_LINES; i++)
		{
			//Clear current column
				display_write_pixel(interface->signal_background_color[0],
									interface->signal_background_color[1],
									interface->signal_background_color[2], currentIndex, i);
			//Clear column CLEAR_DISTANCE pixels ahead signal
				display_write_pixel(interface->signal_background_color[0],
									interface->signal_background_color[1],
									interface->signal_background_color[2], clearIndex, i);
		}

	//Cook ecg data
		uint16_t signal_y_point = 0;
		signal_y_point = SIGNAL_OFFSET + (signalDataPoint->data >> 8);
//		signal_y_point = signalDataPoint->data >> 9;
	//Print data
		display_write_pixel(0xFF, 0xFF, 0xFF, currentIndex, signal_y_point);


	//Increment index
		display_interface_inc_index(interface);
}
