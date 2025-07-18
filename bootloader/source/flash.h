#ifndef FLASH_H
#define FLASH_H

#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash.h"
#include "uart.h"
#include <stdint.h>

void FLASH_Erase(void);
void FLASH_Write(uint32_t address, uint8_t *data, size_t length);

#endif