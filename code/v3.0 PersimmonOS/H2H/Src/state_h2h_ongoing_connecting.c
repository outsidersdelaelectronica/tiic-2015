#include "state_h2h_ongoing_connecting.h"

/* Possible transition to the following states */
#include "state_h2h_ongoing_error.h"
#include "state_h2h_ongoing_action.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"

static void h2h_ongoing_connecting_to_h2h_ongoing_error(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_error(state);
}

static void h2h_ongoing_connecting_to_h2h_ongoing_action(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_action(state);
}

/* State behaviour */
void behaviour_h2h_ongoing_connecting(state_ptr state)
{
  /* Set events to react to */
  state->h2h_error = h2h_ongoing_connecting_to_h2h_ongoing_error;
  state->h2h_ok = h2h_ongoing_connecting_to_h2h_ongoing_action;

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_ongoing_connecting(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_connecting(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);

  /* Set state name */
  strcpy(state->name, "h2h_ongoing_connecting");
}