/*
 * utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "utils.h"

void itoa(uint8_t number, char* string)
{
	uint8_t units, tens, hundreds;

	units = number % 10;
	tens = ((number % 100) - units) / 10;
	hundreds = (number - (tens + units)) / 100;

	string[0] = hundreds + '0';
	string[1] = tens + '0';
	string[2] = units + '0';
	string[3] = 0x00;
}

void delay_ms(int ms)
{
	int i = 0;
	for (i = 0; i < ms; ++i) {
		__delay_cycles(16000);
	}

}
