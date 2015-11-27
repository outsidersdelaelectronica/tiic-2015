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
	volatile int32_t max_array[8] = {0,0,0,0,0,0,0,0};
	volatile int max_pos[8] = {0,0,0,0,0,0,0,0};
	volatile int32_t threshold = 1;

	volatile uint8_t index_1 = 0,index_2 = 0,j = 0;

	volatile int32_t maxerino = 0,dif = 0;
	volatile int maxerino_pos = 0,sample = 0;
	volatile uint16_t calc = 0;
	volatile ecg_data_t aux,prev;
	volatile float bpm = 0;
	char numberino[5];

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

	while(1)
	{
		while(ecg_buffer.index < (BUFFER_SIZE - 1)){
			delay_ms(4);
		}
		__bic_SR_register(GIE);			//Enable global interrupts
		sample = 0;
		index_2 = 0;
		ecg_data_clear(&aux);
		ecg_data_clear(&prev);
		maxerino = 0;
		maxerino_pos = 0;

	    while (ecg_data_circular_buffer_read_full(&ecg_buffer, &aux))
	    {
	        // Iterates over the windows
	        if (aux.data > (threshold * 9) / 10)
	        {
	            // When a sample is higher than threshold
	            // stores the index
	            while((aux.data > (threshold * 9) / 10 )&&(sample < (BUFFER_SIZE - 1)))
				{
	                // Iterates over the windows
	                // while is higher than the TH
	            	ecg_data_circular_buffer_read_full(&ecg_buffer, &aux);
	                if((maxerino < aux.data )&&( aux.data > prev.data))
					{
	                    // Stores max
	                    maxerino = aux.data;
	                    maxerino_pos = sample;
					}
	                sample++;
	                ecg_data_copy(&aux,&prev);
				}
				// Stores the end of the sub windows and the
				// value and position of the local max.

	            if((maxerino_pos < (BUFFER_SIZE - 1))&&(maxerino_pos > 0))
				{
				// to ensure the max is not recorded
				// because the windows ends instead
				// of a real max
	            	max_array[index_1++] = maxerino;

		            if (index_1 == 8)
		            {
		                index_1 = 1;
		            }
	                max_pos[index_2++] = maxerino_pos;
				}

	            maxerino = 0;
	            maxerino_pos = 0;

	        }else{
	        	sample++;
	        }

	    }
	    // After went over the window, we calculate
	    // the bpm
	    for (j = 0; j < 8; j++)
	    {
	    	threshold += max_array[j];
	    }
	    threshold = threshold>>3;

	    if (index_2 == 0)
	    {
	        // no max detected from the previous iter
	        threshold = (threshold * 9)/10 ;

	    }else if (index_2 == 1)
	    {
	        max_pos[1] = max_pos[index_2 - 1] - FS;

	    }else{

	        for (j = 0; j < (index_2 - 2);j++)
	        {
	            dif = max_pos[j + 1] - max_pos[j];

	            if((dif < 600)&&(dif > 70))
				{
				   bpm = 60 * FS/dif;
				   calc = (uint16_t)bpm;
				   itoa(calc, numberino);
	               display_functions_write_string(numberino, COLOR_RED,
	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color, 0x50, 0xC0);
//	               calc = (int)(bpm*1024 - calc*1024);
//	               display_functions_write_char('.', COLOR_RED,
//	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color, 0xA0, 0xC0);
//	               itoa(calc, numberino);
//	               display_functions_write_string(numberino, COLOR_RED,
//	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color, 0xB0, 0xC0);
				}
	        }
	        max_pos[1] = max_pos[index_2 - 1] - FS;
	        index_2 = 1;
	   }
	    __bis_SR_register(GIE);			//Enable global interrupts
	}
}
