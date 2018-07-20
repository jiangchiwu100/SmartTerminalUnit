/**
1.初始化后立刻发送一字节,之后开接收中断
*/

#include "serialport.h"





//全局变量
//UART6
FifoHandle Uart6FifoHandle;
static uint8_t Uart6Buffer[UART_6_BUFFER_LEN];
static PointUint8 Uart6BufferPack;

//UART5
FifoHandle Uart5FifoHandle;
static uint8_t Uart5Buffer[UART_5_BUFFER_LEN];
static PointUint8 Uart5BufferPack;

//UART4
FifoHandle Uart4FifoHandle;
static uint8_t Uart4Buffer[UART_4_BUFFER_LEN];
static PointUint8 Uart4BufferPack;

//UART3
FifoHandle Uart3FifoHandle;
static uint8_t Uart3Buffer[UART_3_BUFFER_LEN];
static PointUint8 Uart3BufferPack;

//UART1
FifoHandle Uart1FifoHandle;
static uint8_t Uart1Buffer[UART_1_BUFFER_LEN];
static PointUint8 Uart1BufferPack;



static void MX_UART5_Init(void);
static void MX_UART4_Init(void);
static void MX_USART3_Init(void);
static void MX_USART1_Init(void);
static void MX_USART6_Init(void);

static void MX_GPIO_Init(void);

static void MX_USART6_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);

    /**USART1 GPIO Configuration
    PC6   ------> USART6_TX
    PC7   ------> USART6_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART6, &USART_InitStruct);

    LL_USART_SetWakeUpMethod(USART6, LL_USART_WAKEUP_IDLELINE);

    LL_USART_ConfigMultiProcessMode(USART6);
    
   // LL_USART_EnableIT_PE(USART6);
    LL_USART_Enable(USART6);
    
    
    LL_USART_EnableIT_RXNE(USART6);
   
      /* USART1 interrupt Init */
    //NVIC_SetPriority(USART6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    //NVIC_EnableIRQ(USART6_IRQn);
}

static void MX_UART5_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);

    /**UART5 GPIO Configuration
    PC12   ------> UART5_TX
    PD2   ------> UART5_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

   

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(UART5, &USART_InitStruct);

    LL_USART_SetWakeUpMethod(UART5, LL_USART_WAKEUP_IDLELINE);

    LL_USART_ConfigMultiProcessMode(UART5);

   
    LL_USART_EnableIT_PE(UART5);
    LL_USART_Enable(UART5);
    
    //LL_USART_TransmitData8(UART5, 0x55);
    LL_mDelay(10);
    LL_USART_EnableIT_RXNE(UART5);
    
     /* UART5_IRQn interrupt configuration */
    NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(UART5_IRQn);

}
/* UART4 init function */
static void MX_UART4_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

    /**UART4 GPIO Configuration
    PC10   ------> UART4_TX
    PC11   ------> UART4_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(UART4, &USART_InitStruct);

    LL_USART_SetWakeUpMethod(UART4, LL_USART_WAKEUP_IDLELINE);

    LL_USART_ConfigMultiProcessMode(UART4);
    
    LL_USART_EnableIT_PE(UART4);

    LL_USART_Enable(UART4);
    
   // LL_USART_TransmitData8(UART4, 0x55);
   // LL_mDelay(10);
    LL_USART_EnableIT_RXNE(UART4);

    /* UART4 interrupt Init */
    NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(UART4_IRQn);
}


/* USART3 init function */
static void MX_USART3_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

    /**USART3 GPIO Configuration
    PB10   ------> USART3_TX
    PB11   ------> USART3_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART3, &USART_InitStruct);

    LL_USART_SetWakeUpMethod(USART3, LL_USART_WAKEUP_IDLELINE);

    LL_USART_ConfigMultiProcessMode(USART3);    
    LL_USART_EnableIT_PE(USART3);
    
    LL_USART_Enable(USART3);
    
    //LL_USART_TransmitData8(USART3, 0x55);
    LL_mDelay(10);
    LL_USART_EnableIT_RXNE(USART3);
    /* USART3 interrupt Init */
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART3_IRQn);

}

/* USART1 init function */
static void MX_USART1_Init(void)
{

    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    /**USART1 GPIO Configuration
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);

    LL_USART_SetWakeUpMethod(USART1, LL_USART_WAKEUP_IDLELINE);

    LL_USART_ConfigMultiProcessMode(USART1);

    LL_USART_EnableIT_PE(USART1);
    LL_USART_Enable(USART1);
    
   // LL_USART_TransmitData8(USART1, 0x55);
    LL_mDelay(10);
    LL_USART_EnableIT_RXNE(USART1);
    
     /* USART1 interrupt Init */
    NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART1_IRQn);

}





static void MX_GPIO_Init(void)
{

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

}

void SerialPortInit(void)
{
    Uart6BufferPack.len = UART_6_BUFFER_LEN;
    Uart6BufferPack.pData = Uart6Buffer;
    FifoInit(&Uart6FifoHandle, &Uart6BufferPack);
    
    Uart5BufferPack.len = UART_5_BUFFER_LEN;
    Uart5BufferPack.pData = Uart5Buffer;
    FifoInit(&Uart5FifoHandle, &Uart5BufferPack);

    Uart4BufferPack.len = UART_4_BUFFER_LEN;
    Uart4BufferPack.pData = Uart4Buffer;
    FifoInit(&Uart4FifoHandle, &Uart4BufferPack);

    Uart3BufferPack.len = UART_3_BUFFER_LEN;
    Uart3BufferPack.pData = Uart3Buffer;
    FifoInit(&Uart3FifoHandle, &Uart3BufferPack);

    Uart1BufferPack.len = UART_1_BUFFER_LEN;
    Uart1BufferPack.pData = Uart1Buffer;
    FifoInit(&Uart1FifoHandle, &Uart1BufferPack);


    MX_GPIO_Init();

    
    MX_UART5_Init();
    MX_UART4_Init();
    MX_USART6_Init();
    MX_USART3_Init();
    MX_USART1_Init();

}
void SerialPort4Init(void)
{


    Uart4BufferPack.len = UART_4_BUFFER_LEN;
    Uart4BufferPack.pData = Uart4Buffer;
    FifoInit(&Uart4FifoHandle, &Uart4BufferPack);
    MX_GPIO_Init();   
    MX_UART4_Init();


}
/**
  * @brief : 串口6初始化.
  * @param : [No]
  * @return: [No]
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void SerialPort6Init(void)
{

    Uart6BufferPack.len = UART_6_BUFFER_LEN;
    Uart6BufferPack.pData = Uart6Buffer;
    FifoInit(&Uart6FifoHandle, &Uart6BufferPack);
    
    MX_GPIO_Init();   
    MX_USART6_Init();


}
 uint8_t UartSend(USART_TypeDef *USARTx, uint8_t* pData, uint8_t len)
{
    uint16_t cn = 0;
    for(uint8_t i = 0; i < len; i++)
    {
        cn = 0;
        while(!LL_USART_IsActiveFlag_TC(USARTx))
         {
             if (cn++ > 10000)
             {
                 return 1;//发送失败，超时
             }
         };
         LL_USART_TransmitData8(USARTx, pData[i]);
         
         
    }
    //等待发送完毕
    cn = 0;
    while(!LL_USART_IsActiveFlag_TC(USARTx))
     {
         if (cn++ > 10000)
         {
             return 1;//发送失败，超时
         }
     };
    return 0;
}
/**
* @brief This function handles USART1 global interrupt.
*/
//void USART1_IRQHandler(void)
//{
//    /* USER CODE BEGIN USART1_IRQn 0 */

//    /* USER CODE END USART1_IRQn 0 */
//    /* USER CODE BEGIN USART1_IRQn 1 */

//    /* USER CODE END USART1_IRQn 1 */
//    uint8_t tmp;
//    if(LL_USART_IsActiveFlag_RXNE(USART1))
//    {
//        tmp = LL_USART_ReceiveData8(USART1);
//        Uart1FifoHandle.Enqueue(&Uart1FifoHandle, tmp);
//    }
//     if(LL_USART_IsActiveFlag_FE(USART1))
//     {
//         while(1);
//     }
//}

/**
* @brief This function handles USART3 global interrupt.
*/
//void USART3_IRQHandler(void)
//{
//    /* USER CODE BEGIN USART3_IRQn 0 */

//    /* USER CODE END USART3_IRQn 0 */
//    /* USER CODE BEGIN USART3_IRQn 1 */

//    /* USER CODE END USART3_IRQn 1 */
//    uint8_t tmp;
//    if(LL_USART_IsActiveFlag_RXNE(USART3))
//    {
//        tmp = LL_USART_ReceiveData8(USART3);
//        Uart3FifoHandle.Enqueue(&Uart3FifoHandle, tmp);
//    }
//}
//void USART6_IRQHandler(void)
//{

//    uint8_t tmp;
//    if(LL_USART_IsActiveFlag_RXNE(USART6))
//    {
//        tmp = LL_USART_ReceiveData8(USART6);
//        Uart6FifoHandle.Enqueue(&Uart6FifoHandle, tmp);
//    }
//}
/**
* @brief This function handles UART4 global interrupt.
*/
void UART4_IRQHandler(void)
{
    /* USER CODE BEGIN UART4_IRQn 0 */

    /* USER CODE END UART4_IRQn 0 */
    /* USER CODE BEGIN UART4_IRQn 1 */

    /* USER CODE END UART4_IRQn 1 */
    uint8_t tmp;
    if(LL_USART_IsActiveFlag_RXNE(UART4))
    {
        tmp = LL_USART_ReceiveData8(UART4);
        Uart4FifoHandle.Enqueue(&Uart4FifoHandle, tmp);
    }
}

//void UART5_IRQHandler(void)
//{

//    /* USER CODE BEGIN UART5_IRQn 0 */

//    /* USER CODE END UART5_IRQn 0 */
//    /* USER CODE BEGIN UART5_IRQn 1 */

//    /* USER CODE END UART5_IRQn 1 */

//    uint8_t tmp;
//    if(LL_USART_IsActiveFlag_RXNE(UART5))
//    {
//        //LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
//        tmp = LL_USART_ReceiveData8(UART5);
//        Uart5FifoHandle.Enqueue(&Uart5FifoHandle, tmp);
//        //LL_USART_TransmitData8(UART5,tmp);


//    }
//    
//}

