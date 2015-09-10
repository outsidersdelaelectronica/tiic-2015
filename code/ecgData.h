/*
 * ecgData.h
 *
 *  Created on: 10/9/2015
 *      Author: Smau
 */

#ifndef ECGDATA_H_
#define ECGDATA_H_

#include <stdint.h>

/*
 * 24 bit AFE data structure (MSB on signal[0])
 */
typedef struct ecgData
{
	uint8_t signal[3];
}
volatile ecgData;

void	ecgData_setup(ecgData* data);
void	ecgData_write(ecgData* data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte);

#endif /* ECGDATA_H_ */
