/*
 * AFE_serial.c
 *
 *  Created on: 9/9/2015
 *      Author: Smau
 */

#include "AFE_serial.h"

volatile uint8_t AFE_TX_data = 0x00;
volatile uint8_t AFE_RX_data = 0x00;

volatile uint8_t AFE_ECG_data[3] = {0x00, 0x00, 0x00};

uint8_t AFE_send(uint8_t data)
{
	AFE_TX_data = data;						//Store data to be sent

	UCB1IE |= UCTXIE;							//Enable TX interrupts
	__bis_SR_register(LPM0_bits | GIE);     	//Enter LPM0 mode, enabling global interrupts

    while(UCB1STATW & UCBUSY){					//Wait for transmission
    }

    AFE_RX_data = UCB1RXBUF;				//Store received data if any
    return AFE_RX_data;
}


/*
 * AFE SPI TX interrupt service routine
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
	switch(__even_in_range(UCB1IV, USCI_SPI_UCTXIFG))
	{
		case USCI_NONE:
			break;

		//RX interrupt ==> New data incoming (buffer full)
		case USCI_SPI_UCRXIFG:
			break;

		//TX interrupt ==> Can send new data (buffer empty)
		case USCI_SPI_UCTXIFG:
			UCB1TXBUF = AFE_TX_data;				//Transmit byte (automatically clears UCTXIFG)
			UCB1IE &= ~UCTXIE;						//Disable TX interrupts
			__bic_SR_register_on_exit(LPM0_bits);	//Exit LPM0
			break;

		default:
			break;
	}
}


/*
 * AFE DRDY interrupt service routine
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
    __bis_SR_register(GIE);		//Enable global interrupts ==> Allows interrupt nesting
    							//							   Needed for AFE_send() TX routine

	//Read 3 ADS1291 status bytes
	int i;
	for (i = 0; i < 3; i++) {
		AFE_send(0x00);
	}

	//Read ECG signal - another 3 bytes
	for (i = 0; i < 3; ++i) {
		AFE_ECG_data[i] = AFE_send(0x00);
	}

	P1IFG &= ~BIT2;                           // Clear DRDY (P1.2) flag
}
