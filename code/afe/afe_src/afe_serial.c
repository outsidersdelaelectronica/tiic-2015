/*
 * afe_serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "afe_serial.h"

uint8_t afe_serial_send(uint8_t data)
{
	UCB1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCB1STATW & UCBUSY);					//Wait for transmission

    return UCB1RXBUF;				//Return received data if any
}


void afe_serial_write_register(uint8_t address, uint8_t value)
{
	afe_serial_send(WREG | address);
	afe_serial_send(0x00);
	afe_serial_send(value);
}


// NOT USED
uint8_t afe_serial_read_register(uint8_t address)
{
	uint8_t value = 0x00;

	afe_serial_send(RREG | address);
	afe_serial_send(0x00);
	value = afe_serial_send(0x00);			//Read value from RX buffer

	return value;
}
