


/* INCLUDE FILES -------------------------------------------------------------*/
#include "dp83848.h"
#include "lwip_comm.h"
#include "basicModule.h"

#include "rtthread.h"

ETH_HandleTypeDef ETH_Handler;      //以太网句柄
ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB];	//以太网DMA接收描述符数据结构体指针
ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB];	//以太网DMA发送描述符数据结构体指针
uint8_t Rx_Buff[ETH_RX_BUF_SIZE *ETH_RXBUFNB]; 					//以太网底层驱动接收buffers指针
uint8_t Tx_Buff[ETH_TX_BUF_SIZE *ETH_TXBUFNB]; 					//以太网底层驱动发送buffers指针

//LAN8720初始化
//返回值:0,成功;
//    其他,失败
HAL_StatusTypeDef eth_result;
uint8_t DP83848_Init(void)
{
    uint8_t macaddress[6];

    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_InitStructure.Pull = GPIO_PULLUP;        //上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;   //高
    HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);	//PI8置1

    //  INTX_DISABLE();                         //关闭所有中断，复位过程不能被打断！
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_RESET);	//硬件复位
    rt_thread_delay(100);
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);	//复位结束
    rt_thread_delay(100);
    // INTX_ENABLE();                          //开启所有中断

    macaddress[0] = lwipdev.mac[0];
    macaddress[1] = lwipdev.mac[1];
    macaddress[2] = lwipdev.mac[2];
    macaddress[3] = lwipdev.mac[3];
    macaddress[4] = lwipdev.mac[4];
    macaddress[5] = lwipdev.mac[5];

    ETH_Handler.Instance = ETH;
    ETH_Handler.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE; //使能自协商模式
    ETH_Handler.Init.Speed = ETH_SPEED_100M; //速度100M,如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.DuplexMode = ETH_MODE_FULLDUPLEX; //全双工模式，如果开启了自协商模式，此配置就无效
    ETH_Handler.Init.PhyAddress = DP83848_PHY_ADDRESS; //DP83848地址
    ETH_Handler.Init.MACAddr = macaddress;          //MAC地址
    ETH_Handler.Init.RxMode = ETH_RXINTERRUPT_MODE; //中断接收模式
    ETH_Handler.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE; //硬件帧校验
    //ETH_Handler.Init.ChecksumMode=ETH_CHECKSUM_BY_SOFTWARE  ;
    ETH_Handler.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII; //RMII接口
    eth_result = HAL_ETH_Init(&ETH_Handler);
    if(eth_result == HAL_OK)
    {
        return 0;   //成功
    }
    else
        return 1;  //失败
}

//ETH底层驱动，时钟使能，引脚配置
//此函数会被HAL_ETH_Init()调用
//heth:以太网句柄
/*************************************
网络引脚设置 RMII接口
ETH_MDIO -------------------------> PA2
ETH_MDC --------------------------> PC1
ETH_RMII_REF_CLK------------------> PA1
ETH_RMII_CRS_DV ------------------> PA7
ETH_RMII_RXD0 --------------------> PC4
ETH_RMII_RXD1 --------------------> PC5
ETH_RMII_TX_EN -------------------> PG11
ETH_RMII_TXD0 --------------------> PG13
ETH_RMII_TXD1 --------------------> PG14
ETH_RESET-------------------------> PI8
****************************************/
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIOs clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* Configure PA1, PA2 and PA7 */
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure PG11 13 14 */
    GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Configure PC1, PC4 and PC5 */
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Enable the Ethernet global Interrupt */
    HAL_NVIC_SetPriority(ETH_IRQn, (INT_ETHERNET_PRIO >> 4) & 0xFF, INT_ETHERNET_PRIO & 0xFF);
    HAL_NVIC_EnableIRQ(ETH_IRQn);

    /* Enable ETHERNET clock  */
    __HAL_RCC_ETH_CLK_ENABLE();
}
//读取PHY寄存器值
uint32_t DP83848_ReadPHY(u16 reg)
{
    uint32_t regval;
    HAL_ETH_ReadPHYRegister(&ETH_Handler, reg, &regval);
    return regval;
}
//向DP83848指定寄存器写入值
//reg:要写入的寄存器
//value:要写入的值
void DP83848_WritePHY(u16 reg, u16 value)
{
    uint32_t temp = value;
    HAL_ETH_ReadPHYRegister(&ETH_Handler, reg, &temp);
}
//得到DP83848的速度模式
//返回值:
//001:10M半双工
//101:10M全双工
//010:100M半双工
//110:100M全双工
//其他:错误.
//uint8_t DP83848_Get_Speed(void)
//{
//    uint8_t speed;
//    speed = ((DP83848_ReadPHY(31) & 0x1C) >> 2); //从LAN8720的31号寄存器中读取网络速度和双工模式
//    return speed;
//}


//中断服务函数
void ETH_IRQHandler(void)
{
    rt_interrupt_enter();
    while(ETH_GetRxPktSize(ETH_Handler.RxDesc))
    {
        lwip_pkt_handle();//处理以太网数据，即将数据提交给LWIP
    }
    //清除中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler, ETH_DMA_IT_NIS);   //清除DMA中断标志位
    __HAL_ETH_DMA_CLEAR_IT(&ETH_Handler, ETH_DMA_IT_R);     //清除DMA接收中断标志?
    rt_interrupt_leave();

}

//获取接收到的帧长度
//DMARxDesc:接收DMA描述符
//返回值:接收到的帧长度
uint32_t  ETH_GetRxPktSize(ETH_DMADescTypeDef *DMARxDesc)
{
    uint32_t frameLength = 0;
    if(((DMARxDesc->Status & ETH_DMARXDESC_OWN) == (uint32_t)RESET) &&
            ((DMARxDesc->Status & ETH_DMARXDESC_ES) == (uint32_t)RESET) &&
            ((DMARxDesc->Status & ETH_DMARXDESC_LS) != (uint32_t)RESET))
    {
        frameLength = ((DMARxDesc->Status & ETH_DMARXDESC_FL) >> ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
    return frameLength;
}






