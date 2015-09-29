/*
 * main.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include <msp430.h>

#include "data/ecg_data_circular_buffer.h"

#include "clocks.h"
#include "afe/afe.h"
#include "touch/touch.h"
#include "display/display.h"
#include "display/display_src/color.h"
#include "buzzer/buzzer.h"

ecg_data_circular_buffer_t ecg_buffer;
touch_coordinate_t touch_last_position;
display_t display;
buzzer_t buzzer;

int main()
{
    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer

    /*
     * Setups
     */
    clocks_setup();

    afe_setup();
    ecg_data_circular_buffer_setup(&ecg_buffer);
    display_setup(&display);
    touch_setup();
    buzzer_setup(&buzzer);

    /*
     * MCU setup
     */
    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    afe_initialize();
    display_init(&display);
    touch_initialize();

    /*
     * Sheits
     */
	display_functions_write_string(" BPM: 820           ", COLOR_RED,
								  	  	  	  	  	  	   display.display_interface.menubar_window_bg_color,
														   0x00, 0xC0);
	display_functions_write_string(" DANGER: Apichusque ", COLOR_WHITE,
														   display.display_interface.menubar_window_bg_color,
														   0x00, 0xD0);

	__bis_SR_register(GIE);			//Enable global interrupts

	while(1)
	{
	}
}
