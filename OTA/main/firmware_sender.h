#ifndef FIRMWARE_SENDER_H
#define FIRMWARE_SENDER_H

#include "spi.h"
#include "esp_log.h"

#define CMD_FIRMWARE_UPDATE 0xA0
#define CMD_FIRMWARE_PAYLOAD 0xA1
#define CMD_FIRMWARE_END 0xA2

void FWSND_SendFirmware();
#endif