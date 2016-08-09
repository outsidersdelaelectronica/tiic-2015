#include "state_h2h_connect.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_ongoing.h"

/* Parent states */
#include "state_h2h.h"

static void h2h_connect_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void h2h_connect_to_h2h_ongoing(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing(state);
}

/* State behaviour */
void behaviour_h2h_connect(state_ptr state)
{
  /* Set events to react to */
  state->back = h2h_connect_to_main;
  state->h2h_connect = h2h_connect_to_h2h_ongoing;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_connect(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_connect(state);
  behaviour_h2h(state);

  /* Set state name */
  strcpy(state->name, "h2h_connect");
}