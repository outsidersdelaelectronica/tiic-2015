#include "state_h2h_connect_1.h"

/* Possible transition to the following states */
#include "state_main.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

#define TIMEOUT 3

/* Mutexes */
extern osMutexId mutex_menuHandle;

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

/* Objects */
extern menu_t current_menu;

static void h2h_connect_1_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

/* State behaviour */
void behaviour_h2h_connect_1(state_ptr state)
{
  device_info_t inquired_bt_devices[MAX_INQUIRY_RESULTS];
  uint32_t number_of_btaddr = 0;

  /* Set events to react to */
  state->back = h2h_connect_1_to_main;

  /* Do state actions */
  number_of_btaddr = bt_get_remote_devices(inquired_bt_devices);
  SPPServiceDiscovery(inquired_bt_devices[0].physical_address);

  /* Set menu */
  osMutexWait(mutex_menuHandle, osWaitForever);
  menu_copy(&menu_h2h_devices, &current_menu);
  osMutexRelease(mutex_menuHandle);
}

/* Entry point to the state */
void entry_to_h2h_connect_1(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_connect_1");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_connect_1(state);
  behaviour_h2h(state);
  behaviour_running(state);
}