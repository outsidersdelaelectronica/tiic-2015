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

	ecg_data_t datarino[3], max_array[50], max_max_array[7];
//	ecg_data_t *aux_dat, *aux_max;
	uint8_t max_pos[50],max_max_pos[7],n_sample,i,j,k,average,dif;
//	uint8_t *aux_pos;
	int32_t max_max,th;
	char bpm[5];

	while(1)
	{
		if(ecg_buffer.index < ecg_buffer.buffer_size - 1)
		{
			delay_ms(7);
		}
		else
		{
//			aux_dat = datarino;
//			aux_max = max_array;
//			aux_pos = max_pos;
		    j=0;								// Variable reset ( this could be avoided in a function)
		    k=0;								// |
		    average=0;							// |
		    muestra = 1;						// |
		    max_max = 0x80000000;				// |
		    									// |
			for(i = 0; i <3; i++ )				// |
			{									// |
				ecg_data_clear(&datarino[i]);	// |
			}									// |

			while(ecg_data_circular_buffer_read_full(&ecg_buffer,&datarino[2])) // Read all the buffer
			{
				th = max_max>>2;	// Establish a dinamic threshold for local maximum to be stored
				// The local max condition is that the previous and next sample both are lower
				// than the current sample ( review that )
				if((datarino[1].data > datarino[0].data)&&(datarino[1].data > datarino[2].data)&&
							(datarino[1].data > th ))
				{
//					ecg_data_copy(&datarino[1],aux_max++);
//					*(aux_pos++) = muestra;
					ecg_data_copy(&datarino[1],&max_array[j]);
					max_pos[j] = n_sample;
					j++;
					if(max_max < datarino[1].data)
					{
						max_max = datarino[1].data;		// Refresh max maximun
					}
				}
				n_sample++;
//				*aux_dat = *(++aux_dat);
//				*aux_dat = *(++aux_dat);
				datarino[0] = datarino[1];		// Shift samples
				datarino[1] = datarino[2];
//				aux_dat = datarino;
			}
//			for(i = 0;i < j; i++)
//			{
//				if(max_max < max_array[i].data)
//				{
//					max_max = max_array[i].data;
//				}
//			}

			th = (max_max*8)/10;	// Threshold for consider a maximun as a R peak

			for(i = 0;i < j; i++)
			{
				if(max_array[i].data > th)
				{
					max_max_pos[k] = max_pos[i];
					k++;
				}
			}

			for(i = 0;i < k-1; i++) // Average the R peaks distance
			{
				dif = max_max_pos[i+1] - max_max_pos[i];
				average = average + dif/k;
			}
			average = average>>7;		//beat per sec
			average = average * 60;		//bpm

			itoa(average,bpm);

		    display_functions_write_string(bpm, COLOR_RED,
										   display.display_interface.menubar_window_bg_color,
										   0x50, 0xC0);

		}
	}
}
