/*
 * utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "utils.h"

void itoa(uint16_t number, char* string)
{
	uint16_t units, tens, hundreds, thousands;

	units = number % 10;
	tens = number % 100 - units;
	hundreds = number % 1000 - units - tens;
	thousands = number % 10000 - units - tens - hundreds;

	string[0] = thousands/1000 + '0';
	string[1] = hundreds/100 + '0';
	string[2] = tens/10 + '0';
	string[3] = units + '0';
	string[4] = 0x00;
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

int32_t multiplication(int32_t op1, int32_t op2){

	int32_t big_op, small_op, result = 0;

	if (op1 > op2){
		big_op = op1;
		small_op = op2;
	}else{
		big_op = op2;
		small_op = op1;
	}

	for(;small_op > 0; small_op--)
	{
		result += big_op;
	}

	return result;
}
int32_t division(int32_t dividend, int32_t divisor){

	int32_t result = 0;

	for(;dividend >= divisor;result++)
	{
		dividend -= divisor;
	}
	return result;
}
