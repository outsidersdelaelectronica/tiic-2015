/*
 * lcd.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#include "color.h"
#include "lcd_constants.h"

#include "fsmc.h"

#define LCD_REG        ((uint32_t *)(FSMC_BASE))
#define LCD_DATA       ((uint32_t *)(FSMC_BASE + 0x00020000U))  //See p.620 of STM32L162VD ref. manual

#define LCD_X_SIZE     (uint16_t) 800
#define LCD_Y_SIZE     (uint16_t) 480

void lcd_init();

void lcd_write_command(uint16_t command);
void lcd_write_data(uint16_t data);

void lcd_read_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);
void lcd_write_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);

void lcd_draw_pixel(uint16_t x_pos, uint16_t y_pos, color_t *color);
void lcd_draw_char(char character,
                   const uint8_t *current_font, 
                   color_t *char_color,
                   uint16_t *x_pos, uint16_t *y_pos);

void lcd_clean_screen(color_t *color);
void lcd_set_brightness(uint8_t level);

#endif /* LCD_LCD_H_ */
