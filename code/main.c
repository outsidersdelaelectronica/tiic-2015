#include <msp430.h> 

#include "clocks.h"
#include "utils.h"
#include "serial.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "CircularBuffer/circularBuffer.h"
#include "Display/display.h"
#include "Touch/touch.h"

volatile circularBuffer ecgSignalBuffer;
volatile uint8_t touch_xPos, touch_yPos;

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

//		//Screen writing
//			display_write_string("Ya no te ", i, j, k, 0x20, 0x20);
//			display_write_string("pasas por", k, i, j, 0x20, 0x20 + 0x10);
//			display_write_string("el parque", j, k, i, 0x20, 0x20 + 0x20);
//			display_write_string(posX_string, j, k, i, 0x20, 0x20 + 0x40);
//			display_write_string(posY_string, j, k, i, 0x20 + 0x40, 0x20 + 0x40);
			display_write_string("Ya no te pasas por e", i, j, k, 0x00, 0xB0);
			display_write_string("l parque ya no te pa", i, j, k, 0x00, 0xC0);
			display_write_string("sas por el parque ya", i, j, k, 0x00, 0xD0);
			display_write_string("no te pasas por el p", i, j, k, 0x00, 0xE0);

		uint16_t hor_var;
		ecgData signalDataPoint;

		//Scroll horizontally
		for (hor_var = 0; hor_var < 320; hor_var++)
		{
			if (circularBuffer_read(&ecgSignalBuffer, &signalDataPoint))	//If there is data available
			{
				display_write_signal(&signalDataPoint);						//Write it
			}
		}

	}
}
