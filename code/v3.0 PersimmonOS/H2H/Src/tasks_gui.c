#include "tasks_gui.h"

/* Semaphores */
osSemaphoreId sem_guiHandle;

/* Queues */
osMailQId queue_guiHandle;

/* Tasks */
osThreadId gui_managerTaskHandle;
void Start_gui_managerTask(void const * argument);

/* Objects */
extern lcd_t lcd;

void tasks_gui_init()
{
  /* Semaphores */
  /* sem_gui */
  osSemaphoreDef(sem_gui);
  sem_guiHandle = osSemaphoreCreate(osSemaphore(sem_gui), 1);

  /* Queues */
  /* queue_gui */
  osMailQDef(queue_gui, 4, buzzer_note_t);
  queue_guiHandle = osMailCreate(osMailQ(queue_gui), NULL);
}

void tasks_gui_start()
{
  /* Tasks */
  /* guiTask */
  osThreadDef(gui_managerTask, Start_gui_managerTask, osPriorityNormal, 0, 128);
  gui_managerTaskHandle = osThreadCreate(osThread(gui_managerTask), NULL);
}

void Start_gui_managerTask(void const * argument)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  item_action_t lcd_config;

  uint8_t is_lcd_on = 0;

  /* Take both semaphores for the first time */
  osSemaphoreWait(sem_periph_button_short_pressHandle, osWaitForever);
  osSemaphoreWait(sem_periph_button_long_pressHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    
  }
}