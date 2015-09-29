/*
 * ecg_data.c
 *
 *  Created on: 29/9/2015
 *      Author: slopez
 */

#include "ecg_data.h"

void ecg_data_clear(ecg_data_t* ecg_data)
{
	ecg_data->data = 0x00000000;
}

void ecg_data_write(ecg_data_t* ecg_data, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
{
	//Store 24 bits
		ecg_data->data = ((uint32_t) msbyte << 16) | ((uint32_t) midbyte << 8) | (uint32_t) lsbyte;
	//Change sign bits
		if((int8_t) msbyte < 0)
		{
			ecg_data->data |= 0xFF000000;
		}
}

void ecg_data_copy(ecg_data_t* origin, ecg_data_t* destination)
{
	destination->data = origin->data;
}
