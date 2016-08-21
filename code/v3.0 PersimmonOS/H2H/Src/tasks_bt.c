#include "tasks_bt.h"

/* This will be probably moved to hacitrans */
/* Semaphores */

/* Queues */

/* Tasks */
osThreadId bt_txTaskHandle;
osThreadId bt_initTaskHandle;
void Start_bt_txTask(void const * argument);
void Start_bt_initTask(void const * argument);

/* Objects */

void tasks_bt_init()
{
  /* Semaphores */

  /* Queues */

  /* Tasks */
  /* bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityHigh, 0, 64);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);
  
  osThreadDef(bt_initTask, Start_bt_initTask, osPriorityRealtime, 0, 512);
  bt_initTaskHandle = osThreadCreate(osThread(bt_initTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  /* Infinite loop */
  
  for(;;)
  {
    
    SendData(1,"HOLAA");
    osDelay(1000);
  }
}

void Start_bt_initTask(void const * argument)
{

  bluetooth_init(); 
  
  while(osThreadTerminate(bt_initTaskHandle) != osOK);
}