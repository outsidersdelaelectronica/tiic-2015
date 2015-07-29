/*
 * ADS1291_module.c
 *
 * ADS1291 Medical AFE functions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#include "ADS1291_module.h"
#include "ADS1291_constants.h"
#include "../circularBuffer.h"
#include <msp430.h>
#include <stdint.h>

extern volatile uint8_t txBufferAFE[3];						//SPI TX buffer
extern volatile unsigned int txBufferAFEIdx;				//SPI TX buffer index

extern volatile circularBuffer ecgSignal;

void ADS1291_setup()
{
	//Configure AFE control lines
		//AFE data ready
		P1DIR &= ~BIT2;								//Set P1.2 to input
														//P1.2 = AFE data ready (interrupt)
		P1IES |= BIT2;								//Interrupt P1.2 triggered with a high-to-low transition
		P1IFG |= BIT2;								//Clear P1.2 interrupt flag

		//Reset, CLKSEL and Start
		P5DIR |= BIT7;								//Set P5.7 to output
														//P5.7 = AFE conversion start
		P7DIR |= BIT3 | BIT4;						//Set P7.3 and P7.4 to outputs
														//P7.3 = AFE reset
														//P7.4 = AFE clock select

	//Hardware AFE reset, clock selection and conversion start
		P5OUT &= ~BIT7;								//Hold start signal low
		P7OUT &= ~BIT3 & ~BIT4;						//Hold AFE in reset
													//and select external clock

	//Configure USCI_B1 for SPI operation
		P4SEL0 &= ~BIT5;							//Set P4.5 as UCB1CLK
		P4SEL1 |= BIT5;								// |
		P4SEL0 &= ~BIT6;							//Set P4.6 as UCB1SIMO
		P4SEL1 |= BIT6;								// |
		P4SEL0 &= ~BIT7;							//Set P4.7 as UCB1SOMI
		P4SEL1 |= BIT7;								// |
		P4DIR |= BIT4;								//Set P4.4 as output (CS)
		P4SEL0 &= ~BIT4;							// |
		P4SEL1 &= ~BIT4;							// |
		P4OUT |= BIT4;								//Hold CS high by default

		UCB1CTLW0 = UCSWRST;						//Hold USCI_B1 in reset state - necessary while configuring registers

		UCB1CTLW0 |= UCCKPH | UCMSB | UCMST | UCMODE_2 | UCSYNC | UCSSEL_2 | UCSTEM;
													//Clock phase = 1
													//Clock polarity = 0
													//MSB first
													//8-bit data
													//Master mode
													//SPI mode / 4-pin SPI with CS active low
													//Synchronous mode
													//Clock source = SMCLK
													//STE = Chip select for a 4-wire slave

		UCB1BRW = 0x0002;							//Clock prescaler = /2
													//SMCLK = 2 MHz => BRCLK = 1 MHz

		UCB1CTLW0 &= ~UCSWRST;						//Get USCI_B1 out of reset state
}


void ADS1291_initialize()
{
	//AFE reset and stop continuous data conversion mode
		P7OUT |= BIT3;								//Power-On-Reset: hold reset line high for 1 second
		__delay_cycles(8000000);						//1 second

		P7OUT &= ~BIT3;								//Reset pulse: (>= 18 t_clk) => (>= 10 us)
		__delay_cycles(1000);							//At least 10 useconds
		P7OUT |= BIT3;

		ADS1291_command(SDATAC);					//Stop continuous data conversion mode (activated by default)


	//Write config commands to AFE
		ADS1291_write_register(REG_CONFIG2, 0xE0);	//Enable voltage reference
													//Enable lead-off comparators
		ADS1291_write_register(REG_CH2SET, 0x81);	//Channel 2 power down
													//Channel 2 input shorted
		ADS1291_write_register(REG_LOFF_STAT, 0x40);//Clock divider selection: Clock input set to 2.048 MHz
		ADS1291_write_register(REG_RESP2, 0x87);	//Enable calibration
		ADS1291_write_register(REG_CH1SET, 0x01);	// |
		ADS1291_command(OFFSETCAL);					// | Calibrate
		ADS1291_write_register(REG_CH1SET, 0x00);	// |
		ADS1291_write_register(REG_RESP2, 0x07);	//Disable calibration


	//Start capturing data
		P5OUT |= BIT7;								//Start conversions
		ADS1291_command(RDATAC);					//Enable continuous output of conversion data
														//In this mode, a SDATAC command must be issued
														//before other commands can be sent to the device
}

void ADS1291_command(uint8_t command)
{
		txBufferAFE[txBufferAFEIdx] = command;			//Write command to be sent into TX buffer
		txBufferAFEIdx++;								//Increment buffer index

		UCB1IE |= UCTXIE;							//Enable TX interrupts
		__bis_SR_register(LPM0_bits | GIE);     	//Enter LPM0 mode, enabling global interrupts
	    __no_operation();                       	//Wait for TX interrupt
	    	//Data transmission on ISR
	    __delay_cycles(1000);						//Delay before next transmission
}

uint8_t	ADS1291_read_register(uint8_t address)
{
	txBufferAFE[txBufferAFEIdx] = 0;						//Dummy byte for RX
	txBufferAFEIdx++;
	txBufferAFE[txBufferAFEIdx] = 0;						//Build command: number of registers to be read - 1
	txBufferAFEIdx++;
	txBufferAFE[txBufferAFEIdx] = RREG | address;			//Build command: read content from 'address'
	txBufferAFEIdx++;

	int i;
	int numberOfBytes = txBufferAFEIdx;
	for (i = 0; i < numberOfBytes; i++)				//Send all the bytes
	{
		UCB1IE |= UCTXIE;								//Enable TX interrupts
		__bis_SR_register(LPM0_bits | GIE);     		//Enter LPM0 mode, enabling global interrupts
		__no_operation();                       		//Wait for TX interrupt
			//Data transmission on ISR
		__delay_cycles(1000);							//Delay before next transmission
	}

	uint8_t value = UCB1RXBUF;						//Read value from RX buffer
	return value;
}

void ADS1291_write_register(uint8_t address, uint8_t value)
{

	txBufferAFE[txBufferAFEIdx] = value;					//Build command: byte to be written
	txBufferAFEIdx++;
	txBufferAFE[txBufferAFEIdx] = 0;						//Build command: number of registers to be read - 1
	txBufferAFEIdx++;
	txBufferAFE[txBufferAFEIdx] = WREG | address;			//Build command: write content to 'address'
	txBufferAFEIdx++;

	int i;
	int numberOfBytes = txBufferAFEIdx;
	for (i = 0; i < numberOfBytes; i++)				//Send all the bytes
	{
		UCB1IE |= UCTXIE;								//Enable TX interrupts
		__bis_SR_register(LPM0_bits | GIE);     		//Enter LPM0 mode, enabling global interrupts
		__no_operation();                       		//Wait for TX interrupt
			//Data transmission on ISR
		__delay_cycles(1000);							//Delay before next transmission
	}
}

