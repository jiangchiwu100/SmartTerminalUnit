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


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct spi_w5500_device device_w5500; 
static struct rt_device_pin *w5500_pin; 


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
	rt_thread_delay(800); // delay 5 ms
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

    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
    ret = WIZCHIP.IF.SPI._read_byte();

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

    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
    WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
    WIZCHIP.IF.SPI._write_byte(wb);

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
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
      for (i = 0; i < len; i++)
	    {
			    pBuf[i] = WIZCHIP.IF.SPI._read_byte();
			}
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
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x00FF0000) >> 16);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x0000FF00) >>  8);
      WIZCHIP.IF.SPI._write_byte((AddrSel & 0x000000FF) >>  0);
	
      for (i = 0; i < len; i++)
	    {
          WIZCHIP.IF.SPI._write_byte(pBuf[i]);
			}
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
int rt_hw_spi2_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        rt_err_t result;

        __HAL_RCC_GPIOI_CLK_ENABLE(); // enable GPIOI clock
			
        GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStructure.Alternate = GPIO_AF5_SPI2; 
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; 
        GPIO_InitStructure.Pull = GPIO_PULLUP; 
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;                    
        HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

//        result = stm32_spi_bus_register(SPI2, 0, "spi2");
        result = stm32_spi_bus_register(SPI2, 0, RT_SPI2_BUS_NAME);
        if (result != RT_EOK)
        {
            return(result);
        }
    }

    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        rt_err_t result;

//        result = rt_spi_bus_attach_device(&spi_device, "spi20", "spi2", NULL);
        result = rt_spi_bus_attach_device(&spi_device, RT_SPI2_DEVICE_NAME, RT_SPI2_BUS_NAME, NULL);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
		
    return(RT_EOK);
}
INIT_BOARD_EXPORT(rt_hw_spi2_init);

/**
  * @brief : w5500 hardware init
  * @param : [param] the parameter.
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
int rt_hw_w5500_spi_init(void)
{	
    struct rt_spi_configuration cfg;	
    rt_spi_w5500_device_t w5500_dev = (rt_spi_w5500_device_t) rt_malloc(sizeof(struct spi_w5500_device));
	
    /* SPI configure */
    w5500_dev->spi_device = (struct rt_spi_device *) rt_device_find(RT_SPI2_DEVICE_NAME);
	
    if (w5500_dev->spi_device == NULL)
    {
        W5500_PRINTF("spi2 bus device spi20 not found! w5500 init failed \r\n"); 
		
        return(RT_ERROR);
    }
    cfg.mode = RT_SPI_MODE_MASK;
    cfg.max_hz = 40 * 1000 * 1000;
    cfg.data_width = 8;	
    rt_spi_configure(w5500_dev->spi_device, &cfg);	
	
    /* initialize lock */
    rt_mutex_init(&(w5500_dev->lock), "w5500", RT_IPC_FLAG_FIFO);	
	
    /* register device */
    w5500_dev->w5500_device.type = RT_Device_Class_SPIDevice;
    w5500_dev->w5500_device.init = RT_NULL;
    w5500_dev->w5500_device.open = RT_NULL;
    w5500_dev->w5500_device.close = RT_NULL;
    w5500_dev->w5500_device.read = rt_hw_w5500_read;
    w5500_dev->w5500_device.write = rt_hw_w5500_write;
    w5500_dev->w5500_device.control = RT_NULL;	
    /* no private */
    w5500_dev->w5500_device.user_data = RT_NULL;
	
    rt_device_register(&(w5500_dev->w5500_device), RT_SPI_ETH_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
	
    return(RT_EOK);
}
INIT_PREV_EXPORT(rt_hw_w5500_spi_init)

/**
  * @brief : read one byte through SPI2.
  * @param : none.
  * @return: Return the reading data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
unsigned char w5500_ReadByte(void)
{		 			 
    unsigned char Rxdata = 0;
    rt_device_read((rt_device_t)device_w5500.spi_device, NULL, &Rxdata, 1);
    return (unsigned char)Rxdata;
}

/**
  * @brief : write one byte through SPI2.
  * @param : [TxData] writing data.
  * @return: none.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void w5500_WriteByte(unsigned char TxData)
{
    rt_device_write((rt_device_t)device_w5500.spi_device, NULL, &TxData, 1);  
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
        			
    struct rt_spi_device * spi_device;
    spi_device = (struct rt_spi_device *)rt_device_find(RT_SPI_ETH_NAME);
			
    if (spi_device == NULL)
    {
        W5500_PRINTF("w5500 is not found! \r\n"); 	
        return(1);        
    }   

    rt_device_open((rt_device_t)spi_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		
		device_w5500.spi_device = spi_device;

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
	
    w5500_reset();	
		
    w5500_config();
		
    return(0);
}

INIT_ENV_EXPORT(rt_hw_w5500_init);


/* END OF FILE ---------------------------------------------------------------*/

