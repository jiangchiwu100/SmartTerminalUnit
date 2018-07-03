/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      w5500.c
  * @brief:     The driver of w5500.
  * @version:   V1.0.0
  * @author:    Lexun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "drv_w5500.h"
#include "drv_w5500_conf.h"
#include "drv_spi.h"
#include "string.h"
#include "drv_gpio.h"
#include "common_data.h"
#include ".\MultiThread\multi_thread.h"
#include "stm32f4xx_ll_spi.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_dma.h"

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct spi_w5500_device device_w5500; 
static struct rt_device_pin *w5500_pin; 
static LL_DMA_InitTypeDef LL_DMA_InitStructure;
static LL_DMA_InitTypeDef LL_DMA_InitStructure_Write;

#define DATA_BUF_SIZE   1024
uint8_t *pTmpBuf1;
uint8_t *pTmpBuf2;
/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  w5500 device select.
  * @param  None.
  * @retval None.
  */
static void w5500_cs_select(void)
{
    pin_status[INDEX_W5500_CS].status = GPIO_PIN_RESET;			
    rt_device_write(&w5500_pin->parent, 0, &pin_status[INDEX_W5500_CS], sizeof(struct rt_device_pin_status));
}

/**
  * @brief  w5500 device deselect.
  * @param  None.
  * @retval None.
  */
static void w5500_cs_deselect(void)
{
    pin_status[INDEX_W5500_CS].status = GPIO_PIN_SET;			
    rt_device_write(&w5500_pin->parent, 0, &pin_status[INDEX_W5500_CS], sizeof(struct rt_device_pin_status));
}

/**
  * @brief  w5500 device reset.
  * @param  None.
  * @retval None.
  */
static void w5500_reset(void)
{	
    pin_status[INDEX_W5500_RESET].status = GPIO_PIN_RESET;			
    rt_device_write(&w5500_pin->parent, 0, &pin_status[INDEX_W5500_RESET], sizeof(struct rt_device_pin_status));
    rt_thread_delay(5); // delay 5 ms
    pin_status[INDEX_W5500_RESET].status = GPIO_PIN_SET;			
    rt_device_write(&w5500_pin->parent, 0, &pin_status[INDEX_W5500_RESET], sizeof(struct rt_device_pin_status));
    rt_thread_delay(800); // delay 1600 ms
}	

/**
  * @brief  w5500 device config.
  * @param  None.
  * @retval None.
  */
static void w5500_config(void)
{
    wiz_NetInfo wiz_netinfo;
		uint8_t tmpstr[6];
    
    memcpy(wiz_netinfo.mac, g_EthW5500.mac, 6);
    memcpy(wiz_netinfo.ip, g_EthW5500.ip, 4);
    memcpy(wiz_netinfo.sn , g_EthW5500.netmask, 4);
    memcpy(wiz_netinfo.gw , g_EthW5500.gateway, 4);
    memcpy(wiz_netinfo.dns, g_EthW5500.dns, 4);
			
	  // Setting default network information
		ctlnetwork(CN_SET_NETINFO, (void*)&wiz_netinfo);
	
	  // Get network configuration information
		ctlnetwork(CN_GET_NETINFO, (void*)&wiz_netinfo);

		// Display Network Information
		ctlwizchip(CW_GET_ID, (void*)tmpstr);
}

void spi_dma_write(uint8_t* Addref, uint8_t* pTxBuf, uint16_t tx_len)
{
//	rt_sem_take(&w5500_sem, RT_WAITING_FOREVER);
	
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

    w5500_cs_select();
    
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    
    LL_SPI_EnableDMAReq_RX(SPI2);
    LL_SPI_EnableDMAReq_TX(SPI2);    
    LL_SPI_Enable(SPI2);
    /* Waiting the end of Data transfer */
    while (!LL_DMA_IsActiveFlag_TC4(DMA1));
    while (!LL_DMA_IsActiveFlag_TC3(DMA1));
    
    w5500_cs_deselect();
    
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
  
    LL_SPI_DisableDMAReq_RX(SPI2);
    LL_SPI_DisableDMAReq_TX(SPI2);
    LL_SPI_Disable(SPI2);
	
//	rt_sem_release(&w5500_sem);
}

void spi_dma_read(uint8_t* Addref, uint8_t* pRxBuf, uint16_t rx_len)
{
//	rt_sem_take(&w5500_sem, RT_WAITING_FOREVER);
	
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
           
    w5500_cs_select();
    
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
    LL_SPI_Enable(SPI2);
   
    /* Waiting the end of Data transfer */
    while (!LL_DMA_IsActiveFlag_TC4(DMA1));
    while (!LL_DMA_IsActiveFlag_TC3(DMA1));
    
    
    w5500_cs_deselect();
    LL_DMA_ClearFlag_TC4(DMA1);
    LL_DMA_ClearFlag_TC3(DMA1);
    
    LL_SPI_DisableDMAReq_RX(SPI2);
    LL_SPI_DisableDMAReq_TX(SPI2);  
    LL_SPI_Disable(SPI2);    
    
    memcpy(pRxBuf, pTmpBuf2 + 3, rx_len);
	
//	rt_sem_release(&w5500_sem);
}

/**
  * @brief : w5500 read data
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_size_t rt_hw_w5500_read(rt_device_t dev, rt_off_t addr, void* buffer, rt_size_t size) 
{
    rt_spi_w5500_device_t w5500_dev;
    
    w5500_dev = (struct spi_w5500_device *)dev;
    
    rt_spi_transfer(w5500_dev->spi_device,NULL,buffer,size);

    return(RT_EOK);	
}

/**
  * @brief : w5500 write data
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_size_t rt_hw_w5500_write(rt_device_t dev, rt_off_t addr, const void* buffer, rt_size_t size) 
{
    rt_spi_w5500_device_t w5500_dev;
    
    w5500_dev = (struct spi_w5500_device *)dev;
    
    rt_spi_transfer(w5500_dev->spi_device,buffer,NULL,size);

    return(RT_EOK);		
}

uint8_t SPI2_SendByte(uint8_t txData)
{
    uint8_t rxdata;    
    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, txData);
    
    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    rxdata = LL_SPI_ReceiveData8(SPI2);    
    return rxdata;
}

uint8_t  IINCHIP_SpiSendData(uint8_t dat)
{
    //rt_sem_take(&w5500_sem, RT_WAITING_FOREVER);
    
	return( SPI2_SendByte(dat));
	
	//rt_sem_release(&w5500_sem);
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief Read the device of w5500.
  * @param [AddrSel] The address command.
  * @retval Return the byte has been readed.
  */
uint8_t WIZCHIP_READ(uint32_t AddrSel)
{
    uint8_t ret;

    WIZCHIP_CRITICAL_ENTER();
    WIZCHIP.CS._select();

#if ((_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif
#if   !defined(SPI_DMA) 
//    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
//    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
//    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
//    ret = WIZCHIP.IF.SPI._read_byte();
   IINCHIP_SpiSendData( (AddrSel & 0x00FF0000)>>16);// Address byte 1
   IINCHIP_SpiSendData( (AddrSel & 0x0000FF00)>> 8);// Address byte 2
   IINCHIP_SpiSendData( (AddrSel & 0x000000F8))    ;// Data read command and Read data length 1
   ret = IINCHIP_SpiSendData(0x00);             // Data read (read 1byte data)	
#else
   uint8_t spi_data[3];
   spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
   spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
   spi_data[2] = (AddrSel & 0x000000F8);
   spi_dma_read(spi_data, &ret, 1);
#endif   

#elif ((_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_))

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5000. !!!"   
#endif

   WIZCHIP.CS._deselect();
   WIZCHIP_CRITICAL_EXIT();
	 
   return(ret);
}

/**
  * @brief Write to the w5500 device.
  * @param [AddrSel] The address command.
  * @param [wb] The data which will been writen to w5500.
  * @retval None.
  */
void WIZCHIP_WRITE(uint32_t AddrSel, uint8_t wb)
{
    WIZCHIP_CRITICAL_ENTER();
    WIZCHIP.CS._select();

#if ((_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_)
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
   	   AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN1_);
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

#if   !defined(SPI_DMA)	
//	WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
//	WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
//	WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
//	WIZCHIP.IF.SPI._write_byte(wb);
   IINCHIP_SpiSendData((AddrSel & 0x00FF0000)>>16);// Address byte 1
   IINCHIP_SpiSendData((AddrSel & 0x0000FF00)>> 8);// Address byte 2
   IINCHIP_SpiSendData((AddrSel & 0x000000F8) + 4);    // Data write command and Write data length 1
   IINCHIP_SpiSendData(wb); 	
#else
   uint8_t spi_data[3];
   spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
   spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
   spi_data[2] =  (AddrSel & 0x000000F8) + 4;
   spi_dma_write(spi_data, &wb, 1);
#endif   

#elif ((_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_))

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!"
#endif

    WIZCHIP.CS._deselect();
    WIZCHIP_CRITICAL_EXIT();
}
         
/**
  * @brief Read the buffer of w5500 device.
  * @param [AddrSel] The address command.
  * @param [pBuf] The buffer pointer.
  * @param [len] The data length.
  * @retval None.
  */
void WIZCHIP_READ_BUF (uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
    uint16_t i = 0;
    WIZCHIP_CRITICAL_ENTER();
    WIZCHIP.CS._select();

#if ((_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_)
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_VDM_OP_);

	#if   !defined(SPI_DMA) 
//   	  WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
//      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
//      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
//      for (i = 0; i < len; i++)
//	    {
//			    pBuf[i] = WIZCHIP.IF.SPI._read_byte();
//			}
		IINCHIP_SpiSendData( (AddrSel & 0x00FF0000)>>16);		// 通过SPI发送16位地址段给MCU
		IINCHIP_SpiSendData( (AddrSel & 0x0000FF00)>> 8);		// 
		IINCHIP_SpiSendData( (AddrSel & 0x000000F8));    		// 设置SPI为读操作
		for(i = 0; i < len; i++)                    	// 将buf中的数据通过SPI发送给MCU
		{
		    pBuf[i] = IINCHIP_SpiSendData(0x00);
		}	
	#else
		uint8_t spi_data[3];
		spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
		spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
		spi_data[2] = (AddrSel & 0x000000F8);
		spi_dma_read(spi_data, pBuf, len);  
	#endif
			
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_READ_ | _W5500_SPI_FDM_OP_LEN4_);
      for (i = 0; i < len/4; i++, j)
      {
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
          pBuf[i*4]   = WIZCHIP.IF.SPI._read_byte();
          pBuf[i*4+1] = WIZCHIP.IF.SPI._read_byte();
          pBuf[i*4+2] = WIZCHIP.IF.SPI._read_byte();            
          pBuf[i*4+3] = WIZCHIP.IF.SPI._read_byte();            
          AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
      len %= 4; // for the rest data
      // M20131220 : remove for loop
      i *= 4;        
      if (len >= 2)
      {
          AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_

       //for (j = 0; j < len/2 ; j++)
          {
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
              pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
              pBuf[i+1] = WIZCHIP.IF.SPI._read_byte();
              i += 2;
              AddrSel = WIZCHIP_OFFSET_INC(AddrSel,2);
          }
      }
      len %= 2;
      if (len)
      {
          AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
          pBuf[i]   = WIZCHIP.IF.SPI._read_byte();
      }      
   #else
      #error "Unsupported _WIZCHIP_IO_MODE_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

    WIZCHIP.CS._deselect();
    WIZCHIP_CRITICAL_EXIT();
}

/**
  * @brief Write the buffer to w5500 device.
  * @param [AddrSel] The address command.
  * @param [pBuf] The buffer pointer.
  * @param [len] The data length.
  * @retval None.
  */
void WIZCHIP_WRITE_BUF(uint32_t AddrSel, uint8_t* pBuf, uint16_t len)
{
    uint16_t i = 0;
//    uint16_t j = 0;
    WIZCHIP_CRITICAL_ENTER();
    WIZCHIP.CS._select();

#if( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SPI_))

   #if  ( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_VDM_OP_);

   #if   !defined(SPI_DMA) 
//   	  WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
//      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
//      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
//	
//      for (i = 0; i < len; i++)
//	    {
//          WIZCHIP.IF.SPI._write_byte(pBuf[i]);
//			}
	   IINCHIP_SpiSendData( (AddrSel & 0x00FF0000)>>16);// Address byte 1
	   IINCHIP_SpiSendData( (AddrSel & 0x0000FF00)>> 8);// Address byte 2
	   IINCHIP_SpiSendData( (AddrSel & 0x000000F8) + 4);    // Data write command and Write data length 1
	   for(i = 0; i < len; i++)                // Write data in loop
	   {
		   IINCHIP_SpiSendData(pBuf[i]);
	   }	
	#else
	   uint8_t spi_data[3];
	   spi_data[0] = (AddrSel & 0x00FF0000) >> 16;
	   spi_data[1] = (AddrSel & 0x0000FF00) >> 8;
	   spi_data[2] = (AddrSel & 0x000000F8) + 4;
	   spi_dma_write(spi_data, pBuf, len);  
	#endif   
			
   #elif( _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_ )
      AddrSel |= (_W5500_SPI_WRITE_ | _W5500_SPI_FDM_OP_LEN4_);
			
      for (i = 0; i < len/4; i++)
      {
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
          WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
          WIZCHIP.IF.SPI._write_byte(pBuf[i*4]  );
          WIZCHIP.IF.SPI._write_byte(pBuf[i*4+1]);
          WIZCHIP.IF.SPI._write_byte(pBuf[i*4+2]);            
          WIZCHIP.IF.SPI._write_byte(pBuf[i*4+3]);            
          AddrSel = WIZCHIP_OFFSET_INC(AddrSel,4);
      }
			
      len %= 4;      // for the rest data
      // M20131220 : Remove for loop
      i *= 4;
      if (len >= 2)
      {
          AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN4_ to _W5500_SPI_FDM_OP_LEN2_

       //for (j = 0; j < len/2 ; j++)
          {
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
              WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
              WIZCHIP.IF.SPI._write_byte(pBuf[i]  );
              WIZCHIP.IF.SPI._write_byte(pBuf[i+1]);
              i += 2;
               AddrSel = WIZCHIP_OFFSET_INC(AddrSel, 2);
          }
         len %= 2;
         if (len)
         {
             AddrSel -= 1;  // change _W5500_SPI_FDM_OP_LEN2_ to _W5500_SPI_FDM_OP_LEN1_
             WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
             WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
             WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
             WIZCHIP.IF.SPI._write_byte(pBuf[i]);
         }      
      }
   #else
      #error "Unsupported _WIZCHIP_IO_SPI_ in W5500 !!!"
   #endif

#elif ( (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_BUS_) )

   #if  (_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_DIR_)

   #elif(_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_BUS_INDIR_)

   #else
      #error "Unsupported _WIZCHIP_IO_MODE_BUS_ in W5500 !!!"
   #endif
#else
   #error "Unknown _WIZCHIP_IO_MODE_ in W5500. !!!!"
#endif

    WIZCHIP.CS._deselect();
    WIZCHIP_CRITICAL_EXIT();
}

/**
  * @brief Get the TX_FSR of w5500 device.
  * @param [sn] The sn number.
  * @retval None.
  */
uint16_t getSn_TX_FSR(uint8_t sn)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = WIZCHIP_READ(Sn_TX_FSR(sn));
        val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn), 1));
        if (val1 != 0)
        {
            val = WIZCHIP_READ(Sn_TX_FSR(sn));
            val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_TX_FSR(sn), 1));
        }
    } while (val != val1);
		
    return(val);
}

/**
  * @brief Get the RX_RSR of w5500 device.
  * @param [sn] The sn number.
  * @retval None.
  */
uint16_t getSn_RX_RSR(uint8_t sn)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = WIZCHIP_READ(Sn_RX_RSR(sn));
        val1 = (val1 << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn), 1));
        if (val1 != 0)
        {
            val = WIZCHIP_READ(Sn_RX_RSR(sn));
            val = (val << 8) + WIZCHIP_READ(WIZCHIP_OFFSET_INC(Sn_RX_RSR(sn), 1));
        }
    } while (val != val1);
		
    return(val);
}

/**
  * @brief Send data through w5500 device.
  * @param [sn] The sn number.
  * @param [wizdata] The data pointer.
  * @param [len] The length of data.
  * @retval None.
  */
void wiz_send_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
    uint16_t ptr = 0;
    uint32_t addrsel = 0;
	
    if (len != 0)
    {
        ptr = getSn_TX_WR(sn);
        // M20140501 : implict type casting -> explict type casting
        // addrsel = (ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);
        addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_TXBUF_BLOCK(sn) << 3);

        WIZCHIP_WRITE_BUF(addrsel, wizdata, len);
   
        ptr += len;
        setSn_TX_WR(sn, ptr);
    }
}

/**
  * @brief Send data through w5500 device.
  * @param [sn] The sn number.
  * @param [wizdata] The data pointer.
  * @param [len] The length of data.
  * @retval None.
  */
void wiz_recv_data(uint8_t sn, uint8_t *wizdata, uint16_t len)
{
    uint16_t ptr = 0;
    uint32_t addrsel = 0;
   
    if (len != 0) 
    {
        ptr = getSn_RX_RD(sn);
        // M20140501 : implict type casting -> explict type casting
        // addrsel = ((ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);
        addrsel = ((uint32_t)ptr << 8) + (WIZCHIP_RXBUF_BLOCK(sn) << 3);

        WIZCHIP_READ_BUF(addrsel, wizdata, len);
        ptr += len;
   
        setSn_RX_RD(sn,ptr);
    }
}

/**
  * @brief Receive data from w5500 device.
  * @param [sn] The sn number.
  * @param [len] The length of data.
  * @retval None.
  */
void wiz_recv_ignore(uint8_t sn, uint16_t len)
{
    uint16_t ptr = 0;
    ptr = getSn_RX_RD(sn);
    ptr += len;
    setSn_RX_RD(sn,ptr);
}

/**
  * @brief : SPI4 initialization. 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  */
//int rt_hw_spi2_init(void)
//{
//    /* register spi bus */
//    {
//        GPIO_InitTypeDef GPIO_InitStructure;
//        rt_err_t result;

//        __HAL_RCC_GPIOI_CLK_ENABLE(); // enable GPIOI clock
//			
//        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
//        GPIO_InitStructure.Alternate = GPIO_AF5_SPI2; 
//        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; 
//        GPIO_InitStructure.Pull = GPIO_PULLUP; 
//        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;                    
//        HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

////        result = stm32_spi_bus_register(SPI2, 0, "spi2");
//        result = stm32_spi_bus_register(SPI2, 0, RT_SPI2_BUS_NAME);
//        if (result != RT_EOK)
//        {
//            return(result);
//        }
//    }

//    /* attach cs */
//    {
//        static struct rt_spi_device spi_device;
//        rt_err_t result;

////        result = rt_spi_bus_attach_device(&spi_device, "spi20", "spi2", NULL);
//        result = rt_spi_bus_attach_device(&spi_device, RT_SPI2_DEVICE_NAME, RT_SPI2_BUS_NAME, NULL);
//        if (result != RT_EOK)
//        {
//            return(result);
//        }
//    }
//		
//    return(RT_EOK);
//}
//INIT_BOARD_EXPORT(rt_hw_spi2_init);

///**
//  * @brief : w5500 hardware init
//  * @param : [param] the parameter.
//  * @return: [RT_EOK] 
//  * @updata: [2017-12-21][Sunxr][newly increased]
//  */
//int rt_hw_w5500_spi_init(void)
//{	
//    struct rt_spi_configuration cfg;	
//    rt_spi_w5500_device_t w5500_dev = (rt_spi_w5500_device_t) rt_malloc(sizeof(struct spi_w5500_device));
//	
//    /* SPI configure */
//    w5500_dev->spi_device = (struct rt_spi_device *) rt_device_find(RT_SPI2_DEVICE_NAME);
//	
//    if (w5500_dev->spi_device == NULL)
//    {
//        W5500_PRINTF("spi2 bus device spi20 not found! w5500 init failed \r\n"); 
//		
//        return(RT_ERROR);
//    }
//    cfg.mode = RT_SPI_MODE_MASK;
//    cfg.max_hz = 45 * 1000 * 1000;
//    cfg.data_width = 8;	
//    rt_spi_configure(w5500_dev->spi_device, &cfg);	
//	
//    /* initialize lock */
//    rt_mutex_init(&(w5500_dev->lock), "w5500", RT_IPC_FLAG_FIFO);	
//	
//    /* register device */
//    w5500_dev->w5500_device.type = RT_Device_Class_SPIDevice;
//    w5500_dev->w5500_device.init = RT_NULL;
//    w5500_dev->w5500_device.open = RT_NULL;
//    w5500_dev->w5500_device.close = RT_NULL;
//    w5500_dev->w5500_device.read = rt_hw_w5500_read;
//    w5500_dev->w5500_device.write = rt_hw_w5500_write;
//    w5500_dev->w5500_device.control = RT_NULL;	
//    /* no private */
//    w5500_dev->w5500_device.user_data = RT_NULL;
//	
//    rt_device_register(&(w5500_dev->w5500_device), RT_SPI_ETH_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
//	
//    return(RT_EOK);
//}
//INIT_PREV_EXPORT(rt_hw_w5500_spi_init)

/**
  * @brief : the Interrupt service of sd2405
  * @param : [args]-entrance parameter pointer
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_w5500_irq_service(void *args)
{
    if (!(HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)))
    { 
	  #if RT_USING_W5500            
		//rt_sem_release(&w5500_rec_irq_sem);
		rt_event_send(&w5500_event, EVENT_REC_IRQ_W5500);
	  #endif /* RT_USING_W5500 */ 	
    }    
}

/**
  * @brief : read one byte through SPI2.
  * @param : none.
  * @return: Return the reading data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint8_t w5500_ReadByte(void)
{	
    uint8_t rxdata;    
    
//    while (!LL_SPI_IsActiveFlag_RXNE(SPI2));
    rxdata = LL_SPI_ReceiveData8(SPI2);    
    return rxdata;	
//	unsigned char Rxdata = 0;
//	rt_device_read((rt_device_t)device_w5500.spi_device, NULL, &Rxdata, 1);
//	return (unsigned char)Rxdata;
}

/**
  * @brief : write one byte through SPI2.
  * @param : [TxData] writing data.
  * @return: none.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void w5500_WriteByte(uint8_t TxData)
{
//    while (!LL_SPI_IsActiveFlag_TXE(SPI2));
    LL_SPI_TransmitData8(SPI2, TxData);	
//    rt_device_write((rt_device_t)device_w5500.spi_device, NULL, &TxData, 1);  
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
  #else
    spi_dma_init();  
  #endif
	while (!LL_SPI_IsActiveFlag_TXE(SPI2));	
}

/**
  * @brief  w5500 device initialization.
  * @param  None.
  * @retval None.
  */
int rt_hw_w5500_init(void)
{
    uint8_t memsize[2][8] = {{4,2,2,2,2,2,1,1}, {4,2,2,2,2,2,1,1}};
    uint8_t tmp;
    uint32_t tickstart = 0;
    pTmpBuf1 = rt_malloc(DATA_BUF_SIZE + 3);
	pTmpBuf2 = rt_malloc(DATA_BUF_SIZE + 3);	
		
    static rt_device_t device;	
    static struct rt_device_pin *w5500_irq_pin;	

	LL_DMA_DeInit(DMA1, LL_DMA_STREAM_3);
    LL_DMA_DeInit(DMA1, LL_DMA_STREAM_4);
		
    LL_SPI2_Init();
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
		
    w5500_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);	
    if (w5500_pin == RT_NULL)
    {
        W5500_PRINTF("w5500 pin device is not found!!! ");         
        return(1);
    }

    // First of all, Should register SPI callback functions implemented by user for accessing WIZCHIP 
    /* Critical section callback */
    reg_wizchip_cris_cbfunc(rt_enter_critical, rt_exit_critical); // registered critical function
			
    /* Chip selection call back */
  #if _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(w5500_cs_select, w5500_cs_deselect); // registered SPI chip select function
  #elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect); // CS must be tried with LOW.
  #else
    #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
    #else
        reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
    #endif
  #endif
			
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(w5500_ReadByte, w5500_WriteByte); // registered read and write function

    w5500_reset();	
	
    /* WIZCHIP SOCKET Buffer initialize */
    if (ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1)
    {
        while(1);
    }
		
    /* Get tick */
    tickstart = HAL_GetTick();
		
    /* PHY link status check */
    do
    {
        if (ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == 0)
        {
            break;
        }
				
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > ETH_TIMEOUT_LINKED_STATE)
        {
            break;
        }

    } while (tmp == PHY_LINK_OFF);

    w5500_irq_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);
	
    if (w5500_irq_pin == RT_NULL)
    {
        W5500_PRINTF("pin device is not found!!!/r/n "); 
    }
    else
    {
        w5500_irq_pin->ops->pin_attach_irq(device, 2, PIN_IRQ_MODE_FALLING, rt_hw_w5500_irq_service, RT_NULL);  
        w5500_irq_pin->ops->pin_irq_enable(device, 2, PIN_IRQ_ENABLE, INT_EXTI3_PRIO); 
    }
    
	w5500_config();
		
    return(0);
}

INIT_ENV_EXPORT(rt_hw_w5500_init);


/* END OF FILE ---------------------------------------------------------------*/

