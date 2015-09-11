/*
 * touch_serial.c
 *
 *  Created on: 11/9/2015
 *      Author: slopez
 */

#include "touch_serial.h"

volatile uint8_t touch_RX_data = 0x00;

uint8_t touch_send(uint8_t data)
{
	UCA1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCA1STATW & UCBUSY){					//Wait for transmission
    }

    return UCA1RXBUF;				//Return received data if any
}

