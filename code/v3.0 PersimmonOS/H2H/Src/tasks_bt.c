#include "tasks_bt.h"

/* Semaphores */

/* Queues */

/* Tasks */
osThreadId bt_txTaskHandle;
void Start_bt_txTask(void const * argument);

/* Objects */

void tasks_bt_init()
{
  /* Semaphores */

  /* Queues */

  /* Tasks */
  /* bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityNormal, 0, 256);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}