#ifndef SPI_H
#define SPI_H

#include "driver/spi_master.h"
#include "driver/dedic_gpio.h"
#include "esp_log.h"

#define PIN_NUM_MOSI 11  // PA7 white
#define PIN_NUM_MISO 13   // PA6 
#define PIN_NUM_CLK  12 //PA5 purple
#define PIN_NUM_CS   10 //Pa4 yello

#define CMD_ESP_READY   0xCA 
#define CMD_STM_READY   0xCB 

void SPI_Init(spi_device_handle_t * spi);
esp_err_t SPI_Transfer(const uint8_t *tx_data, uint8_t *rx_data, size_t len);
uint8_t SPI_IsStmReady();
void SPI_SendReadyCmd();

#endif