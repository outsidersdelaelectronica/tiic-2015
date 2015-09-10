#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "Display/display.h"
#include "CircularBuffer/circularBuffer.h"

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
    volatile int data_read = 0;
	data_read = display_IO_read_reg(0x00);

	P9OUT &= ~BIT6;				//Turn screen on

	long i = 0;
	while(1)
	{
		for(i = 0 ; i < 60L * 320L ; i++)
    	{

			display_IO_write_GRAM(0x00, 0x00, 0xfc);

    	}
    	delay_ms(1000);

    	for(i = 0 ; i < 120L * 320L ; i++)
    	{

    		display_IO_write_GRAM(0x00, 0xfc, 0xfc);
    	}
    	delay_ms(1000);
    	for(i = 0 ; i < 60L * 320L ; i++)
    	{

			display_IO_write_GRAM(0x00, 0x00, 0xfc);
    	}
	}

}
