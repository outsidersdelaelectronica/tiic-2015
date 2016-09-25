#include "state_h2h_ongoing_error.h"

/* Possible transition to the following states */
#include "state_h2h_start_connect.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* State behaviour */
void behaviour_h2h_ongoing_error(state_ptr state)
{
  /* Set events to react to */

  /* Do state actions */

  /* Set menu */
  while(osMailPut(queue_input_menuHandle, (void *) &menu_h2h_ongoing_error) != osOK)
  {
    osDelay(1);
  }

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_h2h_ongoing_error.item_num; i++)
  {
    while(osMailPut(queue_lcdHandle, (void *) &menu_h2h_ongoing_error.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_h2h_ongoing_error(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_ongoing_error");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_error(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);
  behaviour_running(state);
}