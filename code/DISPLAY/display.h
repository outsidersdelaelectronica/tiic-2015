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

#include "../utils.h"
#include "display_utils.h"
#include "font.h"

void	display_setup();
void	display_initialize();

void	display_sleep();
void	display_wake_up();

#endif
