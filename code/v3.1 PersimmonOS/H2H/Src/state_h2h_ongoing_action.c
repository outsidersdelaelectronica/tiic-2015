#include "state_h2h_ongoing_action.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_connect_1.h"

/* Parent states */
#include "state_h2h_ongoing.h"
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

/* Queues */
extern osMailQId queue_input_menuHandle;
extern osMailQId queue_lcdHandle;

static void h2h_ongoing_action_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void h2h_ongoing_connecting_to_h2h_connect_1(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_connect_1(state);
}

/* State behaviour */
void behaviour_h2h_ongoing_action(state_ptr state)
{
  device_info_t inquired_bt_devices[MAX_INQUIRY_RESULTS];
  uint32_t number_of_btaddr = 0,i;
  char full_string[50] = {0};
  
  /* Set events to react to */
  state->back = h2h_ongoing_action_to_main;
  state->h2h_connect_1 = h2h_ongoing_connecting_to_h2h_connect_1;
  
  /* Do state actions */
  number_of_btaddr = bt_get_remote_devices(inquired_bt_devices);
  
  for( i = 0; i < (( number_of_btaddr < 5)? number_of_btaddr:5);i++)
  {
    sprintf(full_string, "%s(%X:%X:%X:%X:%X:%X)", inquired_bt_devices[i].Name
           ,inquired_bt_devices[i].physical_address.BD_ADDR0
            , inquired_bt_devices[i].physical_address.BD_ADDR1
              , inquired_bt_devices[i].physical_address.BD_ADDR2
                , inquired_bt_devices[i].physical_address.BD_ADDR3
                  , inquired_bt_devices[i].physical_address.BD_ADDR4
                    , inquired_bt_devices[i].physical_address.BD_ADDR5);
    item_area_set_text(&menu_h2h_devices.items[i].item.area,full_string);
    }

  /* Set menu */
  while(osMailPut(queue_input_menuHandle, (void *) &menu_h2h_devices) != osOK)
  {
    osDelay(500);
  }

  /* Display menu */
  for (i = 0; i < menu_h2h_devices.item_num; i++)
  {
    while(osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[i]) != osOK)
    {
      osDelay(1);
    }
  }
}

/* Entry point to the state */
void entry_to_h2h_ongoing_action(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_ongoing_action");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_h2h_ongoing_action(state);
  behaviour_h2h_ongoing(state);
  behaviour_h2h(state);
  behaviour_running(state);
}