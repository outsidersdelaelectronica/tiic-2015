/**
  ******************************************************************************
  * File Name          : stm32l1xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
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
#include "stm32l1xx_hal.h"

#include "buzzer.h"
#include "cmsis_os.h"
#include "HCITRANS.h"

extern void Error_Handler(void);

extern buzzer_t buzzer;
extern xSemaphoreHandle DataReceivedEvent;

extern osSemaphoreId sem_ecg_afe_dma_rxHandle;
extern osSemaphoreId sem_ecg_afe_drdyHandle;

extern osSemaphoreId sem_input_touch_penHandle;

extern osSemaphoreId sem_periph_button_short_pressHandle;
extern osSemaphoreId sem_periph_button_long_pressHandle;
extern osSemaphoreId sem_periph_batteryHandle;

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_COMP_CLK_ENABLE();
   __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVC_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/**
  * @brief Tx and Rx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* AFE Rx complete */
  if(hspi->Instance == SPI1)
  {
    if(sem_ecg_afe_dma_rxHandle != NULL)
    {
      osSemaphoreRelease(sem_ecg_afe_dma_rxHandle);
    }
  }
}

/**
  * @brief  Output Compare callback in non blocking mode
  * @param  htim : TIM OC handle
  * @retval None
  */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4)
  {
    buzzer_stop(&buzzer);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* BT UART Rx complete */
  if (huart->Instance == USART1)
  {
    if(DataReceivedEvent != NULL){
      osSemaphoreRelease(DataReceivedEvent);
    }
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  /* BT UART error */
  if (huart->Instance == USART1)
  {
    HAL_GPIO_WritePin(GPIOC, UI_LED_R_Pin,GPIO_PIN_SET);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* AFE DRDY handler */
  if(GPIO_Pin == AFE_DRDY_Pin)
  {
    if(sem_ecg_afe_drdyHandle != NULL)
    {
      osSemaphoreRelease(sem_ecg_afe_drdyHandle);
    }
  }

  /* I/O button interrupt handler */
  if(GPIO_Pin == SYS_WKUP_Pin)
  {
    /* If rising edge */
    if ((EXTI->RTSR) & (SYS_WKUP_Pin))
    {
      if(sem_periph_button_short_pressHandle != NULL)
      {
        osSemaphoreRelease(sem_periph_button_short_pressHandle);
      }
    }
    /* If falling edge */
    if ((EXTI->FTSR) & (SYS_WKUP_Pin))
    {
      if(sem_periph_button_long_pressHandle != NULL)
      {
        osSemaphoreRelease(sem_periph_button_long_pressHandle);
      }
    }
  }

  /* Battery interrupt handlers */
  if((GPIO_Pin == FG_GPOUT_Pin) || (GPIO_Pin == CHRG_CHG_Pin))
  {
    if(sem_periph_batteryHandle != NULL)
    {
      osSemaphoreRelease(sem_periph_batteryHandle);
    }
  }

  /* Touch interrupt handler */
  if(GPIO_Pin == TP_PEN_Pin)
  {
    if(sem_input_touch_penHandle != NULL)
    {
      osSemaphoreRelease(sem_input_touch_penHandle);
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/