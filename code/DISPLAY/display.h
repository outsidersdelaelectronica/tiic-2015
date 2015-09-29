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

typedef struct display_t
{
	//Display interface
		display_interface_t display_interface;
}
display_t;

void	display_setup(display_t* display);
void	display_init(display_t* display);

#endif /* DISPLAY_DISPLAY_H_ */
