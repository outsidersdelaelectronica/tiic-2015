#include "state_h2h_start_gen.h"

/* Possible transition to the following states */
#include "state_main.h"


/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

/* Queues */

/* State behaviour */
void behaviour_h2h_start_gen(state_ptr state)
{
  /* Set events to react to */
//  state->h2h_error = h2h_connect_4_to_ongoing_error;
  
  /* Do state actions */
}

/* Entry point to the state */
void entry_to_h2h_start_gen(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_start_gen");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_start_gen(state);
  behaviour_h2h(state);
  behaviour_running(state);
}