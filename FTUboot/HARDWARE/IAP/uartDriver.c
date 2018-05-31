/**
  * @file   uartDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/03/19
  * @brief  串口驱动
  */
  
#include "uartDriver.h"

UART_HandleTypeDef UART5_Handler;

/* 重定义fputc函数  */
int fputc(int ch, FILE *f)
{ 	
	while((UART5->SR&0X40)==0); 
	UART5->DR = (uint8_t) ch;      
	return ch;
}

/**
  *@brief 引脚初始化
  *@param  None
  *@retval None
  */
static void uart5GpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  *@brief uart6初始化
  *@param  None
  *@retval None
  */
static void uart5Config(void)
{
	__HAL_RCC_UART5_CLK_ENABLE();
	
	UART5_Handler.Instance = UART5;
	UART5_Handler.Init.BaudRate = 115200;
	UART5_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART5_Handler.Init.StopBits = UART_STOPBITS_1;
	UART5_Handler.Init.Parity = UART_PARITY_NONE;
	UART5_Handler.Init.Mode = UART_MODE_TX_RX;
	UART5_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART5_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART5_Handler);
	
	__HAL_UART_DISABLE_IT(&UART5_Handler,UART_IT_TC);
//	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_RXNE);//使能中断
//	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_IDLE);//使能中断
//	HAL_NVIC_SetPriority(UART5_IRQn,3,2); //设置优先级
//	HAL_NVIC_EnableIRQ(UART5_IRQn); //使能中断
}

/**
  *@brief led初始化
  *@param  None
  *@retval None
  */
void uartInit(void)
{
	uart5GpioConfig();
	uart5Config();
}

/**
  *@brief uart使用杀死
  *@param  None
  *@retval None
  */
void UartUseKill(void)
{
	__HAL_UART_DISABLE(&UART5_Handler);
	__HAL_RCC_UART5_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
}
/* END */
