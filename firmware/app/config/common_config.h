
#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#include "FreeRTOS.h"
#include "queue.h"

#define MAX_PASSWORD_SIZE 12

extern QueueHandle_t password_queue;

#endif