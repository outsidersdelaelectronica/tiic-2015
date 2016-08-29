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
  osMailQDef(queue_gui, 4, uint32_t);
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
  /* Infinite loop */
  for(;;)
  {

  }
}