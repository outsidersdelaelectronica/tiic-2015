/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "filters.h"

#define INTEGRATION_LENGTH 16
#define BP_ORDER 579
//#define H_ORDER 311
#define L_ORDER 50
#define DIF_ORDER 32

//// FIR BPF 1-40 hz (500 SPS / ORDER 579)
const int32_t band_pass_coef[BP_ORDER] = {
  -5, 13, 1, -3, -4, -5, -5, -5, -5, -5, -4, -3, -2, -1, 0, 0, 0, 0, -1, -1, -2, 
  -2, -2, -1, -1, 0, 0, 0, 0, 0, -1, -1, -2, -2, -2, -1, -1, 0, 0, 0, 0, 0, -1, 
  -1, -2, -2, -2, -1, -1, -1, 0, 0, 0, 0, -1, -1, -2, -2, -2, -2, -1, -1, 0, 0, 
  0, 0, -1, -1, -2, -2, -2, -2, -2, -1, -1, 0, 0, 0, -1, -1, -2, -2, -2, -2, -2,
  -1, -1, 0, 0, 0, 0, -1, -2, -2, -3, -3, -2, -2, -1, 0, 0, 0, 0, -1, -2, -2, 
  -3, -3, -3, -2, -1, -1, 0, 0, 0, -1, -1, -2, -3, -3, -3, -2, -2, -1, 0, 0, 0, 
  0, -1, -2, -3, -3, -3, -3, -2, -1, 0, 0, 0, 0, -1, -2, -3, -3, -4, -3, -3, -2,
  -1, 0, 0, 0, -1, -2, -3, -4, -4, -4, -3, -2, -1, 0, 0, 0, 0, -1, -3, -4, -4, 
  -4, -4, -3, -1, 0, 1, 1, 0, -1, -2, -4, -4, -5, -4, -3, -2, -1, 1, 1, 1, 0, 
  -2, -3, -5, -5, -5, -4, -3, -1, 0, 1, 1, 0, -1, -3, -5, -6, -6, -5, -3, -2, 0,
  2, 2, 1, 0, -2, -5, -6, -7, -6, -5, -2, 0, 2, 3, 2, 1, -2, -4, -6, -8, -7, -6,
  -3, 0, 2, 4, 4, 2, 0, -4, -7, -9, -9, -8, -5, -1, 2, 5, 6, 5, 2, -3, -7, -10,
  -12, -11, -8, -3, 2, 7, 9, 9, 5, 0, -7, -13, -17, -17, -13, -6, 3, 11, 17, 19,
  15, 6, -7, -21, -33, -38, -35, -21, 3, 34, 71, 106, 136, 156, 163, 156, 136, 
  106, 71, 34, 3, -21, -35, -38, -33, -21, -7, 6, 15, 19, 17, 11, 3, -6, -13, 
  -17, -17, -13, -7, 0, 5, 9, 9, 7, 2, -3, -8, -11, -12, -10, -7, -3, 2, 5, 6, 
  5, 2, -1, -5, -8, -9, -9, -7, -4, 0, 2, 4, 4, 2, 0, -3, -6, -7, -8, -6, -4, 
  -2, 1, 2, 3, 2, 0, -2, -5, -6, -7, -6, -5, -2, 0, 1, 2, 2, 0, -2, -3, -5, -6, 
  -6, -5, -3, -1, 0, 1, 1, 0, -1, -3, -4, -5, -5, -5, -3, -2, 0, 1, 1, 1, -1,  
  -2, -3, -4, -5, -4, -4, -2, -1, 0, 1, 1, 0, -1, -3, -4, -4, -4, -4, -3, -1, 0,
  0, 0, 0, -1, -2, -3, -4, -4, -4, -3, -2, -1, 0, 0, 0, -1, -2, -3, -3, -4, -3, 
  -3, -2, -1, 0, 0, 0, 0, -1, -2, -3, -3, -3, -3, -2, -1, 0, 0, 0, 0, -1, -2, 
  -2, -3, -3, -3, -2, -1, -1, 0, 0, 0, -1, -1, -2, -3, -3, -3, -2, -2, -1, 0, 0,
  0, 0, -1, -2, -2, -3, -3, -2, -2, -1, 0, 0, 0, 0, -1, -1, -2, -2, -2, -2, -2, 
  -1, -1, 0, 0, 0, -1, -1, -2, -2, -2, -2, -2, -1, -1, 0, 0, 0, 0, -1, -1, -2,
  -2, -2, -2, -1, -1, 0, 0, 0, 0, -1, -1, -1, -2, -2, -2, -1, -1, 0, 0, 0, 0, 0,
  -1, -1, -2, -2, -2, -1, -1, 0, 0, 0, 0, 0, -1, -1, -2, -2, -2, -1, -1, 0, 0, 
  0, 0, -1, -2, -3, -4, -5, -5, -5, -5, -5, -4, -3, 1, 13, -5};

// FIR DIF (500 SPS)
const int32_t diferentiator_coef[DIF_ORDER] = {
  -4, 6, -3, 3, -3, 3, -4, 5, -6, 8, -11, 16, -27, 52, -145, 1304, -1304, 145,
  -52, 27, -16, 11, -8, 6, -5, 4, -3, 3, -3, 3, -6, 4};
//const int32_t diferentiator_coef[DIF_ORDER] = {-4, 6, -3, 3, -3, 3, -4, 5, -6, 
//                                              8, -11, 16, -27, 52, -145, 1304 };


int32_t band_pass_filterino(int32_t value)
{
  static int32_t bp_buffer[(BP_ORDER - 1)] = {0}, aux = 0;
  int i;
  int32_t y_n = 0;

  for(i = (BP_ORDER - 2) ; i > 0; i-- ){
    aux = (band_pass_coef[i + 1] * bp_buffer[i]);
    if ((aux < -1024)||(aux > 0)){
      y_n += (aux >> 10);
    }
    bp_buffer[i] = bp_buffer[i-1];
  }
  aux = (band_pass_coef[1] * bp_buffer[0]);
  if ((aux < -1024)||(aux > 0)){
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

//int32_t diferentiator_3000(int32_t value)
//{
//	static int32_t dif_buffer[(DIF_ORDER - 1)];
//	int i;
//	int32_t y_n = 0;
//
//	for(i = (DIF_ORDER - 2); i > 0; i-- ){
//        y_n += ((diferentiator_coef[i + 1] * dif_buffer[i]) >> 10);
//        dif_buffer[i] = dif_buffer[i-1];
//	}
//
//    y_n += ((diferentiator_coef[1] * dif_buffer[0]) >> 10);
//    dif_buffer[0] = value;
//
//    y_n += ((diferentiator_coef[0] * value) >> 10);
//
//    return y_n;
//}
// FOLDED VERSION
int32_t diferentiator_3000(int32_t value)
{
  static int32_t dif_buffer[(DIF_ORDER - 1)];
  int i;
  int32_t y_n = 0;

  for(i = (DIF_ORDER - 2); i > 0; i-- ){
    y_n += ((diferentiator_coef[i + 1] * dif_buffer[i]) >> 10);
    dif_buffer[i] = dif_buffer[i-1];
  }

  y_n += ((diferentiator_coef[1] * dif_buffer[0]) >> 10);
  dif_buffer[0] = value;

  y_n += ((diferentiator_coef[0] * value) >> 10);

  return y_n;
}

//int32_t integrator_3000(int32_t value)
//{
//	static int32_t buffer_x[INTEGRATION_LENGTH];
//	int32_t x_n = value, y_n = 0;
//	uint8_t i;
//
//    for ( i = INTEGRATION_LENGTH - 1; i > 0; i--)
//    {
//    	y_n += (buffer_x[i] / INTEGRATION_LENGTH);
//		buffer_x[i] = buffer_x[i-1];
//    }
//
//    y_n += (buffer_x[0] / INTEGRATION_LENGTH);
//    buffer_x[0] = x_n;
//
//	return y_n;
//}

int32_t filter_sample(int32_t value)
{
	int32_t bp_filtered, dif_filtered, squared, integrated;


	bp_filtered = band_pass_filterino(value);
	dif_filtered = diferentiator_3000(bp_filtered);
//	squared = (bp_filtered >> 2) * (bp_filtered >> 2);
//	integrated = integrator_3000(squared);
//
//	return integrated;
        
        return dif_filtered;
        
}
