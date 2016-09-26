#include "tasks_fsm.h"

/* Semaphores */

/* Queues */
osMailQId queue_fsm_eventsHandle;

/* Tasks */
osThreadId fsm_managerTaskHandle;
void Start_fsm_managerTask(void const * argument);

/* Objects */
fsm_client_t fsm;

void tasks_fsm_init()
{
  /* Semaphores */

  /* Queues */
  /* queue_fsm_events */
  osMailQDef(queue_fsm_events, 4, fsm_event_f);
  queue_fsm_eventsHandle = osMailCreate(osMailQ(queue_fsm_events), NULL);
}

void tasks_fsm_start()
{
  /* Tasks */
  /* fsm_managerTask */
  osThreadDef(fsm_managerTask, Start_fsm_managerTask, osPriorityLow, 0, 512);
  fsm_managerTaskHandle = osThreadCreate(osThread(fsm_managerTask), NULL);
}

void Start_fsm_managerTask(void const * argument)
{
  osEvent event;
  fsm_event_f fsm_event;

  /* Initialize FSM */
  fsm_client_init(&fsm);

  /* Infinite loop */
  for(;;)
  {
    /* Get fsm event */
    event = osMailGet(queue_fsm_eventsHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Retrieve event function */
      fsm_event = *((fsm_event_f *) event.value.v);

      /* Trigger event */
//      if ((fsm_event == fsm_back)||(fsm_event ==fsm_ecg)||(fsm_event ==fsm_h2h)||
//        (fsm_event ==fsm_settings)||(fsm_event ==fsm_ecg_1_lead)||(fsm_event ==fsm_ecg_2_lead)||
//        (fsm_event ==fsm_ecg_6_lead)||(fsm_event ==fsm_h2h_connect)||(fsm_event ==fsm_h2h_selectdevice)||
//        (fsm_event ==fsm_h2h_ok)||(fsm_event ==fsm_h2h_error)||(fsm_event ==fsm_settings_screen)||
//        (fsm_event ==fsm_settings_timedate)||(fsm_event ==fsm_settings_bt)||(fsm_event ==fsm_settings_about)||
//        (fsm_event ==fsm_settings_touch)||(fsm_event ==fsm_settings_touch_calib)||(fsm_event ==fsm_click)||
//        (fsm_event ==fsm_button_short)||(fsm_event ==fsm_button_long)||(fsm_event ==fsm_gui_tick)||
//        (fsm_event ==fsm_no_event))
//      {
        fsm_event(&fsm);
//      }
    }
  }
}