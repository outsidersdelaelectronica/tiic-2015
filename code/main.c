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

#define FS 500

buzzer_t buzzer;
display_t display;
ecg_data_circular_buffer_t ecg_buffer;
touch_t touch;
ecg_data_t last_sample;

int main()
{
    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer

    /*
     * Setups
     */
    clocks_setup();

    afe_setup();
    buzzer_setup(&buzzer);
    ecg_data_circular_buffer_setup(&ecg_buffer);
    display_setup(&display);
    touch_setup(&touch);

    /*
     * MCU setup
     */

    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    afe_init();
    display_init(&display);
    touch_initialize(&touch);

    /*
     * Sheits
     */

    display_functions_write_string(" BPM:           ", COLOR_RED,
	  	  	  	   	   	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color,
														   0x00, 0xC0);
    display_functions_write_string(" DANGER: Apichusque ", COLOR_WHITE,
														   display.display_interface.menubar_window_bg_color,
														   0x00, 0xD0);

	__bis_SR_register(GIE);			//Enable global interrupts

	int32_t max_array[8] = {0,0,0,0,0,0,0,0};
	int max_pos[8] = {0,0,0,0,0,0,0,0};
	int32_t threshold = 0;

	uint8_t index_1 = 0,index_2 = 0,j;

	int32_t maxerino = 0,dif;
	int maxerino_pos = 0,sample;
	ecg_data_t aux,prev;

	ecg_data_clear(&prev);

	while(1)
	{
		while(ecg_buffer.index < BUFFER_SIZE){
			delay_ms(2);
		}
		sample = 0;
	    while (ecg_data_circular_buffer_read_full(&ecg_buffer, &aux))
	    {
	        // Iterates over the windows
	        if (aux.data > 0.9 * threshold)
	        {
	            // When a sample is higher than threshold
	            // stores the index
	            while(aux.data > 0.9 * threshold)&&(sample < (BUFFER_SIZE - 1))
				{
	                // Iterates over the windows
	                // while is higher than the TH
	                if (maxerino < aux.data )&&(  aux.data > prev.data)
					{
	                    // Stores max
	                    maxerino = aux.data;
	                    maxerino_pos = sample;
					}
	                sample++;
				}
				// Stores the end of the sub windows and the
				// value and position of the local max.

	            max_array(index_1) = maxerino;

	            if (index_1 < 5)
	            {
	                index_1++;
	            }else{
	                index_1 = 1;
	            }

	            if (maxerino_pos < (BUFFER_SIZE - 1))&&(maxerino_pos > 0)
				{
				// to ensure the max is not recorded
				// because the windows ends instead
				// of a real max
	                max_pos(index_2) = maxerino_pos;
	                index_2 = index_2 + 1;
				}

	            maxerino = 0;
	            maxerino_pos = 0;

	        }else{
	            j = j + 1;
	        }

	    }
	    // After went over the window, we calculate
	    // the bpm
	    for (j = 0; j < 8; j++)
	    {
	    	threshold += max_array[j];
	    }
	    threshold = threshold>>3;

	    if (index_2 == 1)
	    {
	        // no max detected from the previous iter
	        threshold = 0.9 * threshold;

	    }else if (index_2 == 2){

	        threshold = 0.9 * threshold;
	        max_pos[1] = max_pos[index_2 - 1] - FS;

	    }else{

	        for (j = 0; j < (index_2 - 2);j++)
	        {
	            dif = max_pos[j + 1] - max_pos[j];

	            if (dif < 600)&&(dif > 70)
				{
	               bpm = 60*fs/dif
				}
	        }
	        max_pos[1] = max_pos[index_2 - 1] - FS;
	        index_2 = 2;
	   }
	}
}
