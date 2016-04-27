/*
 * afe.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "afe.h"
//#include "ADS1291_constants.h"
#include "gpio.h"
#include "spi.h"

void afe_init()
{
  //AFE reset and stop continuous data conversion mode	
  //Power-On-Reset: hold reset line high for 1 second
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_SET);
  HAL_Delay(1000);	                              
  //Reset pulse: (>= 18 t_clk) => (>= 10 us)
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_RESET);
  HAL_Delay(1);		//At least 10 useconds ( need to implement us_delay() )
  HAL_GPIO_WritePin(GPIOA,AFE_RESET_Pin,GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOA,AFE_CS_Pin,GPIO_PIN_RESET); //Enable CS        

//  afe_serial_send(SDATAC);	//Stop continuous data conversion mode (activated by default)
//
//  //Write config commands to AFE
//  afe_serial_write_register(REG_CONFIG1, 0x01);	  //Set data rate to 500 SPS
//  afe_serial_write_register(REG_CONFIG2, 0xE3);	  //Enable voltage reference
//                                                  //Enable lead-off comparators
//                                                  //Enable test signal
//                                                  //Test signal @ 1 Hz, +-1 mV
//  afe_serial_write_register(REG_CH2SET, 0x81);	  //Channel 2 power down
//                                                  //Channel 2 input shorted
//  afe_serial_write_register(REG_LOFF_STAT, 0x40); //Clock divider selection: Clock input set to 2.048 MHz
//  afe_serial_write_register(REG_RESP2, 0x87);	  //Enable calibration
//  afe_serial_write_register(REG_CH1SET, 0x01);	  // |
//  afe_serial_send(OFFSETCAL);			  // | Calibrate
//  afe_serial_write_register(REG_CH1SET, 0x10);	  // | ( Set PGA gain to 1)
//  afe_serial_write_register(REG_RESP2, 0x07);	  //Disable calibration
//
//
//  //Start capturing data
//  P5OUT |= BIT7;		//Start conversions
//  afe_serial_send(RDATAC);	//Enable continuous output of conversion data
//                                //In this mode, a SDATAC command must be issued
//                                //before other commands can be sent to the device
//
//  P4OUT |= BIT4;		//Disable CS
//
//  P1IE |= BIT2;			//Enable DRDY interrupt
}
