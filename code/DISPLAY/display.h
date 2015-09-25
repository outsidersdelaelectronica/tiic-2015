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

#include "display_functions.h"
#include "display_interface.h"
#include "display_IO.h"

<<<<<<< HEAD
#include "../utils.h"
#include "../CircularBuffer/circularBuffer.h"
#include "../CircularBuffer/ecgData.h"

#define SWEEP_TIME_DEFAULT	2000 // Miliseconds

#define SWEEP_TIME_10000	10000
#define SWEEP_TIME_5000		5000
#define	SWEEP_TIME_2000		2000
#define SWEEP_TIME_1000		1000
#define SWEEP_TIME_500		500
#define	SWEEP_TIME_200		200


=======
>>>>>>> parent of 13d9eb2... Buzzer with timer
void	display_setup();
void	display_initialize();

#endif
