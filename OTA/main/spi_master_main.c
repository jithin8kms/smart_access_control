/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/dedic_gpio.h"
#include "esp_log.h"

#define PIN_NUM_MOSI 11
#define PIN_NUM_MISO -1   // Not used now 13
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10
spi_device_handle_t spi;

#define TEXT "spi tx"
void app_main(void)
{
  vTaskDelay(pdMS_TO_TICKS(1000)); // small delay after boot

  esp_err_t ret;  
  spi_device_handle_t spi;
  
  spi_bus_config_t bus_cfg = {
  .mosi_io_num = PIN_NUM_MOSI,
  .miso_io_num = PIN_NUM_MISO,
  .sclk_io_num = PIN_NUM_CLK,
  .max_transfer_sz = 0, // default
  };

  ret = spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_CH_AUTO );
  ESP_ERROR_CHECK(ret);

  spi_device_interface_config_t dev_cfg = {
    .mode = 0,
    .clock_speed_hz = 1 * 1000 * 1000, 
    .spics_io_num = PIN_NUM_CS,
    .queue_size = 1      
  };
  ret = spi_bus_add_device(SPI3_HOST, &dev_cfg ,&spi);
  ESP_ERROR_CHECK(ret);

  uint8_t data = 0x42;
  spi_transaction_t trans_desc = {
    .tx_buffer = (uint8_t*)&TEXT,
    .length = 8 * sizeof(TEXT), // bits
  };
  ret = spi_device_transmit(spi, &trans_desc);
  ESP_ERROR_CHECK(ret);
  
  ESP_LOGI("SPI", "Sent 0x%02X", data);

}
