#include "ethernetif.h"
#include "dp83848.h"
#include "lwip_comm.h"

#include "string.h"
#include "rtthread.h"
//由ethernetif_init()调用用于初始化硬  件
//netif:网卡结构体指针
//返回值:ERR_OK,正常
//       其他,失败
static uint8_t low_level_init(void)
{


    HAL_ETH_DMATxDescListInit(&ETH_Handler,DMATxDscrTab,Tx_Buff,ETH_TXBUFNB);//初始化发送描述符
    HAL_ETH_DMARxDescListInit(&ETH_Handler,DMARxDscrTab,Rx_Buff,ETH_RXBUFNB);//初始化接收描述符
    HAL_ETH_Start(&ETH_Handler); //开启MAC和DMA
    return 0;
}
//用于发送数据包的最底层函数(lwip通过netif->linkoutput指向该函数)
//netif:网卡结构体指针
//p:pbuf数据结构体指针
//返回值:ERR_OK,发送正常
//       ERR_MEM,发送失败
uint8_t low_level_output(uint8_t* pData, uint16_t count)
{
	 uint8_t errval;
    uint8_t *buffer=(uint8_t *)(ETH_Handler.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    DmaTxDesc = ETH_Handler.TxDesc;

    if((DmaTxDesc->Status&ETH_DMATXDESC_OWN)!=(uint32_t)RESET)
   {
    	errval=1;
	   	goto error;             //发送描述符无效，不可用
   }
	if (count > ETH_TX_BUF_SIZE)
	{
		 errval=2;
		 goto error;             //发送描述符无效，不可用
	}
    memcpy(buffer, pData , count);
    errval = HAL_ETH_TransmitFrame(&ETH_Handler, count);
    

error:
    //发送缓冲区发生下溢，一旦发送缓冲区发生下溢TxDMA会进入挂起状态
    if((ETH_Handler.Instance->DMASR&ETH_DMASR_TUS)!=(uint32_t)RESET)
    {
        //清除下溢标志
        ETH_Handler.Instance->DMASR = ETH_DMASR_TUS;
        //当发送帧中出现下溢错误的时候TxDMA会挂起，这时候需要向DMATPDR寄存器
        //随便写入一个值来将其唤醒，此处我们写0
        ETH_Handler.Instance->DMATPDR=0;
    }
    return errval;
}
///用于接收数据包的最底层函数
//neitif:网卡结构体指针
//返回值:pbuf数据结构体指针
static uint8_t  low_level_input(uint8_t* pData, uint16_t*  pSize)
{
    struct pbuf *p = NULL;
    struct pbuf *q;
    uint16_t len;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;

    uint32_t i=0;

    if(HAL_ETH_GetReceivedFrame(&ETH_Handler)!=HAL_OK)  //判断是否接收到数据
        return 1;

    len = ETH_Handler.RxFrameInfos.length;                //获取接收到的以太网帧长度
    buffer=(uint8_t *)ETH_Handler.RxFrameInfos.buffer;  //获取接收到的以太网帧的数据buffer

    if (len > *pSize)
    {
    	return 2;
    }
	//拷贝剩余的数据
	memcpy(pData, buffer, len);
	*pSize = len;

    //释放DMA描述符
    dmarxdesc=ETH_Handler.RxFrameInfos.FSRxDesc;
    for(i=0; i<ETH_Handler.RxFrameInfos.SegCount; i++)
    {
        dmarxdesc->Status|=ETH_DMARXDESC_OWN;       //标记描述符归DMA所有
        dmarxdesc=(ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    ETH_Handler.RxFrameInfos.SegCount =0;           //清除段计数器
    if((ETH_Handler.Instance->DMASR &ETH_DMASR_RBUS)!=(uint32_t)RESET) //接收缓冲区不可用
    {
        //清除接收缓冲区不可用标志
        ETH_Handler.Instance->DMASR = ETH_DMASR_RBUS;
        //当接收缓冲区不可用的时候RxDMA会进去挂起状态，通过向DMARPDR写入任意一个值来唤醒Rx DMA
        ETH_Handler.Instance->DMARPDR=0;
    }
    return 0;
}
//网卡接收数据(lwip直接调用)
//netif:网卡结构体指针
//返回值:ERR_OK,发送正常
//       ERR_MEM,发送失败

uint8_t rxData[1600];
uint16_t rxCount = 0;
uint8_t ethernetif_input(void)
{

    rxCount = 1600;
    uint8_t ret = low_level_input(rxData, &rxCount);
    if (ret > 0)
    {
        rt_kprintf("Recive ERROR\n");
    }
    else
    {
        rt_kprintf("ReciveData rxCount : %d HEX:\n", rxCount);
        uint16_t i;
        for(i = 0; i < rxCount; i++)
        {
            rt_kprintf("%2X ", rxData[i]);
        }
       rt_memcpy(rxData + 6,  lwipdev.mac, 6);
        
        ret = low_level_output(rxData, rxCount);
        if (ret)
        {
            rt_kprintf("Send Failure!\n");
        }
        
    
        rt_kprintf("\r\n\n");
    }
    
    
    return 0;
}
//使用low_level_init()函数来初始化网络
//netif:网卡结构体指针
//返回值:ERR_OK,正常
//       其他,失败
uint8_t ethernetif_init(void)
{

    low_level_init();
    return 0;
}














