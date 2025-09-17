#include "chip_init.h"

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void CHIP_ErrorHandler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void CHIP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    CHIP_ErrorHandler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    CHIP_ErrorHandler();
  }
}

//---------init gpio -------------------

void CHIP_GpioInit(void)
{
  GPIO_InitTypeDef gpio_init_struct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  __HAL_RCC_CRC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin | LD3_Pin | LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  gpio_init_struct.Pin = USER_Btn_Pin;
  gpio_init_struct.Mode = GPIO_MODE_IT_RISING;
  gpio_init_struct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &gpio_init_struct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  gpio_init_struct.Pin = LD1_Pin | LD3_Pin | LD2_Pin;
  gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_struct.Pull = GPIO_NOPULL;
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio_init_struct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  gpio_init_struct.Pin = USB_PowerSwitchOn_Pin;
  gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_struct.Pull = GPIO_NOPULL;
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &gpio_init_struct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  gpio_init_struct.Pin = USB_OverCurrent_Pin;
  gpio_init_struct.Mode = GPIO_MODE_INPUT;
  gpio_init_struct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &gpio_init_struct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  gpio_init_struct.Pin = USB_SOF_Pin | USB_ID_Pin | USB_DM_Pin | USB_DP_Pin;
  gpio_init_struct.Mode = GPIO_MODE_AF_PP;
  gpio_init_struct.Pull = GPIO_NOPULL;
  gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_struct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &gpio_init_struct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

void CHIP_Uart3Init(UART_HandleTypeDef *uart3Ptr)
{
  // order matters
  __HAL_RCC_USART3_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);

  uart3Ptr->Instance = USART3;
  uart3Ptr->Init.BaudRate = 115200;
  uart3Ptr->Init.WordLength = UART_WORDLENGTH_8B;
  uart3Ptr->Init.StopBits = UART_STOPBITS_1;
  uart3Ptr->Init.Parity = UART_PARITY_NONE;
  uart3Ptr->Init.Mode = UART_MODE_TX_RX;
  uart3Ptr->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart3Ptr->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(uart3Ptr) != HAL_OK)
  {
    CHIP_ErrorHandler();
  }
}

void CHIP_Spi1Init(SPI_HandleTypeDef *hspi1)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**SPI1 GPIO Configuration
  PA5     ------> SPI1_SCK
  PA6     ------> SPI1_MISO
  PA7     ------> SPI1_MOSI
  PA4     ------> SPI1_NSS
  */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SPI1_IRQn);

  /* SPI1 parameter configuration*/
  hspi1->Instance = SPI1;
  hspi1->Init.Mode = SPI_MODE_SLAVE;
  hspi1->Init.Direction = SPI_DIRECTION_2LINES;
  hspi1->Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1->Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1->Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1->Init.NSS = SPI_NSS_HARD_INPUT;
  hspi1->Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1->Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(hspi1) != HAL_OK)
  {
    CHIP_ErrorHandler();
  }
}