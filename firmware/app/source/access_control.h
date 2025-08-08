#ifndef ACCESS_CONTROL_H
#define ACCESS_CONTROL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "timers.h"
#include <chip_init.h>
#include "../config/common_config.h"
#include "uart.h"
#include "watchdog.h"

#define SAVED_PASSWORD "123"
#define PASSWORD_SIZE sizeof(SAVED_PASSWORD)

void ACTRL_Init(void);
void ACTRL_AuthTask(void *arg);
static void ACTRL_SetLed(uint8_t led_num);
void ACTRL_ResetLedsCallbk(TimerHandle_t xTimer);
#endif