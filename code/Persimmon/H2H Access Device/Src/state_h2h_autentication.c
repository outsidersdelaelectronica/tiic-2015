#include "state_h2h_autentication.h"

/* Possible transition to the following states */
#include "state_main.h"
#include "state_h2h_ongoing_error.h"

/* Parent states */
#include "state_h2h.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"
#include "bluetooth_internal.h"

/* Semaphores */
extern osSemaphoreId sem_ecg_keygenHandle;
/* Queues */
extern osMailQId queue_bt_packet_sendHandle;
extern osMailQId queue_lcdHandle;

static void h2h_autentication_to_main(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_main(state);
}

static void h2h_autentication_to_ongoing_error(state_ptr state)
{
  /* Do transition actions */

  /* Change state */
  entry_to_h2h_ongoing_error(state);
}

/* State behaviour */
void behaviour_h2h_autentication(state_ptr state)
{
  bt_packet_t fsm_send_packet = {.packet_content = {0}};
  /* Set events to react to */
  state->back = h2h_autentication_to_main;
  state->h2h_error = h2h_autentication_to_ongoing_error;
  /* Do state actions */
  
  item_area_set_text(&menu_h2h_devices.items[6].item.area,"KEY VALIDATED YAY!");
  menu_h2h_devices.items[6].item.area.bg_color = (color_t) COLOR_GREEN;
  menu_h2h_devices.items[6].item.area.text_color = (color_t) COLOR_BLACK;
  while (osMailPut(queue_lcdHandle, (void *) &menu_h2h_devices.items[6]) != osOK)
  {
    osDelay(1);
  }  
  
}

/* Entry point to the state */
void entry_to_h2h_autentication(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "h2h_autentication");
  
  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_h2h(state);
  behaviour_h2h_autentication(state);
}