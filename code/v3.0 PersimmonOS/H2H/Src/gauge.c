/*
 * gauge.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "gauge.h"

/* 
 * Fuel gauge design parameters
 *
 * DESIGN_CAPACITY      1050   mAh
 * DESIGN_ENERGY        3885   mWh
 * TERMINATE_VOLTAGE    3500   mV
 * RESERVE_CAPACITY     10     mAh
 * TAPER_RATE           187    0.1 h
 * TAPER_CURRENT        56     mA
 */
uint8_t gauge_param_design_capacity[]      = {0x04, 0x1A};
uint8_t gauge_param_design_energy[]        = {0x0F, 0x2D};
uint8_t gauge_param_terminate_voltage[]    = {0x0D, 0xAC};
uint8_t gauge_param_reserve_capacity[]     = {0x00, 0x0A};
uint8_t gauge_param_taper_rate[]           = {0x00, 0xBB};
uint8_t gauge_operation_config[]           = {0x25, 0xF8};

/**
  * @brief  Initializes FG module
  * @retval None
*/
void gauge_init(gauge_t *gauge, I2C_HandleTypeDef *hi2c)
{
  uint16_t flags;

  /* Initialize gauge structure */
  gauge_hal_init(gauge, hi2c);
  
  /* Unseal device */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_UNSEAL);
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_UNSEAL);     // Send unseal key twice

  /* Enter config update mode */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SET_CFGUPDATE);
  do
  {
    gauge_hal_read_reg16 (gauge, FG_FLAGS, &flags);
  }
  while (!(flags & 0x0010));        // Wait for CFGUPMODE bit to be set
  
  /* Update parameters */
  gauge_hal_ram_update(gauge, 82, 10, gauge_param_design_capacity, 2);
  gauge_hal_ram_update(gauge, 82, 12, gauge_param_design_energy, 2);
  gauge_hal_ram_update(gauge, 82, 16, gauge_param_terminate_voltage, 2);
  gauge_hal_ram_update(gauge, 82, 3,  gauge_param_reserve_capacity, 2);
  gauge_hal_ram_update(gauge, 82, 27, gauge_param_taper_rate, 2);
  gauge_hal_ram_update(gauge, 64, 0,  gauge_operation_config, 2);
  
  /* Exit config update mode (performing a soft reset)*/
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SOFT_RESET);
  do
  {
    gauge_hal_read_reg16 (gauge, FG_FLAGS, &flags);
  }
  while (flags & 0x0010);           // Wait for CFGUPMODE bit to be cleared
  
  /* Seal device */
  gauge_hal_write_reg16(gauge, FG_CNTL, FG_CNTL_SEALED);
}