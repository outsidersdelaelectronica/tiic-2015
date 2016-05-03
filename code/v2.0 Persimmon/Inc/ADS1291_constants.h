/*
 * ADS1291_constants.h
 *
 * ADS1291 Medical Analog Front End constant definitions
 *
 *  Created on: 28/7/2015
 *      Author: slopez
 */

#ifndef AFE_AFE_SRC_ADS1291_CONSTANTS_H_
#define AFE_AFE_SRC_ADS1291_CONSTANTS_H_

///// SPI commands
/*
 * System commands
 */
#define AFE_WAKEUP		0x02
#define AFE_STANDBY		0x04
#define AFE_RESET		0x06
#define AFE_START		0x08
#define AFE_STOP		0x0A
#define AFE_OFFSETCAL		0x1A

/*
 * Data read commands
 */
#define AFE_RDATAC		0x10
#define AFE_SDATAC		0x11
#define AFE_RDATA		0x12

/*
 * Register read commands
 */
#define AFE_RREG		0x20
#define AFE_WREG		0x40


///// Register address definition
/*
 * Device settings
 */
#define AFE_REG_ID		0x00
/*
 * Global settings across channels
 */
#define AFE_REG_CONFIG1		0x01
#define AFE_REG_CONFIG2		0x02
#define AFE_REG_LOFF		0x03
/*
 * Channel-Specific Settings
 */
#define AFE_REG_CH1SET		0x04
#define AFE_REG_CH2SET		0x05
#define AFE_REG_RLD_SENS        0x06
#define AFE_REG_LOFF_SENS       0x07
#define AFE_REG_LOFF_STAT       0x08
/*
 * GPIO and Other Registers
 */
#define AFE_REG_RESP1		0x09
#define AFE_REG_RESP2		0x0A
#define AFE_REG_GPIO		0x0B


#endif /* AFE_AFE_SRC_ADS1291_CONSTANTS_H_ */