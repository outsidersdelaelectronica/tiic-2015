/*
 * touch_module.c
 *
 * Resistive touch surface controller
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#include <msp430.h>
#include <stdint.h>

volatile uint8_t txBuffer[3];						//SPI TX buffer
volatile unsigned int txBufferIdx = 0;				//SPI TX buffer index

void touch_setup()
{
	//Configure touchpad control lines
		//IRQ touch event
		P1DIR &= ~BIT3;								//Set P1.3 to input
														//P1.3 = IRQ touch event (interrupt)
		P1IES |= BIT3;								//Interrupt P1.3 triggered with a high-to-low transition
		P1IFG |= BIT3;								//Clear P1.3 interrupt flag

		//BUSY signal
		P3DIR &= ~BIT0;								//Set P3.0 to input
														//P3.0 = Touchpad controller busy signal

	//Configure USCI_A1 for SPI operation
		P3SEL0 |= BIT6;								//Set P3.6 as UCA1CLK
		P3SEL1 &= ~BIT6;							// |
		P3SEL0 |= BIT4;								//Set P3.4 as UCA1SIMO
		P3SEL1 &= ~BIT4;							// |
		P3SEL0 |= BIT5;								//Set P3.5 as UCA1SOMI
		P3SEL1 &= ~BIT5;							// |
		P3DIR |= BIT7;								//Set P3.7 as output (CS)
		P3SEL0 &= ~BIT7;							// |
		P3SEL1 &= ~BIT7;							// |
		P3OUT |= BIT7;								//Hold CS high by default

		UCA1CTLW0 = UCSWRST;						//Hold USCI_A1 in reset state - necessary while configuring registers

		UCA1CTLW0 |= UCCKPH | UCMSB | UCMST | UCMODE_2 | UCSYNC | UCSSEL_2 | UCSTEM;
													//Clock phase = 1
													//Clock polarity = 0
													//MSB first
													//8-bit data
													//Master mode
													//SPI mode / 4-pin SPI with CS active low
													//Synchronous mode
													//Clock source = SMCLK
													//STE = Chip select for a 4-wire slave

		UCA1BRW = 0x0002;							//Clock prescaler = /2
													//SMCLK = 2 MHz => BRCLK = 1 MHz

		UCA1CTLW0 &= ~UCSWRST;						//Get USCI_A1 out of reset state
}


/*
 * SPI A1 ISR
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCA1IV, USCI_SPI_UCTXIFG))
	{
		case USCI_NONE:
			break;
		case USCI_SPI_UCRXIFG:
			break;
		case USCI_SPI_UCTXIFG:
			if (txBufferIdx > 0){					//If there are bytes left to be sent
				P3OUT &= ~BIT7;							//Enable CS
				txBufferIdx--;								//Decrease buffer index
				UCA1TXBUF = txBuffer[txBufferIdx];			//Transmit last byte in txBuffer
																//TX flag is automatically cleared when writing to UCB1TXBUF
				P3OUT |= BIT7;							//Disable CS
				UCA1IE &= ~UCTXIE;						//Disable TX interrupts
				__bic_SR_register_on_exit(LPM0_bits); 	//Wake up
			}
			break;
		default:
			break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(P1IV, 16))
	{
		case 6:	//P1.2								//If AFE has data ready for transmission
			if (UCB1IFG & UCTXIFG)					//If SPI TX buffer is empty
			{
				P4OUT &= ~BIT4;							//Enable CS
				UCB1TXBUF = txBuffer[txBufferIdx];			//Transmit dummy byte
				P4OUT |= BIT4;							//Disable CS

				if(!circularBuffer_isFull(&ecgSignal))	//If circular buffer is not full
				{
					circularBuffer_write(&ecgSignal, UCB1RXBUF);	//Write received value to circular buffer
				}

				P1IFG &= ~BIT2;                           // Clear P1.2 flag
			}
			break;
		case 8:	//P1.3								//If a touch event is detected
			if (UCA1IFG & UCTXIFG)					//If SPI TX buffer is empty
			{
				P4OUT &= ~BIT4;							//Enable CS
				UCB1TXBUF = txBuffer[txBufferIdx];			//Transmit dummy byte
				P4OUT |= BIT4;							//Disable CS

				if(!circularBuffer_isFull(&ecgSignal))	//If circular buffer is not full
				{
					circularBuffer_write(&ecgSignal, UCB1RXBUF);	//Write received value to circular buffer
				}

				P1IFG &= ~BIT2;                           // Clear P1.2 flag
			}
			break;
		default:
			break;
	}
}
