/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "afe.h"
#include "buzzer.h"
#include "gauge.h"
#include "lcd.h"
#include "touch.h"

#include "fsm_client.h"

#include "tasks_ecg.h"
#include "tasks_input.h"
#include "tasks_bt.h"
#include "tasks_periph.h"
#include "tasks_fsm.h"

/* Hardware */
afe_t afe;
buzzer_t buzzer;
gauge_t gauge;
lcd_t lcd;
touch_t touch;

/* FSM */
fsm_client_t fsm;

/* Menus */
extern menu_t menu_welcome;
extern menu_t menu_main;
extern menu_t menu_top_bar;

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */
void system_init(void);

void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{

}

void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{

}

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /*
    * Run time stack overflow checking is performed if
    * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    * called if a stack overflow is detected.
    */
}

void system_init(void)
{
  afe_init(&afe, &hspi1);
  afe_test_signal_on(&afe);

  buzzer_init(&buzzer, &htim3, &htim4);
  lcd_init(&lcd, &hsram1, LCD_REG, LCD_DATA, LCD_X_SIZE, LCD_Y_SIZE);
  touch_init(&touch, &hspi2, TOUCH_X_SIZE, TOUCH_Y_SIZE);

  fsm_client_init(&fsm);
}

/* TEST Task */
osThreadId testTaskHandle;
void Start_testTask(void const * argument);

extern osMailQId queue_lcdHandle;
extern osMailQId queue_fsm_eventsHandle;

void MX_FREERTOS_Init(void)
{
  /* Initialize system */
  system_init();

  /* Init tasks */
  tasks_ecg_init();
  tasks_bt_init();
  tasks_input_init();
  tasks_periph_init();
  tasks_fsm_init();

  /* Start tasks */
  tasks_ecg_start();
  tasks_bt_start();
  tasks_input_start();
  tasks_periph_start();
  tasks_fsm_start();

  /* TEST Task */
  osThreadDef(testTask, Start_testTask, osPriorityIdle, 0, 64);
  testTaskHandle = osThreadCreate(osThread(testTask), NULL);
}

void Start_testTask(void const * argument)
{
  item_action_t lcd_config;

  /* LCD config */
  item_lcd_config_init(&lcd_config.item.config, 200);
  lcd_config.item_print_function = lcd_set_config;
  osMailPut(queue_lcdHandle, (void *) &lcd_config);

  item_area_set_text(&menu_top_bar.items[0].item.area, "");
  item_area_set_text(&menu_top_bar.items[1].item.area, "16:20");
  osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[0]);
  osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[1]);

  for (;;)
  {
    osDelay(2000);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
