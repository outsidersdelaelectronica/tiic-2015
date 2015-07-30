#ifndef DISPLAY_H
#define DISPLAY_H

#include <msp430.h>
#include <stdint.h>

void	LCD_setup();
void	LCD_initialize();

void	LCD_write_data(uint8_t dataH, uint8_t dataL);
int		LCD_read_data();
void LCD_EnterSleep_ILI9325();
void LCD_EnterSleep_ILI9325();

#endif
