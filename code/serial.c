/*
 * serial.c
 *
 *  Created on: 13/9/2015
 *      Author: Smau
 */

#include "serial.h"

//Global ecg signal storage buffer (located at main.c)
extern volatile circularBuffer ecgSignalBuffer;
//Global touchscreen position variables (located at main.c)
extern volatile uint8_t touch_xPos, touch_yPos;

volatile ecgData AFE_ecgData;

/*
 * Port 1 (AFE and Touch) interrupt service routine
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
	if (P1IFG & BIT2)
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
		circularBuffer_write(&ecgSignalBuffer, &AFE_ecgData);

		P1IFG &= ~BIT2;                       	// Clear DRDY (P1.2) flag
	}
	if (P1IFG & BIT3)
	{
		P3OUT &= ~BIT7;							//Enable CS

		//Request touchscreen x position
			touch_serial_send(TOUCH_X_POS);
			touch_xPos = touch_serial_send(0x00);


		//Request touchscreen y position
			touch_serial_send(TOUCH_Y_POS);
			touch_yPos = touch_serial_send(0x00);

			P3OUT |= BIT7;							//Disable CS

		//Beep
			buzzer_start(A5);
			delay_ms(50);
			buzzer_stop();

		P1IFG &= ~BIT3;                         // Clear IRQ (P1.3) flag
	}
}
