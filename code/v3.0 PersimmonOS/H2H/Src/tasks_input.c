#include "tasks_input.h"

/* Semaphores */
osSemaphoreId sem_input_touch_penHandle;

/* Queues */
osMailQId queue_input_clickHandle;
extern osMailQId queue_periph_buzzerHandle;

/* Tasks */
osThreadId input_touchTaskHandle;
osThreadId input_clickTaskHandle;
void Start_input_touchTask(void const * argument);
void Start_input_clickTask(void const * argument);

/* Objects */
extern touch_t touch;

void tasks_input_init()
{
  /* Semaphores */
  /* sem_input_touch_pen */
  osSemaphoreDef(sem_input_touch_pen);
  sem_input_touch_penHandle = osSemaphoreCreate(osSemaphore(sem_input_touch_pen), 1);

  /* Queues */
  /* queue_input_click */
  osMailQDef(queue_input_click, 4, click_t);
  queue_input_clickHandle = osMailCreate(osMailQ(queue_input_click), NULL);

  /* Tasks */
  /* input_touchTask */
  osThreadDef(input_touchTask, Start_input_touchTask, osPriorityAboveNormal, 0, 64);
  input_touchTaskHandle = osThreadCreate(osThread(input_touchTask), NULL);

  /* input_clickTask */
  osThreadDef(input_clickTask, Start_input_clickTask, osPriorityAboveNormal, 0, 64);
  input_clickTaskHandle = osThreadCreate(osThread(input_clickTask), NULL);
}

void Start_input_touchTask(void const * argument)
{
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
}

void Start_input_clickTask(void const * argument)
{
  osEvent event;

  click_t *click;
  buzzer_note_t beep;

  /* Infinite loop */
  for(;;)
  {
    /* Get click */
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
}