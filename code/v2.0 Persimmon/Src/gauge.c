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
  

}

void fg_read(uint8_t *data)
{
  HAL_I2C_Master_Receive(&(hi2c1), FG_I2C_ADDRESS, data, 1, 100);
}
  
void fg_write(uint8_t *data)
{
  HAL_I2C_Master_Transmit(&(hi2c1), FG_I2C_ADDRESS, data, 1, 100);
}

