/*
 * ADS1291_module.h
 *
 * ADS1291 function definitions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#ifndef AFE_ADS1291_MODULE_H_
#define AFE_ADS1291_MODULE_H_

void	ADS1291_initialize();

void 	ADS1291_command(uint8_t command);
uint8_t	ADS1291_read_register(uint8_t address);
void	ADS1291_write_register(uint8_t address, uint8_t value);

void	ADS1291_read_registers(uint8_t firstAddress, uint8_t numberOfRegisters, uint8_t* storingArray);
void	ADS1291_write_registers(uint8_t firstAddress, uint8_t numberOfRegisters, uint8_t* storingArray);

#endif /* AFE_ADS1291_MODULE_H_ */
