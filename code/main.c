#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"
#include "Touch/touch.h"
#include "serial.h"

extern volatile circularBuffer ecgSignalBuffer;
extern volatile uint8_t touch_xPos, touch_yPos;

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

	uint8_t i,j,k;
	uint8_t posX, posY;
	char posX_string[4];
	char posY_string[4];

	i = 0xFF;
	j = 0xFF;
	k = 0xFF;

	while(1)
	{
		posX = touch_xPos;
		posY = touch_yPos;

		itoa(posX, posX_string);
		itoa(posY, posY_string);

		//Screen writing
			display_write_string("Ya no te ", i, j, k, 0x20, 0x20);
			display_write_string("pasas por", k, i, j, 0x20, 0x20 + 0x10);
			display_write_string("el parque", j, k, i, 0x20, 0x20 + 0x20);
			display_write_string(posX_string, j, k, i, 0x20, 0x20 + 0x40);
			display_write_string(posY_string, j, k, i, 0x20 + 0x40, 0x20 + 0x40);
	}
}
