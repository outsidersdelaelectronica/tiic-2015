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
extern touch_t touch;

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
	uint8_t afe_bytes[3] = {0x00 , 0x00, 0x00};
//	static ecg_data_t afe_data_point;
	unsigned int i = 0;
	static uint8_t g_flag = 0;
	uint16_t gie = __get_SR_register() & GIE; //Store current GIE state

	__disable_interrupt();                    //Make this operation atomic

	if (P1IFG & BIT2)
	{
		//Read 3 ADS1291 status bytes
			P4OUT &= ~BIT4;							//Enable CS


			for (i = 3; i > 0; i--)
			{
				afe_serial_send(0x00);
			}

		//Read ECG signal - another 3 bytes
			for (i = 3; i > 0; i--)
			{
				afe_bytes[3 - i] = afe_serial_send(0x00);
			}

			P4OUT |= BIT4;							//Disable CS

		//Cast to ecg_data type
//			ecg_data_write(&afe_data_point, afe_bytes[0], afe_bytes[1], afe_bytes[2]);

		//Store signal data into ecg signal buffer
//			ecg_data_circular_buffer_write(&ecg_buffer, &afe_data_point);
			ecg_data_circular_buffer_write(&ecg_buffer, afe_bytes[0], afe_bytes[1], afe_bytes[2]);

		P1IFG &= ~BIT2;                       	// Clear DRDY (P1.2) flag

	}
	else if (P1IFG & BIT3)
	{
		//Request last position
			touch_request_position(&touch);

		//Beep
			//buzzer_play(&buzzer, E5, 50);

		//Lambada
//			buzzer_play(&buzzer, E5, 250);
//			__delay_cycles(2500000);
//			__delay_cycles(2500000);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, D5, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, C5, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, B4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, A4, 250);
//			__delay_cycles(2500000);
//			__delay_cycles(2500000);
//
//			buzzer_play(&buzzer, A4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, C5, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, B4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, A4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, G4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, A4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, E4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, D4, 250);
//			__delay_cycles(2500000);
//			buzzer_play(&buzzer, E4, 250);
//			__delay_cycles(2500000);
//			__delay_cycles(2500000);
//			__delay_cycles(2500000);

		//Paint
//			__bic_SR_register(GIE);
//			display_write_pixel(&display, COLOR_WHITE, touch.touch_last_position.xPos, touch.touch_last_position.yPos);
//			display_functions_write_pixel(COLOR_WHITE, touch.touch_last_position.xPos, touch.touch_last_position.yPos);
//			__bis_SR_register_on_exit(GIE);
		if (g_flag == 0)
		{
		    display_functions_write_string(" DANGER: Apechusque ", COLOR_WHITE,
																   display.display_interface.menubar_window_bg_color,
																   0x00, 0xD0);
		    g_flag = 1;
		}
		else
		{
		    display_functions_write_string(" DANGER: Apichusque ", COLOR_WHITE,
																   display.display_interface.menubar_window_bg_color,
																   0x00, 0xD0);
		    g_flag = 0;
		}

		P1IFG &= ~BIT3;                         //Clear IRQ (P1.3) flag
	}

	__bis_SR_register_on_exit(gie);                   //Restore original GIE state

}
