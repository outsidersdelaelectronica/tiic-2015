#include "tasks_bt.h"

/* This will be probably moved to hacitrans */
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
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityAboveNormal, 0, 512);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  /* Infinite loop */
  bluetooth_init(); // Small test 
  for(;;)
  {
    osDelay(1);
  }
}