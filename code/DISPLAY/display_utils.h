/*
 * display_utils.h
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#ifndef DISPLAY_DISPLAY_UTILS_H_
#define DISPLAY_DISPLAY_UTILS_H_

#include <msp430.h>
#include <stdint.h>
#include "../utils.h"

void		LCD_IO_write_reg(uint8_t reg, uint8_t data_hi_af, uint8_t data_lo);
uint16_t	LCD_IO_read_reg(uint8_t reg);

#endif /* DISPLAY_DISPLAY_UTILS_H_ */
