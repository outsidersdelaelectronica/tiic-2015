/*
 * display_utils.h
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_IO_H_
#define DISPLAY_DISPLAY_IO_H_

#include <msp430.h>
#include <stdint.h>

void 		display_IO_input();
void 		display_IO_output();

void		display_IO_write_reg(uint8_t reg, uint8_t data_hi_af, uint8_t data_lo);
uint16_t	display_IO_read_reg(uint8_t reg);

void		display_IO_write_GRAM(uint8_t red, uint8_t green, uint8_t blue);
void		display_IO_set_GRAM_address(uint16_t x, uint16_t y);

#endif /* DISPLAY_DISPLAY_IO_H_ */
