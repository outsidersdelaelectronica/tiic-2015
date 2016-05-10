/*
 * lcd.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "fsmc.h"

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

void    lcd_write_reg(uint8_t reg, uint8_t *param_buffer, uint8_t param_buffer_size);

#endif /* LCD_LCD_H_ */
