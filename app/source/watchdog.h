#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "FreeRTOS.h"
#include "stm32f4xx_hal.h"
#include "task.h"
#include "timers.h"
#include "uart.h"

void WTDG_Init(void);
void WTDG_Kick();
void WTDG_TimeoutCallback(TimerHandle_t xTimer);

#endif