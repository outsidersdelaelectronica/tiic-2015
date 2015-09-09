/*
 * AFE_module.h
 *
 * AFE function definitions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#ifndef AFE_AFE_H_
#define AFE_AFE_H_

#include <stdint.h>

void	AFE_setup();
void	AFE_initialize();

void 	AFE_command(uint8_t command);
uint8_t	AFE_read_register(uint8_t address);
void	AFE_write_register(uint8_t address, uint8_t value);

#endif /* AFE_AFE_H_ */
