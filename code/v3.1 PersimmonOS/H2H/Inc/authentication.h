/*
 * authentication.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef AUTENTICATION_H_
#define AUTENTICATION_H_

#include <stdint.h>

#define NUMBER_OF_IPI 16
typedef enum {ACCEPTED, REJECTED} autentitication_t;
typedef enum {READY, NOT_READY  } key_state_t;

typedef struct validation_key_t
{
  uint64_t token;
  uint8_t index;
  key_state_t state;
}validation_key_t;

key_state_t write_key(uint32_t bpm, validation_key_t* key);
void erase_key(validation_key_t* key);
autentitication_t bpm_preprocessing(int32_t value);


#endif /* AUTENTICATION_H_ */
