#include "uart.h"

static uart_t uart = {0};

void UART_Init(UART_HandleTypeDef *uart3Ptr)
{
	uart.uart3Ptr = uart3Ptr;
}

//---------------------------------------------------------------------------
void UART_LOG(const uint8_t *msg, uint8_t msg_len)
{

	HAL_UART_Transmit_IT(uart.uart3Ptr, msg, msg_len);
}
