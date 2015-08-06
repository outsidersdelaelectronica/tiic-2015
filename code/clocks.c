/*
 * clocks.c
 *
 * Clock configuration
 *
 *  Created on: 28/7/2015
 *      Author: Smau
 */

#include "clocks.h"
#include <msp430.h>

void clocks_setup()
{
	//Activate LFXT pins
		PJSEL0 |= BIT4;								//Set PJ.4 and PJ.5 as LFXIN and LFXOUT
		PJSEL1 &= ~BIT4;							// |

	//Activate HFXT pins
		PJSEL0 |= BIT6;								//Set PJ.6 and PJ.7 as HFXIN and HFXOUT
		PJSEL1 &= ~BIT6;							// |


	//Generate SMCLK
		PJDIR |= BIT0;								//Set PJ.0 to output
		PJSEL0 |= BIT0;								//Set PJ.0 as SMCLK
		PJSEL1 &= ~BIT0;							// |

	//Generate ACLK
		PJDIR |= BIT2;								//Set PJ.2 to output
		PJSEL0 |= BIT2;								//Set PJ.2 as ACLK
		PJSEL1 &= ~BIT2;							// |


	//Clock control registers
		CSCTL0 = CSKEY;								//Unlock clock registers
		CSCTL1 = DCOFSEL_0;							//DCO frecuency set to 1 MHz (not used)
		CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__HFXTCLK;
													//ACLK source = LFXTCLK
													//SMCLK source = HFXTCLK
													//MCLK source = HFXTCLK
		CSCTL3 = DIVA_0 | DIVS_3 | DIVM_0;
													//ACLK divider = /1 => ACLK @ 32.768 KHz
													//SMCLK divider = /8 => SMCLK @ 2 MHz
													//MCLK divider = /1 => MCLK @ 16 MHz
		CSCTL4 &= ~HFXTOFF & ~LFXTOFF;
													//Turn HFXT oscillator on
													//Turn LFXT oscillator on
		CSCTL5 &= ~HFXTOFFG & ~LFXTOFFG;
													//Clear oscillator fault flags

	//Oscillator stabilization
		do
		{
			CSCTL5 &= ~LFXTOFFG;                    //Clear LFXT fault flag
			CSCTL5 &= ~HFXTOFFG;                    //Clear HFXT fault flag
			SFRIFG1 &= ~OFIFG;						//Clear general oscillator fault flag
		}
		while (SFRIFG1 & OFIFG);                	//Test oscillator fault flag until there are no faults
													//to let the oscillators stabilize
}
