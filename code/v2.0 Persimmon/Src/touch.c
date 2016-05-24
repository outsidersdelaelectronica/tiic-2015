/*
 * lcd.c
 *
 *  Created on: 24/05/2016
 *      Author: Smau
 */
#include "touch.h"

/**
  * @brief  Initializes touch module
  * @retval None
*/
void touch_init()
{
}

uint32_t touch_serial_transaction(uint8_t cntl_byte)
{
  uint8_t data_buffer[2];
  uint32_t data;
  
  /* Send control byte */
  HAL_SPI_Transmit(&hspi2, &cntl_byte, 1, 100);
  
  //TO-DO: Wait for busy pin
  
  /* Recieve data */
  HAL_SPI_Receive(&hspi2, data_buffer, 2, 100);

  data = (((uint32_t) data_buffer[0]) << 5) |
         (((uint32_t) data_buffer[1]) >> 3);
  
  return data;
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
  
  //TO-DO: Calculate pressure from z1, z2 and x
}
