/*
 * AFE_serial.c
 *
 *  Created on: 9/9/2015
 *      Author: slopez
 */

#include "AFE_serial.h"

uint8_t AFE_serial_send(uint8_t data)
{
	UCB1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCB1STATW & UCBUSY);					//Wait for transmission

    return UCB1RXBUF;				//Return received data if any
}

uint8_t AFE_send(uint8_t data)
{
	uint8_t rxValue = 0x00;

	P4OUT &= ~BIT4;							//Enable CS

	rxValue = AFE_serial_send(data);

	P4OUT |= BIT4;							//Disable CS

	return rxValue;
}

void AFE_write_register(uint8_t address, uint8_t value)
{
	P4OUT &= ~BIT4;							//Enable CS

	AFE_serial_send(WREG | address);
	AFE_serial_send(0x00);
	AFE_serial_send(value);

	P4OUT |= BIT4;							//Disable CS
}
// YOU AINT SAYING HOW TO LIVE MY LIFE
//* NOT USED
uint8_t AFE_read_register(uint8_t address)
{
	uint8_t value = 0x00;

	P4OUT &= ~BIT4;							//Enable CS

	AFE_serial_send(RREG | address);
	AFE_serial_send(0x00);
	value = AFE_serial_send(0x00);					//Read value from RX buffer

	P4OUT |= BIT4;							//Disable CS

	return value;
}
//*/
