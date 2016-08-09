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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "touch.h"

#include "afe.h"
#include "gauge.h"

#include "buzzer.h"
#include "lcd.h"
#include "touch.h"

#include "click.h"

#include "dsp.h"

#include "bluetooth.h"

#include "fsm_client.h"

#define FS 500
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId ecg_afeTaskHandle;
osThreadId ecg_filterTaskHandle;
osThreadId ecg_leadGenTaskHandle;
osThreadId ecg_bpmDetTaskHandle;
osThreadId ecg_keyGenTaskHandle;
osThreadId periph_buttonTaskHandle;
osThreadId periph_batteryTaskHandle;
osThreadId periph_buzzerTaskHandle;
osThreadId input_touchTaskHandle;
osThreadId input_clickTaskHandle;
osThreadId bt_rxTaskHandle;
osThreadId bt_txTaskHandle;
osThreadId periph_screenTaskHandle;
osThreadId sys_interpreterTaskHandle;
osMessageQId queue_ecg_afe_ch_1Handle;
osMessageQId queue_ecg_afe_ch_2Handle;
osMessageQId queue_ecg_filter_ch_1Handle;
osMessageQId queue_ecg_filter_ch_2Handle;
osMessageQId queue_ecg_lead_IHandle;
osMessageQId queue_ecg_lead_IIHandle;
osMessageQId queue_ecg_lead_IIIHandle;
osMessageQId queue_ecg_lead_aVRHandle;
osMessageQId queue_ecg_lead_aVLHandle;
osMessageQId queue_ecg_lead_aVFHandle;
osMessageQId queue_ecg_bpmHandle;
osSemaphoreId sem_ecg_afe_drdyHandle;
osSemaphoreId sem_ecg_afe_dma_rxHandle;
osSemaphoreId sem_input_touch_penHandle;
osSemaphoreId sem_periph_button_short_pressHandle;
osSemaphoreId sem_periph_button_long_pressHandle;
osSemaphoreId sem_periph_batteryHandle;

/* USER CODE BEGIN Variables */
osThreadId initTaskHandle;
osMailQId queue_periph_buzzerHandle;
osMailQId queue_input_clickHandle;
osMailQId queue_lcdHandle;
osMessageQId queue_bpm_signalHandle;

afe_t afe;
gauge_t gauge;

buzzer_t buzzer;
lcd_t lcd;
touch_t touch;

fsm_client_t fsm;

extern RTC_HandleTypeDef hrtc;

int32_t datalog[3000], *data_ptr;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void Start_defaultTask(void const * argument);
void Start_ecg_afeTask(void const * argument);
void Start_ecg_filterTask(void const * argument);
void Start_ecg_leadGenTask(void const * argument);
void Start_ecg_bpmDetTask(void const * argument);
void Start_ecg_keyGenTask(void const * argument);
void Start_periph_buttonTask(void const * argument);
void Start_periph_batteryTask(void const * argument);
void Start_periph_buzzerTask(void const * argument);
void Start_input_touchTask(void const * argument);
void Start_input_clickTask(void const * argument);
void Start_bt_rxTask(void const * argument);
void Start_bt_txTask(void const * argument);
void Start_periph_screenTask(void const * argument);
void Start_sys_interpreterTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void Start_init_Task(void const * argument);
/* USER CODE END FunctionPrototypes */

/* Pre/Post sleep processing prototypes */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime);
void PostSleepProcessing(uint32_t *ulExpectedIdleTime);

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/* USER CODE BEGIN PREPOSTSLEEP */
void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}

void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */
  data_ptr = datalog;
  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of sem_ecg_afe_drdy */
  osSemaphoreDef(sem_ecg_afe_drdy);
  sem_ecg_afe_drdyHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_drdy), 1);

  /* definition and creation of sem_ecg_afe_dma_rx */
  osSemaphoreDef(sem_ecg_afe_dma_rx);
  sem_ecg_afe_dma_rxHandle = osSemaphoreCreate(osSemaphore(sem_ecg_afe_dma_rx), 1);

  /* definition and creation of sem_input_touch_pen */
  osSemaphoreDef(sem_input_touch_pen);
  sem_input_touch_penHandle = osSemaphoreCreate(osSemaphore(sem_input_touch_pen), 1);

  /* definition and creation of sem_periph_button_short_press */
  osSemaphoreDef(sem_periph_button_short_press);
  sem_periph_button_short_pressHandle = osSemaphoreCreate(osSemaphore(sem_periph_button_short_press), 1);

  /* definition and creation of sem_periph_button_long_press */
  osSemaphoreDef(sem_periph_button_long_press);
  sem_periph_button_long_pressHandle = osSemaphoreCreate(osSemaphore(sem_periph_button_long_press), 1);

  /* definition and creation of sem_periph_battery */
  osSemaphoreDef(sem_periph_battery);
  sem_periph_batteryHandle = osSemaphoreCreate(osSemaphore(sem_periph_battery), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, Start_defaultTask, osPriorityIdle, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of ecg_afeTask */
  osThreadDef(ecg_afeTask, Start_ecg_afeTask, osPriorityHigh, 0, 64);
  ecg_afeTaskHandle = osThreadCreate(osThread(ecg_afeTask), NULL);

  /* definition and creation of ecg_filterTask */
  osThreadDef(ecg_filterTask, Start_ecg_filterTask, osPriorityAboveNormal, 0, 128);
  ecg_filterTaskHandle = osThreadCreate(osThread(ecg_filterTask), NULL);

//  /* definition and creation of ecg_leadGenTask */
//  osThreadDef(ecg_leadGenTask, Start_ecg_leadGenTask, osPriorityAboveNormal, 0, 64);
//  ecg_leadGenTaskHandle = osThreadCreate(osThread(ecg_leadGenTask), NULL);

  /* definition and creation of ecg_bpmDetTask */
  osThreadDef(ecg_bpmDetTask, Start_ecg_bpmDetTask, osPriorityAboveNormal, 0, 128);
  ecg_bpmDetTaskHandle = osThreadCreate(osThread(ecg_bpmDetTask), NULL);

  /* definition and creation of ecg_keyGenTask */
  osThreadDef(ecg_keyGenTask, Start_ecg_keyGenTask, osPriorityAboveNormal, 0, 64);
  ecg_keyGenTaskHandle = osThreadCreate(osThread(ecg_keyGenTask), NULL);

  /* definition and creation of periph_buttonTask */
  osThreadDef(periph_buttonTask, Start_periph_buttonTask, osPriorityHigh, 0, 64);
  periph_buttonTaskHandle = osThreadCreate(osThread(periph_buttonTask), NULL);

  /* definition and creation of periph_batteryTask */
  osThreadDef(periph_batteryTask, Start_periph_batteryTask, osPriorityLow, 0, 64);
  periph_batteryTaskHandle = osThreadCreate(osThread(periph_batteryTask), NULL);

  /* definition and creation of periph_buzzerTask */
  osThreadDef(periph_buzzerTask, Start_periph_buzzerTask, osPriorityLow, 0, 64);
  periph_buzzerTaskHandle = osThreadCreate(osThread(periph_buzzerTask), NULL);

  /* definition and creation of input_touchTask */
  osThreadDef(input_touchTask, Start_input_touchTask, osPriorityNormal, 0, 64);
  input_touchTaskHandle = osThreadCreate(osThread(input_touchTask), NULL);

  /* definition and creation of input_clickTask */
  osThreadDef(input_clickTask, Start_input_clickTask, osPriorityNormal, 0, 64);
  input_clickTaskHandle = osThreadCreate(osThread(input_clickTask), NULL);

  /* definition and creation of bt_txTask */
  osThreadDef(bt_txTask, Start_bt_txTask, osPriorityNormal, 0, 256);
  bt_txTaskHandle = osThreadCreate(osThread(bt_txTask), NULL);

  /* definition and creation of periph_screenTask */
  osThreadDef(periph_screenTask, Start_periph_screenTask, osPriorityLow, 0, 512);
  periph_screenTaskHandle = osThreadCreate(osThread(periph_screenTask), NULL);

  /* definition and creation of sys_interpreterTask */
  osThreadDef(sys_interpreterTask, Start_sys_interpreterTask, osPriorityLow, 0, 512);
  sys_interpreterTaskHandle = osThreadCreate(osThread(sys_interpreterTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* This priority is irrelevant since all other task are blocked until we    */
  /* start the peripherics                                                    */
  osThreadDef(initTask, Start_init_Task, osPriorityLow, 0, 512);
  initTaskHandle = osThreadCreate(osThread(initTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of ecg_afe_ch_1 */
  osMessageQDef(queue_ecg_afe_ch_1, 4, int32_t);
  queue_ecg_afe_ch_1Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_1), NULL);

  /* definition and creation of ecg_afe_ch_2 */
  osMessageQDef(queue_ecg_afe_ch_2, 4, int32_t);
  queue_ecg_afe_ch_2Handle = osMessageCreate(osMessageQ(queue_ecg_afe_ch_2), NULL);

  /* definition and creation of ecg_filter_ch_1 */
  osMessageQDef(queue_ecg_filter_ch_1, 4, int32_t);
  queue_ecg_filter_ch_1Handle = osMessageCreate(osMessageQ(queue_ecg_filter_ch_1), NULL);

  /* definition and creation of ecg_filter_ch_2 */
  osMessageQDef(queue_ecg_filter_ch_2, 4, int32_t);
  queue_ecg_filter_ch_2Handle = osMessageCreate(osMessageQ(queue_ecg_filter_ch_2), NULL);

  /* definition and creation of ecg_lead_I */
  osMessageQDef(queue_ecg_lead_I, 4, int32_t);
  queue_ecg_lead_IHandle = osMessageCreate(osMessageQ(queue_ecg_lead_I), NULL);

  /* definition and creation of ecg_lead_II */
  osMessageQDef(queue_ecg_lead_II, 4, int32_t);
  queue_ecg_lead_IIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_II), NULL);

  /* definition and creation of ecg_lead_III */
  osMessageQDef(queue_ecg_lead_III, 4, int32_t);
  queue_ecg_lead_IIIHandle = osMessageCreate(osMessageQ(queue_ecg_lead_III), NULL);

  /* definition and creation of ecg_lead_aVR */
  osMessageQDef(queue_ecg_lead_aVR, 4, int32_t);
  queue_ecg_lead_aVRHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVR), NULL);

  /* definition and creation of ecg_lead_aVL */
  osMessageQDef(queue_ecg_lead_aVL, 4, int32_t);
  queue_ecg_lead_aVLHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVL), NULL);

  /* definition and creation of ecg_lead_aVF */
  osMessageQDef(queue_ecg_lead_aVF, 4, int32_t);
  queue_ecg_lead_aVFHandle = osMessageCreate(osMessageQ(queue_ecg_lead_aVF), NULL);

  /* definition and creation of ecg_bpm */
  osMessageQDef(queue_ecg_bpm, 4, uint32_t);
  queue_ecg_bpmHandle = osMessageCreate(osMessageQ(queue_ecg_bpm), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* definition and creation of lcdQueue */
  osMailQDef(queue_periph_buzzer, 4, buzzer_note_t);
  queue_periph_buzzerHandle = osMailCreate(osMailQ(queue_periph_buzzer), NULL);

  osMailQDef(queue_input_click, 4, click_t);
  queue_input_clickHandle = osMailCreate(osMailQ(queue_input_click), NULL);

  osMailQDef(queue_lcd, 4, lcd_item_t);
  queue_lcdHandle = osMailCreate(osMailQ(queue_lcd), NULL);

  osMessageQDef(queue_bpmsignal, 4, int32_t);
  queue_bpm_signalHandle = osMessageCreate(osMessageQ(queue_bpmsignal), NULL);
  /* USER CODE END RTOS_QUEUES */
}

/* Start_defaultTask function */
void Start_defaultTask(void const * argument)
{

  /* USER CODE BEGIN Start_defaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_defaultTask */
}

/* Start_ecg_afeTask function */
void Start_ecg_afeTask(void const * argument)
{
  /* USER CODE BEGIN Start_ecg_afeTask */

  /* Reset semaphores */
  osSemaphoreWait(sem_ecg_afe_drdyHandle, osWaitForever);
  osSemaphoreWait(sem_ecg_afe_dma_rxHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* Wait for AFE to inform when data is ready */
    if (osSemaphoreWait(sem_ecg_afe_drdyHandle, osWaitForever) == osOK)
    {
      /* Start reading data from AFE */
      afe_start_read(&afe);

      /* Wait for DMA to complete transaction */
      if (osSemaphoreWait(sem_ecg_afe_dma_rxHandle, osWaitForever) == osOK)
      {
        /* Format received bytes into usable data */
        afe_format_data(&afe);

        /* Output data to queues */
        osMessagePut(queue_ecg_afe_ch_1Handle, afe.last_data.ch1_data, 0);
        osMessagePut(queue_ecg_afe_ch_2Handle, afe.last_data.ch2_data, 0);
      }
    }
  }
  /* USER CODE END Start_ecg_afeTask */
}
/* Start_ecg_filterTask function */
void Start_ecg_filterTask(void const * argument)
{
  /* USER CODE BEGIN Start_ecg_filterTask */
  osEvent event_ch1, event_ch2;

  int32_t ch1_data, ch2_data;
  int32_t filtered_ch1_data = 0, filtered_ch2_data = 0, bpm_signal = 0;
  /* Infinite loop */
  for(;;)
  {
    /* Get channel 1 data */
    event_ch1 = osMessageGet(queue_ecg_afe_ch_1Handle, osWaitForever);
    event_ch2 = osMessageGet(queue_ecg_afe_ch_2Handle, osWaitForever);
    if ((event_ch1.status == osEventMessage)&& (event_ch2.status == osEventMessage))
    {
      /* Retrieve value */
      ch1_data = (int32_t) event_ch1.value.v;
      ch2_data = (int32_t) event_ch2.value.v;

//    /* Filter channel 1 */
//    filtered_ch1_data = show_filter(ch1_data);
//    /* Filter channel 2 */
//    filtered_ch2_data = show_filter(ch2_data);
    /* Apply bpm preprocessing */
//    bpm_signal = bpm_preprocessing(ch1_data);

    *(data_ptr++) = bpm_signal;
    if(data_ptr == &datalog[3000])
    {
      data_ptr = datalog;
    }

    /* Output data to queues */
//    osMessagePut(queue_ecg_filter_ch_1Handle, filtered_ch1_data, 0);
//    osMessagePut(queue_ecg_filter_ch_2Handle, filtered_ch2_data, 0);
    osMessagePut(queue_bpm_signalHandle, bpm_signal, 0);
  }
  /* USER CODE END Start_ecg_filterTask */
 }
}

/* Start_ecg_leadGenTask function */
void Start_ecg_leadGenTask(void const * argument)
{
  /* USER CODE BEGIN Start_ecg_leadGenTask */
  osEvent event;

  int32_t filtered_ch1_data, filtered_ch2_data;

  int32_t lead_I, lead_II, lead_III;
  int32_t lead_aVR, lead_aVL, lead_aVF;

  /* Infinite loop */
  for(;;)
  {
    /* Get filtered channel 1 data */
    event = osMessageGet(queue_ecg_filter_ch_1Handle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      filtered_ch1_data = (int32_t) event.value.v;
    }

    /* Get filtered channel 2 data */
    event = osMessageGet(queue_ecg_filter_ch_2Handle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      filtered_ch2_data = (int32_t) event.value.v;
    }

    /* Generate lead I */
    lead_I = filtered_ch1_data;
    /* Generate lead I */
    lead_II = filtered_ch2_data;
    /* Generate lead III */
    lead_III = lead_II - lead_I;
    /* Generate lead aVR */
    lead_aVR = (lead_I + lead_II)   >> 1;
    /* Generate lead aVL */
    lead_aVL = (lead_I - lead_III)  >> 1;
    /* Generate lead aVF */
    lead_aVF = (lead_II + lead_III) >> 1;

    /* Output data to queues */
    osMessagePut(queue_ecg_lead_IHandle,   lead_I,   0);
    osMessagePut(queue_ecg_lead_IIHandle,  lead_II,  0);
    osMessagePut(queue_ecg_lead_IIIHandle, lead_III, 0);
    osMessagePut(queue_ecg_lead_aVRHandle, lead_aVR, 0);
    osMessagePut(queue_ecg_lead_aVLHandle, lead_aVL, 0);
    osMessagePut(queue_ecg_lead_aVFHandle, lead_aVF, 0);
  }
  /* USER CODE END Start_ecg_leadGenTask */
}

/* Start_ecg_bpmDetTask function */
void Start_ecg_bpmDetTask(void const * argument)
{
  /* USER CODE BEGIN Start_ecg_bpmDetTask */
  osEvent event;
  int32_t ecg_lead = 0;

  static int32_t threshold = 0, maxerino = 0, prev_value = 0;
  static uint32_t maxerino_pos = 0, sample_counter = 0;
  float bpm = 0;

  /* Infinite loop */
  for(;;)
  {
    /* Get lead I data (or any other lead) */
    event = osMessageGet(queue_bpm_signalHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      ecg_lead = (int32_t) event.value.v;
    }

    /* If signal crosses certain threshold */
    if (ecg_lead > threshold)
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_SET);
    }
    else
    {
      HAL_GPIO_WritePin(GPIOC, UI_LED_G_Pin, GPIO_PIN_RESET);
    }

    /* BPM detection in progress */
    if(ecg_lead > threshold)
    {
      if (ecg_lead > maxerino)
      {
        maxerino =         ecg_lead;
        maxerino_pos =     sample_counter;
      }
    }
    else if (prev_value > threshold)
    {
      bpm =              ((60 * FS) << 10 ) / maxerino_pos;
      sample_counter =   sample_counter - maxerino_pos - 1;
      threshold =        (threshold + maxerino) >> 1;
      maxerino =         0;
      ecg_lead =         0;

      /* Output data to queue */
      osMessagePut(queue_ecg_bpmHandle, (uint32_t) bpm, 0);
    }
    else
    {
      threshold = ((threshold > 1) ? ((threshold*511)>>9): 1);
    }

    sample_counter++;
    prev_value = ecg_lead;
  }
  /* USER CODE END Start_ecg_bpmDetTask */
}

/* Start_ecg_keyGenTask function */
void Start_ecg_keyGenTask(void const * argument)
{
  /* USER CODE BEGIN Start_ecg_keyGenTask */
  osEvent event;
  uint32_t bpm = 0;
  char data[5];
  /* Infinite loop */
  for(;;)
  {
    /* Get bpm data */
    event = osMessageGet(queue_ecg_bpmHandle, osWaitForever);
    if (event.status == osEventMessage)
    {
      /* Retrieve value */
      bpm = (uint32_t) event.value.v;
      sprintf(data,"%d",bpm);
      SendData(5,(unsigned char*)data);
    }

    /* Security magic */

  }
  /* USER CODE END Start_ecg_keyGenTask */
}

/* Start_periph_buttonTask function */
void Start_periph_buttonTask(void const * argument)
{
  /* USER CODE BEGIN Start_periph_buttonTask */
  GPIO_InitTypeDef GPIO_InitStruct;

  lcd_config_t lcd_config;
  lcd_item_t lcd_config_item;

  uint8_t is_lcd_on = 0;
  /* Take both semaphores for the first time */
  osSemaphoreWait(sem_periph_button_short_pressHandle, osWaitForever);
  osSemaphoreWait(sem_periph_button_long_pressHandle, osWaitForever);

  /* Associate config item */
  lcd_item_set(&lcd_config_item, (void *) &lcd_config, lcd_set_config);

  /* Infinite loop */
  for(;;)
  {
    /* If I/O button is pressed */
    if (osSemaphoreWait(sem_periph_button_short_pressHandle, osWaitForever) == osOK)
    {
      /* Make falling edge sensitive */
      GPIO_InitStruct.Pin = SYS_WKUP_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(SYS_WKUP_GPIO_Port, &GPIO_InitStruct);

      /* Wait for falling edge */
      if (osSemaphoreWait(sem_periph_button_long_pressHandle, 2000) == osErrorOS)
      {
        /* If falling edge is NOT detected before timeout
         * we have a long press
         */

        // Sleep well little prince
        /* Turn off LED */
        HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);

        /* Disable RTC alarms because they wake the system up */
        __HAL_RTC_WRITEPROTECTION_DISABLE(&hrtc);
        __HAL_RTC_ALARMA_DISABLE(&hrtc);

        /* Enable wakeup pin and go to sleep */
        HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
        __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
        HAL_PWR_EnterSTANDBYMode();
      }

      /* Make rising edge sensitive again */
      GPIO_InitStruct.Pin = SYS_WKUP_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(SYS_WKUP_GPIO_Port, &GPIO_InitStruct);

      /* If falling edge is detected before timeout
       * we have a short press
       */

      /* Do something */

      //TO-DO: Do it through the interpreter!

      /* If lcd is on */
      if (is_lcd_on)
      {
        /* Turn it off */
        lcd_config.backlight_level = 0;
        osMailPut(queue_lcdHandle, (void *) &lcd_config_item);

        is_lcd_on = 0;
      }
      /* If it is off */
      else
      {
        /* Turn it on, turn it on again */
        lcd_config.backlight_level = lcd.backlight_level;
        osMailPut(queue_lcdHandle, (void *) &lcd_config_item);

        is_lcd_on = 1;
      }
    }
  }
  /* USER CODE END Start_periph_buttonTask */
}

/* Start_periph_batteryTask function */
void Start_periph_batteryTask(void const * argument)
{
  /* USER CODE BEGIN Start_periph_batteryTask */

  /* Reset semaphore */
  osSemaphoreWait(sem_periph_batteryHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* Wait for gauge or charger to inform of changes in the battery */
    if (osSemaphoreWait(sem_periph_batteryHandle, osWaitForever) == osOK)
    {
      /* Read gauge values */

      /* Read charger values */

      /* Do something */

    }
  }
  /* USER CODE END Start_periph_batteryTask */
}

/* Start_periph_buzzerTask function */
void Start_periph_buzzerTask(void const * argument)
{
  /* USER CODE BEGIN Start_periph_buzzerTask */
  osEvent event;
  buzzer_note_t *current_note;

  /* HOW TO beep beep
   *
      buzzer_note_t beep;
      beep.note = A5;
      beep.ms = 50;
      osMailPut(buzzerHandle, (void *) &beep);
   *
   * from any task
   */
  for(;;)
  {
    event = osMailGet(queue_periph_buzzerHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get note */
      current_note = (buzzer_note_t *) event.value.p;
      /* Play note */
      buzzer_play(&buzzer, current_note);
    }
  }
  /* USER CODE END Start_periph_buzzerTask */
}

/* Start_input_touchTask function */
void Start_input_touchTask(void const * argument)
{
  /* USER CODE BEGIN Start_input_touchTask */
  uint8_t is_first_click = 1;

  lcd_pos_t current_lcd_pos;
  click_t click;

  /* Infinite loop */
  for(;;)
  {
      if (osSemaphoreWait(sem_input_touch_penHandle, osWaitForever) == osOK)
      {
        /* Mask PEN interrupts */
        EXTI->IMR &= (~TP_PEN_Pin);

        /* A finger is touching the screen.
         * Keep reading values until finger is lifted */
        do
        {
          /* Read value */
          touch_read_position(&touch);
          current_lcd_pos = touch_get_lcd_pos(&touch);

          /* If we detect a touch with enough pressure */
          if (touch.is_clicked == T_CLICKED)
          {
            /* Prepare click position */
            click.pos.x_pos = current_lcd_pos.x_pos;
            click.pos.y_pos = current_lcd_pos.y_pos;

            /* If it is the first one (the first click) */
            if (is_first_click)
            {
              /* A finger just pressed the screen
               * with enough pressure!
               */
              click.click_type = CLICK_DOWN;
              osMailPut(queue_input_clickHandle, (void *) &click);

              /* No longer detecting the first click */
              is_first_click = 0;
            }
            /* If it is not the first one (it keeps pressing the screen) */
            else
            {
              /* Finger is still pressing the screen!
               */
              click.click_type = CLICK_HOLD;
              osMailPut(queue_input_clickHandle, (void *) &click);
            }
          }
          /* Finger detection sample interval */
          osDelay(50);
        }
        while (HAL_GPIO_ReadPin(TP_PEN_GPIO_Port, TP_PEN_Pin) == GPIO_PIN_RESET);
        /* Keep polling until PEN signal goes high */

        /* If we have detected at least one valid click */
        if (!is_first_click)
        {
          /* Finger is lifted from the screen!
           */
          click.click_type = CLICK_UP;
          osMailPut(queue_input_clickHandle, (void *) &click);

          /* Set last position as not clicked */
          touch.is_clicked = T_NOT_CLICKED;
          touch.current_pos.pressure = 0;
        }

        /* Reset first click status */
        is_first_click = 1;

        /* Unmask PEN interrupts */
        EXTI->IMR |= TP_PEN_Pin;
      }
  }
  /* USER CODE END Start_input_touchTask */
}

/* Start_input_clickTask function */
void Start_input_clickTask(void const * argument)
{
  /* USER CODE BEGIN Start_input_clickTask */
  osEvent event;
  click_t *click;

  buzzer_note_t beep;

  /* Infinite loop */
  for(;;)
  {
    event = osMailGet(queue_input_clickHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get click position */
      click = (click_t *) event.value.p;

      /* Beep, search through clickable objects, etc. */
      switch (click->click_type)
      {
        case CLICK_DOWN:
          break;
        case CLICK_HOLD:
          break;
        case CLICK_UP:
          /* Beep */
          beep.note = A5;
          beep.ms = 50;
          osMailPut(queue_periph_buzzerHandle, (void *) &beep);

          /* Search for command */

          /* Send command */

          break;
        default:
          break;
      }
    }
  }
  /* USER CODE END Start_input_clickTask */
}

/* Start_bt_txTask function */
void Start_bt_txTask(void const * argument)
{
  /* USER CODE BEGIN Start_bt_txTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_bt_txTask */
}

/* Start_periph_screenTask function */
void Start_periph_screenTask(void const * argument)
{
  /* USER CODE BEGIN Start_periph_screenTask */
  osEvent event;
  lcd_item_t *current_item;

  for(;;)
  {
    event = osMailGet(queue_lcdHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get item information */
      current_item = (lcd_item_t *) event.value.p;
      /* Do item function */
      (*(current_item->item_print_function))(&lcd, current_item->item);
    }
  }

  /* USER CODE END Start_periph_screenTask */
}

/* Start_sys_interpreterTask function */
void Start_sys_interpreterTask(void const * argument)
{
  /* USER CODE BEGIN Start_sys_interpreterTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_sys_interpreterTask */
}

/* USER CODE BEGIN Application */
void Start_init_Task(void const * argument)
{
  bluetooth_init();
  afe_init(&afe, &hspi1);
//  gauge_init(&gauge, &hi2c1);

  buzzer_init(&buzzer, &htim3, &htim4);
//  lcd_init(&lcd, &hsram1, LCD_REG, LCD_DATA, LCD_X_SIZE, LCD_Y_SIZE);
//  touch_init(&touch, &hspi2, TOUCH_X_SIZE, TOUCH_Y_SIZE);

  afe_test_signal_on(&afe);

  fsm_client_init(&fsm);

  //TEST FSM
  fsm_ecg(&fsm);
  fsm_ecg_6_lead(&fsm);
  fsm_back(&fsm);
  fsm_h2h(&fsm);
  fsm_h2h_connect(&fsm);
  fsm_h2h_error(&fsm);
  fsm_back(&fsm);
  fsm_back(&fsm);

  while( osThreadTerminate (initTaskHandle) != osOK){
    osDelay(1);
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
