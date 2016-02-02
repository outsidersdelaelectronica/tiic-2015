/*
 * ecg_data_circular_buffer.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */


/*
 * Esto va a ser un mierdon muy gordo
 * pero no hay cojones de hacerlo funcionar
 * con la multiplicacion
 */

#include "filters.h"

#define INTEGRATION_LENGTH 32

const int32_t band_pass_coef[30] = { 1024, -1857, 1024, -1881, 981, // Coeficients for first stage
									1024, -2036, 1024, -2013, 1007, // Coeficients for second stage
									1024, -1463, 1024, -1878, 928, 	// Coeficients for third stage
									1024, -2044, 1024, -1957, 963, 	// Coeficients for fourth stage
									1024, -1888, 1024, -1896, 1015, // Coeficients for fifth stage
									1024, -2034, 1024, -2029, 1021};// Coeficients for sixth stage

const int32_t diferentiator_coef[5] = { 2, 1, 0, -1, -2};

int32_t band_pass_filterino(int32_t value)
{
	static int32_t buffer_x_0[2] = {0,0},buffer_x_1[2] = {0,0},
			buffer_x_2[2] = {0,0},buffer_x_3[2] = {0,0},
			buffer_x_4[2] = {0,0},buffer_x_5[2] = {0,0};

	static int32_t buffer_y_0[2] = {0,0},buffer_y_1[2] = {0,0},
			buffer_y_2[2] = {0,0},buffer_y_3[2] = {0,0},
			buffer_y_4[2] = {0,0},buffer_y_5[2] = {0,0};

	int32_t x_n = value, y_n = 0;

    y_n = (multiplication(band_pass_coef[0], x_n) + multiplication(band_pass_coef[1], buffer_x_0[0])
			+ multiplication(band_pass_coef[2], buffer_x_0[1]) - multiplication(band_pass_coef[3], buffer_y_0[0])
			- multiplication(band_pass_coef[4], buffer_y_0[1]))>>10;

    buffer_y_0[1] = buffer_y_0[0];
    buffer_y_0[0] = y_n;
    buffer_x_0[1] = buffer_x_0[0];
    buffer_x_0[0] = x_n;
    x_n = y_n;

    y_n = (multiplication(band_pass_coef[5], x_n) + multiplication(band_pass_coef[6], buffer_x_1[0])
    		+ multiplication(band_pass_coef[7], buffer_x_1[1]) - multiplication(band_pass_coef[8], buffer_y_1[0])
			- multiplication(band_pass_coef[9], buffer_y_1[1]))>>10;
    buffer_y_1[1] = buffer_y_1[0];
    buffer_y_1[0] = y_n;
    buffer_x_1[1] = buffer_x_1[0];
    buffer_x_1[0] = x_n;

    x_n = y_n;

    y_n = (multiplication(band_pass_coef[10], x_n) + multiplication(band_pass_coef[11], buffer_x_2[0])
			+ multiplication(band_pass_coef[12], buffer_x_2[1]) - multiplication(band_pass_coef[13], buffer_y_2[0])
			- multiplication(band_pass_coef[14], buffer_y_2[1]))>>10;
    buffer_y_2[1] = buffer_y_2[0];
    buffer_y_2[0] = y_n;
    buffer_x_2[1] = buffer_x_2[0];
    buffer_x_2[0] = x_n;

    x_n = y_n;

    y_n = (multiplication(band_pass_coef[15], x_n) + multiplication(band_pass_coef[16], buffer_x_3[0])
    		+ multiplication(band_pass_coef[17], buffer_x_3[1]) - multiplication(band_pass_coef[18], buffer_y_3[0])
			- multiplication(band_pass_coef[19], buffer_y_3[1]))>>10;
    buffer_y_3[1] = buffer_y_3[0];
    buffer_y_3[0] = y_n;
    buffer_x_3[1] = buffer_x_3[0];
    buffer_x_3[0] = x_n;
    x_n = y_n;

    y_n = (multiplication(band_pass_coef[20], x_n) + multiplication(band_pass_coef[21], buffer_x_4[0])
    		+ multiplication(band_pass_coef[22], buffer_x_4[1])	- multiplication(band_pass_coef[23], buffer_y_4[0])
			- multiplication(band_pass_coef[24], buffer_y_4[1]))>>10;
    buffer_y_4[1] = buffer_y_4[0];
    buffer_y_4[0] = y_n;
    buffer_x_4[1] = buffer_x_4[0];
    buffer_x_4[0] = x_n;
    x_n = y_n;

    y_n = (multiplication(band_pass_coef[25], x_n) + multiplication(band_pass_coef[26], buffer_x_5[0])
    		+ multiplication(band_pass_coef[27], buffer_x_5[1]) - multiplication(band_pass_coef[28], buffer_y_5[0])
			- multiplication(band_pass_coef[29], buffer_y_5[1]))>>10;
    buffer_y_5[1] = buffer_y_5[0];
    buffer_y_5[0] = y_n;
    buffer_x_5[1] = buffer_x_5[0];
    buffer_x_5[0] = x_n;

	return y_n;
}

int32_t diferentiator_3000(int32_t value)
{
	static int32_t buffer_x[4] = {0,0,0,0};
	int32_t x_n = value, y_n = 0;
	uint8_t i;

    y_n = (multiplication(diferentiator_coef[0], x_n) + multiplication(diferentiator_coef[1], buffer_x[0])
			+ multiplication(diferentiator_coef[2], buffer_x[1]) + multiplication(diferentiator_coef[3], buffer_x[2])
			+ multiplication(diferentiator_coef[4], buffer_x[3]))>>3;

    for ( i = 3; i > 0; i--)
    {
		buffer_x[i] = buffer_x[i-1];
    }
    buffer_x[0] = x_n;

	return y_n;
}

int32_t integrator_3000(int32_t value)
{
	static int32_t buffer_x[INTEGRATION_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
													0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int32_t x_n = value, y_n = 0;
	uint8_t i;

    for ( i = INTEGRATION_LENGTH - 1; i > 0; i--)
    {
    	y_n += buffer_x[i];
		buffer_x[i] = buffer_x[i-1];
    }

    y_n += buffer_x[0];
    buffer_x[0] = x_n;

	return y_n;
}

int32_t filter_sample(int32_t value)
{
	int32_t filtered_value = value;


	filtered_value = band_pass_filterino(filtered_value);
	filtered_value = diferentiator_3000(filtered_value);

	filtered_value = (filtered_value)*(filtered_value);

	filtered_value = integrator_3000(filtered_value);

	return filtered_value;											//Return true
}
