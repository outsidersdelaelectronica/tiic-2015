#ifndef DISPLAY_H
#define DISPLAY_H

#include <msp430.h>

void LCD_set_reg(char b1, char reg);
void LCD_write_data(char dataH, char dataL);
int LCD_read_data();

#endif
