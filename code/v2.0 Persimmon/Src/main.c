/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
#include "stm32l1xx_hal.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* USER CODE BEGIN Includes */
#include "afe.h"
#include "gauge.h"
#include "lcd.h"
#include "touch.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define QUARTER 350
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void two_secs_wakeup(void);
void note_flash(uint16_t dur , uint16_t frec);
void epic_sax_guy(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
extern SRAM_HandleTypeDef hsram1;
extern color_t background_color;

uint16_t fg_soc;
touch_pos_t last_pos;
char bat_soc[4];

char x_pos[5];
char y_pos[5];
char pressure[5];

#define LCD_REG        ((uint32_t *)(FSMC_BASE))
#define LCD_DATA       ((uint32_t *)(FSMC_BASE + 0x00020000U))  //See p.620 of STM32L162VD ref. manual
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */
    uint8_t level = 80;
    color_t dot_color;
    color_t text_color;
    uint8_t cntl_byte = 0;

    uint16_t voltage, temperature, int_temp;
    int16_t avg_current, max_current;
    
    afe_init();
    lcd_init();
    lcd_set_brightness(level);
    fg_init();
    touch_init();
    
    background_color = (color_t) COLOR_BLACK;
    dot_color = (color_t) COLOR_WHITE;
    text_color = (color_t) COLOR_GREEN;
       
    char string[] = "60";

    fg_soc = fg_read_reg16(FG_SOC);
    /* Convert to string
     * http://www.keil.com/support/man/docs/c51/c51_sprintf.htm
     */
    sprintf(bat_soc, "%d", fg_soc);

    /* Draw it */
    lcd_draw_string(bat_soc, myriad_pro_semibold28x39_num, &dot_color, 470, 100);
    
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
      /* Fuel gauge data */
//      temperature = fg_read_reg16(FG_TEMP);
//      voltage = fg_read_reg16(FG_VOLT);
//      avg_current = fg_read_reg16(FG_AVG_CURRENT);
//      int_temp = fg_read_reg16(FG_INTERNAL_TEMP);
//      max_current = fg_read_reg16(FG_MAX_LOAD_CURRENT);
      
      /* Draw text */
//      lcd_draw_string("Ya no te pasas por el parque a", myriad_pro_semibold17x23, &text_color, 230, 240);
//      lcd_draw_string(string, myriad_pro_semibold28x39_num, &dot_color, 470, 230);
//      lcd_draw_string("bpm", myriad_pro_semibold17x23, &text_color, 510, 240);   
      
      touch_get_position(&last_pos);
      
      sprintf(x_pos, "%d", last_pos.x_pos);
      sprintf(y_pos, "%d", last_pos.y_pos);
      sprintf(pressure, "%d", last_pos.pressure);
      
      lcd_draw_string(x_pos, myriad_pro_semibold17x23, &text_color, 100, 160);
      lcd_draw_string(y_pos, myriad_pro_semibold17x23, &text_color, 250, 160);
      lcd_draw_string(pressure, myriad_pro_semibold17x23, &text_color, 400, 160);

      /* Wait and erase text */
      //HAL_GPIO_TogglePin(GPIOC, UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin);
//      HAL_Delay(500); 
//      lcd_delete_string(string, myriad_pro_semibold28x39_num, 470, 230);

      lcd_delete_string(x_pos, myriad_pro_semibold17x23, 100, 160);
      lcd_delete_string(y_pos, myriad_pro_semibold17x23, 250, 160);
      lcd_delete_string(pressure, myriad_pro_semibold17x23, 400, 160);
      
      /* Count */
//      if(string[1] < '9')
//      {
//        string[1] = string[1] + 1;
//      }
//      else
//      {
//        string[1] = '0';
//      }  
    }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

  HAL_RCC_EnableCSS();

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Implements the initial delay of 2secs for wake up .
  * @param  None
  * @retval None
*/
void two_secs_wakeup(void){
  // Checks if the system is resumed from standby
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_WU) != RESET)
  {
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
    wk_falling_edge_detection();
    HAL_Delay(2000);
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0) != RESET)
    {
      __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
      HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
      HAL_PWR_EnterSTANDBYMode();
    }
  }
  
}
/**
  * @brief  Activate PWM for note and 3 leds for white flashing.
  * @param  dur:duration of the note in ms.
  * @param  frec:frequency of the note in Hz.
  * @retval None
*/

void note_flash(uint16_t dur , uint16_t frec){   
  uint16_t period = 16000000/((htim3.Init.Prescaler + 1)*frec);
  htim3.Init.Period = period;
  HAL_TIM_PWM_Init(&htim3);
  HAL_GPIO_WritePin(GPIOC, nSHUTD_Pin|UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin, GPIO_PIN_SET);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_Delay(dur);
  HAL_GPIO_WritePin(GPIOC, nSHUTD_Pin|UI_LED_R_Pin|UI_LED_G_Pin|UI_LED_B_Pin, GPIO_PIN_RESET);
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
}

/**
  * @brief  Epic sax guy song.
  * @param  None
  * @retval None
*/    
void epic_sax_guy(void){
  
  int i = 0;
  for( i = 0; i < 2; i++){
    note_flash(QUARTER>>1, 784);
    HAL_Delay((QUARTER*3)>>1);
    note_flash(QUARTER>>1, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 784);
    HAL_Delay(50);
    note_flash(QUARTER>>2, 698);
    HAL_Delay(50);
    note_flash((QUARTER*3)>>2, 784);
    HAL_Delay(50);
  }
  note_flash(QUARTER>>1, 784);
  HAL_Delay(QUARTER);
  note_flash(QUARTER, 932);
  HAL_Delay(50);
  note_flash(QUARTER, 784);
  HAL_Delay(50);
  note_flash(QUARTER, 698);
  HAL_Delay(50);
  note_flash(QUARTER, 622);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 587);
  HAL_Delay(50);
  note_flash(QUARTER>>1, 622);
  HAL_Delay(50);    
  note_flash(QUARTER>>1, 523);
  HAL_Delay(50); 
  note_flash(QUARTER, 784);
}

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
