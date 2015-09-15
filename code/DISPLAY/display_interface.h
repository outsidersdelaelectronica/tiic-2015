/*
 * display_interface.h
 *
 *  Created on: 14/9/2015
 *      Author: Smau
 */

#ifndef DISPLAY_DISPLAY_INTERFACE_H_
#define DISPLAY_DISPLAY_INTERFACE_H_

#include <msp430.h>
#include <stdint.h>

#include "../CircularBuffer/ecgData.h"
#include "display_functions.h"
#include "display_IO.h"

#define SIGNAL_OFFSET	0x57

#define DISPLAY_LINES	240
#define DISPLAY_COLUMNS	320

#define SIGNAL_LINES	176
#define SIGNAL_COLUMNS	320

#define CLEAR_DISTANCE  25

typedef struct display_interface_t
{
	uint16_t currentIndex;

	uint16_t signal_line_origin;
	uint16_t signal_line_end;

	uint16_t menubar_line_origin;
	uint16_t menubar_line_end;

	uint8_t signal_background_color[3];
	uint8_t menubar_background_color[3];
}
display_interface_t;

void 	display_interface_setup(display_interface_t* interface);
void	display_write_signal(display_interface_t* interface, ecgData_t* signalDataPoint);

#endif /* DISPLAY_DISPLAY_INTERFACE_H_ */
