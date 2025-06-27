#include "FreeRTOS.h"
#include "task.h"

#include <chip_init.h>

extern "C" void SystemInit(void);

 void blink_task(void* arg) {
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}



int main() {
    HAL_Init();
    CHIP_SystemClockConfig();
    CHIP_GpioInit();           

    xTaskCreate(blink_task, "Blink", 128, nullptr, 1, nullptr);
    vTaskStartScheduler();

    while (1);  // Should never reach here
}
