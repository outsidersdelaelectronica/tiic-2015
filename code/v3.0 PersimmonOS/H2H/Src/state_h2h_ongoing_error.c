#include "state_h2h_ongoing_error.h"

/* Possible transition to the following states */
#include "state_h2h_connect.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"

/* State behaviour */
void behaviour_h2h_ongoing_error(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_ongoing_error(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_error(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);

  /* Set state name */
  strcpy(state->name, "h2h_ongoing_error");
}