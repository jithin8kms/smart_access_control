#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "uart.h"
#include "flash.h"
#include <stdint.h>

#define APP_FLASH_START_ADDRESS 0x08008000

#define BUFFER_SIZE 70 // [cmd + len + payload + crc] [1+1+64+4]
#define APP_FLASH_BUFFER 30000
#define CMD_OFFSET 0
#define LEN_OFFSET 1
#define PAYLOAD_OFFSET 2
#define FIRMWARE_SIZE_OFFSET 2
#define FIRMWARE_CRC_OFFSET 6
#define MAX_PAYLOAD_LENGTH 64
#define CRC_1_OFFSET (BUFFER_SIZE - 4)
#define CRC_2_OFFSET (BUFFER_SIZE - 3)
#define CRC_3_OFFSET (BUFFER_SIZE - 2)
#define CRC_4_OFFSET (BUFFER_SIZE - 1)

#define CMD_FIRMWARE_UPDATE 0xA0
#define CMD_FIRMWARE_PAYLOAD 0xA1
#define CMD_FIRMWARE_END 0xA2

#define CRC32_POLY 0xEDB88320 // reversed 0x04C11DB7 (little-endian)
#define CRC32_INIT 0xFFFFFFFF
#define CRC32_FINAL_XOR 0xFFFFFFFF

typedef uint8_t (*func_ptr)(void);

func_ptr SPI_Init(SPI_HandleTypeDef *spi1);
void SPI_ProcessData(void);
uint8_t SPI_FlashApp(void);
uint8_t SPI_CheckCrc(uint8_t *data_frame);
uint32_t SPI_SoftwareCrc32(const uint8_t *data, size_t len);
uint8_t SPI_ValidateFirmware(void);
uint32_t read_u32_le(const uint8_t *src);
#endif
