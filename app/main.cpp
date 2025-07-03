#include "FreeRTOS.h"
#include "task.h"

#include "chip_init.h"
#include "uart.h"
#include <string.h>

extern "C" void SystemInit(void);
UART_HandleTypeDef uart3; 

void blink_task(void* arg) 
{
	while (1)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void uart_echo_task(void* arg) {
    uint8_t ch;
    while (1) {
        HAL_StatusTypeDef status = HAL_UART_Receive(&uart3, &ch, 1, 5000);
        if (status == HAL_OK) {
            HAL_UART_Transmit(&uart3, &ch, 1, 5000);
        }
    }
}

int main() 
{
	HAL_Init();
	CHIP_SystemClockConfig(); 
	CHIP_GpioInit();

	CHIP_Uart3Init(&uart3);
	//UART_Init(&uart3);

	//HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // before Receive
	//HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);



	//xTaskCreate(UART_UartRxTask, "Uart_RX", 128, nullptr, 1, nullptr);
	
	xTaskCreate(blink_task, "Blink", 256, nullptr, 2, nullptr);
	xTaskCreate(uart_echo_task, "echo", 256, nullptr, 1, nullptr);

	vTaskStartScheduler();
	while (1);  // Should never reach here
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}