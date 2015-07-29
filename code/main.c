#include <msp430.h>
#include "clocks.h"
#include "circularBuffer.h"
#include "AFE/ADS1291_module.h"

volatile circularBuffer ecgSignal;

/*
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;					//Stop watchdog timer

	//Setups
		clocks_setup();								//Clock configuration
		ADS1291_setup();							//AFE ports and serial configuration
		circularBuffer_setup(&ecgSignal);			//ECG signal storing buffer setup

	PM5CTL0 &= ~LOCKLPM5;						//Disable the GPIO power-on default high-impedance mode

	//Initializations
		ADS1291_initialize();						//AFE power-up and initialization

	return 0;
}
