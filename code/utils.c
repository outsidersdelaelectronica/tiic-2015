/*
 * utils.c
 *
 *  Created on: 1/9/2015
 *      Author: slopez
 */


// Timer A1 interrupt service routine

#include "utils.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	TA1CTL = TASSEL__ACLK | MC_0  ;                      // halts the timer
	__bic_SR_register_on_exit(LPM3_bits);
}

void delay_ms(int ms)
{
	int i = 0;
	for (i = 0; i < ms; ++i) {
		__delay_cycles(16000);
	}
//	TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
//	TA1CCR0 = ms*1000;						  // 1MHz
//	TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS;   // SMCLK, continuous mode
//
//	__bis_SR_register(LPM3_bits | GIE);       // Enter SMCLK, enable interrupts
//	__no_operation();                         // For debugger
}

void delay_ns(int ns)
{
	TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	TA1CCR0 = ns;							  // 1MHz
	TA1CTL = TASSEL__SMCLK | MC__CONTINUOUS;   // ACLK); continuous mode

	__bis_SR_register(LPM3_bits | GIE);       // Enter LPM3); enable interrupts
	__no_operation();                         // For debugger
}
