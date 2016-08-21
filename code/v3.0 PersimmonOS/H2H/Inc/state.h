#include <string.h>/*
 *  state.h
 *
 *  Created on: 29/9/2015
 *      Author: Smau
 */

#ifndef STATE_H_
#define STATE_H_

#include <string.h>

#include "cmsis_os.h"

typedef struct state_t* state_ptr;

typedef void (*event_back_f)(state_ptr);

typedef void (*event_ecg_f)(state_ptr);
typedef void (*event_h2h_f)(state_ptr);
typedef void (*event_settings_f)(state_ptr);

typedef void (*event_ecg_1_lead_f)(state_ptr);
typedef void (*event_ecg_2_lead_f)(state_ptr);
typedef void (*event_ecg_6_lead_f)(state_ptr);

typedef void (*event_h2h_connect_f)(state_ptr);
typedef void (*event_h2h_ok_f)(state_ptr);
typedef void (*event_h2h_error_f)(state_ptr);

typedef void (*event_settings_screen_f)(state_ptr);
typedef void (*event_settings_timedate_f)(state_ptr);
typedef void (*event_settings_bt_f)(state_ptr);
typedef void (*event_settings_about_f)(state_ptr);
typedef void (*event_settings_touch_f)(state_ptr);

typedef void (*event_settings_touch_calib_f)(state_ptr);
typedef void (*event_click_f)(state_ptr);

struct state_t
{
  char name[32];

  event_back_f          back;

  event_ecg_f           ecg;
  event_h2h_f           h2h;
  event_settings_f      settings;

  event_ecg_1_lead_f    ecg_1_lead;
  event_ecg_2_lead_f    ecg_2_lead;
  event_ecg_6_lead_f    ecg_6_lead;

  event_h2h_connect_f   h2h_connect;
  event_h2h_ok_f        h2h_ok;
  event_h2h_error_f     h2h_error;

  event_settings_screen_f       settings_screen;
  event_settings_timedate_f     settings_timedate;
  event_settings_bt_f           settings_bt;
  event_settings_about_f        settings_about;
  event_settings_touch_f        settings_touch;

  event_settings_touch_calib_f  settings_touch_calib;
  event_click_f                 click;
};

void default_implementation(state_ptr state);

#endif /* MENU_STATE_H_ */
