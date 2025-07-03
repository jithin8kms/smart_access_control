#include "FreeRTOS.h"
#include "task.h"

#include <chip_init.h>

extern "C" void SystemInit(void);

 void blink_task(void* arg) {
    while (1) {
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}



int main() {
    HAL_Init();
    CHIP_SystemClockConfig(); 
    CHIP_GpioInit();  
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    xTaskCreate(blink_task, "Blink", 256, nullptr, 1, nullptr);
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