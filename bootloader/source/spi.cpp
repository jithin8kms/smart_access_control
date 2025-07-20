
#include "spi.h"

SPI_HandleTypeDef *spi1 = NULL;
uint8_t buffer[256];

void SPI_Init(SPI_HandleTypeDef *self_spi1)
{
  spi1 = self_spi1;
}

void SPI_Receive(void)
{
  uint8_t buffer[7] = {0};
  HAL_SPI_Receive(spi1, buffer, 7, HAL_MAX_DELAY);

  /*
  switch (cmd)
  {
  case 0xA5:
    break;

  default:
    break;
  }
    */
  SendMessage((const uint8_t *)buffer,7);
}