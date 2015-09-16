/*
 * display_utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */

#include "display_IO.h"

void display_IO_input()
{
	P6DIR |= 0x7F;					//Clear P6.0 to P6.6
	P6DIR &= ~0x7F;					//Set P6.0 to P6.6 as input
	P7DIR &= ~BIT0;					//Set P7.0 as input
}

void display_IO_output()
{
	P6DIR &= ~0x7F;					//Clear P6.0 to P6.6
	P6DIR |= 0x7F;					//Set P6.0 to P6.6 as output
	P7DIR |= BIT0;					//Set P7.0 as output
}

static void display_IO_write(uint8_t data_out)
{
	//display_IO_output();			//Set GPIO as output

	P6OUT &= ~0x7F;					//Clear P6.0 to P6.6
	P6OUT |= 0x7F & (data_out >> 1);	//Write 7 MSB of data to P6.0 to P6.6

	P7OUT &= ~0x01;					//Clear P7.0
	P7OUT |= 0x01 & data_out;			//Write LSB of data to P7.0
}

static uint8_t display_IO_read()
{
	uint8_t data_in = 0x00;			//Returned value
	//display_IO_input();				//Set GPIO as input

	data_in |= (P6IN & 0x7F) << 1;	//Read from P6.0 to P6.6
	data_in &= ~0x01;				//Clear LSB in data_in
	data_in |= (P7IN & 0x01);		//Read LSB from P7.0

	return data_in;
}

void display_IO_write_reg(uint8_t reg, uint8_t data_hi_af, uint8_t data_lo)
{
	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_IO_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write register to be accessed
		display_IO_write(reg);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write data
		P2OUT |= BIT3;			//Set display_RS to '1'

		display_IO_write(data_hi_af);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_IO_write(data_lo);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'
}

uint16_t display_IO_read_reg(uint8_t reg)
{
	uint8_t data_hi_af = 0;
	uint8_t data_lo = 0;
	uint16_t data = 0;

	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_IO_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write register to be accessed
		display_IO_write(reg);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Read data
		P2OUT |= BIT3;			//Set display_RS to '1'

		P9OUT &= ~BIT5;			//Set display_RD to '0'
		data_hi_af = display_IO_read();
		P9OUT |= BIT5;			//Set display_RD to '1'

		P9OUT &= ~BIT5;			//Set display_RD to '0'
		data_lo = display_IO_read();
		P9OUT |= BIT5;			//Set display_RD to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'

	data = ((uint16_t) data_hi_af << 8) | data_lo;
	return data;
}

void display_IO_write_GRAM(uint8_t red, uint8_t green, uint8_t blue)
{
	//Start operation
		P9OUT &= ~BIT7;			//Set display_CS to '0'

	//Write command
		P2OUT &= ~BIT3;			//Set display_RS to '0'

		display_IO_write(0x00);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Request GRAM writing operation
		display_IO_write(0x22);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//Write data
		P2OUT |= BIT3;			//Set display_RS to '1'

		display_IO_write(red);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_IO_write(green);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

		display_IO_write(blue);
		P9OUT &= ~BIT4;			//Set display_WR to '0'
		P9OUT |= BIT4;			//Set display_WR to '1'

	//End operation
		P9OUT |= BIT7;			//Set display_CS to '1'
}

void display_IO_set_GRAM_address(uint16_t x, uint16_t y)
{
	uint8_t x_low, x_high, y_low, y_high;

	x_low = (uint8_t) x;
	x_high = (uint8_t) (x>>8);
	y_low = (uint8_t) y;
	y_high = (uint8_t) (y>>8);

	display_IO_write_reg(0x21, x_high, x_low);		//X position register
	display_IO_write_reg(0x20, y_high, y_low);		//Y position register
}
