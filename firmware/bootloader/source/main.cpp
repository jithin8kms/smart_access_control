#include "chip_init.h"
#include "uart.h"
#include "spi.h"
#include <string.h>
#include "flash.h"

#define BOOTLOADER_MAGIC 0xDEADBEEF
#define BOOTLOADER_FLAG_ADDR 0x2001FFF0
#define APP_START_ADDRESS 0x08008000

extern "C" void SystemInit(void);

UART_HandleTypeDef uart3;
SPI_HandleTypeDef hspi1;

typedef void (*AppEntryFunc)(void);

void JumpToApplication(void)
{
	uint32_t appStack = *((volatile uint32_t *)APP_START_ADDRESS);
	uint32_t appEntry = *((volatile uint32_t *)(APP_START_ADDRESS + 4));

	// De-init all peripherals
	HAL_RCC_DeInit();
	HAL_DeInit();

	// Disable SysTick
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	// Set vector table base address to app
	SCB->VTOR = APP_START_ADDRESS;

	// Set MSP
	__set_MSP(appStack);

	// Jump to app
	AppEntryFunc app = (AppEntryFunc)appEntry;
	app(); // never returns
}

void InitAllModules(void)
{
	HAL_Init();
	CHIP_SystemClockConfig();
	CHIP_GpioInit();

	CHIP_Uart3Init(&uart3);
	CHIP_Spi1Init(&hspi1);
	UART_Init(&uart3);
}

void PrintAppVersion()
{
	const char *app_ver = FLASH_GetAppVersion();
	UART_LOG((const uint8_t *)app_ver, (uint16_t)(strlen(app_ver)));
	HAL_Delay(10);
}

void FirmwareUpdateLoop(void)
{
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
			HAL_Delay(10);

			PrintAppVersion();
			JumpToApplication();
		}
		else
		{
		}
	}
}

int main()
{
	InitAllModules();
	UART_LOG((const uint8_t *)"Bootloader active", 17);
	HAL_Delay(10);

	uint32_t boot_flag = *((volatile uint32_t *)BOOTLOADER_FLAG_ADDR);

	if (boot_flag != BOOTLOADER_MAGIC)
	{
		PrintAppVersion();
		JumpToApplication(); // go to app
	}

	*((volatile uint32_t *)BOOTLOADER_FLAG_ADDR) = 0; // clear flag

	// Wait for firmware update
	FirmwareUpdateLoop();

	// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);° // before Receive
	// HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

	return 0;
}
