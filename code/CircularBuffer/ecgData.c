/*
 * ecgData.c
 *
 *  Created on: 10/9/2015
 *      Author: Smau
 */

#include "ecgData.h"

void ecgData_setup(ecgData* data)
{
	int i;
	for (i = 0; i < 3; i++) {
		data->signal[i] = 0x00;
	}
}

void ecgData_write(ecgData* data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
{
	data->signal[0] = msbyte;
	data->signal[1] = midbyte;
	data->signal[2] = lsbyte;
}
