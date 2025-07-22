#ifndef SPI_H
#define SPI_H

#include "driver/spi_master.h"
#include "driver/dedic_gpio.h"
#include "esp_log.h"

#define PIN_NUM_MOSI 11
#define PIN_NUM_MISO -1   // Not used now 13
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10

void SPI_Init(spi_device_handle_t * spi);
esp_err_t SPI_Transmit(const uint8_t* data, size_t len);

#endif