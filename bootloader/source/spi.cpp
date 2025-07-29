
#include "spi.h"
#include <cstdio>

SPI_HandleTypeDef *spi1 = NULL;

uint8_t buffer[BUFFER_SIZE];
uint8_t app_binary_buffer[APP_FLASH_BUFFER];
uint32_t app_buffer_next_addr = 0;
uint8_t buffer_rx_len = 1;
uint8_t write_to_flash_flag = false;
uint32_t firmware_size = 0;
uint32_t rx_firmware_crc = 0;

func_ptr SPI_Init(SPI_HandleTypeDef *self_spi1)
{
  spi1 = self_spi1;
  HAL_SPI_Receive_IT(spi1, buffer, buffer_rx_len); // Start receiving 64 bytes
  memset(app_binary_buffer, 0xFF, sizeof(app_binary_buffer));
  return SPI_FlashApp;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *spi)
{
  if (spi->Instance == SPI1)
  {
    SPI_ProcessData();
    HAL_SPI_Receive_IT(spi1, buffer, buffer_rx_len);
  }
}

void SPI_ProcessData()
{
  switch (buffer[CMD_OFFSET])
  {
  case CMD_FIRMWARE_UPDATE:

    // prepare to receiv
    app_buffer_next_addr = 0;
    UART_LOG((const uint8_t *)"START", 5);
    buffer_rx_len = BUFFER_SIZE;
    break;

  case CMD_FIRMWARE_PAYLOAD:

    if (SPI_CheckCrc(buffer) == true)
    {
      memcpy(&app_binary_buffer[app_buffer_next_addr], &buffer[PAYLOAD_OFFSET], buffer[LEN_OFFSET]);
      app_buffer_next_addr += buffer[LEN_OFFSET];
    }
    else
    {
      // sendCRC error;
    }

    // UART_LOG((const uint8_t *)"RX", 2);
    break;

  case CMD_FIRMWARE_END:
    // UART_LOG((const uint8_t *)"END", 3);
    firmware_size = read_u32_le(&buffer[FIRMWARE_SIZE_OFFSET]);
    rx_firmware_crc = read_u32_le(&buffer[FIRMWARE_CRC_OFFSET]);

    if (SPI_ValidateFirmware())
    {
      write_to_flash_flag = true;
      UART_LOG((const uint8_t *)"FIRMWARE VALID", 14);
    }
    else
    {
      UART_LOG((const uint8_t *)"FIRMWARE INVALID", 16);
    }
    break;
    // end
  default:
    break;
  }
}
uint8_t SPI_FlashApp(void)
{
  uint8_t succes_flag = 2; // flash write not started
  if (write_to_flash_flag)
  {
    succes_flag = FLASH_Write(APP_FLASH_START_ADDRESS, app_binary_buffer, APP_FLASH_BUFFER);
    write_to_flash_flag = 0;
  }
  return succes_flag;
}

uint8_t SPI_CheckCrc(uint8_t *data_frame)
{
  uint32_t calc_crc = SPI_SoftwareCrc32((uint8_t *)data_frame, 66);
  // Extract the last 4 bytes as received CRC
  uint32_t rx_crc = read_u32_le(&data_frame[CRC_1_OFFSET]);
  return (rx_crc == calc_crc);
}

uint32_t SPI_SoftwareCrc32(const uint8_t *data, size_t len)
{
  uint32_t crc = CRC32_INIT;

  for (size_t i = 0; i < len; ++i)
  {
    crc ^= data[i];
    for (int j = 0; j < 8; ++j)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ CRC32_POLY;
      else
        crc >>= 1;
    }
  }

  return crc ^ CRC32_FINAL_XOR;
}

uint8_t SPI_ValidateFirmware(void)
{
  uint32_t calc_fw_crc = SPI_SoftwareCrc32(app_binary_buffer, firmware_size);
  return (rx_firmware_crc == calc_fw_crc);
}

uint32_t read_u32_le(const uint8_t *src)
{
  return ((uint32_t)src[0]) |
         ((uint32_t)src[1] << 8) |
         ((uint32_t)src[2] << 16) |
         ((uint32_t)src[3] << 24);
}