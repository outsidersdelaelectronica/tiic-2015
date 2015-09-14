/*
 * display.h
 *
 *  Created on: 1/8/2015
 *      Author: tomasvalno
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <msp430.h>
#include <stdint.h>

#include "display_IO.h"
#include "display_functions.h"
#include "display_interface.h"

#include "../utils.h"

void	display_setup();
void	display_initialize();

#endif
