#include "uart.h"

static uart_t uart;

#define UART_RX_BUF_SIZE 128

volatile uint8_t uart_rx_buf[UART_RX_BUF_SIZE];
volatile uint8_t uart_tx_buf[UART_RX_BUF_SIZE];
volatile uint16_t uart_rx_head = 0;
volatile uint16_t uart_tx_head = 0;
volatile uint16_t uart_tx_tail = 0;
uint8_t tx_busy = 0;

void UART_Init(UART_HandleTypeDef *uart3Ptr)
{
	configASSERT(uart3Ptr != NULL);
	uart.uart3Ptr = uart3Ptr;
	HAL_UART_Receive_IT(uart.uart3Ptr, &uart.rx_char, 1);
}

void UART_UartRxTask(void *arg)
{

	/*
	uart.queue_rx = xQueueCreate( 10, sizeof( uint8_t ) );
	configASSERT(uart.queue_rx != NULL);

	while(1)
	{
		uint8_t data;

		xQueueReceive(uart.queue_rx, &data, portMAX_DELAY);
	}*/
}

// uart rx interrupt handler
//---------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/*
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (huart->Instance == USART3)
	{
		xQueueSendFromISR(uart.queue_rx, &uart.rx_char, &xHigherPriorityTaskWoken);
	}
	HAL_UART_Receive_IT(uart.uart3Ptr, &uart.rx_char, 1);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);*/

	if (huart->Instance == USART3)
	{
		uint8_t c = uart.rx_char;
		uart_rx_buf[uart_rx_head++] = c;
		if (uart_rx_head >= UART_RX_BUF_SIZE)
			uart_rx_head = 0;

		// queue for TX
		uart_tx_buf[uart_tx_head++] = c;

		if (uart_tx_head >= UART_RX_BUF_SIZE)
			uart_tx_head = 0;

		if (!tx_busy)
		{
			tx_busy = 1;
			HAL_UART_Transmit_IT(uart.uart3Ptr, (uint8_t *)&uart_tx_buf[uart_tx_tail], 1);
		}
		HAL_UART_Receive_IT(uart.uart3Ptr, &uart.rx_char, 1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
		uart_tx_tail++;
		if (uart_tx_tail >= UART_RX_BUF_SIZE)
			uart_tx_tail = 0;
		if (uart_tx_tail != uart_tx_head)
		{
			HAL_UART_Transmit_IT(uart.uart3Ptr, (uint8_t *)&uart_tx_buf[uart_tx_tail], 1);
		}
		else
		{
			tx_busy = 0;
		}
	}
}
