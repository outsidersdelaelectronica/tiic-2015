/*
 * afe_serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "afe_serial.h"

uint8_t afe_serial_send(uint8_t data)
{
	__delay_cycles(120);                    //Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk

	UCB1RXBUF;								//Dummy read to empty RX buffer
											//and clear any overrun conditions

	while (!(UCB1IFG & UCTXIFG));           //Wait while not ready for TX
	UCB1TXBUF = data;						//Transmit byte (automatically clears UCTXIFG)

	while(UCB1STATW & UCBUSY);

    return UCB1RXBUF;				//Return received data if any
}


void afe_serial_write_register(uint8_t address, uint8_t value)
{
	__delay_cycles(96);                     //Wait 6us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk
											//needed to ensure previous byte was not sent within this time period

	while (!(UCB1IFG & UCTXIFG)) ;          //Wait while not ready for TX
	UCB1TXBUF = address | WREG;
	__delay_cycles(120);                    //Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk

	while (!(UCB1IFG & UCTXIFG));           //Wait while not ready for TX
	UCB1TXBUF = 0x00;						// Default size = 1
	__delay_cycles(120);                   	//Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk

	while (!(UCB1IFG & UCTXIFG));          	//Wait while not ready for TX
	UCB1TXBUF = value;

	while ((UCB1STAT & UCBUSY));            //Wait for all TX/RX to finish

	UCB1RXBUF;                            	//Dummy read to empty RX buffer
											//and clear any overrun conditions
}

// NOT USED
uint8_t afe_serial_read_register(uint8_t address)
{
	__delay_cycles(96);                 //Wait 6us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk
										//needed to ensure previous byte was not sent within this time period
										//this value was determined experimentally
	//Clock the actual data transfer and send the bytes. Note that we
	//intentionally do not read out the receive buffer during frame transmission
	//in order to optimize transfer speed, however we need to take care of the
	//resulting overrun condition.
	while (!(UCB1IFG & UCTXIFG)) ;      //Wait while not ready for TX
	UCB1TXBUF = address | RREG;
	__delay_cycles(120);               	//Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk
										//this value was determined experimentally
	while (!(UCB1IFG & UCTXIFG));       //Wait while not ready for TX
	UCB1TXBUF = 0x00;					// Default size = 1
	while ((UCB1STAT & UCBUSY));        //Wait for all TX/RX to finish

	UCB1RXBUF;                          //Dummy read to empty RX buffer
											//and clear any overrun conditions
	UCB1IFG &= ~UCRXIFG;                //Ensure RXIFG is clear

	// Clock the actual data transfer and receive the bytes

	__delay_cycles(120);                //Wait 7.5us (assuming 24MHz MCLK), required to allow t_sdecode to be 4 T_clk

	while (!(UCB1IFG & UCTXIFG));   	//Wait while not ready for TX
	UCB1TXBUF = 0xff;					//Write dummy byte
	while (!(UCB1IFG & UCRXIFG));       //Wait for RX buffer (full)

	return UCB1RXBUF;
}
