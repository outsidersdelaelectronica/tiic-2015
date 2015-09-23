/*
 * AFE_serial.h
 *
 *  Created on: 9/9/2015
 *      Author: slopez
 */

#ifndef AFE_AFE_SERIAL_H_
#define AFE_AFE_SERIAL_H_

#include <msp430.h>
#include <stdint.h>

#include "ADS1291_constants.h"

uint8_t	AFE_serial_send(uint8_t data);

uint8_t	AFE_send(uint8_t data);
void	AFE_write_register(uint8_t address, uint8_t value);
uint8_t AFE_read_register(uint8_t address);
#endif /* AFE_AFE_SERIAL_H_ */
