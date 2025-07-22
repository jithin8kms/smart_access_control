#include "spi.h"

spi_device_handle_t *spi_ptr;

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

//-----------------------------------------------------------
esp_err_t SPI_Transmit(const uint8_t* data, size_t len)
{
  esp_err_t ret; 
  spi_transaction_t trans_desc = {
    .tx_buffer = data,
    .length = 8 * len, // bits
  };
  ret = spi_device_transmit(*spi_ptr, &trans_desc);
  return ret;
}