#ifndef UART_H
#define UART_H

#include "chip_init.h"

#include <string.h>

struct uart_t
{
    UART_HandleTypeDef *uart3Ptr;
    bool tx_busy_flag;
};

void UART_Init(UART_HandleTypeDef *uart3);

void SendMessage(const uint8_t *msg, uint8_t msg_len);

#endif