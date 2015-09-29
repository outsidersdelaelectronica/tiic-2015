/*
 * display_functions.c
 *
 *  Created on: 14/9/2015
 *      Author: Smau
 */

#include "display_functions.h"

extern const font displayFont;
extern display_interface_t display_interface;

void display_sleep()
{
	display_IO_write_reg(0x07, 0x01, 0x31); // Set D1=0,  D0=1
	delay_ms(10);
	display_IO_write_reg(0x07, 0x01, 0x30); // Set D1=0,  D0=0
	delay_ms(10);
	display_IO_write_reg(0x07, 0x00, 0x00); // display OFF
	//************* Power OFF sequence **************//
	display_IO_write_reg(0x10, 0x00, 0x80); // SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP
	display_IO_write_reg(0x11, 0x00, 0x00); // DC1[2:0],  DC0[2:0],  VC[2:0]
	display_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	display_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	display_IO_write_reg(0x10, 0x00, 0x82); // SAP,  BT[3:0],  APE,  AP,  DSTB,  SLP.
}

void display_wake_up()
{
	//*************Power On sequence ******************//
	display_IO_write_reg(0x10, 0x00, 0x80); // SAP,  BT[3:0],  AP,  DSTB,  SLP
	display_IO_write_reg(0x11, 0x00, 0x00); // DC1[2:0],  DC0[2:0],  VC[2:0]
	display_IO_write_reg(0x12, 0x00, 0x00); // VREG1OUT voltage
	display_IO_write_reg(0x13, 0x00, 0x00); // VDV[4:0] for VCOM amplitude
	delay_ms(200); // Dis-charge capacitor power voltage
	display_IO_write_reg(0x10, 0x12, 0x90); // SAP,  BT[3:0],  AP,  DSTB,  SLP,  STB
	display_IO_write_reg(0x11, 0x02, 0x27); // Set DC1[2:0],  DC0[2:0],  VC[2:0]
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x12, 0x00, 0x1B); // External reference voltage =Vci;
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x13, 0x19, 0x00); // Set VDV[4:0] for VCOM amplitude
	display_IO_write_reg(0x29, 0x00, 0x0F); // Set VCM[5:0] for VCOMH
	delay_ms(50); // Delay 50ms
	display_IO_write_reg(0x07, 0x01, 0x33); // 262K color and display ON
}

void display_write_pixel(uint8_t red, uint8_t green, uint8_t blue, uint16_t x, uint16_t y)
{
	display_IO_set_GRAM_address(x, y);
	display_IO_write_GRAM(red, green, blue);
}

void display_write_char(char character, uint8_t red, uint8_t green, uint8_t blue, uint16_t posH, uint16_t posV)
{
	uint8_t* charStartingPosition;
	charStartingPosition = font_get_char(&displayFont, character);		//Pointer to starting byte

	uint16_t line;
	uint16_t local_posH = posH;
	uint16_t local_posV = posV;

	int16_t i,j;
	uint16_t mask;

	for(i = 0; i < displayFont.fontHeight; i++){
		line = *charStartingPosition++;
		line = (line << 8) | (*charStartingPosition++);

		mask = 0x8000;
		//Draw line
		for(j = 0; j < displayFont.fontWidth; j++)
		{
			if (line & mask)
			{
				display_write_pixel(red, green, blue, local_posH, local_posV);
			}
			else
			{
				display_write_pixel(display_interface.menubar_background_color[0],
									display_interface.menubar_background_color[1],
									display_interface.menubar_background_color[2], local_posH, local_posV);
			}
			local_posH++;
			mask = mask >> 1;
		}

		//Move to the beginning of the next line
		local_posV++;
		local_posH = posH;
	}
}

void display_write_string(char* string, uint8_t red, uint8_t green, uint8_t blue, uint16_t posH, uint16_t posV)
{
	int i = 0;
	char character;

	while ((character = string[i]) != 0)
	{
		i++;
		display_write_char(character, red, green, blue, posH, posV);
		posH = posH + displayFont.fontWidth;
	}
}

