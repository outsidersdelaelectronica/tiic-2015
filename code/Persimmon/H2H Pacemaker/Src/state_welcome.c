#include "state_welcome.h"

/* Possible transition to the following states */
#include "state_running.h"

/* State includes */
#include "cmsis_os.h"
#include "menu.h"

/* State behaviour */
void behaviour_welcome(state_ptr state)
{
  uint32_t i;

  /* Set events to react to */

  /* Do state actions */

  /* Blink green pin */
  HAL_GPIO_WritePin(GPIOC,UI_LED_R_Pin|UI_LED_B_Pin|UI_LED_G_Pin,GPIO_PIN_RESET);
  for (i = 0; i < 4; i++)
  {
    HAL_GPIO_TogglePin(GPIOC,UI_LED_G_Pin);
    osDelay(200);
  }

  /* Turn on green LED */
  HAL_GPIO_WritePin(GPIOC,UI_LED_G_Pin,GPIO_PIN_SET);
  osDelay(100);

  entry_to_running(state);
}

/* Entry point to the state */
void entry_to_welcome(state_ptr state)
{
  /* Set state name */
  strcpy(state->name, "welcome");

  /* - Initialize with default implementation
   * - Set event behaviour
   * - Set parent events behaviour (bottom-up)
   */
  default_implementation(state);
  behaviour_welcome(state);
}