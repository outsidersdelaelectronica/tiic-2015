/*
 * display.c
 *
 *  Created on: 1/8/2015
 *      Author: slopez
 */

#include "display.h"

//Display interface (located at main.c)
extern display_interface_t display_interface;
//Global ecg signal storage buffer (located at main.c)
extern circularBuffer_t ecgSignalBuffer;

// Timer A2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) Timer2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	//Paint ECG value every display tick
		__bic_SR_register(GIE);

		static ecgData_t signalDataPoint;										//Temporary storage variable
		if (circularBuffer_read_full(&ecgSignalBuffer, &signalDataPoint))		//If there is data available
		{
			display_write_signal(&display_interface, &signalDataPoint);			//Write it
		}

		__bis_SR_register_on_exit(GIE);
}

/*
 * Sweep time is limited between 500 ms and 10 sg
 */
static void display_set_sweep_time(int disp_sweep_time_ms)
{
	//Interrupt period -> Calculated from display sweep time and horizontal display resolution
	uint16_t timer_compare_value;

	switch (disp_sweep_time_ms)
	{
		case SWEEP_TIME_10000:
			timer_compare_value = 62500;
			break;
		case SWEEP_TIME_5000:
			timer_compare_value = 31250;
			break;
		case SWEEP_TIME_2000:
			timer_compare_value = 12500;
			break;
		case SWEEP_TIME_1000:
			timer_compare_value = 6250;
			break;
		case SWEEP_TIME_500:
			timer_compare_value = 3125;
			break;
		case SWEEP_TIME_200:
			timer_compare_value = 1250;
			break;
		default:
			break;
	}

	//TODO
	//Make enum type

	TA2CCR0 = timer_compare_value;						//2000 equals to 1 ms @ 2 MHz
}

static void display_timer_setup()
{
	//Timer A2 register configuration
		TA2CTL = TASSEL__SMCLK | MC__STOP | TACLR;			//Clock source = SMCLK
															//Mode = Stop mode
															//Reset timer A3
		TA2CCTL0 &= ~CCIFG;									//Clear interrupt flag

		display_set_sweep_time(SWEEP_TIME_DEFAULT);			//2 sg sweep time
}

static void display_start_sweep()
{
	TA2CCTL0 |= CCIE;									//Enable compare interrupt
	TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;			//Start timer
}

void display_setup()
{
	P7DIR |= BIT1;					//Set display_RESET (P7.1) as output
	P9DIR |= BIT7 | BIT5 | BIT4;	//Set display_CS (P9.7), display_WR (P9.4) and display_RD (P9.5) as outputs
	P2DIR |= BIT3;					//Set display_RS (P2.3) as output
	P9DIR |= BIT6;					//Set display_SLEEP (P9.6) as output

	P9OUT |= BIT7 | BIT5 | BIT4;	//Set display_CS, display_WR and display_RD
	P2OUT |= BIT3;					//Set display_RS

	P9OUT |= BIT6;					//Turn screen off

	display_timer_setup();			//Setup refresh rate timer
}

void display_initialize()
{
	display_IO_output();
	//************* Reset display Driver ****************//
	P7OUT |= BIT1;					//Write 1 to display_RESET
	delay_ms(5);
	P7OUT &= ~BIT1;  				//Write 0 to display_RESET
	delay_ms(5);
	P7OUT |= BIT1;					//Write 1 to display_RESET
	delay_ms(5);

	//************* Start Initial Sequence **********//
	//display_IO_write_reg(0xE3, 0x30, 0x08); // Set internal timing
	//display_IO_write_reg(0xE7, 0x00, 0x12); // Set internal timing
	//display_IO_write_reg(0xEF, 0x12, 0x31); // Set internal timing
	display_IO_write_reg(0x01, 0x00, 0x00); // set SS and SM bit
	display_IO_write_reg(0x02, 0x06, 0x00); // set 1 line inversion
	display_IO_write_reg(0x03, 0xD0, 0x38); // set GRAM write direction and BGR=1.
	display_IO_write_reg(0x04, 0x00, 0x00); // Resize register
	display_IO_write_reg(0x08, 0x02, 0x02); // set the back porch and front porch
	display_IO_write_reg(0x09, 0x00, 0x00); // set non-display area refresh cycle ISC[3:0]
	display_IO_write_reg(0x0A, 0x00, 0x00); // FMARK function
	display_IO_write_reg(0x0C, 0x00, 0x00); // RGB interface setting
	display_IO_write_reg(0x0D, 0x00, 0x00); // Frame marker Position
	display_IO_write_reg(0x0F, 0x00, 0x00); // RGB interface polarity
	//*************Power On sequence ****************//
	display_IO_write_reg(0x10, 0x00, 0x00); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	display_IO_write_reg(0x11, 0x00, 0x07); // DC1[2:0]); DC0[2:0]); VC[2:0]
	display_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	display_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	display_IO_write_reg(0x10, 0x12, 0x90); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	display_IO_write_reg(0x11, 0x02, 0x27); // Set DC1[2:0]); DC0[2:0]); VC[2:0]
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x12, 0x00, 0x1B); // External reference voltage= Vci;
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x13, 0x19, 0x00); // Set VDV[4:0] for VCOM amplitude
	display_IO_write_reg(0x29, 0x00, 0x0F); // SetVCM[5:0] for VCOMH
	display_IO_write_reg(0x2B, 0x00, 0x0D); // Set Frame Rate
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x20, 0x00, 0x00); // GRAM horizontal Address
	display_IO_write_reg(0x21, 0x00, 0x00); // GRAM vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	display_IO_write_reg(0x30, 0x00, 0x00);
	display_IO_write_reg(0x31, 0x04, 0x06);
	display_IO_write_reg(0x32, 0x00, 0x04);
	display_IO_write_reg(0x35, 0x03, 0x05);
	display_IO_write_reg(0x36, 0x00, 0x04);
	display_IO_write_reg(0x37, 0x02, 0x07);
	display_IO_write_reg(0x38, 0x01, 0x03);
	display_IO_write_reg(0x39, 0x07, 0x07);
	display_IO_write_reg(0x3C, 0x05, 0x03);
	display_IO_write_reg(0x3D, 0x00, 0x04);
	//------------------ Set GRAM area ---------------//
	display_IO_write_reg(0x50, 0x00, 0x00); // Horizontal GRAM Start Address
	display_IO_write_reg(0x51, 0x00, 0xEF); // Horizontal GRAM End Address
	display_IO_write_reg(0x52, 0x00, 0x00); // Vertical GRAM Start Address
	display_IO_write_reg(0x53, 0x01, 0x3F); // Vertical GRAM End Address
	display_IO_write_reg(0x60, 0xA7, 0x00); // Gate Scan Line
	display_IO_write_reg(0x61, 0x00, 0x01); // NDL);VLE); REV
	display_IO_write_reg(0x6A, 0x00, 0x00); // set scrolling line
	//-------------- Partial Display Control ---------//
	display_IO_write_reg(0x80, 0x00, 0x00);
	display_IO_write_reg(0x81, 0x00, 0x00);
	display_IO_write_reg(0x82, 0x00, 0x00);
	display_IO_write_reg(0x83, 0x00, 0x00);
	display_IO_write_reg(0x84, 0x00, 0x00);
	display_IO_write_reg(0x85, 0x00, 0x00);
	//-------------- Panel Control -------------------//
	display_IO_write_reg(0x90, 0x00, 0x10);
	display_IO_write_reg(0x92, 0x06, 0x00);
	display_IO_write_reg(0x07, 0x01, 0x33); // 262K color and display ON

	/*
	 * Paint interface
	 */
	display_interface_setup(&display_interface);
	display_IO_write_reg(0x03, 0xD0, 0x30); // set GRAM horizontal write direction

	/*
	 * Start signal sweeping
	 */
	display_start_sweep();

}
