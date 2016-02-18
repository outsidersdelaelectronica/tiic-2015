/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "filters.h"

#define INTEGRATION_LENGTH 16
#define BP_ORDER 253
//#define H_ORDER 311
#define L_ORDER 50
#define DIF_ORDER 32

// BPF 5-40 hz (500 SPS)
const int32_t band_pass_coef[BP_ORDER] = {-1, -1, -2, -3, -3, -3, -3, -3, -2, 0, 1, 2, 4, 4, 4, 4, 3, 2, 1, 0, 0, 0, 1, 2,
		3, 3, 4, 4, 3, 2, 1, 0, 0, 0, 1, 1, 3, 4, 4, 4, 3, 2, 1, 0, -1, -1, -1, 1, 2, 3, 4, 4, 3, 1, 0, -2, -3, -3, -3, -1,
		0, 2, 3, 3, 2, 0, -3, -5, -6, -7, -6, -4, -2, 0, 1, 1, 0, -3, -6, -9, -11, -12, -11, -9, -6, -2, 0, 0, -2, -5, -9,
		-14, -17, -18, -17, -14, -9, -4, 0, 1, -1, -6, -13, -20, -25, -27, -26, -20, -12, -3, 5, 8, 6, -2, -15, -30, -43,
		-50, -49, -37, -13, 19, 56, 94, 126, 147, 155, 147, 126, 94, 56, 19, -13, -37, -49, -50, -43, -30, -15, -2, 6, 8, 5,
		-3, -12, -20, -26, -27, -25, -20, -13, -6, -1, 1, 0, -4, -9, -14, -17, -18, -17, -14, -9, -5, -2, 0, 0, -2, -6, -9,
		-11, -12, -11, -9, -6, -3, 0, 1, 1, 0, -2, -4, -6, -7, -6, -5, -3, 0, 2, 3, 3, 2, 0, -1, -3, -3, -3, -2, 0, 1, 3, 4,
		4, 3, 2, 1, -1, -1, -1, 0, 1, 2, 3, 4, 4, 4, 3, 1, 1, 0, 0, 0, 1, 2, 3, 4, 4, 3, 3, 2, 1, 0, 0, 0, 1, 2, 3, 4, 4, 4,
		4, 2, 1, 0, -2, -3, -3, -3, -3, -3, -2, -1, -1
};

const int32_t diferentiator_coef[DIF_ORDER] = {-4, 6, -3, 3, -3, 3, -4, 5, -6, 8, -11, 16, -27, 52, -145, 1304, -1304, 145,
												-52, 27, -16, 11, -8, 6, -5, 4, -3, 3, -3, 3, -6, 4};

int32_t band_pass_filterino(int32_t value)
{
	static int32_t bp_buffer[(BP_ORDER - 1)] = {0}, aux = 0;
	int i;
	int32_t y_n = 0;

	for(i = (BP_ORDER - 2) ; i > 0; i-- ){
		aux = (band_pass_coef[i + 1] * bp_buffer[i]);
		if ((aux < -1024)||(aux > 0))
		{
			y_n += (aux >> 10);
		}
        bp_buffer[i] = bp_buffer[i-1];
	}

	aux = (band_pass_coef[1] * bp_buffer[0]);
	if ((aux < -1024)||(aux > 0))
	{
		y_n += (aux >> 10);
	}

    bp_buffer[0] = value;

	aux = (band_pass_coef[0] * value);
	if ((aux < -1024)||(aux > 0))
	{
		y_n += (aux >> 10);
	}

    return y_n;
}

//int32_t low_pass_filterino(int32_t value)
//{
//	static int32_t low_buffer[(L_ORDER - 1)];
//	int i;
//	int32_t y_n = 0;
//
//	for(i = (L_ORDER - 2); i > 0; i-- ){
//        y_n += (low_coef[i + 1]) * (low_buffer[i]);
//        low_buffer[i] = low_buffer[i-1];
//	}
//
//    y_n += low_coef[1] * low_buffer[0];
//    low_buffer[0] = value;
//
//    y_n = y_n + low_coef[0] * value;
//
//    y_n = (y_n >> 10);
//
//    return y_n;
//}

int32_t diferentiator_3000(int32_t value)
{
	static int32_t dif_buffer[(DIF_ORDER - 1)];
	int i;
	int32_t y_n = 0;

	for(i = (DIF_ORDER - 2); i > 0; i-- ){
        y_n += (diferentiator_coef[i + 1]) * (dif_buffer[i]);
        dif_buffer[i] = dif_buffer[i-1];
	}

    y_n += diferentiator_coef[1] * dif_buffer[0];
    dif_buffer[0] = value;

    y_n = y_n + diferentiator_coef[0] * value;

    y_n = (y_n >> 10);

    return y_n;
}
int32_t integrator_3000(int32_t value)
{
	static int32_t buffer_x[INTEGRATION_LENGTH];
	int32_t x_n = value, y_n = 0;
	uint8_t i;

    for ( i = INTEGRATION_LENGTH - 1; i > 0; i--)
    {
    	y_n += buffer_x[i];
		buffer_x[i] = buffer_x[i-1];
    }

    y_n += buffer_x[0];
    buffer_x[0] = x_n;

	return ( y_n >> 5);
}

int32_t filter_sample(int32_t value)
{
	int32_t filtered_value = value;


	filtered_value = band_pass_filterino(filtered_value);

//	filtered_value = diferentiator_3000(filtered_value);
//
//	filtered_value = (filtered_value >> 4) * (filtered_value >> 4);
//
//	filtered_value = integrator_3000(filtered_value);

	return filtered_value;											//Return true
}
