/*
 * filters.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "gpio.h"
#include "spi.h"

#include "afe_constants.h"

int32_t bpm_preprocessing(int32_t value);
int32_t show_filter(int32_t value);

#endif /* FILTERS_H_ */
