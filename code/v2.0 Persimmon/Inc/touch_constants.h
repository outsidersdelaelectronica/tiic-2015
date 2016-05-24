/*
 * touch_constants.h
 *
 * XPT2046 touch controller definitions
 *
 *  Created on: 11/05/2016
 *      Author: slopez
 */

#ifndef TOUCH_TOUCH_CONSTANTS_H_
#define TOUCH_TOUCH_CONSTANTS_H_

/* Control byte bit definitions */
#define TOUCH_CNTL_S                    0x80

#define TOUCH_CNTL_X                    0x50
#define TOUCH_CNTL_Z2                   0x40
#define TOUCH_CNTL_Z1                   0x30
#define TOUCH_CNTL_Y                    0x10

#define TOUCH_CNTL_MODE_8_BIT           0x08
#define TOUCH_CNTL_MODE_12_BIT          0x00

#define TOUCH_CNTL_SER                  0x04
#define TOUCH_CNTL_DFR                  0x00

#define TOUCH_CNTL_REF_ON_ADC_ON        0x03
#define TOUCH_CNTL_REF_ON_ADC_OFF       0x02
#define TOUCH_CNTL_REF_OFF_ADC_ON       0x01
#define TOUCH_CNTL_PD                   0x00

#endif /* TOUCH_TOUCH_CONSTANTS_H_ */