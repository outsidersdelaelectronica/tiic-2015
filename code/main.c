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
//	while(1)
//    {
		for(i = 0 ; i < 60L * 320L ; i++)
    	{

			display_IO_write_GRAM(0x00, 0x00, 0xfc);

    	}
//    	delay_ms(1000);

    	for(i = 0 ; i < 120L * 320L ; i++)
    	{

    		display_IO_write_GRAM(0x00, 0xfc, 0xfc);
    	}
//    	delay_ms(1000);
    	for(i = 0 ; i < 60L * 320L ; i++)
    	{

			display_IO_write_GRAM(0x00, 0x00, 0xfc);
    	}
//    }
       	buzzer_start(E3);
        	__delay_cycles(3000000);
        	buzzer_start(E3);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);

        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_start(F4);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);

        	buzzer_start(D4);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_start(F4);
        	__delay_cycles(3000000);
        	buzzer_start(E4);
        	__delay_cycles(3000000);

        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_stop();
        	__delay_cycles(3000000);
        	__delay_cycles(3000000);
        	__delay_cycles(3000000);

        	buzzer_start(E4);
        	__delay_cycles(3000000);
        	buzzer_start(D4);
        	__delay_cycles(3000000);
        	buzzer_start(C4);
        	__delay_cycles(3000000);
        	buzzer_start(D4);
        	__delay_cycles(3000000);

        	buzzer_stop();
}
