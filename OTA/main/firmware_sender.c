#include "firmware_sender.h"

void FWSND_SendFirmware()
{  
  uint8_t dummy_data[256];
	for (int i = 0; i < 256; ++i)
	{
		dummy_data[i] = i; // Fills with 0x00, 0x01, ..., 0xFF
  }

  //sent start
  esp_err_t ret;

  uint8_t cmd = CMD_FIRMWARE_UPDATE;
  ret = SPI_Transmit(&cmd, 1);
  ESP_ERROR_CHECK(ret);

  // send data frame
  for (int i = 0; i < (256/64); i++)
  {
    uint8_t data_frame[68] = {0xff};
    data_frame[0] = CMD_FIRMWARE_PAYLOAD;
    data_frame[1] = 64;

    for(int j = 0; j < 64; j++)
    {
      data_frame[j+2] = dummy_data[(64 * i) + j];
    }

    ret = SPI_Transmit(data_frame, 68);
    ESP_ERROR_CHECK(ret);
  }

  // sent data end command
  uint8_t end_frame[68] = {0xff};
  end_frame[0] = CMD_FIRMWARE_END;
  ret = SPI_Transmit(end_frame, 68);
  ESP_ERROR_CHECK(ret);
    
  ESP_LOGI("SPI", "Sent 0x%02X", CMD_FIRMWARE_UPDATE);
}