
/*
 * serial.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "serial.h"

extern buzzer_t buzzer;
extern display_t display;
extern touch_t touch;
extern ecg_data_t last_sample;
extern int bpm;

extern uint8_t afe_bytes[3];
extern uint8_t i,flag ;
extern int maxerino_pos = 0,sample_counter = 0;
extern int32_t threshold, maxerino,current_value, prev_value;


#define FS 250

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
	uint16_t gie = __get_SR_register() & GIE; //Store current GIE state

//	uint8_t afe_bytes[3] = {0x00 , 0x00, 0x00};
//	uint8_t i = 0;
//	static uint8_t flag = 0;
//	static int maxerino_pos = 0,sample_counter = 0;
//	static int32_t threshold = 0x00FFFFFF, maxerino = 0,current_value = 0, prev_value = 0;

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

		//Store signal data into ecg signal buffer

		ecg_data_write(&last_sample, afe_bytes[0], afe_bytes[1], afe_bytes[2]);

		current_value = filter_sample(last_sample.data);

		if(current_value >= (threshold * 7) >>3 )
		{
			if (current_value >= maxerino )
			{
				maxerino = current_value;
				maxerino_pos = sample_counter;
			}else if (flag == 0){
				threshold = (maxerino * 7) >>3;
				flag = 1;
			}
		}else if ((prev_value >= (threshold * 7) >>3 ) && (maxerino > 0))
		{
			bpm = (60 * FS) / maxerino_pos;
			threshold = (( threshold * 7 + maxerino) >> 3);
			sample_counter = sample_counter - maxerino_pos -1;
			maxerino = 0;
		}else{
			threshold = (threshold * 127) >> 7;
			maxerino = 0;
		}

		sample_counter++;
		prev_value = current_value;

		P1IFG &= ~BIT2;                       	// Clear DRDY (P1.2) flag

	}else if (P1IFG & BIT3){
		//Request last position
			touch_request_position(&touch);

		//Beep
			//buzzer_play(&buzzer, E5, 50);

		//Paint
		display_functions_write_pixel(COLOR_WHITE, touch.touch_last_position.xPos, touch.touch_last_position.yPos);

		P1IFG &= ~BIT3;                         //Clear IRQ (P1.3) flag
	}

	__bis_SR_register_on_exit(gie);                   //Restore original GIE state

}
