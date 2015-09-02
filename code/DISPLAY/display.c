/*
 * display.c
 *
 *  Created on: 1/8/2015
 *      Author: slopez
 */

#include "display.h"

void LCD_setup()
{
	P7DIR |= BIT1;					//Set LCD_RESET (P7.1) as output
	P9DIR |= BIT7 | BIT5 | BIT4;	//Set LCD_CS (P9.7), LCD_WR (P9.4) and LCD_RD (P9.5) as outputs
	P2DIR |= BIT3;					//Set LCD_RS (P2.3) as output
	P9DIR |= BIT6;					//Set LCD_SLEEP (P9.6) as output

	P9OUT |= BIT7 | BIT5 | BIT4;	//Set LCD_CS, LCD_WR and LCD_RD
	P2OUT |= BIT3;					//Set LCD_RS
	P9OUT &= ~BIT6;					//Turn on screen
}

void LCD_initialize()
{
	 //************* Reset LCD Driver ****************//
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(1);
	P7OUT &= ~BIT1;  				//Write 0 to LCD_RESET
	delay_ms(10);
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(50);

	//************* Start Initial Sequence **********//
	LCD_IO_write_reg(0xE3, 0x30, 0x08); // Set internal timing
	LCD_IO_write_reg(0xE7, 0x00, 0x12); // Set internal timing
	LCD_IO_write_reg(0xEF, 0x12, 0x31); // Set internal timing
	LCD_IO_write_reg(0x01, 0x01, 0x00); // set SS and SM bit
	LCD_IO_write_reg(0x02, 0x07, 0x00); // set 1 line inversion
	LCD_IO_write_reg(0x03, 0x10, 0x30); // set GRAM write direction and BGR=1.
	LCD_IO_write_reg(0x04, 0x00, 0x00); // Resize register
	LCD_IO_write_reg(0x08, 0x02, 0x02); // set the back porch and front porch
	LCD_IO_write_reg(0x09, 0x00, 0x00); // set non-display area refresh cycle ISC[3:0]
	LCD_IO_write_reg(0x0A, 0x00, 0x00); // FMARK function
	LCD_IO_write_reg(0x0C, 0x00, 0x00); // RGB interface setting
	LCD_IO_write_reg(0x0D, 0x00, 0x00); // Frame marker Position
	LCD_IO_write_reg(0x0F, 0x00, 0x00); // RGB interface polarity
	//*************Power On sequence ****************//
	LCD_IO_write_reg(0x10, 0x00, 0x00); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	LCD_IO_write_reg(0x11, 0x00, 0x07); // DC1[2:0]); DC0[2:0]); VC[2:0]
	LCD_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	LCD_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_IO_write_reg(0x10, 0x12, 0x90); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	LCD_IO_write_reg(0x11, 0x02, 0x27); // Set DC1[2:0]); DC0[2:0]); VC[2:0]
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x12, 0x00, 0x1B); // External reference voltage= Vci;
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x13, 0x19, 0x00); // Set VDV[4:0] for VCOM amplitude
	LCD_IO_write_reg(0x29, 0x00, 0x0F); // SetVCM[5:0] for VCOMH
	LCD_IO_write_reg(0x2B, 0x00, 0x0C); // Set Frame Rate
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x20, 0x00, 0x00); // GRAM horizontal Address
	LCD_IO_write_reg(0x21, 0x00, 0x00); // GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	LCD_IO_write_reg(0x30, 0x00, 0x00);
	LCD_IO_write_reg(0x31, 0x04, 0x06);
	LCD_IO_write_reg(0x32, 0x00, 0x04);
	LCD_IO_write_reg(0x35, 0x03, 0x05);
	LCD_IO_write_reg(0x36, 0x00, 0x04);
	LCD_IO_write_reg(0x37, 0x02, 0x07);
	LCD_IO_write_reg(0x38, 0x01, 0x03);
	LCD_IO_write_reg(0x39, 0x07, 0x07);
	LCD_IO_write_reg(0x3C, 0x05, 0x03);
	LCD_IO_write_reg(0x3D, 0x00, 0x04);
	//------------------ Set GRAM area ---------------//
	LCD_IO_write_reg(0x50, 0x00, 0x00); // Horizontal GRAM Start Address
	LCD_IO_write_reg(0x51, 0x00, 0xEF); // Horizontal GRAM End Address
	LCD_IO_write_reg(0x52, 0x00, 0x00); // Vertical GRAM Start Address
	LCD_IO_write_reg(0x53, 0x01, 0x3F); // Vertical GRAM Start Address
	LCD_IO_write_reg(0x60, 0xA7, 0x00); // Gate Scan Line
	LCD_IO_write_reg(0x61, 0x00, 0x01); // NDL);VLE); REV
	LCD_IO_write_reg(0x6A, 0x00, 0x00); // set scrolling line
	//-------------- Partial Display Control ---------//
	LCD_IO_write_reg(0x80, 0x00, 0x00);
	LCD_IO_write_reg(0x81, 0x00, 0x00);
	LCD_IO_write_reg(0x82, 0x00, 0x00);
	LCD_IO_write_reg(0x83, 0x00, 0x00);
	LCD_IO_write_reg(0x84, 0x00, 0x00);
	LCD_IO_write_reg(0x85, 0x00, 0x00);
	//-------------- Panel Control -------------------//
	LCD_IO_write_reg(0x90, 0x00, 0x10);
	LCD_IO_write_reg(0x92, 0x06, 0x00);
	LCD_IO_write_reg(0x07, 0x01, 0x33); // 262K color and display ON

}

void LCD_sleep()
{
	LCD_IO_write_reg(0x07, 0x01, 0x31); // Set D1=0,  D0=1
	delay_ms(10);
	LCD_IO_write_reg(0x07, 0x01, 0x30); // Set D1=0,  D0=0
	delay_ms(10);
	LCD_IO_write_reg(0x07, 0x00, 0x00); // display OFF
	//************* Power OFF sequence **************//
	LCD_IO_write_reg(0x10, 0x00, 0x80); // SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP
	LCD_IO_write_reg(0x11, 0x00, 0x00); // DC1[2:0],  DC0[2:0],  VC[2:0]
	LCD_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	LCD_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_IO_write_reg(0x10, 0x00, 0x82); // SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP.
}
void LCD_wakeUp()
{
	//*************Power On sequence ******************//
	LCD_IO_write_reg(0x10, 0x00, 0x80); // SAP,  BT[3:0],  AP,  DSTB,  SLP
	LCD_IO_write_reg(0x11, 0x00, 0x00); // DC1[2:0],  DC0[2:0],  VC[2:0]
	LCD_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	LCD_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_IO_write_reg(0x10, 0x12, 0x90); // SAP,  BT[3:0],  AP,  DSTB,  SLP,  STB
	LCD_IO_write_reg(0x11, 0x02, 0x27); // Set DC1[2:0],  DC0[2:0],  VC[2:0]
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x12, 0x00, 0x1B); // External reference voltage =Vci;
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x13, 0x19, 0x00); // Set VDV[4:0] for VCOM amplitude
	LCD_IO_write_reg(0x29, 0x00, 0x0F); // Set VCM[5:0] for VCOMH
	delay_ms(50); // Delay 50ms
	LCD_IO_write_reg(0x07, 0x01, 0x33); // 262K color and display ON
}

