/*
 * buzzer_module.c
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#include "buzzer.h"

void buzzer_setup()
{
	//Timer B0 register configuration
		TB0CCR0 = A4;					//PWM period => A4 = 2272
										//Toggle period = PWM period/2 = 440 Hz = A4
		TB0CCR4 = A4;					//Signal toggled on timer reset

		TB0CTL = TBSSEL__SMCLK | MC__STOP | TBCLR;		//Clock source = SMCLK
														//Mode = Stop mode
														//Reset timer B0

		P2DIR |= BIT2;					//Set P2.2 as output
		P2SEL0 &= ~BIT2;				//Set P2.2 as TB0.4
		P2SEL1 |= BIT2;					// |
}

static void buzzer_set_freq(int note)
{
	TB0CCR0 = note;
	TB0CCR4 = note;
}

void buzzer_start(int note)
{
	buzzer_set_freq(note);
	TB0CCTL4 = OUTMOD_4;							//Output mode = Toggle
	TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;
													//Clock source = SMCLK
													//Mode = Up mode (Counts up to TB0CL0)
													//Reset timer B0
}

void buzzer_stop()
{
	TB0CCTL4 = OUTMOD_0;							//Output mode = OUT bit value
	TB0CCTL4 &= ~OUT;								//OUT bit value = 0
	TB0CTL = TBSSEL__SMCLK | MC__STOP | TBCLR;
													//Clock source = SMCLK
													//Mode = Stop mode
													//Reset timer B0
}
