#include "chip_init.h"
#include "uart.h"
#include "spi.h"
#include <string.h>
#include "flash.h"

#define APP_START_ADDRESS 0x08008000

extern "C" void SystemInit(void);
UART_HandleTypeDef uart3;
SPI_HandleTypeDef hspi1;

int main()
{
	HAL_Init();
	CHIP_SystemClockConfig();
	CHIP_GpioInit();

	CHIP_Uart3Init(&uart3);
	CHIP_Spi1Init(&hspi1);
	UART_Init(&uart3);

	UART_LOG((const uint8_t *)"Bootloader active", 17);

	// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);° // before Receive
	// HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	func_ptr spi_flash_callback = SPI_Init(&hspi1);

	while (1)
	{
		uint8_t flash_write_success_flag = spi_flash_callback();
		if (flash_write_success_flag == 0)
		{
			UART_LOG((const uint8_t *)"Firmware update failed", 22);
		}
		else if (flash_write_success_flag == 1)
		{
			UART_LOG((const uint8_t *)"Firmware update success", 23);
			UART_LOG((const uint8_t *)"Switch to app", 13);
		}
		else
		{
		}
	}

	return 0;
}
