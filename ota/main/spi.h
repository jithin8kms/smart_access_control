#ifndef SPI_H
#define SPI_H

#include "driver/spi_master.h"
#include "driver/dedic_gpio.h"
#include "esp_log.h"

#define PIN_NUM_MOSI 11  // PA7 white
#define PIN_NUM_MISO -1   // Not used now 13 pA6 
#define PIN_NUM_CLK  12 //PA5 purple
#define PIN_NUM_CS   10 //Pa4 yello

void SPI_Init(spi_device_handle_t * spi);
esp_err_t SPI_Transmit(const uint8_t* data, size_t len);

#endif