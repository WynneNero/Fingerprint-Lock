/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
volatile uint8_t Uart4_RxBuffer[1];
volatile uint8_t Uart6_RxBuffer[1];
/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart6;

/* UART4 init function */
void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */
  HAL_UART_Receive_IT(&huart4, (uint8_t *)Uart4_RxBuffer, 1);
  /* USER CODE END UART4_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
  HAL_UART_Receive_IT(&huart6, (uint8_t *)Uart6_RxBuffer, 1);
  /* USER CODE END USART6_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**UART4 GPIO Configuration
    PA0-WKUP     ------> UART4_TX
    PA1     ------> UART4_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */
    HAL_UART_Receive_IT(&huart4, (uint8_t *)Uart4_RxBuffer, 1);
  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */
    HAL_UART_Receive_IT(&huart6, (uint8_t *)Uart6_RxBuffer, 1);
  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();

    /**UART4 GPIO Configuration
    PA0-WKUP     ------> UART4_TX
    PA1     ------> UART4_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void uart4SendByte(uint8_t ch){
	while((UART4->SR&0X40) == 0);
	UART4->DR = (uint8_t)ch;
}
uint8_t aRxBuffer[100];
int len = 0;
extern int open_or_close;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	  if(UartHandle->Instance == USART6){
//		  if(Uart6_RxBuffer[0] == 0x0a){
//			  if(aRxBuffer[len-1] == 0x0d){
//				  if(aRxBuffer[len-5]=='o' && aRxBuffer[len-4]=='p' && aRxBuffer[len-3]=='e' && aRxBuffer[len-2]=='n'){
//					  disableSimpleFOC(1);
//					  open_or_close = 10000;
//					  setTarget(5);
//				  }else if(aRxBuffer[0]=='c' && aRxBuffer[1]=='l' && aRxBuffer[2]=='o' && aRxBuffer[3]=='s' && aRxBuffer[4]=='e'){
//					  disableSimpleFOC(1);
//					  open_or_close = 10000;
//					  setTarget(-5);
//				  }
//				  len = 0;
//			  }else{
//				  aRxBuffer[len] = Uart6_RxBuffer[0];
//				  len++;
//			  }
//		  }else {
//			  aRxBuffer[len] = Uart6_RxBuffer[0];
//			  len++;
//		  }
//		  if(len >= 100) len=0;
		  HAL_UART_Receive_IT(&huart6, (uint8_t *)Uart6_RxBuffer, 1);
	  }
	  if(UartHandle->Instance == UART4){
		  if(Uart4_RxBuffer[0] == 0x0a){
			  if(aRxBuffer[len-1] == 0x0d){
				  if(len >= 5){
					  if(aRxBuffer[len-5]=='o' && aRxBuffer[len-4]=='p' && aRxBuffer[len-3]=='e' && aRxBuffer[len-2]=='n'){
						  disableSimpleFOC(1);
						  open_or_close = 5000;
						  setTarget(10);
					  }else if(aRxBuffer[len-6]=='c' && aRxBuffer[len-5]=='l' && aRxBuffer[len-4]=='o' && aRxBuffer[len-3]=='s' && aRxBuffer[len-2]=='e'){
						  disableSimpleFOC(1);
						  open_or_close = 5000;
						  setTarget(-10);
					  }
					  len = 0;
				  }
			  }else{
				  aRxBuffer[len] = Uart4_RxBuffer[0];
				  len++;
			  }
		  }else {
			  aRxBuffer[len] = Uart4_RxBuffer[0];
			  len++;
		  }
		  if(len >= 100) len=0;
		  HAL_UART_Receive_IT(&huart4, (uint8_t *)Uart4_RxBuffer, 1);
		  //uart4SendByte(Uart4_RxBuffer[0]);
	  }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
