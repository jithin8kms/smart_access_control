#include "spi.h"

spi_device_handle_t *spi_ptr;
uint8_t dmy[64] = {0xFF};
uint8_t rx_buff = 0xff;

void SPI_Init(spi_device_handle_t * spi)
{
  spi_ptr = spi;

  esp_err_t ret;  

  
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
  ret = spi_bus_add_device(SPI3_HOST, &dev_cfg ,spi_ptr);
  ESP_ERROR_CHECK(ret);
}

esp_err_t SPI_Transfer(const uint8_t *tx_data, uint8_t *rx_data, size_t len)
{
    spi_transaction_t trans = {
        .length    = 8 * len,       // bits
        .tx_buffer = tx_data,
        .rx_buffer = rx_data,
    };

    esp_err_t ret = spi_device_transmit(*spi_ptr, &trans);
    ESP_ERROR_CHECK(ret);
    return ret;
}


void SPI_SendReadyCmd()
{
  esp_err_t ret;

  uint8_t cmd = CMD_ESP_READY;
  ret = SPI_Transfer(&cmd, &rx_buff, 1);
  //ESP_ERROR_CHECK(ret);  
  rx_buff = 0xff;
  ret = SPI_Transfer(dmy, &rx_buff, 1);
}

uint8_t SPI_IsStmReady()
{
  static int ready_timeout = 0;
  if (rx_buff == CMD_STM_READY)
  {
    ESP_LOGI("SPI", "STM32 Ready: 0x%02X", rx_buff);
    return 1;
  }
  else if (ready_timeout == 100)
  {
    ESP_LOGI("SPI", "STM32 timeout");
    return 1;
  }
  else
  {
    ESP_LOGI("SPI", "waiting: 0x%02X", rx_buff);
    ready_timeout++;
    vTaskDelay(pdMS_TO_TICKS(100)); // small wait before retry
    return 0;
  }
}