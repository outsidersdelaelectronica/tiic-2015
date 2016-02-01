/*
 * utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "utils.h"

void itoa(long unsigned int value, char* result, int base)
{
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0';}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
	tmp_value = value;
	value /= base;
	*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
	tmp_char = *ptr;
	*ptr--= *ptr1;
	*ptr1++ = tmp_char;
	}

}

void delay_ms(int ms)
{
	int i = 0;
	for (i = ms; i > 0; i--) {
		__delay_cycles(16000);
	}

	//TODO
	//Future work: make delay with timerino
}
