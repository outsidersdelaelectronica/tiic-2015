#include "state_h2h_waitkey.h"

/* Possible transition to the following states */
#include "state_h2h_wait.h"
#include "state_h2h_auth.h"

/* Parent states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* State behaviour */
void behaviour_h2h_waitkey(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_waitkey(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_waitkey");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_waitkey(state);
  behaviour_running(state);
}