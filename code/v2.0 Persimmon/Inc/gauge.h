/*
 * gauge.h
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "gauge_constants.h"
#include "i2c.h"

#include <stdint.h>

#ifndef GAUGE_GAUGE_H_
#define GAUGE_GAUGE_H_

void fg_init();

void fg_write_command(uint16_t command);
void fg_write_data(uint16_t data);

void fg_read_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);
void fg_write_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size);

#endif /* GAUGE_GAUGE_H_ */
