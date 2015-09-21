/*
 * ecgData.h
 *
 *  Created on: 10/9/2015
 *      Author: Smau
 */

#ifndef ECGDATA_H_
#define ECGDATA_H_

#include <msp430.h>
#include <stdint.h>

/*
 * 24 bit AFE data structure (MSB on signal[0])
 */
typedef struct ecgData_t
{
	int32_t data;
}
volatile ecgData_t;

void	ecgData_clear(ecgData_t* data);
void	ecgData_write(ecgData_t* data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte);
void	ecgData_copy(ecgData_t* origin, ecgData_t* destination);

#endif /* ECGDATA_H_ */
