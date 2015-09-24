/*
 * buzzer_module.c
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#include "buzzer.h"

volatile uint16_t note_duration = 0;
volatile uint16_t ms_count = 0;

static void buzzer_stop();

// Timer A3 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_A0_VECTOR
__interrupt void Timer3_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_A0_VECTOR))) Timer3_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	//Go through ISR as many times as note_duration says
		if (ms_count > note_duration)
		{
			buzzer_stop();									//Stop buzzer timer
			TA3CTL = TASSEL__SMCLK | MC__STOP | TACLR;		//Stop duration timer
		}
		ms_count++;

	//No need to clear CCIFG
}

void buzzer_setup()
{
	//Timer B0 register configuration (PWM generation)
		TB0CCR0 = A4;					//PWM period => A4 = 2272
										//Toggle period = PWM period/2 = 440 Hz = A4
		TB0CCR4 = A4;					//Signal toggled on timer reset

		TB0CTL = TBSSEL__SMCLK | MC__STOP | TBCLR;		//Clock source = SMCLK
														//Mode = Stop mode
														//Reset timer B0

		P2DIR |= BIT2;					//Set P2.2 as output
		P2SEL0 &= ~BIT2;				//Set P2.2 as TB0.4
		P2SEL1 |= BIT2;					// |

	//Timer A3 register configuration (Note duration)
		TA3CCR0 = 2000;					//SMCLK = 2 MHz --> TA3CCR0 = 2000 --> 1 interruption every ms
										//Used to control note duration by ms

		TA3CTL = TASSEL__SMCLK | MC__STOP | TACLR;		//Clock source = SMCLK
														//Mode = Stop mode
														//Reset timer A3
		TA3CCTL0 |= CCIE;								//Enable compare interrupt

}

static void buzzer_set_freq(int note)
{
	TB0CCR0 = note;
	TB0CCR4 = note;
}

static void buzzer_stop()
{
	TB0CCTL4 = OUTMOD_0;								//Output mode = OUT bit value
	TB0CCTL4 &= ~OUT;									//OUT bit value = 0
	TB0CTL = TBSSEL__SMCLK | MC__STOP | TBCLR;			//Stop timer
}

void buzzer_play(int note, int ms)
{
	TA3CCTL0 &= ~CCIE;								//Disable compare interrupt

	//Set note
		buzzer_set_freq(note);
		TB0CCTL4 = OUTMOD_4;							//Output mode = Toggle
		TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;		//Start timer

	//Set duration
		ms_count = 0;
		note_duration = ms;
		TA3CTL = TASSEL__SMCLK | MC__UP | TACLR;		//Start timer

	TA3CCTL0 |= CCIE;								//Enable compare interrupt

	//Wait for timer A3 to turn off buzzer
}
