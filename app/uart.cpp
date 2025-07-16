#include "uart.h"

static uart_t uart = {0};
TaskHandle_t tx_task_handle;
#define UART_RX_BUF_SIZE 128

void UART_Init(UART_HandleTypeDef *uart3Ptr)
{
	xTaskCreate(UART_UartRxTask, "RxTask", 256, nullptr, 2, nullptr);
	xTaskCreate(UART_UartTxTask, "TxTask", 256, nullptr, 1, nullptr);

	configASSERT(uart3Ptr != NULL);
	uart.uart3Ptr = uart3Ptr;
	HAL_UART_Receive_IT(uart.uart3Ptr, &uart.rx_char, 1);

	// create Q
	uart.queue_rx = xQueueCreate(Q_SIZE, sizeof(uint8_t));
	uart.queue_tx = xQueueCreate(Q_SIZE, sizeof(tx_struct));

	configASSERT(uart.queue_rx != NULL);
	configASSERT(uart.queue_tx != NULL);
}

// rx task
//--------------------------------------------
void UART_UartRxTask(void *arg)
{
	static int buffer_head = 0;
	static bool discard_data = false;
	uint8_t data = 0;
	while (1)
	{
		//WTDG_Kick();
		xQueueReceive(uart.queue_rx, &data, portMAX_DELAY);

		if (discard_data == true)
		{
			if (data == '\r' || data == '\n' || data == '\0')
			{
				discard_data = false;
			}
		}

		else if (data == '\r' || data == '\n' || data == '\0')
		{
			uart.password_buffer[buffer_head] = '\0';
			// clear Q
			uint8_t dummy;
			while (xQueueReceive(uart.queue_rx, &dummy, 0) == pdTRUE)
				;
			SendMessage((const uint8_t *)uart.password_buffer, buffer_head);
			buffer_head = 0;

			xQueueOverwrite(password_queue, uart.password_buffer);
		}

		else if (buffer_head < BUFFER_SIZE - 1)
		{
			// add incoming char to password buffer
			uart.password_buffer[buffer_head++] = data;
		}
		else
		{
			buffer_head = 0;
			discard_data = true;
			memset(uart.password_buffer, 0, sizeof(uart.password_buffer));

			uint8_t dummy;
			while (xQueueReceive(uart.queue_rx, &dummy, 0) == pdTRUE)
				;

			SendMessage((const uint8_t *)WRONG_PASSWORD_MSG, strlen(WRONG_PASSWORD_MSG));
		}
	}
}

// tx task
//--------------------------------------------
void UART_UartTxTask(void *arg)
{
	tx_task_handle = xTaskGetCurrentTaskHandle();
	while (1)
	{
		//WTDG_Kick();
		tx_struct tx_data = {0};
		xQueueReceive(uart.queue_tx, &tx_data, portMAX_DELAY);

		HAL_UART_Transmit_IT(uart.uart3Ptr, (const uint8_t *)&tx_data.msg, tx_data.size);

		// transmit data and task waits untill ISR notifies
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

// uart rx interrupt handler
//---------------------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (huart->Instance == USART3)
	{
		xQueueSendFromISR(uart.queue_rx, &uart.rx_char, &xHigherPriorityTaskWoken);
	}
	HAL_UART_Receive_IT(uart.uart3Ptr, &uart.rx_char, 1);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// uart tx interrupt handler
//---------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	// notify tx task that tx is complete
	vTaskNotifyGiveFromISR(tx_task_handle, &xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// construct and send msg to Q
//---------------------------------------------------------------------------
void SendMessage(const uint8_t *msg, uint8_t msg_len)
{
	tx_struct msg_struct = {0};

	memcpy(msg_struct.msg, msg, msg_len);
	msg_struct.size = msg_len;

	xQueueSend(uart.queue_tx, &msg_struct, portMAX_DELAY);
}
