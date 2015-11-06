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

	ecg_data_t datarino[3], max_array[10],max_max_array[7],last_max;
//	ecg_data_t *aux_dat, *aux_max;
	uint16_t max_pos[10],max_max_pos[7];
	uint16_t n_sample,last_pos = 0,average;

	uint8_t i,j,k;

//	uint8_t *aux_pos;
	volatile int max_max,th;
	volatile int filtered_signal[3];
	volatile int32_t bpm_interino=0;

	char bpm[5];
	itoa(0,bpm);

	ecg_data_clear(&last_max);

	while(1)
	{
		while(ecg_buffer.index < ecg_buffer.buffer_size - 1);

//			aux_dat = datarino;
//			aux_max = max_array;
//			aux_pos = max_pos;

		j=1;								// Variable reset ( this could be avoided in a function)
		k=0;								// |

		n_sample = 250;						// |
		max_max = 0x8000;					// |
											// |
		for(i = 0; i <2; i++ )				// |
		{									// |
//			ecg_data_clear(&datarino[i]);	// |
			ecg_data_circular_buffer_read_full(&ecg_buffer,&datarino[i]);
		}									// |
											// |
		for(i = 0; i <10; i++ )				// |
		{									// |
			ecg_data_clear(&max_array[i]);				// |
			max_pos[i] = 0;
		}									// |
		*max_pos = last_pos;
		ecg_data_copy(&last_max,max_array);
		for(i = 0; i <7; i++ )				// |
		{									// |
			ecg_data_clear(&max_max_array[i]);				// |
			max_max_pos[i] = 0;
		}

		while(ecg_data_circular_buffer_read_full(&ecg_buffer,&datarino[2])) // Read all the buffer
		{

			// Establish a dinamic threshold for local maximum to be stored
			// The local max condition is that the previous and next sample both are lower
			// than the current sample ( review that )

			if(max_max < datarino[2].data)
			{
				max_max = datarino[2].data;		// Refresh max maximun
			}

			th = max_max >> 1;

			if((datarino[1].data > datarino[0].data)&&(datarino[1].data > datarino[2].data)&&
						(datarino[1].data > th ))
			{
//					ecg_data_copy(&datarino[1],aux_max++);
//					*(aux_pos++) = muestra;
				ecg_data_copy(&datarino[1],&max_array[j]);
				max_pos[j] = n_sample;
				j++;

			}
			n_sample++;
//				*aux_dat = *(++aux_dat);
//				*aux_dat = *(++aux_dat);
			datarino[0] = datarino[1];		// Shift samples
			datarino[1] = datarino[2];

//				aux_dat = datarino;
		}

		th = (max_max * 9)/10;	// Threshold for consider a maximun as a R peak
		average = 0;							// |

		for(i = 0;i < j ; i++)
		{
			if(max_array[i].data > th)
			{
				ecg_data_copy(&max_array[i],&max_max_array[k]);
				max_max_pos[k] = max_pos[i];
				k++;
			}
		}

		if (k > 1)
		{
			for(i = 0;i < k - 1; i++) // Average the R peaks distance
			{
				average = average + (max_max_pos[i+1] - max_max_pos[i]) / (k - 1);
			}

			bpm_interino = (60 * 250) / average;		//bpm
			itoa((uint16_t)bpm_interino,bpm);
		}

		display_functions_write_string(bpm, COLOR_RED,
									   display.display_interface.menubar_window_bg_color,
									   0x50, 0xC0);

		last_pos = max_max_pos[k-1] - 250;
		ecg_data_copy(&max_max_array[k-1],&last_max);


	}
}
