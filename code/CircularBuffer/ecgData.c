/*
 * ecgData.c
 *
 *  Created on: 10/9/2015
 *      Author: Smau
 */

#include "ecgData.h"

void ecgData_clear(ecgData_t* ecgData)
{
	ecgData->data = 0x00000000;
}

void ecgData_write(ecgData_t* ecgData, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
{
	//Store 24 bits
		ecgData->data = ((uint32_t) msbyte << 16) | ((uint32_t) midbyte << 8) | (uint32_t) lsbyte;
	//Change sign bits
		if((int8_t) msbyte < 0)
		{
			ecgData->data |= 0xFF000000;
		}
}

void ecgData_copy(ecgData_t* origin, ecgData_t* destination)
{
	destination->data = origin->data;
}
