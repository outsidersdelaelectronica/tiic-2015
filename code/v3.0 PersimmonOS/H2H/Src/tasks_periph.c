#include "tasks_periph.h"

/* Semaphores */
osSemaphoreId sem_periph_button_short_pressHandle;
osSemaphoreId sem_periph_button_long_pressHandle;
osSemaphoreId sem_periph_batteryHandle;
osSemaphoreId sem_periph_gauge_dma_rxHandle;
osSemaphoreId sem_periph_rtcHandle;

/* Queues */
osMailQId queue_periph_buzzerHandle;
osMailQId queue_lcdHandle;

/* Tasks */
osThreadId periph_buttonTaskHandle;
osThreadId periph_batteryTaskHandle;
osThreadId periph_buzzerTaskHandle;
osThreadId periph_screenTaskHandle;
osThreadId periph_rtcTaskHandle;
void Start_periph_buttonTask(void const * argument);
void Start_periph_batteryTask(void const * argument);
void Start_periph_buzzerTask(void const * argument);
void Start_periph_screenTask(void const * argument);
void Start_periph_rtcTask(void const * argument);

/* Objects */
extern RTC_HandleTypeDef hrtc;
extern buzzer_t buzzer;
extern gauge_t gauge;
extern lcd_t lcd;

void tasks_periph_init()
{
  /* Semaphores */
  /* sem_periph_button_short_press */
  osSemaphoreDef(sem_periph_button_short_press);
  sem_periph_button_short_pressHandle = osSemaphoreCreate(osSemaphore(sem_periph_button_short_press), 1);

  /* sem_periph_button_long_press */
  osSemaphoreDef(sem_periph_button_long_press);
  sem_periph_button_long_pressHandle = osSemaphoreCreate(osSemaphore(sem_periph_button_long_press), 1);

  /* sem_periph_battery */
  osSemaphoreDef(sem_periph_battery);
  sem_periph_batteryHandle = osSemaphoreCreate(osSemaphore(sem_periph_battery), 1);

  /* sem_periph_gauge_dma_rx */
  osSemaphoreDef(sem_periph_gauge_dma_rx);
  sem_periph_gauge_dma_rxHandle = osSemaphoreCreate(osSemaphore(sem_periph_gauge_dma_rx), 1);

  /* sem_periph_rtc */
  osSemaphoreDef(sem_periph_rtc);
  sem_periph_rtcHandle = osSemaphoreCreate(osSemaphore(sem_periph_rtc), 1);

  /* Queues */
  /* queue_periph_buzzer */
  osMailQDef(queue_periph_buzzer, 4, buzzer_note_t);
  queue_periph_buzzerHandle = osMailCreate(osMailQ(queue_periph_buzzer), NULL);

  /* queue_lcd */
  osMailQDef(queue_lcd, 12, item_action_t);
  queue_lcdHandle = osMailCreate(osMailQ(queue_lcd), NULL);
}

void tasks_periph_start()
{
  /* Tasks */
  /* periph_buttonTask */
  osThreadDef(periph_buttonTask, Start_periph_buttonTask, osPriorityHigh, 0, 128);
  periph_buttonTaskHandle = osThreadCreate(osThread(periph_buttonTask), NULL);

  /* periph_batteryTask */
  osThreadDef(periph_batteryTask, Start_periph_batteryTask, osPriorityLow, 0, 128);
  periph_batteryTaskHandle = osThreadCreate(osThread(periph_batteryTask), NULL);

  /* periph_buzzerTask */
  osThreadDef(periph_buzzerTask, Start_periph_buzzerTask, osPriorityNormal, 0, 64);
  periph_buzzerTaskHandle = osThreadCreate(osThread(periph_buzzerTask), NULL);

  /* periph_screenTask */
  osThreadDef(periph_screenTask, Start_periph_screenTask, osPriorityNormal, 0, 256);
  periph_screenTaskHandle = osThreadCreate(osThread(periph_screenTask), NULL);

  /* periph_rtcTask */
  osThreadDef(periph_rtcTask, Start_periph_rtcTask, osPriorityLow, 0, 128);
  periph_rtcTaskHandle = osThreadCreate(osThread(periph_rtcTask), NULL);
}

void Start_periph_buttonTask(void const * argument)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  item_action_t lcd_config;

  uint8_t is_lcd_on = 0;

  /* Take both semaphores for the first time */
  osSemaphoreWait(sem_periph_button_short_pressHandle, osWaitForever);
  osSemaphoreWait(sem_periph_button_long_pressHandle, osWaitForever);

  /* Create config item */
  item_lcd_config_init(&lcd_config.item.config, 0);
  lcd_config.item_print_function = lcd_set_config;

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
      /* If lcd is on */
      if (is_lcd_on)
      {
        /* Turn it off */
        lcd_config.item.config.backlight_level = 0;
        osMailPut(queue_lcdHandle, (void *) &lcd_config);

        is_lcd_on = 0;
      }
      /* If it is off */
      else
      {
        /* Turn it on, turn it on again */
        lcd_config.item.config.backlight_level = lcd.backlight_level;
        osMailPut(queue_lcdHandle, (void *) &lcd_config);

        is_lcd_on = 1;
      }
    }
  }
}

void Start_periph_batteryTask(void const * argument)
{
  char batt_soc_string[5];

  /* Reset semaphore */
  osSemaphoreWait(sem_periph_gauge_dma_rxHandle, osWaitForever);

  /* Infinite loop */
  for(;;)
  {
    /* Wait for gauge or charger to inform of changes in the battery */
    if (osSemaphoreWait(sem_periph_batteryHandle, osWaitForever) == osOK)
    {
      /* Read gauge values */
      gauge_start_read(&gauge);

      /* Wait for DMA to complete transaction */
      if (osSemaphoreWait(sem_periph_gauge_dma_rxHandle, osWaitForever) == osOK)
      {
        /* Format received bytes into usable data */
        gauge_format_data(&gauge);
      }

      /* Read charger values */

      /* Display battery status */
        /* Create batt soc string */
        sprintf(batt_soc_string, "%u%%", gauge.last_data.soc);
        item_area_set_text(&menu_top_bar.items[2].item.area, batt_soc_string);

        /* Create batt icon */
        if (gauge.last_data.soc < 5)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF244");
        }
        else if (gauge.last_data.soc < 25)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF243");
        }
        else if (gauge.last_data.soc < 50)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF242");
        }
        else if (gauge.last_data.soc < 75)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF241");
        }
        else if (gauge.last_data.soc <= 100)
        {
          item_area_set_text_16(&menu_top_bar.items[1].item.area, L"\xF240");
        }

        /* Display screen items */
        osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[1]);
        osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[2]);
    }
  }
}

void Start_periph_buzzerTask(void const * argument)
{
  osEvent event;
  buzzer_note_t *current_note;

  /* HOW TO beep beep
   *
   *  buzzer_note_t beep;
   *  beep.note = A5;
   *  beep.ms = 50;
   *  osMailPut(queue_periph_buzzerHandle, (void *) &beep);
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
}

void Start_periph_screenTask(void const * argument)
{
  osEvent event;

  item_action_t *current_item;

  for(;;)
  {
    event = osMailGet(queue_lcdHandle, osWaitForever);
    if (event.status == osEventMail)
    {
      /* Get item information */
      current_item = (item_action_t *) event.value.p;
      /* Do item function */
      (*(current_item->item_print_function))(&lcd, &current_item->item);
    }
  }
}

void Start_periph_rtcTask(void const * argument)
{
  /* RTC time and date structures */
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  /* Time and date strings */
  char time_string[6];
  char date_string[11];

  /* Infinite loop */
  for(;;)
  {
    /* When rtc alarm rings */
    if (osSemaphoreWait(sem_periph_rtcHandle, osWaitForever) == osOK)
    {
      /* Get time and date from rtc calendar */
      HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
      HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

      /* Create time and date strings */
      if (sTime.Hours < 10)
      {
        if (sTime.Minutes < 10)
        {
          sprintf(time_string, "0%u:0%u", sTime.Hours, sTime.Minutes);
        }
        else
        {
          sprintf(time_string, "0%u:%u", sTime.Hours, sTime.Minutes);
        }
      }
      else
      {
        if (sTime.Minutes < 10)
        {
          sprintf(time_string, "%u:0%u", sTime.Hours, sTime.Minutes);
        }
        else
        {
          sprintf(time_string, "%u:%u", sTime.Hours, sTime.Minutes);
        }
      }
      sprintf(date_string, "%u/%u/20%u", sDate.Date, sDate.Month, sDate.Year);

      /* Update screen items */
      item_area_set_text(&menu_top_bar.items[4].item.area, time_string);
      item_area_set_text(&menu_top_bar.items[3].item.area, date_string);

      /* Draw time and date on screen */
      osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[4]);
      osMailPut(queue_lcdHandle, (void *) &menu_top_bar.items[3]);
    }
  }
}