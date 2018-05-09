/** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_usart.c
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


/* INCLUDE FILES -------------------------------------------------------------*/
#include "board.h"
#include "drv_usart.h"


/* PRIVATE FUNCTIONS ---------------------------------------------------------*/
static rt_err_t _usart_configure(struct rt_serial_device *serial, struct serial_configure *cfg);
static rt_err_t _usart_control(struct rt_serial_device *serial, int cmd, void *arg);
static int _usart_putc(struct rt_serial_device *serial, char c);
static int _usart_getc(struct rt_serial_device *serial);


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static const struct rt_uart_ops stm32_uart_ops =
{
    _usart_configure,
    _usart_control,
    _usart_putc,
    _usart_getc,
};

#if defined(RT_USING_USART1)
struct rt_serial_device serial1;

/* UART1 device driver structure */
struct stm32_uart uart1 =
{
    {USART1},
    USART1_IRQn,
};
#endif /* RT_USING_USART1 */

#if defined(RT_USING_USART2)
struct rt_serial_device serial2;

/* UART2 device driver structure */
struct stm32_uart uart2 =
{
    {USART2},
    USART2_IRQn,
};
#endif /* RT_USING_USART2 */

#if defined(RT_USING_USART3)
struct rt_serial_device serial3;

/* UART3 device driver structure */
struct stm32_uart uart3 =
{
    {USART3},
    USART3_IRQn,
};
#endif /* RT_USING_USART3 */

#if defined(RT_USING_UART4)
struct rt_serial_device serial4;

/* UART4 device driver structure */
struct stm32_uart uart4 =
{
    {UART4},
    UART4_IRQn,
};
#endif /* RT_USING_UART4 */

#if defined(RT_USING_UART5)
/* UART5 device driver structure */
struct stm32_uart uart5 =
{
    {UART5},
    UART5_IRQn,
};
struct rt_serial_device serial5;
#endif /* RT_USING_UART5 */

#if defined(RT_USING_USART6)
struct rt_serial_device serial6;

/* UART6 device driver structure */
struct stm32_uart uart6 =
{
    {USART6},
    USART6_IRQn,
};
#endif /* RT_USING_USART6 */

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : config the usart.
  * @param : [serial] the serial port.
  * @param : [cfg] the serial config.
  * @return: [RT_ERROR] error; [RT_EOK] ok. 
  */  
static rt_err_t _usart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct stm32_uart *)serial->parent.user_data;

	HAL_UART_DeInit(&uart->UartHandle);
	
    uart->UartHandle.Init.BaudRate     = cfg->baud_rate;
    uart->UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    uart->UartHandle.Init.Mode             = UART_MODE_TX_RX;
    uart->UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    switch (cfg->data_bits)
    {
        case DATA_BITS_8:
            uart->UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            break;
				
        case DATA_BITS_9:
            uart->UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
            break;
				
        default:
            uart->UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            break;
    }
		
    switch (cfg->stop_bits)
    {
        case STOP_BITS_1:
            uart->UartHandle.Init.StopBits   = UART_STOPBITS_1;
            break;
				
        case STOP_BITS_2:
            uart->UartHandle.Init.StopBits   = UART_STOPBITS_2;
            break;
	
        default:
            uart->UartHandle.Init.StopBits   = UART_STOPBITS_1;
            break;
  }
		
   switch (cfg->parity)
   {
        case PARITY_NONE:
            uart->UartHandle.Init.Parity     = UART_PARITY_NONE;
            break;
        case PARITY_ODD:
            uart->UartHandle.Init.Parity     = UART_PARITY_ODD;
            break;
        case PARITY_EVEN:
            uart->UartHandle.Init.Parity     = UART_PARITY_EVEN;
            break;
        default:
            uart->UartHandle.Init.Parity     = UART_PARITY_NONE;
            break;
    }
	
    if (HAL_UART_Init(&uart->UartHandle) != HAL_OK)
    {
        return(RT_ERROR);
    }

    return(RT_EOK);
}

/**
  * @brief : control the usart.
  * @param : [serial] the serial port.
  * @param : [arg] the serial parameter.
  * @return: [RT_EOK] ok. 
  */  
static rt_err_t _usart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct stm32_uart *)serial->parent.user_data;

    switch (cmd)
    {
        case RT_DEVICE_CTRL_CLR_INT:
            /* disable rx irq */
            NVIC_DisableIRQ(uart->irq);
            /* disable interrupt */
            __HAL_UART_DISABLE_IT(&uart->UartHandle, UART_IT_RXNE);
            break;
				
        case RT_DEVICE_CTRL_SET_INT:
            /* enable rx irq */
            NVIC_EnableIRQ(uart->irq);
            /* enable interrupt */
            __HAL_UART_ENABLE_IT(&uart->UartHandle, UART_IT_RXNE);
            break;
    }

    return(RT_EOK);
}

/**
  * @brief : send data to the usart.
  * @param : [serial] the serial port.
  * @param : [c] the data has been send.
  * @return: [RT_EOK] ok. 
  */  
static int _usart_putc(struct rt_serial_device *serial, char c)
{
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct stm32_uart *)serial->parent.user_data;

    while((__HAL_UART_GET_FLAG(&uart->UartHandle, UART_FLAG_TXE) == RESET));
    uart->UartHandle.Instance->DR = c;
	
    return(1);
}

/**
  * @brief : get the data from the usart.
  * @param : [serial] the serial port.
  * @param : [c] the data has been send.
  * @return: [RT_EOK] ok. 
  */  
static int _usart_getc(struct rt_serial_device *serial)
{
    int ch;
    struct stm32_uart *uart;

    RT_ASSERT(serial != RT_NULL);
	
    uart = (struct stm32_uart *)serial->parent.user_data;

    ch = -1;
    if (__HAL_UART_GET_FLAG(&uart->UartHandle, UART_FLAG_RXNE) != RESET)
	{
        ch = uart->UartHandle.Instance->DR & 0xff;
	}
		
    return(ch);
}

/**
  * @brief : uart common interrupt process, This need add to uart ISR.
  * @param : [serial] the serial device.
  * @return: None. 
  */  
static void uart_isr(struct rt_serial_device *serial)
{
    struct stm32_uart *uart = (struct stm32_uart *) serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    /* UART in mode Receiver -------------------------------------------------*/
    if ((__HAL_UART_GET_FLAG(&uart->UartHandle, UART_FLAG_RXNE) != RESET) &&
            (__HAL_UART_GET_IT_SOURCE(&uart->UartHandle, UART_IT_RXNE) != RESET))
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
        /* Clear RXNE interrupt flag */
        __HAL_UART_CLEAR_FLAG(&uart->UartHandle, UART_FLAG_RXNE);
    }
    else if ((__HAL_UART_GET_FLAG(&uart->UartHandle, UART_FLAG_ORE) != RESET))
    {
        (uart->UartHandle.Instance)->DR;
        /* Clear RXNE interrupt flag */
        __HAL_UART_CLEAR_FLAG(&uart->UartHandle, UART_FLAG_ORE);
    }
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for UART interrupt request enable
  * @param : [huart] UART handle pointer.
  * @return: None. 
  */  
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

#if defined(RT_USING_USART1)
    if (huart->Instance == USART1)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        USART1_TX_GPIO_CLK_ENABLE();
        USART1_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART1_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = USART1_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = USART1_TX_AF;

        HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = USART1_RX_PIN;
        GPIO_InitStruct.Alternate = USART1_RX_AF;

        HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART1_IRQn, (INT_USART1_PRIO >> 4) & 0x0F, INT_USART1_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
#endif

#if defined(RT_USING_USART2)
    if (huart->Instance == USART2)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        USART2_TX_GPIO_CLK_ENABLE();
        USART2_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART2_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = USART2_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = USART2_TX_AF;

        HAL_GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = USART2_RX_PIN;
        GPIO_InitStruct.Alternate = USART2_RX_AF;

        HAL_GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART2_IRQn, (INT_USART2_PRIO>>4) & 0x0F, INT_USART2_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
#endif

#if defined(RT_USING_USART3)
    if (huart->Instance == USART3)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        USART3_TX_GPIO_CLK_ENABLE();
        USART3_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART3_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = USART3_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = USART3_TX_AF;

        HAL_GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = USART3_RX_PIN;
        GPIO_InitStruct.Alternate = USART3_RX_AF;

        HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART3_IRQn, (INT_USART3_PRIO>>4) & 0x0F, INT_USART3_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
#endif /* RT_USING_USART3 */

#if defined(RT_USING_UART4)
    if (huart->Instance == UART4)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        UART4_TX_GPIO_CLK_ENABLE();
        UART4_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_UART4_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = UART4_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = UART4_TX_AF;

        HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = UART4_RX_PIN;
        GPIO_InitStruct.Alternate = UART4_RX_AF;

        HAL_GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(UART4_IRQn, (INT_UART4_PRIO >> 4) & 0x0F, INT_UART4_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
    }
#endif /* RT_USING_UART4 */

#if defined(RT_USING_UART5)
    if (huart->Instance == UART5)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        UART5_TX_GPIO_CLK_ENABLE();
        UART5_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_UART5_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = UART5_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = UART5_TX_AF;

        HAL_GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = UART5_RX_PIN;
        GPIO_InitStruct.Alternate = UART5_RX_AF;

        HAL_GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(UART5_IRQn, (INT_UART5_PRIO >> 4) & 0x0F, INT_UART5_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(UART5_IRQn);
    }
#endif /* RT_USING_UART5 */
		
#if defined(RT_USING_USART6)
    if (huart->Instance == USART6)
    {
        /* #1 - Enable peripherals and GPIO Clocks */
        /* Enable GPIO TX/RX clock */
        USART6_TX_GPIO_CLK_ENABLE();
        USART6_RX_GPIO_CLK_ENABLE();

        /* Enable USARTx clock */
        __HAL_RCC_USART6_CLK_ENABLE();

        /* #2 - Configure peripheral GPIO */
        /* UART TX GPIO pin configuration */
        GPIO_InitStruct.Pin       = USART6_TX_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = USART6_TX_AF;

        HAL_GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration */
        GPIO_InitStruct.Pin = USART6_RX_PIN;
        GPIO_InitStruct.Alternate = USART6_RX_AF;

        HAL_GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART6_IRQn, (INT_USART6_PRIO>>4) & 0x0F, INT_USART6_PRIO & 0x0F);
        HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
#endif /* RT_USING_USART6 */	
}

/**
  * @brief : UART MSP De-Initialization
  *          This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param : [huart] UART handle pointer.
  * @return: None. 
  */  
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#if defined(RT_USING_USART1)
    if (huart->Instance == USART1)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(USART1_TX_GPIO_PORT, USART1_TX_PIN);
        /* Configure UART Rx as alternate function */
        HAL_GPIO_DeInit(USART1_RX_GPIO_PORT, USART1_RX_PIN);

        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
#endif

#if defined(RT_USING_USART2)
    if (huart->Instance == USART2)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(USART2_TX_GPIO_PORT, USART2_TX_PIN);
			
        /* Configure UART Rx as alternate function */
        HAL_GPIO_DeInit(USART2_RX_GPIO_PORT, USART2_RX_PIN);

        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
#endif

#if defined(RT_USING_USART3)
    if (huart->Instance == USART3)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(USART3_TX_GPIO_PORT, USART3_TX_PIN);
        /* Configure UART Rx as alternate function */
        HAL_GPIO_DeInit(USART3_RX_GPIO_PORT, USART3_RX_PIN);

        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }
#endif

#if defined(RT_USING_UART4)
    if (huart->Instance == UART4)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(UART4_TX_GPIO_PORT, UART4_TX_PIN);
        /* Configure UART Rx as alternate function */
        HAL_GPIO_DeInit(UART4_RX_GPIO_PORT, UART4_RX_PIN);

        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
#endif

#if defined(RT_USING_UART5)
    if (huart->Instance == UART5)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_UART5_FORCE_RESET();
        __HAL_RCC_UART5_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(UART5_TX_GPIO_PORT, UART5_TX_PIN);
        /* Configure UART Rx as alternate function */
        HAL_GPIO_DeInit(UART5_RX_GPIO_PORT, UART5_RX_PIN);

        HAL_NVIC_DisableIRQ(UART5_IRQn);
    }
#endif
#if defined(RT_USING_USART6)
    if (huart->Instance == USART6)
    {
        /* #1 - Reset peripherals */
        __HAL_RCC_USART6_FORCE_RESET();
        __HAL_RCC_USART6_RELEASE_RESET();

        /* #2 - Disable peripherals and GPIO Clocks */
        /* Configure UART Tx as alternate function */
        HAL_GPIO_DeInit(USART6_TX_GPIO_PORT, USART6_TX_PIN);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(USART6_RX_GPIO_PORT, USART6_RX_PIN);

        HAL_NVIC_DisableIRQ(USART6_IRQn);
    }
#endif	
}


#if defined(RT_USING_USART1)
/**
  * @brief : USART1 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void USART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial1);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_USART1 */

#if defined(RT_USING_USART2)
/**
  * @brief : USART2 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void USART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial2);

    /* leave interrupt */
    rt_interrupt_leave();
}

#endif /* RT_USING_USART2 */

#if defined(RT_USING_USART3)
/**
  * @brief : USART3 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void USART3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial3);

    /* leave interrupt */
    rt_interrupt_leave();
}

#endif /* RT_USING_USART3 */

#if defined(RT_USING_UART4)
/**
  * @brief : UART4 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void UART4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial4);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART4 */

#if defined(RT_USING_UART5)
/**
  * @brief : UART5 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void UART5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial5);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART5 */

#if defined(RT_USING_USART6)
/**
  * @brief : USART6 IRQHanlder
  * @param : None.
  * @return: None. 
  */  
void USART6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial6);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART5 */

/**
  * @brief : USART hardware initialization
  * @param : None.
  * @return: None. 
  */  
int rt_hw_usart_init(void)
{
    struct stm32_uart *uart;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

#ifdef RT_USING_USART1
    uart = &uart1;
    uart->UartHandle.Instance = USART1;
	
    serial1.ops    = &stm32_uart_ops;
    serial1.config = config;

    /* register USART1 device */
//    rt_hw_serial_register(&serial1, "uart1", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial1, RT_USART1_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_USART1 */

#ifdef RT_USING_USART2
    uart = &uart2;
    uart->UartHandle.Instance = USART2;

    serial2.ops    = &stm32_uart_ops;
    serial2.config = config;

    /* register USART2 device */
//    rt_hw_serial_register(&serial2, "uart2", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial2, RT_USART2_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_USART2 */

#ifdef RT_USING_USART3
    uart = &uart3;
    uart->UartHandle.Instance = USART3;

    serial3.ops    = &stm32_uart_ops;
    serial3.config = config;

    /* register USART3 device */
//    rt_hw_serial_register(&serial3, "uart3", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial3, RT_USART3_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_USART3 */

#ifdef RT_USING_UART4
    uart = &uart4;
    uart->UartHandle.Instance = UART4;

    serial4.ops    = &stm32_uart_ops;
    serial4.config = config;

    /* register UART4 device */
//    rt_hw_serial_register(&serial4, "uart4", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial4, RT_UART4_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_UART4 */

#ifdef RT_USING_UART5
    uart = &uart5;
    uart->UartHandle.Instance = UART5;

    serial5.ops    = &stm32_uart_ops;
    serial5.config = config;

    /* register UART5 device */
//    rt_hw_serial_register(&serial5, "uart5", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial5, RT_UART5_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_UART5 */

#ifdef RT_USING_USART6
    uart = &uart6;
    uart->UartHandle.Instance = USART6;

    serial6.ops    = &stm32_uart_ops;
    serial6.config = config;

    /* register USART6 device */
//    rt_hw_serial_register(&serial6, "uart6", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
    rt_hw_serial_register(&serial6, RT_USART6_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* RT_USING_USART6 */

    return(0);
}
INIT_BOARD_EXPORT(rt_hw_usart_init);


/* END OF FILE ---------------------------------------------------------------*/

