#include "state_h2h_auth.h"

/* Possible transition to the following states */
#include "state_h2h_wait.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"

/* State behaviour */
void behaviour_h2h_auth(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
  osDelay(500);
}

/* Entry point to the state */
void entry_to_h2h_auth(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_auth");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h_auth(state);

  // TEST
  entry_to_h2h_wait(state);
}