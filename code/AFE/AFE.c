/*
 * AFE_module.c
 *
 * AFE Medical AFE functions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#include "AFE.h"

static void AFE_command(uint8_t command)
{
	P4OUT &= ~BIT4;							//Enable CS

	AFE_send(command);

	P4OUT |= BIT4;							//Disable CS
}

static void AFE_write_register(uint8_t address, uint8_t value)
{
	P4OUT &= ~BIT4;							//Enable CS

	AFE_send(WREG | address);
	AFE_send(0x00);
	AFE_send(value);

	P4OUT |= BIT4;							//Disable CS
}

/* NOT USED
static uint8_t AFE_read_register(uint8_t address)
{
	uint8_t value = 0x00;

	P4OUT &= ~BIT4;							//Enable CS

	AFE_send(RREG | address);
	AFE_send(0x00);
	value = AFE_send(0x00);					//Read value from RX buffer

	P4OUT |= BIT4;							//Disable CS

	return value;
}
*/

void AFE_setup()
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


void AFE_initialize()
{
	//AFE reset and stop continuous data conversion mode
		P7OUT |= BIT3;								//Power-On-Reset: hold reset line high for 1 second
		__delay_cycles(16000000);						//1 second

		P7OUT &= ~BIT3;								//Reset pulse: (>= 18 t_clk) => (>= 10 us)
		__delay_cycles(2000);							//At least 10 useconds
		P7OUT |= BIT3;

		AFE_command(SDATAC);						//Stop continuous data conversion mode (activated by default)


	//Write config commands to AFE
		AFE_write_register(REG_CONFIG2, 0xE0);		//Enable voltage reference
													//Enable lead-off comparators
		AFE_write_register(REG_CH2SET, 0x81);		//Channel 2 power down
													//Channel 2 input shorted
		AFE_write_register(REG_LOFF_STAT, 0x40);	//Clock divider selection: Clock input set to 2.048 MHz
		AFE_write_register(REG_RESP2, 0x87);		//Enable calibration
		AFE_write_register(REG_CH1SET, 0x01);		// |
		AFE_command(OFFSETCAL);						// | Calibrate
		AFE_write_register(REG_CH1SET, 0x00);		// |
		AFE_write_register(REG_RESP2, 0x07);		//Disable calibration


	//Start capturing data
		P5OUT |= BIT7;								//Start conversions
		AFE_command(RDATAC);						//Enable continuous output of conversion data
														//In this mode, a SDATAC command must be issued
														//before other commands can be sent to the device

		P1IE |= BIT2;								//Enable DRDY interrupt
}