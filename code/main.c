#include <msp430.h> 

#include "clocks.h"
#include "utils.h"
#include "serial.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"
#include "Touch/touch.h"

volatile circularBuffer_t ecgSignalBuffer;
volatile touch_coordinate_t touch_last_position;

extern display_interface_t display_interface;
extern uint8_t signal_background_color[3];
extern uint8_t menubar_background_color[3];

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	//Stop watchdog timer
	
    /*
     * Setups
     */
    clocks_setup();								//System clocks configuration
    AFE_setup();								//AFE (ADS1291) port setup
    buzzer_setup();								//Buzzer PWM configuration
    display_setup();							//Display port setup
    circularBuffer_setup(&ecgSignalBuffer);		//Ecg signal storage buffer setup
    touch_setup();								//Touch screen setup

    /*
     * MCU setup
     */
    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    AFE_initialize();				//AFE communication start up and configuration commands
    display_initialize();			//Display communication start up and configuration commands
    touch_initialize();				//Touch screen communication start up and configuration commands

    __bis_SR_register(GIE);			//Enable global interrupts

    /*
     * Sheits
     */
	P9OUT &= ~BIT6;				//Turn screen on

	uint16_t hor_var;
	ecgData_t signalDataPoint;

	while(1)
	{
		//Scroll horizontally
		for (hor_var = 0; hor_var < 320; hor_var++)
		{
//			if (circularBuffer_read(&ecgSignalBuffer, &signalDataPoint))		//If there is data available
//			{
				display_write_signal(&display_interface, &signalDataPoint);		//Write it
//			}
		}

	}
}
