 /** 
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_eth.c
  * @brief:     The driver of ethernet,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version)
  * @author:    tanek
  * @date:      2017-06-08
  * @update:    [2017-11-22][Lexun][The STM32F7 version(V1.0.0)]
  */

/* INCLUDE FILES -------------------------------------------------------------*/
#include <finsh.h>
#include <netif/ethernetif.h>
#include "drv_eth.h"
#include "lwipopts.h"
#include "board.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static ETH_DMADescTypeDef DMARxDscrTab[ETH_RXBUFNB];
static ETH_DMADescTypeDef DMATxDscrTab[ETH_TXBUFNB];
static rt_uint8_t Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE];
static rt_uint8_t Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE];
static rt_bool_t tx_is_waiting = RT_FALSE;
static  ETH_HandleTypeDef EthHandle;
static struct rt_stm32_eth stm32_eth_device;
static struct rt_semaphore tx_wait;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  delay function.
  * @param  [nms] n(ms) delay.
  * @retval None
  */
static void delay_ms(rt_uint32_t nms)
{
//    rt_thread_delay((RT_TICK_PER_SECOND * nms + 999) / 1000);
    while (nms--)
    {
        int i;
        for (i = 0; i < 10000; i++)
        {
            __NOP();
        }
    }
}

/**
  * @brief  phy pin reset.
  * @param  None.
  * @retval None
  */
static void phy_pin_reset(void)
{
    rt_base_t level;
    
    level = rt_hw_interrupt_disable();
    
    ETH_WKRST_OFF;
    delay_ms(100);
	
    ETH_WKRST_ON;
    delay_ms(100);
    
    rt_hw_interrupt_enable(level);
}
#ifdef DEBUG
FINSH_FUNCTION_EXPORT(phy_pin_reset, phy hardware reset);
#endif

/**
  * @brief  NVIC configuration function.
  * @param  None.
  * @retval None
  */
static void NVIC_Configuration(void)
{	
    /* Enable the Ethernet global Interrupt */
    HAL_NVIC_SetPriority(ETH_IRQn, (INT_ETH_PRIO>>4) & 0x0F, INT_ETH_PRIO&0x0F);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

/**
  * @brief  GPIO Configuration for ethernet.
  * @param  None.
  * @retval None
  */
static void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    ETH_PRINTF("GPIO_Configuration...\n");
    
    /* Enable SYSCFG clock */
    __HAL_RCC_ETH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF11_ETH;
    GPIO_InitStructure.Pull  = GPIO_NOPULL;

    GPIO_InitStructure.Pin = ETH_MDIO_PIN;
    HAL_GPIO_Init(ETH_MDIO_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_MDC_PIN;
    HAL_GPIO_Init(ETH_MDC_PORN,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = ETH_RMII_REF_CLK_PIN;
    HAL_GPIO_Init(ETH_RMII_REF_CLK_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_RMII_CRS_DV_PIN;
    HAL_GPIO_Init(ETH_RMII_CRS_DV_PORN,&GPIO_InitStructure);

    GPIO_InitStructure.Pin = ETH_RMII_REF_CLK_PIN;
    HAL_GPIO_Init(ETH_RMII_REF_CLK_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_RMII_CRS_DV_PIN;
    HAL_GPIO_Init(ETH_RMII_CRS_DV_PORN,&GPIO_InitStructure);
	
    GPIO_InitStructure.Pin = ETH_RMII_RXD0_PIN;
    HAL_GPIO_Init(ETH_RMII_RXD0_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_RMII_RXD1_PIN;
    HAL_GPIO_Init(ETH_RMII_RXD1_PORN,&GPIO_InitStructure);
	
    GPIO_InitStructure.Pin = ETH_RMII_TX_EN_PIN;
    HAL_GPIO_Init(ETH_RMII_TX_EN_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_RMII_TXD0_PIN;
    HAL_GPIO_Init(ETH_RMII_TXD0_PORN,&GPIO_InitStructure);
    GPIO_InitStructure.Pin = ETH_RMII_TXD1_PIN;
    HAL_GPIO_Init(ETH_RMII_TXD1_PORN,&GPIO_InitStructure);
    
    GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pin = ETH_WKRST_PIN;
    HAL_GPIO_Init(ETH_WKRST_PORN,&GPIO_InitStructure);
    phy_pin_reset();
	
    HAL_NVIC_SetPriority(ETH_IRQn, (INT_ETH_PRIO>>4)&0x0F, INT_ETH_PRIO&0x0F);
    HAL_NVIC_EnableIRQ(ETH_IRQn);
}

/**
  * @brief  initialize the interface.
  * @param  [dev] the device pointer.
  * @retval [RT_EOK] success.
  */
static rt_err_t rt_stm32_eth_init(rt_device_t dev)
{
    ETH_PRINTF("rt_stm32_eth_init...\n");
	
    __HAL_RCC_ETH_CLK_ENABLE();
	
    phy_pin_reset();

    /* ETHERNET Configuration --------------------------------------------------*/
    EthHandle.Instance = ETH;  
    EthHandle.Init.MACAddr = (rt_uint8_t*)&stm32_eth_device.dev_addr[0];
    EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
    EthHandle.Init.Speed = ETH_SPEED_100M;
    EthHandle.Init.DuplexMode = ETH_MODE_FULLDUPLEX;
    EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;
    EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
    EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_SOFTWARE;
    //EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
    EthHandle.Init.PhyAddress = DP83848_PHY_ADDRESS;
	
    HAL_ETH_DeInit(&EthHandle);
	
    /* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
    if (HAL_ETH_Init(&EthHandle) == HAL_OK)
    {
        ETH_PRINTF("eth hardware init sucess...\n");
    }
    else
    {
        ETH_PRINTF("eth hardware init faild...\n");
    }

    /* Initialize Tx Descriptors list: Chain Mode */
    HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	 
    /* Initialize Rx Descriptors list: Chain Mode  */
    HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    /* Enable MAC and DMA transmission and reception */    
    if (HAL_ETH_Start(&EthHandle) == HAL_OK)
    {
        ETH_PRINTF("eth hardware start success...\n");
    }
    else
    {
        ETH_PRINTF("eth hardware start faild...\n");
    }
    
    //phy_monitor_thread_entry(NULL);

    return(RT_EOK);
}

/**
  * @brief  open the ethernet interface.
  * @param  [dev] the device pointer.
  * @param  [oflag] the flag.
  * @retval [RT_EOK] success.
  */
static rt_err_t rt_stm32_eth_open(rt_device_t dev, rt_uint16_t oflag)
{
    ETH_PRINTF("rt_stm32_eth_open...\n");
    return(RT_EOK);
}

/**
  * @brief  close the ethernet interface.
  * @param  [dev] the device pointer.
  * @retval [RT_EOK] success.
  */
static rt_err_t rt_stm32_eth_close(rt_device_t dev)
{
    ETH_PRINTF("rt_stm32_eth_close...\n");
    return(RT_EOK);
}

/**
  * @brief  read the ethernet interface.
  * @param  [dev] the device pointer.
  * @param  [pos] the pos.
  * @param  [buffer] the buffer.
  * @param  [size] the size of buffer.
  * @retval [0] success.
  */
static rt_size_t rt_stm32_eth_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    ETH_PRINTF("rt_stm32_eth_read...\n");
    rt_set_errno(-RT_ENOSYS);
    return(0);
}

/**
  * @brief  write the ethernet interface.
  * @param  [dev] the device pointer.
  * @param  [pos] the pos.
  * @param  [buffer] the buffer.
  * @param  [size] the size of buffer.
  * @retval [0] success.
  */
static rt_size_t rt_stm32_eth_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    ETH_PRINTF("rt_stm32_eth_write...\n");
    rt_set_errno(-RT_ENOSYS);
    return(0);
}

/**
  * @brief  control the ethernet interface.
  * @param  [dev] the device pointer.
  * @param  [cmd] the command.
  * @param  [args] the parameter.
  * @retval [RT_EOK] success.
  */
static rt_err_t rt_stm32_eth_control(rt_device_t dev, int cmd, void *args)
{
    ETH_PRINTF("rt_stm32_eth_control...\n");
    switch(cmd)
    {
    case NIOCTL_GADDR:
        /* get mac address */
        if (args) 
        {
            rt_memcpy(args, stm32_eth_device.dev_addr, 6);
        }
        else 
        {
            return -RT_ERROR;
        }
        break;

    default:
        break;
    }

    return(RT_EOK);
}


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
    GPIO_Configuration();
    NVIC_Configuration();
}

/**
  * @brief  ethernet interrupt service.
  * @param  None.
  * @retval None.
  */
void ETH_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();
    
    HAL_ETH_IRQHandler(&EthHandle);
    
    /* leave interrupt */
    rt_interrupt_leave();
}

/**
  * @brief  ethernet tx callback function.
  * @param  [heth] the ETH_HandleTypeDef pointer.
  * @retval None.
  */
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
    if (tx_is_waiting == RT_TRUE)
    {
        tx_is_waiting = RT_FALSE;
        rt_sem_release(&tx_wait);
    }
}

/**
  * @brief  ethernet rx callback function.
  * @param  [heth] the ETH_HandleTypeDef pointer.
  * @retval None.
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
    rt_err_t result;
    result = eth_device_ready(&(stm32_eth_device.parent));
    if (result != RT_EOK)
		{
        ETH_PRINTF("RX err =%d\n", result );
		}
}

/**
  * @brief  ethernet error callback function.
  * @param  [heth] the ETH_HandleTypeDef pointer.
  * @retval None.
  */
void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
    ETH_PRINTF("eth err\n");
}

/**
  * @brief  ethernet device interface, transmit packet.
  * @param  [dev] the ETH_HandleTypeDef pointer.
  * @param  [p] the ETH_HandleTypeDef pointer.
  * @retval [RT_ERROR] error;[ERR_OK] success.
  */
rt_err_t rt_stm32_eth_tx( rt_device_t dev, struct pbuf* p)
{
    rt_err_t ret = RT_ERROR;
    HAL_StatusTypeDef state;
    struct pbuf *q;
    uint8_t *buffer = (uint8_t *)(EthHandle.TxDesc->Buffer1Addr);
    __IO ETH_DMADescTypeDef *DmaTxDesc;
    uint32_t framelength = 0;
    uint32_t bufferoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t payloadoffset = 0;
        
    DmaTxDesc = EthHandle.TxDesc;
    bufferoffset = 0;
    
    ETH_PRINTF("rt_stm32_eth_tx...\n");
   
    /* Check if the descriptor is owned by the ETHERNET DMA (when set) or CPU (when reset) */
    while ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
        rt_err_t result;
        rt_uint32_t level;

        level = rt_hw_interrupt_disable();
        tx_is_waiting = RT_TRUE;
        rt_hw_interrupt_enable(level);

        /* it's own bit set, wait it */
        result = rt_sem_take(&tx_wait, RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            break;
        }
        if (result == -RT_ERROR)
        {
            return -RT_ERROR;
        }
    }
    
    /* copy frame from pbufs to driver buffers */
    for(q = p; q != NULL; q = q->next)
    {
        /* Is this buffer available? If not, goto error */
        if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
        {
            ETH_PRINTF("buffer not valid ...\n");
            ret = ERR_USE;
            goto error;
        }
        
        ETH_PRINTF("copy one frame\n");
        
        /* Get bytes in current lwIP buffer */
        byteslefttocopy = q->len;
        payloadoffset = 0;

        /* Check if the length of data to copy is bigger than Tx buffer size*/
        while ((byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE)
        {
            /* Copy data to Tx buffer*/
            memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

            /* Point to next descriptor */
            DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

            /* Check if the buffer is available */
            if ((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
            {
                ETH_PRINTF("dmatxdesc buffer not valid ...\n");
                ret = ERR_USE;
                goto error;
            }

            buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);

            byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
            payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
            framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
            bufferoffset = 0;
        }

        /* Copy the remaining bytes */
        memcpy((uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy);
        bufferoffset = bufferoffset + byteslefttocopy;
        framelength = framelength + byteslefttocopy;
    }
    
#ifdef ETH_TX_DUMP
    {
        rt_uint32_t i;
        rt_uint8_t *ptr = buffer;

        ETH_PRINTF("tx_dump, len:%d\r\n", p->tot_len);
        for (i=0; i<p->tot_len; i++)
        {
            ETH_PRINTF("%02x ",*ptr);
            ptr++;

            if (((i+1)%8) == 0)
            {
                ETH_PRINTF("  ");
            }
            if(((i+1)%16) == 0)
            {
                ETH_PRINTF("\r\n");
            }
        }
        ETH_PRINTF("\r\ndump done!\r\n");
    }
#endif

    /* Prepare transmit descriptors to give to DMA */ 
    ETH_PRINTF("transmit frame, length: %d\n", framelength);
    state = HAL_ETH_TransmitFrame(&EthHandle, framelength);
    if (state != HAL_OK)
    {
        ETH_PRINTF("eth transmit frame faild: %d\n", state);
    }

    ret = ERR_OK;
  
error:
  
    /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
    {
        /* Clear TUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_TUS;

        /* Resume DMA transmission*/
        EthHandle.Instance->DMATPDR = 0;
    }
        
    return ret;
}

/**
  * @brief  ethernet device interface, reception packet.
  * @param  [dev] the ETH_HandleTypeDef pointer.
  * @retval return the reception packet.
  */
struct pbuf *rt_stm32_eth_rx(rt_device_t dev)
{
    struct pbuf *p = NULL;
    struct pbuf *q = NULL;
    HAL_StatusTypeDef state;
    uint16_t len = 0;
    uint8_t *buffer;
    __IO ETH_DMADescTypeDef *dmarxdesc;
    uint32_t bufferoffset = 0;
    uint32_t payloadoffset = 0;
    uint32_t byteslefttocopy = 0;
    uint32_t i = 0;
	
    ETH_PRINTF("rt_stm32_eth_rx\n");

    /* Get received frame */
    state = HAL_ETH_GetReceivedFrame_IT(&EthHandle);
    if (state != HAL_OK)
    {
        ETH_PRINTF("receive frame faild\n");
        return NULL;
    }
    
    /* Obtain the size of the packet and put it into the "len" variable. */
    len = EthHandle.RxFrameInfos.length;
    buffer = (uint8_t *)EthHandle.RxFrameInfos.buffer;

    ETH_PRINTF("receive frame len : %d\n", len);

    if (len > 0)
    {
        /* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
        p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    }

#ifdef ETH_RX_DUMP
    {
        rt_uint32_t i;
        rt_uint8_t *ptr = buffer;

        ETH_PRINTF("rx_dump, len:%d\r\n", p->tot_len);
        for (i = 0; i < len; i++)
        {
            ETH_PRINTF("%02x ", *ptr);
            ptr++;

            if (((i + 1) % 8) == 0)
            {
                ETH_PRINTF("  ");
            }
            if (((i + 1) % 16) == 0)
            {
                ETH_PRINTF("\r\n");
            }
        }
        ETH_PRINTF("\r\ndump done!\r\n");
    }
#endif
    
    if (p != NULL)
    {
        dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
        bufferoffset = 0;
        for(q = p; q != NULL; q = q->next)
        {
            byteslefttocopy = q->len;
            payloadoffset = 0;

            /* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size*/
            while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
            {
                /* Copy data to pbuf */
                memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

                /* Point to next descriptor */
                dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
                buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);

                byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
                payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
                bufferoffset = 0;
            }
            /* Copy remaining data in pbuf */
            memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
            bufferoffset = bufferoffset + byteslefttocopy;
        }
    }  
  
    /* Release descriptors to DMA */
    /* Point to first descriptor */
    dmarxdesc = EthHandle.RxFrameInfos.FSRxDesc;
		
    /* Set Own bit in Rx descriptors: gives the buffers back to DMA */
    for (i=0; i< EthHandle.RxFrameInfos.SegCount; i++)
    {  
        dmarxdesc->Status |= ETH_DMARXDESC_OWN;
        dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
    }
    
    /* Clear Segment_Count */
    EthHandle.RxFrameInfos.SegCount =0;  
  
    /* When Rx Buffer unavailable flag is set: clear it and resume reception */
    if ((EthHandle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
    {
        /* Clear RBUS ETHERNET DMA flag */
        EthHandle.Instance->DMASR = ETH_DMASR_RBUS;
        /* Resume DMA reception */
        EthHandle.Instance->DMARPDR = 0;
    }

    return p;
}

/**
  * @brief  ethernet hardware initializes.
  * @param  None.
  * @retval [RT_EOK] success.
  */
int rt_hw_eth_init(void)
{
    rt_err_t state;
    
    stm32_eth_device.ETH_Speed = ETH_SPEED_100M;
    stm32_eth_device.ETH_Mode  = ETH_MODE_FULLDUPLEX;

    /* OUI 00-80-E1 STMICROELECTRONICS. */
    stm32_eth_device.dev_addr[0] = 0x00;
    stm32_eth_device.dev_addr[1] = 0x80;
    stm32_eth_device.dev_addr[2] = 0xE1;
	
    /* generate MAC addr from 96bit unique ID (only for test). */
    stm32_eth_device.dev_addr[3] = *(rt_uint8_t*)(UID_BASE+4);
    stm32_eth_device.dev_addr[4] = *(rt_uint8_t*)(UID_BASE+2);
    stm32_eth_device.dev_addr[5] = *(rt_uint8_t*)(UID_BASE+0);

    stm32_eth_device.parent.parent.init       = rt_stm32_eth_init;
    stm32_eth_device.parent.parent.open       = rt_stm32_eth_open;
    stm32_eth_device.parent.parent.close      = rt_stm32_eth_close;
    stm32_eth_device.parent.parent.read       = rt_stm32_eth_read;
    stm32_eth_device.parent.parent.write      = rt_stm32_eth_write;
    stm32_eth_device.parent.parent.control    = rt_stm32_eth_control;
    stm32_eth_device.parent.parent.user_data  = RT_NULL;

    stm32_eth_device.parent.eth_rx     = rt_stm32_eth_rx;
    stm32_eth_device.parent.eth_tx     = rt_stm32_eth_tx;

    ETH_PRINTF("sem init: tx_wait\r\n");
    /* init tx semaphore */
    rt_sem_init(&tx_wait, "tx_wait", 0, RT_IPC_FLAG_FIFO);

    /* register eth device */
    ETH_PRINTF("eth_device_init start\r\n");
//    state = eth_device_init(&(stm32_eth_device.parent), "e0");
    state = eth_device_init(&(stm32_eth_device.parent), RT_LWIP_ETH_NAME);
		
    if (RT_EOK == state)
    {
        ETH_PRINTF("eth_device_init success\r\n");
    }
    else
    {
        ETH_PRINTF("eth_device_init faild: %d\r\n", state);
    }
		
    return state;
}
INIT_DEVICE_EXPORT(rt_hw_eth_init);


/* END OF FILE ---------------------------------------------------------------*/


