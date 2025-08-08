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
#include "spi.h"
#include "firmware_sender.h"


spi_device_handle_t spi;

void app_main(void)
{
  vTaskDelay(pdMS_TO_TICKS(1000)); // small delay after boot
  
  SPI_Init(&spi);
  FWSND_LocalInit();
  FWSND_SendFirmware();
}
