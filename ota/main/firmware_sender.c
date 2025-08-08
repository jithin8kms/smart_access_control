#include "firmware_sender.h"
#include "../../firmware/app/build/smart_access_control_bin.h"
#include "esp_crc.h"

uint8_t sig_buf[128];
size_t sig_len;
uint8_t sig_frame_size = 0;
uint32_t app_bin_len;
uint8_t *app_bin_data = NULL;



void FWSND_LocalInit()
{
  // prepere signature data
  const char* sig_file_path = "../../firmware/app/build/smart_access_control.sig";
  FILE* f = fopen(sig_file_path, "rb");  //

  if (!f) {
      perror("Failed to open signature file");
  }
  sig_len = fread(sig_buf, 1, sizeof(sig_buf), f);
  fclose(f);
  sig_frame_size = 1 + 1 + sig_len + 4;

  app_bin_len = build_smart_access_control_bin_len;
  app_bin_data  = build_smart_access_control_bin;
}

void FWSND_SendFirmware()
{

  // sent start
  esp_err_t ret;

  uint8_t cmd = CMD_FIRMWARE_UPDATE;
  ret = SPI_Transmit(&cmd, 1);
  ESP_ERROR_CHECK(ret);

  // send data frame
  for (int i = 0; i < app_bin_len; i += 64)
  {

    uint8_t data_frame[BUFFER_SIZE] = {0xff};
    uint8_t payload_len = MIN(MAX_PAYLOAD_LENGTH, app_bin_len - i);
    data_frame[CMD_OFFSET] = CMD_FIRMWARE_PAYLOAD;
    data_frame[LEN_OFFSET] = payload_len;

    memcpy(&data_frame[PAYLOAD_OFFSET], &app_bin_data[i], payload_len);
    FWSND_AddCrc(data_frame);

    ret = SPI_Transmit(data_frame, BUFFER_SIZE);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("SPI", "Frame %d sent", i);
  }

  // sent firmware end frame
  uint8_t end_frame[BUFFER_SIZE] = {0xff};
  end_frame[CMD_OFFSET] = CMD_FIRMWARE_END;
  end_frame[LEN_OFFSET] = MAX_PAYLOAD_LENGTH;
  end_frame[SIG_FRAME_SIZE_OFFSET] = sig_frame_size;

  write_u32_le(&end_frame[FIRMWARE_SIZE_OFFSET], app_bin_len);

  uint32_t firmware_crc = SPI_SoftwareCrc32(app_bin_data, app_bin_len);
  write_u32_le(&end_frame[FIRMWARE_CRC_OFFSET], firmware_crc);

  ret = SPI_Transmit(end_frame, BUFFER_SIZE);
  ESP_ERROR_CHECK(ret);

  // sent sha256 signature

  uint8_t sig_frame[sig_frame_size];
  sig_frame[CMD_OFFSET] = CMD_SIG;
  sig_frame[LEN_OFFSET] = sig_len;
  memcpy(&sig_frame[PAYLOAD_OFFSET], sig_buf, sig_len);

  ret = SPI_Transmit(sig_frame, sig_frame_size);
  ESP_ERROR_CHECK(ret);
}

uint32_t SPI_SoftwareCrc32(const uint8_t *data, size_t len)
{
  uint32_t crc = CRC32_INIT;

  for (size_t i = 0; i < len; ++i)
  {
    crc ^= data[i];
    for (int j = 0; j < 8; ++j)
    {
      if (crc & 1)
        crc = (crc >> 1) ^ CRC32_POLY;
      else
        crc >>= 1;
    }
  }

  return crc ^ CRC32_FINAL_XOR;
}

void FWSND_AddCrc(uint8_t *data_frame)
{
  uint32_t data_frame_crc = SPI_SoftwareCrc32(data_frame, BUFFER_SIZE - 4);
  // Append CRC in little-endian format

  write_u32_le(&data_frame[CRC_1_OFFSET], data_frame_crc);
}

void write_u32_le(uint8_t *dest, uint32_t value)
{
    dest[0] = (uint8_t)(value & 0xFF);
    dest[1] = (uint8_t)((value >> 8) & 0xFF);
    dest[2] = (uint8_t)((value >> 16) & 0xFF);
    dest[3] = (uint8_t)((value >> 24) & 0xFF);
}