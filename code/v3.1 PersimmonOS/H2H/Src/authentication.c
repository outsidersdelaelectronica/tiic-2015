/*
 * authentication.c
 *
 *  Created on: 15/12/2015
 *      Author: tvalno
 */

#include "authentication.h"

#define MASK_X4 0x1111111111111111
#define MASK_X3 0x2222222222222222
#define MASK_X2 0x4444444444444444
#define MASK_X1 0x8888888888888888

#define N_P_THRESHOLD -37.54
/* Tables with the log of the probabilty of each position x4,x3,x2,x1 and the */
/* adversary probability( which is constant for every position)               */
static const float log_Q_x4[NUMBER_OF_IPI +1] = {
  -0.0628215282355947, -0.900532690625621, -2.06730257227987, -3.44012693636725, 
  -4.97007472043432, -6.63169462376867, -8.41028433404004, -10.2972135191002, 
  -12.2878921416988, -14.3808758091921, -16.5776089142238, -18.8826814940444, 
  -21.3047238808019, -23.8584383868268, -26.5692763128313, -29.4861687212689, 
  -32.7321198489708};

static const float log_Q_x3[NUMBER_OF_IPI +1] = {
  -0.126216195408805, -0.658935195436524, -1.52071291472847, -2.58854511645354, 
  -3.81350073815831, -5.17012847913034, -6.64372602703941, -8.2256630497373, 
  -9.91134950997357, -11.6993410151046, -13.591081957774, -15.5911623752322, 
  -17.7082125996275, -19.95693494329, -22.3627807069322, -24.9746809530075, 
  -27.9156399183471};

static const float log_Q_x2[NUMBER_OF_IPI +1] = {
  -0.276425797303274, -0.463964595289396, -0.980562112539742, -1.70321411222321, 
  -2.58298953188638, -3.59443707081682, -4.72285441668429, -5.95961123734059, 
  -7.30011749553526, -8.74292879862468, -10.2894895392525, -11.9443897546691, 
  -13.7162597770228, -15.6198019186437, -17.6804674802443, -19.947187524278, 
  -22.542966287576};

static const float log_Q_x1[NUMBER_OF_IPI +1] = {
  -0.579394762471116, -0.435972620168803, -0.621609197130714, -1.01330025652575, 
  -1.56211473590049, -2.24260133454249, -3.04005774012152, -3.94585362048939, 
  -4.95539893839562, -6.06724930119662, -7.28284910153597, -8.60678837666416, 
  -10.0476974587294, -11.6202786600619, -13.3499832813741, -15.2857423851194, 
  -17.5505602081289};

static const float log_P_x4[NUMBER_OF_IPI +1] = {
  -4.8164799306237, -3.61235994796777, -2.73729868457607, -2.0682919036175, 
  -1.55640854263862, -1.17619730092702, -0.912955866152437, -0.758053906166694, 
  -0.706901383719312, -0.758053906166694, -0.912955866152437, -1.17619730092702, 
  -1.55640854263862, -2.0682919036175, -2.73729868457607, -3.61235994796777, 
  -4.8164799306237};

void init_key(validation_key_t* key, key_origin_t origin)
{
  key->token = 0;
  key->index = 0;
  key->state = NOT_READY;
  key->origin = origin;  
}

key_state_t write_bpm_key(validation_key_t* key, uint32_t bpm)
{
  uint8_t IPI;
  
  IPI = (uint8_t) ( bpm & 0x0000000F);
  key->token |= (IPI << ( 4 * key->index));
  key->index++;
  if (key->index > 15 )
  {
    key->token = READY;
  }
  return key->state;
}

void write_token_key(validation_key_t* key, uint64_t token)
{
  key->token = token;
  key->index = 15;
  key->token = READY;
}

void erase_key(validation_key_t* key)
{
  key->token = 0;
  key->index = 0;
  key->state = NOT_READY;
}

autentitication_t validate(validation_key_t* master_key, validation_key_t* received_key)
{
  uint64_t diff;
  uint64_t diff_x4,diff_x3,diff_x2,diff_x1;
  uint8_t dif_bits_x4,dif_bits_x3,dif_bits_x2,dif_bits_x1;
  float hypothesis;
  diff = (master_key->token) ^ (master_key->token);
  
  // we mask and align so the first posible one is the least significant bit */
  diff_x4 = diff & MASK_X4;
  diff_x3 = ((diff & MASK_X3)>>1);
  diff_x2 = ((diff & MASK_X2)>>2);
  diff_x1 = ((diff & MASK_X1)>>3);
  
  while(diff_x4 > 0)
  {
    dif_bits_x4++;
    diff_x4 = diff_x4 >> 4;
  }
  
  while(diff_x3 > 0)
  {
    dif_bits_x3++;
    diff_x3 = diff_x3 >> 4;
  }  
  
  while(diff_x2 > 0)
  {
    dif_bits_x2++;
    diff_x2 = diff_x2 >> 4;
  } 
  while(diff_x1 > 0)
  {
    dif_bits_x1++;
    diff_x1 = diff_x1 >> 4;
  }  
  
  hypothesis = (log_P_x4[dif_bits_x4] + log_P_x4[dif_bits_x3] +
               log_P_x4[dif_bits_x2] +log_P_x4[dif_bits_x1]) -
               (log_Q_x4[dif_bits_x4] + log_Q_x3[dif_bits_x3] +
               log_Q_x2[dif_bits_x2] +log_Q_x1[dif_bits_x1]);
  
  if(hypothesis > N_P_THRESHOLD)
  {          
    return ACCEPTED;
    
  }
  else
  {
    return REJECTED;
  }
}
