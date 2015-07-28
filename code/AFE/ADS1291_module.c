/*
 * ADS1291_module.c
 *
 * ADS1291 Medical AFE functions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#include "ADS1291_module.h"
#include "ADS1291_constants.h"
#include <stdint.h>

#define ADS1291REGNUM	12

void ADS1291_initialize()
{
	//AFE reset and stop
		ADS1291_command(RESET);						//Reset AFE - This can also be done by activating the AFE reset pin
		__delay_cycles(10000)
		ADS1291_command(SDATAC);					//Stop data conversion (activated by default)

	//Read all registers - optional
		//uint8_t	afeRegisters[ADS1291REGNUM];
		//ADS1291_read_registers(REG_ID, ADS1291REGNUM, afeRegisters);

	//Write config commands to AFE
		ADS1291_write_register(REG_CONFIG2, 0xA3);	//Enable voltage reference
													//Enable test signal      - optional
													//Set signal freq to 1 Hz - optional

		ADS1291_write_register(REG_RESP2, 0x83);	//Enable calibration
		ADS1291_write_register(REG_CH1SET, 0x01);	// |
		ADS1291_command(OFFSETCAL);					// | Calibrate
		ADS1291_write_register(REG_CH1SET, 0x00);	// |
		ADS1291_write_register(REG_RESP2, 0x03);	//Disable calibration

	//Start capturing data
		ADS1291_command(RDATAC);					//Enable continuous output of conversion data
														//In this mode, a SDATAC command must be issued
														//before other commands can be sent to the device
		ADS1291_command(START);						//Start conversions
}


