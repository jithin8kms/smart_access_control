#ifndef FIRMWARE_SENDER_H
#define FIRMWARE_SENDER_H

#include "spi.h"
#include "esp_log.h"
#include <string.h>

#define BUFFER_SIZE 70 // [cmd + len + payload + crc1 + crc2] [1+1+64+1+1]
#define CMD_OFFSET 0
#define LEN_OFFSET 1
#define PAYLOAD_OFFSET 2
#define FIRMWARE_SIZE_OFFSET 2
#define FIRMWARE_CRC_OFFSET 6  
#define SIG_FRAME_SIZE_OFFSET 10  
#define MAX_PAYLOAD_LENGTH 64
#define CRC_1_OFFSET (BUFFER_SIZE - 4)
#define CRC_2_OFFSET (BUFFER_SIZE - 3)
#define CRC_3_OFFSET (BUFFER_SIZE - 2)
#define CRC_4_OFFSET (BUFFER_SIZE - 1)

#define CMD_FIRMWARE_UPDATE 0xA0
#define CMD_FIRMWARE_PAYLOAD 0xA1
#define CMD_FIRMWARE_END 0xA2
#define CMD_SIG 0xA3
#define CMD_REQ_APP_VERSION 0xA4

#define CRC32_POLY 0xEDB88320 // reversed 0x04C11DB7 (little-endian)
#define CRC32_INIT 0xFFFFFFFF
#define CRC32_FINAL_XOR 0xFFFFFFFF

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

void FWSND_LocalInit();
void FWSND_ReqFwVersion();
void FWSND_SendFirmware();
void FWSND_AddCrc(uint8_t* data_frame);
uint32_t SPI_SoftwareCrc32(const uint8_t *data, size_t len);
void write_u32_le(uint8_t *dest, uint32_t value);
#endif