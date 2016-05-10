/*
 * lcd.c
 *
 *  Created on: 10/05/2016
 *      Author: Smau
 */
#include "lcd.h"

extern SRAM_HandleTypeDef hsram1;

#define LCD_REG        ((uint32_t *)(FSMC_BASE))
#define LCD_DATA       ((uint32_t *)(FSMC_BASE + 0x00020000U))  //See p.620 of STM32L162VD ref. manual

void lcd_read_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size)
{
  /* Send register */
  HAL_SRAM_Write_16b(&(hsram1), LCD_REG, &reg, 1);
  /* Send parameters */
  HAL_SRAM_Read_16b(&(hsram1), LCD_DATA, param_buffer, param_buffer_size);
}

void lcd_write_reg(uint16_t reg, uint16_t *param_buffer, uint32_t param_buffer_size)
{
  /* Send register */
  HAL_SRAM_Write_16b(&(hsram1), LCD_REG, &reg, 1);
  /* Send parameters */
  HAL_SRAM_Write_16b(&(hsram1), LCD_DATA, param_buffer, param_buffer_size);
}