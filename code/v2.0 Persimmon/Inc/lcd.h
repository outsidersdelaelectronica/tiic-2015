/*
 * lcd.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd_constants.h"
#include "color.h"

#include "fsmc.h"

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

void lcd_init();

void lcd_write_command(uint16_t command);
void lcd_write_data(uint16_t data);

void lcd_read_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);
void lcd_write_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);

void lcd_draw_pixel(uint16_t x_pos, uint16_t y_pos, color_t *color);

void lcd_clean_screen(color_t *color);

#endif /* LCD_LCD_H_ */
