/*
 * afe_serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "afe_serial.h"

uint8_t afe_serial_send(uint8_t data)
{

	while  (!(UCB1IFG & UCTXIFG));
	UCB1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCB1STATW & UCBUSY);					//Wait for transmission
	while  (!(UCB1IFG & UCRXIFG));
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

//uint8_t afe_send(uint8_t data)
//{
//	uint8_t rxValue = 0x00;
//
//	P4OUT &= ~BIT4;							//Enable CS
//	rxValue = afe_serial_send(data);
//	P4OUT |= BIT4;							//Disable CS
//
//	return rxValue;
//}
//
//void afe_write_register(uint8_t address, uint8_t value)
//{
//	P4OUT &= ~BIT4;							//Enable CS
//	afe_serial_send(WREG | address);
//	afe_serial_send(0x00);
//	afe_serial_send(value);
//	P4OUT |= BIT4;							//Disable CS
//}
//
//
//// NOT USED
//uint8_t afe_read_register(uint8_t address)
//{
//	uint8_t value = 0x00;
//
//	P4OUT &= ~BIT4;							//Enable CS
//	afe_serial_send(RREG | address);
//	afe_serial_send(0x00);
//	value = afe_serial_send(0x00);			//Read value from RX buffer
//	P4OUT |= BIT4;							//Disable CS
//
//	return value;
//}
//
