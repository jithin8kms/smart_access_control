#include "flash.h"

void FLASH_Erase(void)
{
  HAL_FLASH_Unlock();

  FLASH_EraseInitTypeDef erase_init =
      {
          .TypeErase = FLASH_TYPEERASE_SECTORS,
          .Banks = FLASH_BANK_1,
          .Sector = FLASH_SECTOR_2,
          .NbSectors = 10,
          .VoltageRange = FLASH_VOLTAGE_RANGE_3};
  uint32_t sector_error = 0;
  HAL_StatusTypeDef erase_status = HAL_FLASHEx_Erase(&erase_init, &sector_error);
  if (erase_status == HAL_ERROR)
  {
    SendMessage((const uint8_t *)"Flash erase FAILED!!!", 21);
  }
  else if (erase_status == HAL_OK)
  {
    SendMessage((const uint8_t *)"Flash erase SUCCESS!!!", 21);
  }
  else
  {
    SendMessage((const uint8_t *)"Flash erase INVALID!!!", 21);
  }

  HAL_FLASH_Lock();
}

void FLASH_Write(uint32_t address, uint8_t *data, size_t length)
{
  assert_param((address % 4) == 0);
  assert_param((length % 4) == 0);

  FLASH_Erase();
  uint32_t next_address = address;
  HAL_FLASH_Unlock();
  for (int i = 0; i < (int)length; i += 4)
  {
    uint32_t packed_data = 0xFFFFFFFF;
    packed_data = ((uint32_t)data[i + 0]) | ((uint32_t)data[i + 1] << 8) | ((uint32_t)data[i + 2] << 16) | ((uint32_t)data[i + 3] << 24);

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, next_address, packed_data);

    next_address += 4;
    SendMessage((const uint8_t *)".", 1);
  }
  HAL_FLASH_Lock();
  SendMessage((const uint8_t *)"Done writing!", 12);
}