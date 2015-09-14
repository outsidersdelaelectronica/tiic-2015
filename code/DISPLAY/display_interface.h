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

#include "display_IO.h"
#include "display_functions.h"

#include "../CircularBuffer/ecgData.h"

void 	display_interface_setup();
void	display_interface_set_window(uint8_t* color, uint16_t line_origin, uint16_t line_end);

void	display_write_signal(ecgData* signalDataPoint);

#endif /* DISPLAY_DISPLAY_INTERFACE_H_ */
