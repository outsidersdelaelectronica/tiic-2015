#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"

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

    /*
     * MCU setup
     */
    PM5CTL0 &= ~LOCKLPM5;			//Disable the GPIO power-on default high-impedance mode
    __bis_SR_register(GIE);			//Enable global interrupts

    /*
     * Initializations
     */
    AFE_initialize();				//AFE communication start up and configuration commands
    display_initialize();			//Display communication start up and configuration commands

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
		i++;
		while(1){
			j++;
			while(1){
				k++;
				display_write_string("Ya no te ", i, j, k, 0x40, 0x40);
				display_write_string("pasas por", i, j, k, 0x40, 0x60);
				display_write_string("el parque", i, j, k, 0x40, 0x80);
			}
		}
	}

}
