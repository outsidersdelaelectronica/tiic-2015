/*
 * utils.h
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <msp430.h>
#include <stdint.h>

void 	itoa(uint16_t number, char* string);
void	delay_ms(int ms);

inline int32_t multiplication(int32_t op1, int32_t op2);
inline int32_t division(int32_t op1, int32_t op2);

#endif /* UTILS_H_ */
