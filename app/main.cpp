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
