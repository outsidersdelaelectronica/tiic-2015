/*
 * afe.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */
#include "ADS1291_constants.h"
#include "afe.h"
#include "gpio.h"
#include "spi.h"

/**
  * @brief  Writes a single command to one register.
  * @param  reg: Address of the afe register to write.
  * @param  command: command to be send.
  * @retval None
*/
void afe_serial_write_register(uint8_t reg, uint8_t command){
  // This is specific for one command in one register since the AFE,
  // allows to write multiple commands to consecutive registers in 
  // one spi transmision.
  uint8_t data[3] = { reg | AFE_WREG, 0x00 , command};
  
  HAL_SPI_Transmit(&hspi1, data, 3, 100);
}
void afe_init()
{
  uint8_t data[1];
  //AFE reset and stop continuous data conversion mode	
  //Power-On-Reset: hold reset line high for 1 second
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_SET);
  HAL_Delay(1000);	                              
  //Reset pulse: (>= 18 t_clk) => (>= 10 us)
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_RESET);
  HAL_Delay(1);		//At least 10 useconds ( need to implement us_delay() )
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOA,AFE_CS_Pin,GPIO_PIN_RESET); //Enable CS        
  data[0] = AFE_SDATAC;
  HAL_SPI_Transmit(&hspi1, data, 1, 100);
  //Write config commands to AFE
  
  afe_serial_write_register(AFE_REG_CONFIG1, 0x01); //Set data rate to 500 SPS
  afe_serial_write_register(AFE_REG_CONFIG2, 0xE3); //Enable voltage reference
                                                    //Enable lead-off comparators
                                                    //Enable test signal
                                                    //Test signal @ 1 Hz, +-1 mV
  afe_serial_write_register(AFE_REG_CH2SET, 0x81);  //Channel 2 power down
                                                    //Channel 2 input shorted
  afe_serial_write_register(AFE_REG_LOFF_STAT, 0x40); //Clock divider selection: 
                                                      //Clock input set to 2.048 MHz
  afe_serial_write_register(AFE_REG_RESP2, 0x87);   //Enable calibration
  afe_serial_write_register(AFE_REG_CH1SET, 0x01);  // |
  data[0] = AFE_OFFSETCAL;                          // | Calibrate
  HAL_SPI_Transmit(&hspi1, data, 1, 100);           // |
  afe_serial_write_register(AFE_REG_CH1SET, 0x10);  // | ( Set PGA gain to 1)
  afe_serial_write_register(AFE_REG_RESP2, 0x07);   //Disable calibration

  //Start capturing data		
  HAL_GPIO_WritePin(GPIOA,AFE_START_Pin,GPIO_PIN_SET); //Start conversions
  data[0] = AFE_RDATAC;                        
  HAL_SPI_Transmit(&hspi1, data, 1, 100);     //Enable continuous output of conversion data
                                              //In this mode, a SDATAC command must be issued
                                              //before other commands can be sent to the device

  HAL_GPIO_WritePin(GPIOA,AFE_CS_Pin,GPIO_PIN_SET); //Disable CS     
}
