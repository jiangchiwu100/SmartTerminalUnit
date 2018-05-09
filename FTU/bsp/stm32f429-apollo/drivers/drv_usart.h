/** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_usart.h
  * @brief:     The driver of USART,this file is part of RT-Thread RTOS.
  * @version:   V1.0.0(the first version)
  * @author:    Bernard
  * @date:      2009-01-05
  * @update:    [2010-03-29][Bernard][remove interrupt Tx and DMA Rx mode]
  * @update:    [2012-02-08][aozima][update for F4]
  * @update:    [2012-07-28][aozima][update for ART board]
  * @update:    [2016-05-28][armink][add DMA Rx mode]
  * @update:    [2018-02-01][Lexun][make the code clean up]
  */

#ifndef __DRV_USART_H__
#define __DRV_USART_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "stm32f4xx.h"


/* DEFINES -------------------------------------------------------------------*/
#ifdef RT_USING_USART1
#define USART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for USART1 Pins */
#define USART1_TX_PIN                    GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_TX_AF                     GPIO_AF7_USART1
#define USART1_RX_PIN                    GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_RX_AF                     GPIO_AF7_USART1
#endif

#ifdef RT_USING_USART2
#define USART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

/* Definition for USART2 Pins */
#define USART2_TX_PIN                    GPIO_PIN_5
#define USART2_TX_GPIO_PORT              GPIOD
#define USART2_TX_AF                     GPIO_AF7_USART2
#define USART2_RX_PIN                    GPIO_PIN_3
#define USART2_RX_GPIO_PORT              GPIOA
#define USART2_RX_AF                     GPIO_AF7_USART2
#endif

#ifdef RT_USING_USART3
#define USART3_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART3_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

/* Definition for USART3 Pins */
#define USART3_TX_PIN                    GPIO_PIN_10
#define USART3_TX_GPIO_PORT              GPIOB
#define USART3_TX_AF                     GPIO_AF7_USART3
#define USART3_RX_PIN                    GPIO_PIN_11
#define USART3_RX_GPIO_PORT              GPIOB
#define USART3_RX_AF                     GPIO_AF7_USART3
#endif

#ifdef RT_USING_UART4
#define UART4_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition for USART4 Pins */
#define UART4_TX_PIN                     GPIO_PIN_10
#define UART4_TX_GPIO_PORT               GPIOC
#define UART4_TX_AF                      GPIO_AF8_UART4
#define UART4_RX_PIN                     GPIO_PIN_11
#define UART4_RX_GPIO_PORT               GPIOC
#define UART4_RX_AF                      GPIO_AF8_UART4
#endif

#ifdef RT_USING_UART5
#define UART5_RX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART5_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition for UART5 Pins */
#define UART5_TX_PIN                     GPIO_PIN_12
#define UART5_TX_GPIO_PORT               GPIOC
#define UART5_TX_AF                      GPIO_AF8_UART5
#define UART5_RX_PIN                     GPIO_PIN_2
#define UART5_RX_GPIO_PORT               GPIOD
#define UART5_RX_AF                      GPIO_AF8_UART5
#endif

#ifdef RT_USING_USART6
#define USART6_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USART6_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

/* Definition for UART6 Pins */
#define USART6_TX_PIN                    GPIO_PIN_6
#define USART6_TX_GPIO_PORT              GPIOC
#define USART6_TX_AF                     GPIO_AF8_USART6
#define USART6_RX_PIN                    GPIO_PIN_7
#define USART6_RX_GPIO_PORT              GPIOC
#define USART6_RX_AF                     GPIO_AF8_USART6
#endif

#define UART_ENABLE_IRQ(n)               NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)              NVIC_DisableIRQ((n))


/* STRUCTS -------------------------------------------------------------------*/
/* STM32 uart driver */
struct stm32_uart
{
    UART_HandleTypeDef UartHandle;
    IRQn_Type irq;
};


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
int rt_hw_usart_init(void);


#endif /* __DRV_USART_H__ */


/* END OF FILE ---------------------------------------------------------------*/


