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

//    display_functions_write_string(" BPM: 820           ", COLOR_RED,
//	  	  	  	   	   	   	   	   	   	   	   	   	   	   display.display_interface.menubar_window_bg_color,
//														   0x00, 0xC0);
    display_functions_write_string(" DANGER: Apichusque ", COLOR_WHITE,
														   display.display_interface.menubar_window_bg_color,
														   0x00, 0xD0);

	__bis_SR_register(GIE);			//Enable global interrupts

	ecg_data_t datarino[3]={0x00,0x00,0x00}, max_array[50], max_max_array[7], aux, *aux_dat, *aux_max;
	uint8_t max_pos[50],max_max_pos[7],muestra,i,j,average,*aux_pos,dif;
	int32_t max_max = -1000000;
	char bpm[5];

	for(i = 0; i <3; i++ )
	{
		ecg_data_clear(&datarino[i]);
	}

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
		    j=0;
		    average=0;
		    muestra = 1;
			while(ecg_data_circular_buffer_read_full(&ecg_buffer,&aux))
			{
//				*aux_dat = *(++aux_dat);
//				*aux_dat = *(++aux_dat);
//				*aux_dat = aux;
				datarino[0] = datarino[1];
				datarino[1] = datarino[2];
				datarino[2] = aux;
				if((datarino[1].data < datarino[0].data)&&(datarino[1].data < datarino[2].data)&&(j<50))
				{
//					ecg_data_copy(&datarino[1],aux_max++);
//					*(aux_pos++) = muestra;
					ecg_data_copy(&datarino[1],&max_array[j]);
					max_pos[j] = muestra;
					j++;
				}
				muestra++;
//				aux_dat = datarino;
			}
			for(i = 0;i < 50; i++)
			{
				if(max_max < max_array[i].data)
				{
					max_max = max_array[i].data;
				}
			}
			j=0;
			for(i = 0;i < 50; i++)
			{
				if(max_array[i].data > 0)
				{
					max_max_pos[j] = max_pos[i];
					j++;
				}
			}
			for(i = 0;i < j-1; i++)
			{
				dif = max_max_pos[i+1] - max_max_pos[i];
				average = average + dif/j;
			}
			itoa(j,bpm);
		    display_functions_write_string(bpm, COLOR_RED,
										   display.display_interface.menubar_window_bg_color,
										   0x00, 0xC0);
		    max_max = -1000000;
		}
	}
}
