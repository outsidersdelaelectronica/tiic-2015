/*
 * AFE_serial.c
 *
 *  Created on: 9/9/2015
 *      Author: Smau
 */

#include "AFE_serial.h"

volatile uint8_t AFE_RX_data = 0x00;

volatile ecgData AFE_ecgData;

//Global ecg signal storage buffer (located at main.c)
extern volatile circularBuffer ecgSignalBuffer;

uint8_t AFE_send(uint8_t data)
{
	UCB1TXBUF = data;				//Transmit byte (automatically clears UCTXIFG)
    while(UCB1STATW & UCBUSY){					//Wait for transmission
    }

    AFE_RX_data = UCB1RXBUF;				//Store received data if any
    return AFE_RX_data;
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
	//Read 3 ADS1291 status bytes
	int i;
	for (i = 0; i < 3; i++) {
		AFE_send(0x00);
	}

	//Read ECG signal - another 3 bytes
	for (i = 0; i < 3; ++i) {
		AFE_ecgData.signal[i] = AFE_send(0x00);
	}

	//Store signal data into ecg signal buffer
	circularBuffer_write(&ecgSignalBuffer, AFE_ecgData);

	P1IFG &= ~BIT2;                           // Clear DRDY (P1.2) flag
}
