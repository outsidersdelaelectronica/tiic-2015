#include "state_h2h_ongoing_connecting.h"

/* Possible transition to the following states */
#include "state_h2h_ongoing_error.h"
#include "state_h2h_ongoing_action.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

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
  bluetooth_init();

  /* Set menu */
  osMailPut(queue_input_menuHandle, (void *) &menu_h2h_ongoing_connecting);

  /* Display menu */
  uint32_t i;
  for (i = 0; i < menu_h2h_ongoing_connecting.item_num; i++)
  {
    osMailPut(queue_lcdHandle, (void *) &menu_h2h_ongoing_connecting.items[i]);
  }
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