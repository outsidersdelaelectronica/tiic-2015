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

#include "display_utils.h"
#include "../utils.h"

void	LCD_initialize();
void	LCD_sleep();
void	LCD_wakeUp();

#endif
