#include "state_main.h"

/* Possible transition to the following states */
#include "state_ecg.h"
#include "state_h2h.h"
#include "state_settings.h"

static void main_to_ecg(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_ecg(state);
}

static void main_to_h2h(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h(state);
}

static void main_to_settings(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_settings(state);
}

/* State behaviour */
void behaviour_main(state_ptr state)
{
  /* Set events to react to */
  state->ecg = main_to_ecg;
  state->h2h = main_to_h2h;
  state->settings = main_to_settings;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_main(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_main(state);

  /* Set state name */
  strcpy(state->name, "main");
}