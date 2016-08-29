#include "state_settings_configtabs_screen.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_settings_configtabs.h"
#include "state_settings.h"
#include "state_running.h"

/* State behaviour */
void behaviour_settings_configtabs_screen(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_settings_configtabs_screen(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_settings_configtabs_screen(state);
  behaviour_settings_configtabs(state);
  behaviour_settings(state);
  behaviour_running(state);

  /* Set state name */
  strcpy(state->name, "settings_configtabs_screen");
}