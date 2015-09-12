#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"
#include "Touch/touch.h"

volatile circularBuffer ecgSignalBuffer;

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
	P9OUT &= ~BIT6;				//Turn screen o

	uint8_t ecg_0;
	uint8_t ecg_1;
	uint8_t ecg_2;

	uint8_t i,j,k;
	while(1)
	{
		for(i = 0x80; i < 0xFF; i = i + 0x40)
		{
			for(j = 0x80; j < 0xFF; j = j + 0x40)
			{
				for(k = 0x80; k < 0xFF; k = k + 0x40)
				{
					display_write_string("Ya no te ", i, j, k, 0x40, 0x40);
					display_write_string("pasas por", k, i, j, 0x40, 0x60);
					display_write_string("el parque", j, k, i, 0x40, 0x80);
				}
			}
		}
	}
}
