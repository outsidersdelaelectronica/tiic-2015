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
#include "display/display.h"
#include "display/display_src/display_functions.h"
#include "touch/touch.h"

//buzzer_t buzzer;

display_t display;
//touch_t touch;
ecg_data_t last_sample;
int bpm;

int main()
{
    WDTCTL = WDTPW | WDTHOLD;		//Stop watchdog timer
    bpm = 0;
    ecg_data_clear(&last_sample);
    /*
     * Setups
     */
    clocks_setup();

    afe_setup();
    display_setup(&display);
//    touch_setup(&touch);
//    buzzer_setup(&buzzer);

    /*
     * MCU setup
     */

    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    afe_init();
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
	}
}
