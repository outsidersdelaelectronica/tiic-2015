#include <msp430.h>
#include "clocks.h"
#include "circularBuffer.h"
#include "Touch/touch_constants.h"
#include "Touch/touch_module.h"
#include "AFE/ADS1291_module.h"
#include "Display/display.h"

volatile circularBuffer ecgSignal;

volatile uint8_t txBufferAFE[3];						//AFE SPI TX buffer
volatile unsigned int txBufferAFEIdx = 0;				//AFE SPI TX buffer index

volatile uint8_t txBufferTouch[3];						//Touch SPI TX buffer
volatile unsigned int txBufferTouchIdx = 0;				//Touch SPI TX buffer index
volatile uint8_t posX, posY;							//Touch event position

/*
 * Touch - SPI A1 ISR
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
			if (txBufferTouchIdx > 0){					//If there are bytes left to be sent
				P3OUT &= ~BIT7;							//Enable CS
				txBufferTouchIdx--;								//Decrease buffer index
				UCA1TXBUF = txBufferTouch[txBufferTouchIdx];			//Transmit last byte in txBuffer
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

/*
 * AFE - SPI B1 ISR
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B1_VECTOR))) USCI_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
	{
		case USCI_NONE:
			break;
		case USCI_SPI_UCRXIFG:
			break;
		case USCI_SPI_UCTXIFG:
			if (txBufferAFEIdx > 0){					//If there are bytes left to be sent
				P4OUT &= ~BIT4;							//Enable CS
				txBufferAFEIdx--;								//Decrease buffer index
				UCB1TXBUF = txBufferAFE[txBufferAFEIdx];			//Transmit last byte in txBuffer
																//TX flag is automatically cleared when writing to UCB1TXBUF
				P4OUT |= BIT4;							//Disable CS
				UCB1IE &= ~UCTXIE;						//Disable TX interrupts
				__bic_SR_register_on_exit(LPM0_bits); 	//Wake up
			}
			break;
		default:
			break;
  }
}


/*
 * Port 1 ISR
 */
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
				UCB1TXBUF = 0x00;						//Transmit dummy byte
				while(UCB1IFG & UCRXIFG)				//Wait until data reception is completed
				{
				}

				if(!circularBuffer_isFull(&ecgSignal))	//If circular buffer is not full
				{
					circularBuffer_write(&ecgSignal, UCB1RXBUF);	//Write received value to circular buffer
				}

				P4OUT |= BIT4;							//Disable CS

				P1IFG &= ~BIT2;                         // Clear P1.2 flag
			}
			break;
		case 8:	//P1.3								//If a touch event is detected
			if (UCA1IFG & UCTXIFG)					//If SPI TX buffer is empty
			{
				P3OUT &= ~BIT7;							//Enable CS

				UCA1TXBUF = GETX;						//Transmit get X position command
				while (!(P3IN & BIT0))					//Wait until touch controller is not busy
				{
				}
				UCB1TXBUF = 0x00;						//Transmit dummy command
				while(UCA1IFG & UCRXIFG)				//Wait until data reception is completed
				{
				}
				posX = UCA1RXBUF;

				UCA1TXBUF = GETY;						//Transmit get X position command
				while (!(P3IN & BIT0))					//Wait until touch controller is not busy
				{
				}
				UCB1TXBUF = 0x00;						//Transmit dummy command
				while(UCA1IFG & UCRXIFG)				//Wait until data reception is completed
				{
				}
				posY = UCA1RXBUF;


				P3OUT |= BIT7;							//Disable CS

				P1IFG &= ~BIT3;                         // Clear P1.3 flag
			}
			break;
		default:
			break;
	}
}

/*
 * main.c
 */
int main(void) {
	//WDTCTL = WDTPW | WDTHOLD;						//Stop watchdog timer

	//Setups
		clocks_setup();								//Clock configuration
		circularBuffer_setup(&ecgSignal);			//ECG signal storing buffer setup
		ADS1291_setup();							//AFE ports and serial configuration
		touch_setup();								//Touchpad ports and serial configuration
		LCD_setup();								//Screen configuration

	PM5CTL0 &= ~LOCKLPM5;						//Disable the GPIO power-on default high-impedance mode

	P9OUT &= ~BIT6;					//Turn on screen

	//Initializations
		ADS1291_initialize();						//AFE power-up and initialization
		LCD_initialize();							//Screen initialization

	return 0;
}
