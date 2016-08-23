/*
 * gauge_hal.h
 *
 *  Created on: 23/06/2016
 *      Author: slopez
 */
#ifndef GAUGE_HAL_H_
#define GAUGE_HAL_H_

#include <stdint.h>

#include "i2c.h"
#include "gpio.h"

#include "gauge_constants.h"

typedef struct gauge_data_t
{
  int32_t temp;
  int32_t volt;
  uint32_t flags;

  uint32_t nom_avail_cap;
  uint32_t full_avail_cap;
  uint32_t remaining_cap;
  uint32_t full_chg_cap;

  int32_t avg_current;
  int32_t standby_current;
  int32_t max_load_current;
  int32_t avg_power;

  uint32_t soc;
  uint32_t soh;
}
gauge_data_t;

typedef struct gauge_t
{
  /* Hardware/memory map location */
  I2C_HandleTypeDef *hi2c;

  /* Last data received */
  uint8_t last_data_buf[32];
  gauge_data_t last_data;
}
gauge_t;

void gauge_hal_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c);

void gauge_hal_write_reg8 (gauge_t *gauge, uint8_t reg, uint8_t   param);
void gauge_hal_write_reg16(gauge_t *gauge, uint8_t reg, uint16_t  param);

void gauge_hal_read_reg8  (gauge_t *gauge, uint8_t reg, uint8_t  *param);
void gauge_hal_read_reg16 (gauge_t *gauge, uint8_t reg, uint16_t *param);

void gauge_hal_ram_update(gauge_t *gauge,
                          uint8_t data_subclass, uint8_t data_offset,
                          uint8_t *data_buffer,  uint8_t data_size);

#endif /* GAUGE_HAL_H_ */