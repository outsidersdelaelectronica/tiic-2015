#include "state_ecg_6_lead.h"

/* Possible transition to the following states */

/* Parent states */
#include "state_ecg.h"
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

#include "leads.h"

/* Mutexes */
extern osMutexId mutex_ecg_leadsHandle;

/* Queues */
extern osMailQId queue_lcdHandle;
extern osMailQId queue_input_menuHandle;
extern osMessageQId queue_ecg_bpm_screenHandle;

/* Objects */
extern leads_t leads;

static void ecg_6_lead_gui_tick(state_ptr state)
{
  osEvent event;
  int32_t lead_I = 0, lead_II = 0, lead_III = 0;
  int32_t lead_aVR = 0, lead_aVL = 0, lead_aVF = 0;
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
  
  /* Graph display */
  osMutexWait(mutex_ecg_leadsHandle, osWaitForever);
  lead_I = leads.lead_I;
  lead_II = leads.lead_II;
  lead_III = leads.lead_III;
  lead_aVR = leads.lead_aVR;
  lead_aVL = leads.lead_aVL;
  lead_aVF = leads.lead_aVF;
  osMutexRelease(mutex_ecg_leadsHandle);
  
}

/* State behaviour */
void behaviour_ecg_6_lead(state_ptr state)
{
  /* Set events to react to */
  state->gui_tick = ecg_6_lead_gui_tick;

  /* Do state actions */
  menu_copy(&menu_ecg, &current_menu);
  
  current_menu.items[3].item.area.is_active = GUI_ACTIVE;
  current_menu.items[3].item.area.text_color = (color_t) COLOR_WHITE;
  current_menu.items[3].item.area.bg_color = (color_t) COLOR_BLUE;

  current_menu.items[4].item.area.is_active = GUI_ACTIVE;
  current_menu.items[4].item.area.text_color = (color_t) COLOR_WHITE;
  current_menu.items[4].item.area.bg_color = (color_t) COLOR_BLUE;
  
  current_menu.items[5].item.area.is_active = GUI_INACTIVE;
  current_menu.items[5].item.area.text_color = (color_t) COLOR_BLUE;
  current_menu.items[5].item.area.bg_color = (color_t) COLOR_WHITE;
}

/* Entry point to the state */
void entry_to_ecg_6_lead(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "ecg_6_lead");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_running(state);
  behaviour_ecg(state);
  behaviour_ecg_6_lead(state);
}