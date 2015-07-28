#include <msp430.h>
#include "clocks.h"
#include "AFE/ADS1291_module.h"

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;					//Stop watchdog timer

    clocks_setup();								//Clock configuration
    ADS1291_setup();							//AFE ports and serial configuration

	PM5CTL0 &= ~LOCKLPM5;						//Disable the GPIO power-on default high-impedance mode

	ADS1291_initialize();						//AFE power-up and initialization

	return 0;
}
