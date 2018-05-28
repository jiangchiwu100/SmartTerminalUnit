/**
  * @file   uartDriver.c
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/03/19
  * @brief  串口驱动
  */
  
#include "uartDriver.h"
#include "gwprotocal.h"

UART_HandleTypeDef UART6_Handler;
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
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_RXNE);//使能中断
	__HAL_UART_ENABLE_IT(&UART5_Handler,UART_IT_IDLE);//使能中断
	HAL_NVIC_SetPriority(UART5_IRQn,3,2); //设置优先级
	HAL_NVIC_EnableIRQ(UART5_IRQn); //使能中断
}


/**
  *@brief 引脚初始化
  *@param  None
  *@retval None
  */
static void uart6GpioConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/**
  *@brief uart6初始化
  *@param  None
  *@retval None
  */
static void uart6Config(void)
{
	__HAL_RCC_USART6_CLK_ENABLE();
	
	UART6_Handler.Instance = USART6;
	UART6_Handler.Init.BaudRate = 115200;
	UART6_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART6_Handler.Init.StopBits = UART_STOPBITS_1;
	UART6_Handler.Init.Parity = UART_PARITY_NONE;
	UART6_Handler.Init.Mode = UART_MODE_TX_RX;
	UART6_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART6_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART6_Handler);
	
	__HAL_UART_DISABLE_IT(&UART6_Handler,UART_IT_TC);
	__HAL_UART_ENABLE_IT(&UART6_Handler,UART_IT_RXNE);//使能中断
	__HAL_UART_ENABLE_IT(&UART6_Handler,UART_IT_IDLE);//使能中断
	HAL_NVIC_SetPriority(USART6_IRQn,1,2); //设置优先级
	HAL_NVIC_EnableIRQ(USART6_IRQn); //使能中断
}


/**
  *@brief  UART6底层发送接口
  *@param  None
  *@retval None
  */
void SendData(uint8_t *pBuff, uint16_t length)
{
	uint16_t i;
	for(i=0;i<length;i++){
		while(__HAL_UART_GET_FLAG(&UART6_Handler,USART_FLAG_TC)!= SET);
		USART6->DR = (uint8_t)*(pBuff+i);	
	}
}

/**
  *@brief 串口初始化
  *@param  None
  *@retval None
  */
void uartInit(void)
{
	uart6GpioConfig();
	uart6Config();
	uart5GpioConfig();
	uart5Config();
}

/**
  *@brief disableUART
  *@param  None
  *@retval None
  */
void uartDisable(void)
{
	__HAL_UART_DISABLE(&UART5_Handler);
	__HAL_UART_DISABLE(&UART6_Handler);
}

void USART6_IRQHandler(void)
{
	if(__HAL_UART_GET_FLAG(&UART6_Handler,USART_FLAG_ORE) == SET){
		USART6->SR;
		USART6->DR;
	}
	if(__HAL_UART_GET_FLAG(&UART6_Handler,USART_FLAG_RXNE) == SET){
		ReceiveDealWith((uint8_t)USART6->DR);
	}
	else if(__HAL_UART_GET_FLAG(&UART6_Handler,UART_FLAG_IDLE)!=RESET){
		USART6->SR;
		USART6->DR;
		ReceiveFinish();
	}
}
/* END */
