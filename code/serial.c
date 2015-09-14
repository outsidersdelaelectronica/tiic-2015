/*
 * serial.c
 *
 *  Created on: 13/9/2015
 *      Author: Smau
 */

#include "serial.h"

//Global ecg signal storage buffer (located at main.c)
extern volatile circularBuffer_t ecgSignalBuffer;
//Global touchscreen position variables (located at main.c)
extern volatile touch_coordinate_t touch_last_position;

volatile ecgData_t AFE_ecgData;

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

			uint8_t touch_xPos_low, touch_xPos_high;
			uint8_t touch_yPos_low, touch_yPos_high;

			uint16_t touch_xPos, touch_yPos;

		//Request touchscreen x position
			touch_serial_send(TOUCH_X_POS);
			touch_xPos_high = touch_serial_send(0x00);
			touch_xPos_low = touch_serial_send(0x00);

		//Request touchscreen y position
			touch_serial_send(TOUCH_Y_POS);
			touch_yPos_high = touch_serial_send(0x00);
			touch_yPos_low = touch_serial_send(0x00);

		P3OUT |= BIT7;							//Disable CS

		//Rebuild 12-bit positions
			touch_xPos = (((uint16_t) touch_xPos_high) << 5) | (((uint16_t) touch_xPos_low) >> 3);
			touch_yPos = (((uint16_t) touch_yPos_high) << 5) | (((uint16_t) touch_yPos_low) >> 3);

			touch_coordinate_set(&touch_last_position, touch_xPos, touch_yPos);

		//Beep
			buzzer_start(E5);
			delay_ms(50);
			buzzer_stop();

		//Debug
			char xPos[5];
			char yPos[5];

			itoa(touch_last_position.xPos, xPos);
			itoa(touch_last_position.yPos, yPos);

//			display_write_string("X = ", 0xFF, 0xFF, 0xFF, 0x60, 0xC0);
//			display_write_string("Y = ", 0xFF, 0xFF, 0xFF, 0x60, 0xD0);
//			display_write_string(xPos, 0xFF, 0xFF, 0xFF, 0xA0, 0xC0);
//			display_write_string(yPos, 0xFF, 0xFF, 0xFF, 0xA0, 0xD0);

			display_write_pixel(0xFF, 0xFF, 0xFF, touch_last_position.xPos, touch_last_position.yPos);

			display_write_string("Ya no te pasas", 0xFF, 0xFF, 0xFF, 0x30, 0xC0);
			display_write_string("por el parque", 0xFF, 0xFF, 0xFF, 0x30, 0xD0);
			delay_ms(500);
			display_write_string("              ", 0xFF, 0xFF, 0xFF, 0x30, 0xC0);
			display_write_string("              ", 0xFF, 0xFF, 0xFF, 0x30, 0xD0);


		P1IFG &= ~BIT3;                         // Clear IRQ (P1.3) flag
	}
}
