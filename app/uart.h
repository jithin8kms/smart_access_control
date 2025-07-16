#ifndef UART_H
#define UART_H

#include "FreeRTOS.h"

#include "chip_init.h"
#include "queue.h"
#include "../config/common_config.h"
#include <string.h>

#define BUFFER_SIZE (MAX_PASSWORD_SIZE + 1) // 1 byte for termination char
#define Q_SIZE 30

#define SAVED_PASSWORD "123"
#define WRONG_PASSWORD_MSG "Wrong password"
#define ACCESS_GRANTED_MSG "Access Granted"

struct tx_struct
{
    uint8_t msg[100];
    uint8_t size;
};

struct uart_t
{
    UART_HandleTypeDef *uart3Ptr;
    uint8_t rx_char;
    QueueHandle_t queue_rx;
    QueueHandle_t queue_tx;
    uint8_t password_buffer[BUFFER_SIZE];
    bool tx_busy_flag;
};

void UART_Init(UART_HandleTypeDef *uart3);
void UART_UartRxTask(void *arg);
void UART_UartTxTask(void *arg);
void SendMessage(const uint8_t *msg, uint8_t msg_len);

#endif