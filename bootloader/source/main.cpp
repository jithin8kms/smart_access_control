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

	SendMessage((const uint8_t *)"Bootloader active", 17);

	// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);° // before Receive
	// HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	SPI_Init(&hspi1);

	uint8_t dummy_data[256];
	for (int i = 0; i < 256; ++i)
	{
		dummy_data[i] = i; // Fills with 0x00, 0x01, ..., 0xFF
	}
	// FLASH_Write(APP_START_ADDRESS, dummy_data, 256);

	return 0;
}
