/*
 * serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "serial.h"

extern buzzer_t buzzer;
extern display_t display;
extern ecg_data_circular_buffer_t ecg_buffer;
extern touch_coordinate_t touch_last_position;

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
		static uint8_t afe_bytes[3];
		static ecg_data_t afe_data_point;

		//Read 3 ADS1291 status bytes
			P4OUT &= ~BIT4;							//Enable CS

			int i;
			for (i = 0; i < 3; i++)
			{
				afe_serial_send(0x00);
			}

		//Read ECG signal - another 3 bytes
			for (i = 0; i < 3; i++)
			{
				afe_bytes[i] = afe_serial_send(0x00);
			}


			P4OUT |= BIT4;							//Disable CS

		//Cast to ecg_data type
			ecg_data_write(&afe_data_point, afe_bytes[0], afe_bytes[1], afe_bytes[2]);

		//Store signal data into ecg signal buffer
			ecg_data_circular_buffer_write(&ecg_buffer, &afe_data_point);

		P1IFG &= ~BIT2;                       	// Clear DRDY (P1.2) flag

	}
	if (P1IFG & BIT3)
	{
		P3OUT &= ~BIT7;							//Enable CS

			static uint8_t touch_xPos_low, touch_xPos_high;
			static uint8_t touch_yPos_low, touch_yPos_high;

			static uint16_t touch_xPos, touch_yPos;

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
			//buzzer_play(&buzzer, E5, 50);

		//Paint
			__bic_SR_register(GIE);
			display_write_pixel(&display, COLOR_WHITE, touch_last_position.xPos, touch_last_position.yPos);
			__bis_SR_register_on_exit(GIE);

		P1IFG &= ~BIT3;                         //Clear IRQ (P1.3) flag
	}
}
