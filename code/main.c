/*
 * main.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include <msp430.h>

#include "clocks.h"

#include "afe/afe.h"
#include "buzzer/buzzer.h"
#include "data/ecg_data_circular_buffer.h"
#include "display/display.h"
#include "display/display_src/display_functions.h"
#include "touch/touch.h"
#include "filters/filters.h"

#define FS 250

buzzer_t buzzer;
display_t display;
ecg_data_circular_buffer_t ecg_buffer;
touch_t touch;
ecg_data_t last_sample;
int bpm;

int main()
{

	volatile uint8_t afe_bytes[3] = {0x00 , 0x00, 0x00};
	volatile uint8_t i = 0;
	volatile int prev_max_pos = -1, maxerino_pos = 0, sample_counter = 0, dif = 0;
	volatile int32_t threshold = 350, maxerino = 0, current_value = 0, prev_value = 0, visibility = 0;

    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer
    bpm = 0;
    /*
     * Setups
     */
    clocks_setup();

//    afe_setup();
//    buzzer_setup(&buzzer);
//    ecg_data_circular_buffer_setup(&ecg_buffer);
    display_setup(&display);
//    touch_setup(&touch);

    /*
     * MCU setup
     */

    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
//    afe_init();
    display_init(&display);
//    touch_initialize(&touch);

    /*
     * Sheits
     */

    display_functions_write_string(" BPM:           ", COLOR_RED,
	  	  	  	   	   	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color,
														   0x00, 0xC0);
    display_functions_write_string(" DANGER: Apechusque ", COLOR_WHITE,
														   display.display_interface.menubar_window_bg_color,
														   0x00, 0xD0);

	__bis_SR_register(GIE);			//Enable global interrupts

	while(1)
	{
		delay_ms(8);
		//Read ECG signal - another 3 bytes
		for (i = 3; i > 0; i--)
		{
			afe_bytes[3 - i] = afe_serial_read(0x00);
		}

		//Store signal data into ecg signal buffer

		ecg_data_write(&last_sample, afe_bytes[0], afe_bytes[1], afe_bytes[2]);

		visibility = last_sample.data;

		last_sample.data = (last_sample.data << 4) * ( -1);

		current_value = filter_sample(visibility);

		if(current_value >= threshold )
		{
			if (current_value >= maxerino )
			{
				maxerino = current_value;
				maxerino_pos = sample_counter;
			}
		}else if (prev_value >= threshold )
		{
			dif = maxerino_pos - prev_max_pos;

//			if ((dif <= FS<<1) &&(dif >= FS>>2))
//			{
				bpm = (60 * FS) / dif;
				threshold = (( threshold * 49 + maxerino * 7) >> 6);
				prev_max_pos = maxerino_pos;
				maxerino = 0;
//			}
		}else{
			threshold = threshold - 1;
			maxerino = 0;
		}

		sample_counter++;

		if ( sample_counter == BUFFER_SIZE)
		{
			sample_counter = 0;
			prev_max_pos = prev_max_pos - BUFFER_SIZE;
		}

		prev_value = current_value;
	}
}
