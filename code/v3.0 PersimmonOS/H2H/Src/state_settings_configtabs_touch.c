#include "state_settings_configtabs_touch.h"

/* Possible transition to the following states */
#include "state_settings_touchcalib.h"

/* Parent states */
#include "state_settings_configtabs.h"
#include "state_settings.h"

static void settings_configtabs_touch_to_settings_touchcalib(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_touchcalib(state);
}

/* State behaviour */
void behaviour_settings_configtabs_touch(state_ptr state)
{
  /* Set events to react to */
  state->settings_touch_calib = settings_configtabs_touch_to_settings_touchcalib;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_settings_configtabs_touch(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_settings_configtabs_touch(state);
  behaviour_settings_configtabs(state);
  behaviour_settings(state);

  /* Set state name */
  strcpy(state->name, "settings_configtabs_touch");
}