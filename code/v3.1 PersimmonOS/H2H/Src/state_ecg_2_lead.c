#include "state_ecg_2_lead.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_ecg.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* Queues */
extern osMailQId queue_lcdHandle;
extern osMessageQId queue_ecg_bpm_screenHandle;
extern osMessageQId queue_ecg_lead_IHandle;

static void ecg_2_lead_gui_tick(state_ptr state)
{
  osEvent event;
  uint32_t bpm = 0;
  char str_bpm[4] = "-?-";

  /* Do transition actions */
  /* Get bpm data */
  event = osMessageGet(queue_ecg_bpm_screenHandle, 0);
  if (event.status == osEventMessage)
  {
    /* Retrieve value */
    bpm = (((uint32_t) event.value.v) >> 10);
    sprintf(str_bpm, "%u", bpm);

    menu_copy(&menu_ecg, &current_menu);

    item_area_set_text(&current_menu.items[1].item.area, str_bpm);
    osMailPut(queue_lcdHandle, (void *) &current_menu.items[1]);
  }
}

/* State behaviour */
void behaviour_ecg_2_lead(state_ptr state)
{
  uint32_t i = 0;

  /* Set events to react to */
  state->gui_tick = ecg_2_lead_gui_tick;

  /* Do state actions */
  menu_copy(&menu_ecg, &current_menu);

  current_menu.items[3].item.area.is_active = GUI_ACTIVE;
  current_menu.items[3].item.area.text_color = (color_t) COLOR_WHITE;
  current_menu.items[3].item.area.bg_color = (color_t) COLOR_BLUE;

  current_menu.items[4].item.area.is_active = GUI_INACTIVE;
  current_menu.items[4].item.area.text_color = (color_t) COLOR_BLUE;
  current_menu.items[4].item.area.bg_color = (color_t) COLOR_WHITE;

  current_menu.items[5].item.area.is_active = GUI_ACTIVE;
  current_menu.items[5].item.area.text_color = (color_t) COLOR_WHITE;
  current_menu.items[5].item.area.bg_color = (color_t) COLOR_BLUE;

  /* Print graphs */
  for (i = 0; i < graph_ecg_2_lead.item_num; i++)
  {
    graph_ecg_2_lead.items[i].item_print_function = lcd_print_graph;
    osMailPut(queue_lcdHandle, (void *) &graph_ecg_2_lead.items[i]);
  }
}

/* Entry point to the state */
void entry_to_ecg_2_lead(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "ecg_2_lead");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_ecg(state);
  behaviour_ecg_2_lead(state);
}