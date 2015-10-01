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
	uint32_t data_tmp;

	//Make 32 bit data
		data_tmp = ((uint32_t) msbyte << 16) | ((uint32_t) midbyte << 8) | (uint32_t) lsbyte;

		//Change sign bits
		if((int8_t) msbyte < 0)
		{
			data_tmp |= 0xFF000000;
		}

	//Store data
	ecg_data->data = (int32_t) data_tmp;
}

void ecg_data_copy(ecg_data_t* origin, ecg_data_t* destination)
{
	destination->data = origin->data;
}
