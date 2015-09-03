#include <msp430.h> 

#include "clocks.h"
#include "utils.h"

#include "Display/display.h"
#include "Buzzer/buzzer.h"
#include "Buzzer/buzzer_notes.h"

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

    PM5CTL0 &= ~LOCKLPM5;		//Disable the GPIO power-on default high-impedance mode

    /*
     * Initializations
     */
    display_initialize();

    /*
     * Sheits
     */
    volatile int data_read = 0;
	data_read = display_IO_read_reg(0x00);

	P9OUT &= ~BIT6;				//Turn screen on

	long i = 0;
	while(1)
    {
		for(i = 0 ; i < 40L * 320L ; i++)
    	{
    		//display_IO_write_GRAM(0xF3, 0x38);
			display_IO_write_GRAM(0xf0, 0x00, 0xff);
    	}
    	delay_ms(1000);

    	for(i = 0 ; i < 40L * 320L ; i++)
    	{
    		//display_IO_write_GRAM(0x03, 0x38);
    		display_IO_write_GRAM(0x00, 0xff, 0x00);
    	}
    	delay_ms(1000);
    	for(i = 0 ; i < 40L * 320L ; i++)
    	{
    		//display_IO_write_GRAM(0x03, 0x38);
    		display_IO_write_GRAM(0xff, 0x00, 0x00);
    	}
    	delay_ms(1000);
    }

}
