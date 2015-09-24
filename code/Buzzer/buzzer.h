/*
 * buzzer_module.h
 *
 *  Created on: 29/7/2015
 *      Author: slopez
 */

#ifndef BUZZER_BUZZER_H_
#define BUZZER_BUZZER_H_

#include <msp430.h>
#include <stdint.h>

#include "buzzer_notes.h"

void	buzzer_setup();
void	buzzer_play(int note, int ms);

#endif /* BUZZER_BUZZER_H_ */
