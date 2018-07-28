
/**

*2018.4.10
* 1.修改MAC回获取方式为STM32 ID号
*/
#include "ll_driver.h"
#include "device.h"
#include "config.h"
#include "socket.h"
#include "ult.h"
#include "w5500.h"
#include "main.h"
#include "stm32f4xx_ll_spi.h"

#include <stdio.h> 
#include <string.h>

CONFIG_MSG  ConfigMsg, RecvMsg;

uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// 选择8个Socket每个Socket发送缓存的大小，在w5500.c的void sysinit()有设置过程
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};		// 选择8个Socket每个Socket接收缓存的大小，在w5500.c的void sysinit()有设置过程

extern uint8 MAC[6];

uint8 pub_buf[1460];

#define DATA_BUF_SIZE   1024
DMA_InitTypeDef DMA_InitStructure;

static LL_DMA_InitTypeDef LL_DMA_InitStructure;
static LL_DMA_InitTypeDef LL_DMA_InitStructure_Write;

uint8_t pTmpBuf1[DATA_BUF_SIZE + 3];
uint8_t pTmpBuf2[DATA_BUF_SIZE + 3];

static void spi_dma_init(void);




static void GPIO_Configuration(void)
{
     GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOI_CLK_ENABLE();           //使能GPIOI时钟
      
    //PI8  RESET
    GPIO_Initure.Pin=GPIO_PIN_8;            //PI8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速         
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);     //初始化
    
    
    //PI0 CS
    GPIO_Initure.Pin=GPIO_PIN_0;            //PI0
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速         
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);     //初始化
  
}
void Reset_W5500(void)
{
    WIZ_RESET = 0;
    LL_mDelay(5);
    WIZ_RESET = 1;
    LL_mDelay(1600);
  
}
//reboot 
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}




uint8_t compare_array( void* const p1, void* const  p2, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        if (((uint8_t *)p1)[i] != ((uint8_t *)p2)[i])
        {
            return i + 1;
        }
    }
    return 0;
    
}
 uint8_t ip[12];

/**
  * @brief : 设置网络
  * @param : None.
  * @return: 0-设置成功，非0失败
  */
uint8_t set_network(void)														// 配置初始化IP信息并打印，初始化8个Socket
{
 
    
    getVER(ip);
    if (ip[0] != 0x04)
    {
        return 11;
    }

    while(1)
    {
        setSHAR(ConfigMsg.mac);
        getSHAR(ip);
        if (compare_array(ip, ConfigMsg.mac, 6))
        {
            //return 12;
            continue;
        } 
        else
        {
            break;
        }            
    }
    
    setSUBR(ConfigMsg.sub);
    getSUBR(ip);
    if (compare_array(ip, ConfigMsg.sub, 4))
    {
        return 12;
    }
    
    setGAR(ConfigMsg.gw);
    getGAR(ip);
    if (compare_array(ip, ConfigMsg.gw, 4))
    {
        return 13;
    }
    
    setSIPR(ConfigMsg.lip);
    getSIPR (ip);
    if (compare_array(ip, ConfigMsg.lip, 4))
    {
        return 11;
    }
    
    sysinit(txsize, rxsize); 												// 初始化8个socket

    setRTR(2000);																		// 设置超时时间
    setRCR(3);																			// 设置最大重新发送次数

  



   


   

    return 0;
    
}
    uint8 lip[4]={192,168,10,114};
    uint8 sub[4]={255,255,255,0};
    uint8 gw[4]={192,168,10,254};
    uint8 dns[4]={8,8,8,8};
void set_default(void)														// 设置默认MAC、IP、GW、SUB、DNS
{  
    uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

    uint32_t sn0=*(__IO uint32_t*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字 
    mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
    mac[1]=0;
    mac[2]=0;
    mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
    mac[4]=(sn0>>8)&0XFFF;
    mac[5]=sn0&0XFF; 



    memcpy(ConfigMsg.lip, lip, 4);
    memcpy(ConfigMsg.sub, sub, 4);
    memcpy(ConfigMsg.gw,  gw, 4);
    memcpy(ConfigMsg.mac, mac,6);
    memcpy(ConfigMsg.dns,dns,4);

    ConfigMsg.dhcp=0;
    ConfigMsg.debug=1;
    ConfigMsg.fw_len=0;

    ConfigMsg.state=NORMAL_STATE;
    ConfigMsg.sw_ver[0]=FW_VER_HIGH;
    ConfigMsg.sw_ver[1]=FW_VER_LOW;
  
}
void write_config_to_eeprom(void)
{
  uint8 data;
  uint16 i,j;
  uint16 dAddr=0;
  for (i = 0, j = 0; i < (uint8)(sizeof(ConfigMsg)-4);i++) 
  {
    data = *(uint8 *)(ConfigMsg.mac+j);
    
    dAddr += 1;
    j +=1;
  }
}
void get_config(void)
{
  uint8 tmp=0;
  uint16 i;
  for (i =0; i < CONFIG_MSG_LEN; i++) 
  {
   // tmp=at24c16_read(i);
    *(ConfigMsg.mac+i) = 0xff;
  }
  if((ConfigMsg.mac[0]==0xff)&&(ConfigMsg.mac[1]==0xff)&&(ConfigMsg.mac[2]==0xff)&&(ConfigMsg.mac[3]==0xff)&&(ConfigMsg.mac[4]==0xff)&&(ConfigMsg.mac[5]==0xff))
  {
    set_default();
  }
}

// 中断配置程序
//void NVIC_Configuration(void)
//{
//    LL_EXTI_InitTypeDef EXTI_InitStruct;
//    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
//    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE3);
//    
//    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
//    EXTI_InitStruct.LineCommand = ENABLE;
//    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
//    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
//    LL_EXTI_Init(&EXTI_InitStruct);
//    
//    LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
//    LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
//    
//    
// //   LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE3);
//    
//  /* EXTI interrupt init*/
//    NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
//    NVIC_EnableIRQ(EXTI3_IRQn);    
//}

/* SPI2 init function */
void LL_SPI2_Init(void)
{
    
  LL_SPI_InitTypeDef SPI_InitStruct;

  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
  
  /**SPI2 GPIO Configuration  
  PI1   ------> SPI2_SCK
  PI2   ------> SPI2_MISO
  PI3   ------> SPI2_MOSI 
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* SPI2 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI2, &SPI_InitStruct);

  LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
#if   !defined(SPI_DMA) 
   LL_SPI_Enable(SPI2);  
#endif
  while (!LL_SPI_IsActiveFlag_TXE(SPI2));
  
  spi_dma_init();

}
uint8_t error = 0;
uint8_t SPI2_SendByte(uint8_t txData)
{
    error = 0;
    u8 rxdata;    
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, txData);
    
    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    rxdata = LL_SPI_ReceiveData8(SPI2);    
    return rxdata;
}
void w5500_spi_init(void)
{
    GPIO_Configuration();   
    LL_SPI2_Init();
    
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_3);
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_4);
}
void spi_dma_init(void)
{
    
    LL_DMA_InitStructure_Write.FIFOMode = LL_DMA_FIFOMODE_DISABLE;
    LL_DMA_InitStructure_Write.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
    LL_DMA_InitStructure_Write.MemBurst = DMA_MBURST_SINGLE;
    LL_DMA_InitStructure_Write.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure_Write.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    LL_DMA_InitStructure_Write.Mode = LL_DMA_MODE_NORMAL;
    
    LL_DMA_InitStructure_Write.PeriphOrM2MSrcAddress = (uint32_t)(&(SPI2->DR));
    LL_DMA_InitStructure_Write.PeriphBurst = DMA_PBURST_SINGLE;
    LL_DMA_InitStructure_Write.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    LL_DMA_InitStructure_Write.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure_Write.PeriphOrM2MSrcIncMode =  LL_DMA_PERIPH_NOINCREMENT;
    LL_DMA_InitStructure_Write.Priority = LL_DMA_PRIORITY_HIGH;
    
    
    
    
    LL_DMA_InitStructure.FIFOMode = LL_DMA_FIFOMODE_ENABLE;
    LL_DMA_InitStructure.FIFOThreshold = LL_DMA_FIFOTHRESHOLD_1_4;
    LL_DMA_InitStructure.MemBurst = DMA_MBURST_SINGLE;
    LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    LL_DMA_InitStructure.Mode = LL_DMA_MODE_NORMAL;
    
    LL_DMA_InitStructure.PeriphOrM2MSrcAddress = (uint32_t)(&(SPI2->DR));
    LL_DMA_InitStructure.PeriphBurst = DMA_PBURST_SINGLE;
    LL_DMA_InitStructure.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    LL_DMA_InitStructure.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    LL_DMA_InitStructure.PeriphOrM2MSrcIncMode =  LL_DMA_PERIPH_NOINCREMENT;
    LL_DMA_InitStructure.Priority = LL_DMA_PRIORITY_HIGH;
}
void spi_dma_write(uint8_t* Addref, uint8_t* pTxBuf, uint16_t tx_len)
{
	uint16_t i;
	memset(pTmpBuf1, 0, tx_len + 3);
    
	pTmpBuf1[0] = Addref[0];
	pTmpBuf1[1] = Addref[1];
	pTmpBuf1[2] = Addref[2];
    
//	for(i=0; i<tx_len; i++)
//		pTmpBuf1[3 + i] = pTxBuf[i];
    
    memcpy(pTmpBuf1 + 3, pTxBuf, tx_len);
   
    LL_DMA_InitStructure_Write.NbData =  (uint32_t)(tx_len + 3);
    
    /* Configure Tx DMA */
    LL_DMA_InitStructure_Write.Channel = LL_DMA_CHANNEL_0;
    LL_DMA_InitStructure_Write.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    LL_DMA_InitStructure_Write.MemoryOrM2MDstAddress = (uint32_t) pTmpBuf1;    
    LL_DMA_Init(DMA1, LL_DMA_STREAM_4, &LL_DMA_InitStructure_Write);
    
    /* Configure Rx DMA */
    LL_DMA_InitStructure_Write.Channel = LL_DMA_CHANNEL_0;
    LL_DMA_InitStructure_Write.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY ;
    LL_DMA_InitStructure_Write.MemoryOrM2MDstAddress = (uint32_t) pTmpBuf1;    
    LL_DMA_Init(DMA1, LL_DMA_STREAM_3, &LL_DMA_InitStructure_Write);
    
    LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_3);
    LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_4);

    W5500_CS_SELECT();
    
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    
    LL_SPI_EnableDMAReq_RX(SPI2);
    LL_SPI_EnableDMAReq_TX(SPI2);    
    LL_SPI_Enable(SPI2);
    /* Waiting the end of Data transfer */
    while (!LL_DMA_IsActiveFlag_TC4(DMA1));
    while (!LL_DMA_IsActiveFlag_TC3(DMA1));
    
    W5500_CS_DESELECT();
    
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
  
    LL_SPI_DisableDMAReq_RX(SPI2);
    LL_SPI_DisableDMAReq_TX(SPI2);
    LL_SPI_Disable(SPI2);
}

void spi_dma_read(uint8_t* Addref, uint8_t* pRxBuf, uint16_t rx_len)
{
	uint16_t i;
	memset(pTmpBuf1, 0, rx_len + 3);
	memset(pTmpBuf2, 0, rx_len + 3);
	pTmpBuf1[0] = Addref[0];
	pTmpBuf1[1] = Addref[1];
	pTmpBuf1[2] = Addref[2];
    
  
    LL_DMA_InitStructure.NbData =  rx_len + 3;
    /* Configure Tx DMA */
    LL_DMA_InitStructure.Channel = LL_DMA_CHANNEL_0;
    LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t) pTmpBuf1;    
    LL_DMA_Init(DMA1, LL_DMA_STREAM_4, &LL_DMA_InitStructure);
    
    /* Configure Rx DMA */
    LL_DMA_InitStructure.Channel = LL_DMA_CHANNEL_0;
    LL_DMA_InitStructure.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY ;
    LL_DMA_InitStructure.MemoryOrM2MDstAddress = (uint32_t) pTmpBuf2;
    
    LL_DMA_Init(DMA1, LL_DMA_STREAM_3, &LL_DMA_InitStructure);
    
     
    LL_SPI_EnableDMAReq_RX(SPI2);
    LL_SPI_EnableDMAReq_TX(SPI2);
           
    W5500_CS_SELECT();
    
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    LL_SPI_Enable(SPI2);
   
    /* Waiting the end of Data transfer */
    while (!LL_DMA_IsActiveFlag_TC4(DMA1));
    while (!LL_DMA_IsActiveFlag_TC3(DMA1));
    
    
    W5500_CS_DESELECT();
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
    
    LL_SPI_DisableDMAReq_RX(SPI2);
    LL_SPI_DisableDMAReq_TX(SPI2);  
    LL_SPI_Disable(SPI2);    
    
    memcpy(pRxBuf, pTmpBuf2 + 3, rx_len);
}
