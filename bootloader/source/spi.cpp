
#include "spi.h"

SPI_HandleTypeDef *spi1 = NULL;

uint8_t buffer[BUFFER_SIZE];
uint8_t app_binary_buffer[APP_FLASH_BUFFER];
uint8_t app_buffer_next_addr = 0;
uint8_t buffer_rx_len = 1;
uint8_t write_to_flash_flag = false;

#define CMD_OFFSET 0
#define LEN_OFFSET 1
#define PAYLOAD_OFFSET 2
#define PAYLOAD_LENGTH 64
#define CRC_1_OFFSET (PAYLOAD_OFFSET + PAYLOAD_LENGTH)
#define CRC_2_OFFSET (CRC_1_OFFSET + 1)

#define CMD_FIRMWARE_UPDATE 0xA0
#define CMD_FIRMWARE_PAYLOAD 0xA1
#define CMD_FIRMWARE_END 0xA2

func_ptr SPI_Init(SPI_HandleTypeDef *self_spi1)
{
  spi1 = self_spi1;
  HAL_SPI_Receive_IT(spi1, buffer, buffer_rx_len); // Start receiving 64 bytes
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
    memcpy(&app_binary_buffer[app_buffer_next_addr], &buffer[PAYLOAD_OFFSET], buffer[LEN_OFFSET]);
    app_buffer_next_addr += PAYLOAD_LENGTH;
    UART_LOG((const uint8_t *)"RX", 2);
    UART_LOG((const uint8_t *)&app_binary_buffer[app_buffer_next_addr], 64);
    break;

  case CMD_FIRMWARE_END:
    UART_LOG((const uint8_t *)"END", 3);
    write_to_flash_flag = true;
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