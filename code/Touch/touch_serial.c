/*
 * touch_serial.c
 *
 *  Created on: 11/9/2015
 *      Author: slopez
 */

#include "touch_serial.h"

uint8_t touch_serial_send(uint8_t data)
{
	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY);					//Wait for transmission

    return UCA1RXBUF;				//Return received data if any
}

uint8_t touch_send(uint8_t data)
{
	uint8_t rxValue = 0x00;

	P3OUT &= ~BIT7;							//Enable CS

	rxValue = touch_serial_send(data);

	P3OUT |= BIT7;							//Disable CS

	return rxValue;
}
