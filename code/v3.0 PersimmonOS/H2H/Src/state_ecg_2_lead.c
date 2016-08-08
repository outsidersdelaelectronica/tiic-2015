#include "state_ecg_2_lead.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_ecg.h"

/* State behaviour */
void behaviour_ecg_2_lead(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */
}

/* Entry point to the state */
void entry_to_ecg_2_lead(state_ptr state)
{
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_ecg_2_lead(state);
  behaviour_ecg(state);

  /* Set state name */
  strcpy(state->name, "ecg_2_lead");
}