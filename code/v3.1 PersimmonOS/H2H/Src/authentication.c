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

#define N_P_THRESHOLD -5.0
/* Tables with the log of the probabilty of each position x4,x3,x2,x1 and the */
/* adversary probability( which is constant for every position)               */
static const float log_P_x4[NUMBER_OF_IPI +1] = {
  -0.0628215282355947, -2.10465267328155, -4.1464838183275, -6.18831496337345, 
  -8.2301461084194, -10.2719772534653, -12.3138083985113, -14.3556395435572, 
  -16.3974706886032, -18.4393018336492, -20.4811329786951, -22.5229641237411, 
  -24.564795268787, -26.606626413833, -28.6484575588789, -30.6902887039249, 
  -32.7321198489708};

static const float log_P_x3[NUMBER_OF_IPI +1] = {
  -0.126216195408805, -1.86305517809245, -3.59989416077609, -5.33673314345974, 
  -7.07357212614338, -8.81041110882702, -10.5472500915107, -12.2840890741943, 
  -14.020928056878, -15.7577670395616, -17.4946060222452, -19.2314450049289,
  -20.9682839876125, -22.7051229702962, -24.4419619529798, -26.1788009356635, 
  -27.9156399183471};

static const float log_P_x2[NUMBER_OF_IPI +1] = {
  -0.276425797303275, -1.66808457794532, -3.05974335858737, -4.45140213922941, 
  -5.84306091987146, -7.23471970051351, -8.62637848115555, -10.0180372617976, 
  -11.4096960424396, -12.8013548230817, -14.1930136037237, -15.5846723843658, 
  -16.9763311650078, -18.3679899456499, -19.7596487262919, -21.151307506934,
  -22.542966287576};

static const float log_P_x1[NUMBER_OF_IPI +1] = {
  -0.579394762471115, -1.64009260282473, -2.70079044317834, -3.76148828353195,
  -4.82218612388556, -5.88288396423917, -6.94358180459279, -8.0042796449464, 
  -9.06497748530001, -10.1256753256536, -11.1863731660072, -12.2470710063608, 
  -13.3077688467145, -14.3684666870681, -15.4291645274217, -16.4898623677753, 
  -17.5505602081289};

static const float log_Q_x4[NUMBER_OF_IPI +1] = {
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
  
  hypothesis = (log_P_x4[dif_bits_x4] + log_P_x3[dif_bits_x3] +
               log_P_x2[dif_bits_x2] +log_P_x1[dif_bits_x1]) -
               (log_Q_x4[dif_bits_x4] + log_Q_x4[dif_bits_x3] +
               log_Q_x4[dif_bits_x2] +log_Q_x4[dif_bits_x1]);
  
  if(hypothesis > N_P_THRESHOLD)
  {          
    return ACCEPTED;
    
  }
  else
  {
    return REJECTED;
  }
}
