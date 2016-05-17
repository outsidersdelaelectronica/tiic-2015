/*
 * filters.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "filters.h"

#define INTEGRATION_LENGTH 32
#define BP_ORDER 600
#define DIF_ORDER 32//16

//// FIR BPF 1-40 hz (1000 SPS / ORDER 600)
const int32_t band_pass_coef[BP_ORDER + 1] = {
  -2, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, 
  -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, 
  -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, 
  -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -1, 
  -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 
  -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -3, -2, -2, -2, -2, -2, -1, -1, -1, 
  -1, 0, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -2, 
  -2, -2, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -2, -2, -3, -3, -3, -3, -4, -4,
  -3, -3, -3, -3, -2, -2, -1, -1, 0, 0, 0, 0, 0, 0, -1, -1, -2, -2, -3, -3, -4,
  -4, -4, -4, -4, -4, -4, -3, -3, -2, -1, -1, 0, 0, 1, 1, 1, 0, 0, -1, -1, -2, 
  -3, -4, -4, -5, -5, -5, -5, -5, -4, -4, -3, -2, -1, -1, 0, 1, 1, 2, 2, 1, 1, 
  0, -1, -2, -3, -4, -5, -6, -6, -7, -7, -6, -6, -5, -4, -3, -1, 0, 1, 2, 3, 3,
  3, 3, 2, 1, 0, -1, -3, -5, -6, -8, -9, -10, -10, -10, -9, -7, -6, -4, -2, 1, 
  3, 5, 7, 8, 8, 8, 7, 5, 3, 0, -3, -7, -10, -14, -17, -19, -20, -21, -20, -17,
  -13, -8, -2, 6, 15, 24, 33, 43, 52, 60, 68, 74, 78, 81, 82, 81, 78, 74, 68, 
  60, 52, 43, 33, 24, 15, 6, -2, -8, -13, -17, -20, -21, -20, -19, -17, -14, 
  -10, -7, -3, 0, 3, 5, 7, 8, 8, 8, 7, 5, 3, 1, -2, -4, -6, -7, -9, -10, -10, 
  -10, -9, -8, -6, -5, -3, -1, 0, 1, 2, 3, 3, 3, 3, 2, 1, 0, -1, -3, -4, -5, 
  -6, -6, -7, -7, -6, -6, -5, -4, -3, -2, -1, 0, 1, 1, 2, 2, 1, 1, 0, -1, -1, 
  -2, -3, -4, -4, -5, -5, -5, -5, -5, -4, -4, -3, -2, -1, -1, 0, 0, 1, 1, 1, 0, 
  0, -1, -1, -2, -3, -3, -4, -4, -4, -4, -4, -4, -4, -3, -3, -2, -2, -1, -1, 0, 
  0, 0, 0, 0, 0, -1, -1, -2, -2, -3, -3, -3, -3, -4, -4, -3, -3, -3, -3, -2, -2,
  -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3,
  -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3,
  -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2,
  -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, -1, -1, -1,
  -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0,
  0, 0, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -1, -1, -1, -1, -1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -2, -2,
  -2, -2, -2, -1, -1, -1, -2
};

// FIR DIF (500 SPS)
const int32_t diferentiator_coef[DIF_ORDER] = {
  -4, 6, -3, 3, -3, 3, -4, 5, -6, 8, -11, 16, -27, 52, -145, 1304, -1304, 145,
  -52, 27, -16, 11, -8, 6, -5, 4, -3, 3, -3, 3, -6, 4};
//const int32_t diferentiator_coef[DIF_ORDER] = {-4, 6, -3, 3, -3, 3, -4, 5, -6, 
//                                              8, -11, 16, -27, 52, -145, 1304 };

int32_t band_pass_filterino(int32_t value)
{
  static int32_t bp_buffer[BP_ORDER] = {0};
  int i;
  int32_t y_n = 0;

  for(i = (BP_ORDER - 1) ; i > 0; i-- ){
    y_n += ((band_pass_coef[i + 1] * bp_buffer[i])>>10);
    bp_buffer[i] = bp_buffer[i-1];
  }
  y_n += ((band_pass_coef[1] * bp_buffer[0])>>10);

  bp_buffer[0] = value;

  y_n += ((band_pass_coef[0] * value)>>10);

  return y_n;
}

int32_t diferentiator_3000(int32_t value)
{
  static int32_t dif_buffer[(DIF_ORDER - 1)] = {0};
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
//FOLDED VERSION
//int32_t diferentiator_3000(int32_t value)
//{
//  static int32_t dif_buffer[((DIF_ORDER<<1) - 1)] = {0};
//  int i;
//  int32_t y_n = 0;
//
//  y_n = diferentiator_coef[(DIF_ORDER - 1)];
//  return y_n;
//}
int32_t integrator_3000(int32_t value)
{
  static int32_t buffer_x[INTEGRATION_LENGTH] = {0};
  int32_t x_n = value, y_n = 0;
  uint8_t i;

  for ( i = INTEGRATION_LENGTH - 1; i > 0; i--){
    y_n += (buffer_x[i] / INTEGRATION_LENGTH);
    buffer_x[i] = buffer_x[i-1];
  }

  y_n += (buffer_x[0] / INTEGRATION_LENGTH);
  buffer_x[0] = x_n;

  return y_n;
}

int32_t chuster_abs(int32_t value){
  if(value > 0){
    return value;
  }else{
    return (-1)*value;
  }
}
int32_t filter_sample(int32_t value)
{
  int32_t bp_filtered, dif_filtered, denormalized, integrated;
  static int32_t max_value = 1;
  float normalized, squared;

  bp_filtered = band_pass_filterino(value);
  dif_filtered = diferentiator_3000(bp_filtered);
  
  if( (chuster_abs(dif_filtered) > max_value) &&(dif_filtered != 0)){
    max_value = chuster_abs(dif_filtered);
  }
  
  normalized = ((float)dif_filtered)/max_value;
  squared = (normalized) * (normalized);
  denormalized = ((int32_t)(squared * max_value));
  integrated = integrator_3000(denormalized);

  return integrated;     

}
