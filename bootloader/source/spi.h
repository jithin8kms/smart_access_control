#ifndef SPI_H
#define SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "uart.h"

void SPI_Init(SPI_HandleTypeDef *spi1);
void SPI_Receive(void);
#endif
