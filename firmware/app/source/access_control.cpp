#include "access_control.h"


QueueHandle_t password_queue = NULL;
TimerHandle_t led_timer = NULL;

void ACTRL_Init(void)
{
  xTaskCreate(ACTRL_AuthTask, "AuthTask", 256, nullptr, 3, NULL);
  password_queue = xQueueCreate(1, sizeof(uint8_t) * MAX_PASSWORD_SIZE);
  led_timer = xTimerCreate("led_timer",  pdMS_TO_TICKS(5000), pdFALSE, NULL , ACTRL_ResetLedsCallbk);
}

void ACTRL_AuthTask(void *arg)
{
  uint8_t password[MAX_PASSWORD_SIZE];

  while (1)
  {
    //WTDG_Kick();
    xQueueReceive(password_queue, password, portMAX_DELAY);

    if ((strcmp((char *)password, SAVED_PASSWORD) == 0))
    {
      SendMessage((const uint8_t *)ACCESS_GRANTED_MSG, strlen(ACCESS_GRANTED_MSG));
      ACTRL_SetLed(1);
    }
    else
    {
      SendMessage((const uint8_t *)WRONG_PASSWORD_MSG, strlen(WRONG_PASSWORD_MSG));
      ACTRL_SetLed(2);
    }
  }
}

static void ACTRL_SetLed(uint8_t led_num)
{
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  if (led_num == 1)
  {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
  }

  else if (led_num == 2)
  {
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
  }
  xTimerStart(led_timer, 0);
}

void ACTRL_ResetLedsCallbk(TimerHandle_t xTimer)
{
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}