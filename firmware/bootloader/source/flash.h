#ifndef FLASH_H
#define FLASH_H

#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash.h"
#include "uart.h"
#include <stdint.h>

#define FW_VERSION_ADDRESS 0x08008400

const char *FLASH_GetAppVersion(void);
void FLASH_Erase(void);
uint8_t FLASH_Write(uint32_t address, uint8_t *data, size_t length);

#endif