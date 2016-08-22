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

}

void tasks_bt_start()
{
  /* Tasks */
  /* bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityHigh, 0, 256);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);

  osThreadDef(bt_initTask, Start_bt_initTask, osPriorityHigh, 0, 512);
  bt_initTaskHandle = osThreadCreate(osThread(bt_initTask), NULL);
}

void Start_bt_txTask(void const * argument)
{
  /* Infinite loop */

  for(;;)
  {
    SendData(sizeof("HOLA\n"),"HOLA\n");
    osDelay(5000);
  }
}

void Start_bt_initTask(void const * argument)
{

  bluetooth_init();

  while(osThreadTerminate(bt_initTaskHandle) != osOK);
}