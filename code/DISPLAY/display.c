/*
 * display.c
 *
 *  Created on: 1/8/2015
 *      Author: slopez
 */

#include "display.h"

void LCD_initialize()
{
	 //************* Reset LCD Driver ****************//
	P9OUT |= BIT7|BIT4;				//Write 1 to LCD_CS and LCD_WR
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(1);
	P7OUT &= ~BIT1;  				//Write 0 to LCD_RESET
	delay_ms(10);
	P7OUT |= BIT1;					//Write 1 to LCD_RESET
	delay_ms(50);

	P9OUT &= ~BIT7;					//Write 0 to LCD_CS

	//************* Start Initial Sequence **********//
	LCD_set_reg(0x00,0xE3);
	LCD_write_data(0x30,0x08); // Set internal timing
	LCD_set_reg(0x00,0xE7);
	LCD_write_data(0x00,0x12); // Set internal timing
	LCD_set_reg(0x00,0xEF);
	LCD_write_data(0x12,0x31); // Set internal timing
	LCD_set_reg(0x00,0x01);
	LCD_write_data(0x01,0x00); // set SS and SM bit
	LCD_set_reg(0x00,0x02);
	LCD_write_data(0x07,0x00); // set 1 line inversion
	LCD_set_reg(0x00,0x03);
	LCD_write_data(0x10,0x30); // set GRAM write direction and BGR=1.
	LCD_set_reg(0x00,0x04);
	LCD_write_data(0x00,0x00); // Resize register
	LCD_set_reg(0x00,0x08);
	LCD_write_data(0x02,0x02); // set the back porch and front porch
	LCD_set_reg(0x00,0x09);
	LCD_write_data(0x00,0x00); // set non-display area refresh cycle ISC[3:0]
	LCD_set_reg(0x00,0x0A);
	LCD_write_data(0x00,0x00); // FMARK function
	LCD_set_reg(0x00,0x0C);
	LCD_write_data(0x00,0x00); // RGB interface setting
	LCD_set_reg(0x00,0x0D);
	LCD_write_data(0x00,0x00); // Frame marker Position
	LCD_set_reg(0x00,0x0F);
	LCD_write_data(0x00,0x00); // RGB interface polarity
	//*************Power On sequence ****************//
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x00,0x00); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x00,0x07); // DC1[2:0]); DC0[2:0]); VC[2:0]
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x00); // VREG1OUT voltage
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x00,0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x12,0x90); // SAP); BT[3:0]); AP); DSTB); SLP); STB
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x02,0x27); // Set DC1[2:0]); DC0[2:0]); VC[2:0]
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x1B); // External reference voltage= Vci;
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x19,0x00); // Set VDV[4:0] for VCOM amplitude
	LCD_set_reg(0x00,0x29);
	LCD_write_data(0x00,0x0F); // SetVCM[5:0] for VCOMH
	LCD_set_reg(0x00,0x2B);
	LCD_write_data(0x00,0x0C); // Set Frame Rate
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x20);
	LCD_write_data(0x00,0x00); // GRAM horizontal Address
	LCD_set_reg(0x00,0x21);
	LCD_write_data(0x00,0x00); // GRAM Vertical Address
	// ----------- Adjust the Gamma Curve ----------//
	LCD_set_reg(0x00,0x30);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x31);
	LCD_write_data(0x04,0x06);
	LCD_set_reg(0x00,0x32);
	LCD_write_data(0x00,0x04);
	LCD_set_reg(0x00,0x35);
	LCD_write_data(0x03,0x05);
	LCD_set_reg(0x00,0x36);
	LCD_write_data(0x00,0x04);
	LCD_set_reg(0x00,0x37);
	LCD_write_data(0x02,0x07);
	LCD_set_reg(0x00,0x38);
	LCD_write_data(0x01,0x03);
	LCD_set_reg(0x00,0x39);
	LCD_write_data(0x07,0x07);
	LCD_set_reg(0x00,0x3C);
	LCD_write_data(0x05,0x03);
	LCD_set_reg(0x00,0x3D);
	LCD_write_data(0x00,0x04);
	//------------------ Set GRAM area ---------------//
	LCD_set_reg(0x00,0x50);
	LCD_write_data(0x00,0x00); // Horizontal GRAM Start Address
	LCD_set_reg(0x00,0x51);
	LCD_write_data(0x00,0xEF); // Horizontal GRAM End Address
	LCD_set_reg(0x00,0x52);
	LCD_write_data(0x00,0x00); // Vertical GRAM Start Address
	LCD_set_reg(0x00,0x53);
	LCD_write_data(0x01,0x3F); // Vertical GRAM Start Address
	LCD_set_reg(0x00,0x60);
	LCD_write_data(0xA7,0x00); // Gate Scan Line
	LCD_set_reg(0x00,0x61);
	LCD_write_data(0x00,0x01); // NDL);VLE); REV
	LCD_set_reg(0x00,0x6A);
	LCD_write_data(0x00,0x00); // set scrolling line
	//-------------- Partial Display Control ---------//
	LCD_set_reg(0x00,0x80);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x81);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x82);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x83);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x84);
	LCD_write_data(0x00,0x00);
	LCD_set_reg(0x00,0x85);
	LCD_write_data(0x00,0x00);
	//-------------- Panel Control -------------------//
	LCD_set_reg(0x00,0x90);
	LCD_write_data(0x00,0x10);
	LCD_set_reg(0x00,0x92);
	LCD_write_data(0x06,0x00);
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x01,0x33); // 262K color and display ON

}

void LCD_sleep()
{
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x01,0x31); // Set D1=0, D0=1
	delay_ms(10);
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x01,0x30); // Set D1=0, D0=0
	delay_ms(10);
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x00,0x00); // display OFF
	//************* Power OFF sequence **************//
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x00,0x80); // SAP, BT[3:0], APE, AP, DSTB, SLP
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x00,0x00); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x00); // VREG1OUT voltage
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x00,0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x00,0x82); // SAP, BT[3:0], APE, AP, DSTB, SLP
}
void LCD_wakeUp()
{
	//*************Power On sequence ******************//
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x00,0x80); // SAP, BT[3:0], AP, DSTB, SLP
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x00,0x00); // DC1[2:0], DC0[2:0], VC[2:0]
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x00); // VREG1OUT voltage
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x00,0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	LCD_set_reg(0x00,0x10);
	LCD_write_data(0x12,0x90); // SAP, BT[3:0], AP, DSTB, SLP, STB
	LCD_set_reg(0x00,0x11);
	LCD_write_data(0x02,0x27); // Set DC1[2:0], DC0[2:0], VC[2:0]
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x12);
	LCD_write_data(0x00,0x1B); // External reference voltage =Vci;
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x13);
	LCD_write_data(0x19,0x00); // Set VDV[4:0] for VCOM amplitude
	LCD_set_reg(0x00,0x29);
	LCD_write_data(0x00,0x0F); // Set VCM[5:0] for VCOMH
	delay_ms(50); // Delay 50ms
	LCD_set_reg(0x00,0x07);
	LCD_write_data(0x01,0x33); // 262K color and display ON
}

void LCD_write_GRAM(uint8_t dataH, uint8_t dataL)
{
	LCD_set_reg(0x00,0x22);
	LCD_write_data(dataH,dataL);
}

