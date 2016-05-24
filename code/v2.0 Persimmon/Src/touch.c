/*
 * lcd.c
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#include "touch.h"

uint32_t touch_serial_transaction(uint8_t cntl_byte)
{
  uint8_t data_buffer[2];
  uint32_t data;
  
  /* Enable CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_RESET);  // Enable CS
  
  /* Send control byte */
  HAL_SPI_Transmit(&hspi2, &cntl_byte, 1, 100);
  
  /* Recieve data */
  HAL_SPI_Receive(&hspi2, data_buffer, 2, 100);

  /* Disable CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_SET);    // Disable CS
  
  data = (((uint32_t) data_buffer[0]) << 5) |
         (((uint32_t) data_buffer[1]) >> 3);
  
  return data;
}

/**
  * @brief  Initializes touch module
  * @retval None
*/
void touch_init()
{
  uint8_t cntl_byte = 0;
  
  /* Prepare CS */
  HAL_GPIO_WritePin(GPIOB, TP_CS_Pin, GPIO_PIN_SET);    // Disable CS     

  /* First control byte */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_X |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  touch_serial_transaction(cntl_byte);
}

void touch_get_position(touch_pos_t *position)
{
  uint8_t cntl_byte = 0;
  uint32_t z1, z2;

  /* Get X param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_X |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  position->x_pos = touch_serial_transaction(cntl_byte);
  
  /* Get Y param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Y |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  position->y_pos = touch_serial_transaction(cntl_byte);
  
  /* Get Z1 param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Z1 |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  z1 = touch_serial_transaction(cntl_byte);
  
  /* Get Z2 param */
  cntl_byte = TOUCH_CNTL_S | TOUCH_CNTL_Z2 |
              TOUCH_CNTL_MODE_12_BIT |
              TOUCH_CNTL_DFR | TOUCH_CNTL_PD;
  z2 = touch_serial_transaction(cntl_byte);
  
  /* Calculate and store pressure level
   * http://www.unclelarry.com/?p=101
   */
  position->pressure = (position->x_pos * (((z2 << 12) / z1) - 4096)) >> 12;
}
