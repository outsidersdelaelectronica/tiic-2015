/*
 * touch_serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "touch_serial.h"

uint8_t touch_serial_send(uint8_t data)
{
	UCB1RXBUF;								//Dummy read to empty RX buffer
											//and clear any overrun conditions
	while (!(UCB1IFG & UCTXIFG));           //Wait while not ready for TX
	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY);					//Wait for transmission

    return UCA1RXBUF;				//Return received data if any
}

uint8_t touch_send(uint8_t data)
{
	uint8_t rxValue = 0x00;

	P3OUT &= ~BIT7;							//Enable CS
	
	// rxValue = touch_serial_send(data);
	UCB1RXBUF;								//Dummy read to empty RX buffer
											//and clear any overrun conditions
	while (!(UCB1IFG & UCTXIFG));           //Wait while not ready for TX
	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY);					//Wait for transmission

    rxValue = UCA1RXBUF;				//Return received data if any
	
	P3OUT |= BIT7;							//Disable CS

	return rxValue;
}
