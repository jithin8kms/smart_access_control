#ifndef UART_H
#define UART_H

#include "FreeRTOS.h"

#include "chip_init.h"
#include "queue.h"
struct uart_t
{
    UART_HandleTypeDef *uart3Ptr;
    uint8_t rx_char;
    QueueHandle_t queue_rx;
};

void UART_Init(UART_HandleTypeDef *uart3);
void UART_UartRxTask(void *arg);

#endif