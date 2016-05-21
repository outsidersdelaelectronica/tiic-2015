/*
 * gauge.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "gauge.h"

/**
  * @brief  Initializes LCD module
  * @retval None
*/
void fg_init()
{
  uint16_t flags;

  /* Data memory parameter update */
  fg_write_reg16(FG_CNTL, FG_UNSEAL);
  fg_write_reg16(FG_CNTL, FG_UNSEAL);     // Send unseal key twice

  fg_write_reg16(FG_CNTL, FG_CNTL_SET_CFGUPDATE);
  flags = fg_read_reg16(FG_FLAGS);





}


uint8_t fg_read_reg8(uint8_t reg)
{
  uint8_t param_buffer;

  HAL_I2C_Master_Transmit(&(hi2c1), FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(&(hi2c1), FG_I2C_ADDRESS, &param_buffer, 2, 100);

  /* Recieve one byte of data from a register position */
  return param_buffer;
}

void fg_write_reg8(uint8_t reg, uint8_t param)
{
  uint8_t param_buffer[2];

  param_buffer[0] = reg;
  param_buffer[1] = param;

  /* Send register position, and one byte of data */
  HAL_I2C_Master_Transmit(&(hi2c1), FG_I2C_ADDRESS, param_buffer, 2, 100);
}

uint16_t fg_read_reg16(uint8_t reg)
{
  uint8_t param_buffer[2];

  HAL_I2C_Master_Transmit(&(hi2c1), FG_I2C_ADDRESS, &reg, 1, 100);
  HAL_I2C_Master_Receive(&(hi2c1), FG_I2C_ADDRESS, param_buffer, 2, 100);

  /* Recieve two bytes of data from a register position */
  return (((uint16_t) param_buffer[1]) << 8) | ((uint16_t) param_buffer[0]);
}

void fg_write_reg16(uint8_t reg, uint16_t param)
{
  uint8_t param_buffer[3];

  param_buffer[0] = reg;
  param_buffer[1] = (uint8_t)  (param & 0x00FF);         // param LSB
  param_buffer[2] = (uint8_t) ((param & 0xFF00) >> 8);   // param MSB

  /* Send register position, and two bytes of data */
  HAL_I2C_Master_Transmit(&(hi2c1), FG_I2C_ADDRESS, param_buffer, 3, 100);
}