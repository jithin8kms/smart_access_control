#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "uart.h"
#include "flash.h"
#define BUFFER_SIZE 68 // [cmd + len + payload + crc1 + crc2] [1+1+64+1+1]
#define APP_FLASH_BUFFER 256
#define APP_FLASH_START_ADDRESS 0x08008000

typedef uint8_t (*func_ptr)(void);

func_ptr SPI_Init(SPI_HandleTypeDef *spi1);
void SPI_ProcessData(void);
uint8_t SPI_FlashApp(void);
#endif
