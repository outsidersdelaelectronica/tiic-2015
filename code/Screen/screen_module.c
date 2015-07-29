/*
 * screen_module.c
 *
 * Screen module
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#include <msp430.h>
#include <stdint.h>

void screen_setup()
{
	//Port configuration
		P7DIR |= BIT1;		//Screen reset		//Set P7.1 as output
		P9DIR |= BIT6;		//Screen sleep		//Set P9.6 as output

		P9DIR |= BIT4;		//Screen nWR		//Set P9.4 as output
		P9DIR |= BIT5;		//Screen nRD		//Set P9.5 as output
		P9DIR |= BIT7;		//Screen nCS		//Set P9.7 as output
		P2DIR |= BIT3;		//Screen RS			//Set P2.3 as output

		P9OUT |= BIT4 | BIT5 | BIT 7;		//Set nWR, nRD and nCS to high (default value)

}

void screen_databus_input()
{
		P6DIR &=
}

void screen_databus_output()
{

}
