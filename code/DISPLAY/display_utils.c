/*
 * display_utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "display_utils.h"

void LCD_setup()
{
	P7DIR |= BIT1;					//Set LCD_RESET (P7.1) as output
	P9DIR |= BIT7 | BIT5 | BIT4;	//Set LCD_CS (P9.7), LCD_WR (P9.4) and LCD_RD (P9.5) as outputs
	P2DIR |= BIT3;					//Set LCD_RS (P2.3) as output
	P9DIR |= BIT6;					//Set LCD_SLEEP (P9.6) as output

	P9OUT |= BIT7 | BIT5 | BIT4;	//Set LCD_CS, LCD_WR and LCD_RD
	P2OUT |= BIT3;					//Set LCD_RS
	P9OUT &= ~BIT6;					//Turn on screen

}

void LCD_IO_input()
{
	P6DIR |= 0x7F;					//Clear P6.0 to P6.6
	P6DIR &= ~0x7F;					//Set P6.0 to P6.6 as input
	P7DIR &= ~BIT0;					//Set P7.0 as input
}

void LCD_IO_output()
{
	P6DIR &= ~0x7F;					//Clear P6.0 to P6.6
	P6DIR |= 0x7F;					//Set P6.0 to P6.6 as output
	P7DIR |= BIT0;					//Set P7.0 as output
}

void LCD_IO_write(uint8_t data_out)
{
	LCD_IO_output();				//Set GPIO as output

	P6OUT &= ~0x7F;					//Clear P6.0 to P6.6
	P6OUT |= 0x7F & (data_out >> 1);	//Write 7 MSB of data to P6.0 to P6.6

	P7OUT &= ~0x01;					//Clear P7.0
	P7OUT |= 0x01 | data_out;			//Write LSB of data to P7.0
}

uint8_t LCD_IO_read()
{
	uint8_t data_in = 0x00;
	LCD_IO_input();

	data_in |= (P6IN & 0x7F) << 1;
	data_in &= ~0x01;
	data_in |= (P7IN & 0x01);

	return data_in;
}

void LCD_IO_write_reg(uint8_t reg, uint8_t data_hi_af, uint8_t data_lo)
{
	P9OUT &= ~BIT7;			//Set LCD_CS to '0'
	P2OUT &= ~BIT3;			//Set LCD_RS to '0'

	LCD_IO_write(0x00);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	LCD_IO_write(reg);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	P2OUT |= BIT3;			//Set LCD_RS to '1'

	LCD_IO_write(data_hi_af);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	LCD_IO_write(data_lo);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	P9OUT |= BIT7;			//Set LCD_CS to '1'
}

uint16_t LCD_IO_read_reg(uint8_t reg)
{
	uint8_t data_hi_af = 0;
	uint8_t data_lo = 0;
	uint16_t data = 0;

	P9OUT &= ~BIT7;			//Set LCD_CS to '0'
	P2OUT &= ~BIT3;			//Set LCD_RS to '0'

	LCD_IO_write(0x00);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	LCD_IO_write(reg);
	P9OUT &= ~BIT4;			//Set LCD_WR to '0'
	delay_ms(1);			//Wait
	P9OUT |= BIT4;			//Set LCD_WR to '1'

	P2OUT |= BIT3;			//Set LCD_RS to '1'

	P9OUT &= ~BIT5;			//Set LCD_RD to '0'
	delay_ms(1);			//Wait
	data_hi_af = LCD_IO_read();
	P9OUT |= BIT5;			//Set LCD_RD to '1'


	P9OUT &= ~BIT4;			//Set LCD_RD to '0'
	delay_ms(1);			//Wait
	data_lo = LCD_IO_read();
	P9OUT |= BIT4;			//Set LCD_RD to '1'

	P9OUT |= BIT7;			//Set LCD_CS to '1'

	data = ((uint16_t) data_hi_af << 8) | data_lo;
	return data;
}
