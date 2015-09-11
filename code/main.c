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

	long i;
//	while(1)
//	{
		for(i = 0 ; i < 320L*240L ; i++)
    	{

			ecg_0 = ecgSignalBuffer.ecgBuffer[i%256].signal[0];
			ecg_1 = ecgSignalBuffer.ecgBuffer[i%256].signal[1];
			ecg_2 = ecgSignalBuffer.ecgBuffer[i%256].signal[2];
			display_IO_write_GRAM(0xf2, ecg_1, ecg_2);

    	}

		display_write_char('y', 0xff, 0xff, 0xff, 0x24, 0x30);
//		for(i = 0 ; i < 240 ; i++)
//		{
//
//			display_write_pixel(0xff, 0xff, 0xff, (uint16_t)i , (uint16_t)i);
//
//		}



//	}

}
