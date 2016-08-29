#include "state_settings_touchcalib_point_2.h"

/* Possible transition to the following states */
#include "state_settings_touchcalib_point_3.h"

/* Parent states */
#include "state_settings_touchcalib.h"
#include "state_settings.h"
#include "state_running.h"

static void settings_touchcalib_point_2_to_settings_touchcalib_point_3(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings_touchcalib_point_3(state);
}

/* State behaviour */
void behaviour_settings_touchcalib_point_2(state_ptr state)
{
  /* Set events to react to */
  state->click = settings_touchcalib_point_2_to_settings_touchcalib_point_3;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_settings_touchcalib_point_2(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_settings_touchcalib_point_2(state);
  behaviour_settings_touchcalib(state);
  behaviour_settings(state);
  behaviour_running(state);

  /* Set state name */
  strcpy(state->name, "settings_touchcalib_point_2");
}