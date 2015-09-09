#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "AFE/AFE.h"
#include "Buzzer/buzzer.h"
#include "Display/display.h"

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	//Stop watchdog timer
	
    /*
     * Setups
     */
    clocks_setup();
    display_setup();
    buzzer_setup();
    AFE_setup();

    /*
     * MCU setup
     */
    PM5CTL0 &= ~LOCKLPM5;		//Disable the GPIO power-on default high-impedance mode
    __bis_SR_register(GIE);		//Enable global interrupts

    /*
     * Initializations
     */
    display_initialize();
    AFE_initialize();


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
