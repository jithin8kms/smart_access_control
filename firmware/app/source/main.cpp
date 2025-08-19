#include "FreeRTOS.h"
#include "task.h"

#include "chip_init.h"
#include "uart.h"
#include <string.h>
#include "access_control.h"
#include "watchdog.h"

extern "C" void SystemInit(void);
UART_HandleTypeDef uart3;

const char fw_version[] __attribute__((section(".app_version"))) = FW_VERSION; 

void blink_task(void *arg)
{
	while (1)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

int main()
{
	HAL_Init();
	CHIP_SystemClockConfig();
	CHIP_GpioInit();
	// WTDG_Init();
	CHIP_Uart3Init(&uart3);
	UART_Init(&uart3);
	ACTRL_Init();

	// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);° // before Receive
	// HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	// xTaskCreate(UART_UartRxTask, "Uart_RX", 128, nullptr, 1, nullptr);

	xTaskCreate(blink_task, "Blink", 256, nullptr, 3, nullptr);

	vTaskStartScheduler();
	while (1)
		; // Should never reach here
}