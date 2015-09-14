/*
 * display.c
 *
 *  Created on: 1/8/2015
 *      Author: slopez
 */

#include "display.h"

void display_setup()
{
	P7DIR |= BIT1;					//Set display_RESET (P7.1) as output
	P9DIR |= BIT7 | BIT5 | BIT4;	//Set display_CS (P9.7), display_WR (P9.4) and display_RD (P9.5) as outputs
	P2DIR |= BIT3;					//Set display_RS (P2.3) as output
	P9DIR |= BIT6;					//Set display_SLEEP (P9.6) as output

	P9OUT |= BIT7 | BIT5 | BIT4;	//Set display_CS, display_WR and display_RD
	P2OUT |= BIT3;					//Set display_RS

	P9OUT |= BIT6;					//Turn screen off
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
	display_interface_setup();
	display_IO_write_reg(0x03, 0xD0, 0x30); // set GRAM horizontal write direction

}
