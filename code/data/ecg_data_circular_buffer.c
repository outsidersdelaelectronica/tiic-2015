/*
 * ecg_data_circular_buffer.c
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#include "ecg_data_circular_buffer.h"

#define INTEGRATION_LENGTH 32

void ecg_data_circular_buffer_setup(ecg_data_circular_buffer_t* buf)
{
	int i;

	buf->index = 0;
	buf->full_read_index = 0;
	buf->buffer_size = BUFFER_SIZE;

	// Clear all buffer on initialization is time consuming
	// and probably unnecesary, a test without clearing
	// should be done
	for(i = 0; i < buf->buffer_size; i++)
	{
		ecg_data_clear(&(buf->ecg_buffer[i]));			//Fill with zeros every ecgData value in ecgBuffer
	}
}

ecg_data_t band_pass_filterino(ecg_data_t* value)
{
	static int32_t buffer_x_0[2] = {0,0},buffer_x_1[2] = {0,0},
			buffer_x_2[2] = {0,0},buffer_x_3[2] = {0,0},
			buffer_x_4[2] = {0,0},buffer_x_5[2] = {0,0};

	static int32_t buffer_y_0[2] = {0,0},buffer_y_1[2] = {0,0},
			buffer_y_2[2] = {0,0},buffer_y_3[2] = {0,0},
			buffer_y_4[2] = {0,0},buffer_y_5[2] = {0,0};
	int32_t x_n = value->data, y_n;
	ecg_data_t aux;
	const int32_t coef[30] = { 	1024, -1857, 1024, -1881, 981, 	// Coeficients for first stage
							1024, -2036, 1024, -2013, 1007, // Coeficients for second stage
							1024, -1463, 1024, -1878, 928, 	// Coeficients for third stage
							1024, -2044, 1024, -1957, 963, 	// Coeficients for fourth stage
							1024, -1888, 1024, -1896, 1015, // Coeficients for fifth stage
							1024, -2034, 1024, -2029, 1021};// Coeficients for sixth stage

    y_n = (coef[0] * x_n + coef[1] * buffer_x_0[0] + coef[2] * buffer_x_0[1]
				- coef[3] * buffer_y_0[0] - coef[4] * buffer_y_0[1])>>10;
    buffer_y_0[1] = buffer_y_0[0];
    buffer_y_0[0] = y_n;
    buffer_x_0[1] = buffer_x_0[0];
    buffer_x_0[0] = x_n;
    x_n = y_n;

    y_n = (coef[5] * x_n + coef[6] * buffer_x_1[0] + coef[7] * buffer_x_1[1]
				- coef[8] * buffer_y_1[0] - coef[9] * buffer_y_1[1])>>10;
    buffer_y_1[1] = buffer_y_1[0];
    buffer_y_1[0] = y_n;
    buffer_x_1[1] = buffer_x_1[0];
    buffer_x_1[0] = x_n;

    x_n = y_n;

    y_n = (coef[10] * x_n + coef[11] * buffer_x_2[0] + coef[12] * buffer_x_2[1]
				- coef[13] * buffer_y_2[0] - coef[14] * buffer_y_2[1])>>10;
    buffer_y_2[1] = buffer_y_2[0];
    buffer_y_2[0] = y_n;
    buffer_x_2[1] = buffer_x_2[0];
    buffer_x_2[0] = x_n;

    x_n = y_n;

    y_n = (coef[15] * x_n + coef[16] * buffer_x_3[0] + coef[17] * buffer_x_3[1]
				- coef[18] * buffer_y_3[0] - coef[19] * buffer_y_3[1])>>10;
    buffer_y_3[1] = buffer_y_3[0];
    buffer_y_3[0] = y_n;
    buffer_x_3[1] = buffer_x_3[0];
    buffer_x_3[0] = x_n;
    x_n = y_n;

    y_n = (coef[20] * x_n + coef[21] * buffer_x_4[0] + coef[22] * buffer_x_4[1]
				- coef[23] * buffer_y_4[0] - coef[24] * buffer_y_4[1])>>10;
    buffer_y_4[1] = buffer_y_4[0];
    buffer_y_4[0] = y_n;
    buffer_x_4[1] = buffer_x_4[0];
    buffer_x_4[0] = x_n;
    x_n = y_n;

    y_n = (coef[25] * x_n + coef[26] * buffer_x_5[0] + coef[27] * buffer_x_5[1]
				- coef[28] * buffer_y_5[0] - coef[29] * buffer_y_5[1])>>10;
    buffer_y_5[1] = buffer_y_5[0];
    buffer_y_5[0] = y_n;
    buffer_x_5[1] = buffer_x_5[0];
    buffer_x_5[0] = x_n;

    ecg_data_clear(&aux);
    aux.data = (int)(y_n / 90);
	return aux;
}
ecg_data_t diferentiator_3000(ecg_data_t* value)
{
	static int32_t buffer_x[4] = {0,0,0,0};
	int32_t x_n = value->data, y_n;
	ecg_data_t aux;
	const int32_t coef[5] = { 	2, 1, 0, -1, -2};
	int8_t i;
    y_n = (coef[0] * x_n + coef[1] * buffer_x[0] + coef[2] * buffer_x[1]
				- coef[3] * buffer_x[2] - coef[4] * buffer_x[3])>>3;

    for ( i = 3; i > 0; i--)
    {
		buffer_x[i] = buffer_x[i-1];
    }
    buffer_x[0] = x_n;

    ecg_data_clear(&aux);
    aux.data = (int)(y_n);
	return aux;

}
ecg_data_t integrator_3000(ecg_data_t* value)
{
	static int32_t buffer_x[INTEGRATION_LENGTH] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int32_t y_n = value->data;
	ecg_data_t aux;
	int8_t i;
    for ( i = INTEGRATION_LENGTH - 1; i > 0; i--)
    {
    	y_n += buffer_x[i];
		buffer_x[i] = buffer_x[i-1];
    }
    y_n += buffer_x[0];
    buffer_x[0] = (value->data);
    ecg_data_clear(&aux);
    aux.data = (int)(y_n/INTEGRATION_LENGTH);
	return aux;
}
//int	ecg_data_circular_buffer_write(ecg_data_circular_buffer_t* buf, uint8_t msbyte, uint8_t midbyte, uint8_t lsbyte)
int		ecg_data_circular_buffer_write(ecg_data_circular_buffer_t* buf, ecg_data_t* value)
{
	ecg_data_t filtered_value;
	int32_t aux;
	buf->index++;
	if (buf->index == buf->buffer_size)			//if reached the end, start again
	{
		buf->index = 0;
	}

//	ecg_data_write(&value, msbyte, midbyte , lsbyte);

	filtered_value = band_pass_filterino(value);
	filtered_value = diferentiator_3000(&filtered_value);

	aux = (((int32_t)filtered_value.data)*((int32_t)filtered_value.data))>>8;
	filtered_value.data = (int)aux;

	filtered_value = integrator_3000(&filtered_value);

	ecg_data_copy(&filtered_value,&(buf->ecg_buffer[buf->index]));
//	ecg_data_write(&(buf->ecg_buffer[buf->index]), msbyte, midbyte , lsbyte);

	return 1;											//Return true
}

int ecg_data_circular_buffer_read_last(ecg_data_circular_buffer_t* buf, ecg_data_t* value)
{
	ecg_data_clear(value);								//Fill with zeros

	*value = buf->ecg_buffer[buf->index];				//Returns the last writen data
	return 1;											//Return true
}

int ecg_data_circular_buffer_read_full(ecg_data_circular_buffer_t* buf, ecg_data_t* value)
{
	ecg_data_clear(value);								//Fill with zeros

	buf->full_read_index++;

	if (buf->full_read_index == buf->buffer_size)		//If reading out of the memory, return 0
	{
		buf->full_read_index = 0;
		return 0;
	}

	*value = buf->ecg_buffer[buf->full_read_index];		//Returns the last writen data
	return 1;											//Return true
}
