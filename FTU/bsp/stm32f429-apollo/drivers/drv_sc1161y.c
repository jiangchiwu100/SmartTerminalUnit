/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      sc1161y.c
  * @brief:     The driver of sc1161y.
  * @version:   V1.0.0
  * @author:    Lexun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "drv_sc1161y.h"
#include "drv_spi.h"
#include "string.h"
#include "drv_gpio.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t device_sc1161y = RT_NULL; 
static struct rt_device_pin *sc1161y_pin; 


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  sc1161y device select.
  * @param  None.
  * @retval None.
  */
static void sc1161y_cs_select(void)
{
    pin_status[INDEX_SC1161Y_CS].status = GPIO_PIN_RESET;			
    rt_device_write(&sc1161y_pin->parent, 0, &pin_status[INDEX_SC1161Y_CS], sizeof(struct rt_device_pin_status));
}

/**
  * @brief  sc1161y device deselect.
  * @param  None.
  * @retval None.
  */
static void sc1161y_cs_deselect(void)
{
    pin_status[INDEX_SC1161Y_CS].status = GPIO_PIN_SET;			
    rt_device_write(&sc1161y_pin->parent, 0, &pin_status[INDEX_SC1161Y_CS], sizeof(struct rt_device_pin_status));
}

/**
  * @brief : fm25vxx read data
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_size_t rt_hw_sc1161y_read(rt_device_t dev, rt_off_t addr, void* buffer, rt_size_t size) 
{
    rt_spi_sc1161y_device_t rtt_dev;
    
    rtt_dev = (struct spi_sc1161y_device *)dev;
    
    rt_spi_transfer(rtt_dev->rt_spi_device,NULL,buffer,size);

    return RT_EOK;	
}

/**
  * @brief : fm25vxx write data
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_size_t rt_hw_sc1161y_write(rt_device_t dev, rt_off_t addr, const void* buffer, rt_size_t size) 
{
    rt_spi_sc1161y_device_t rtt_dev;
    
    rtt_dev = (struct spi_sc1161y_device *)dev;
    
    rt_spi_transfer(rtt_dev->rt_spi_device,buffer,NULL,size);

    return RT_EOK;		
}

/**
  * @brief : SPI5 initialization. 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  */
static int rt_hw_spi5_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        rt_err_t result;

        __HAL_RCC_GPIOI_CLK_ENABLE(); // 使能GPIOI时钟
        
        GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStructure.Alternate = GPIO_AF5_SPI5; 
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; 
        GPIO_InitStructure.Pull = GPIO_PULLUP; 
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;                    
        HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

//        result = stm32_spi_bus_register(SPI5, 0, "spi5");
        result = stm32_spi_bus_register(SPI5, 0, RT_SPI5_BUS_NAME);
        if (result != RT_EOK)
        {
            return(result);
        }
    }

    /* attach cs */
    {
        static struct rt_spi_device spi_device;
        rt_err_t result;

//        result = rt_spi_bus_attach_device(&spi_device, "spi50", "spi5", NULL);
        result = rt_spi_bus_attach_device(&spi_device, RT_SPI5_DEVICE_NAME, RT_SPI5_BUS_NAME, NULL);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
		
    return(RT_EOK);
}
INIT_BOARD_EXPORT(rt_hw_spi5_init);


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : sc1161y hardware init
  * @param : [param] the parameter.
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
int rt_hw_sc1161y_spi_init(void)
{	
    struct rt_spi_configuration cfg;	
    rt_spi_sc1161y_device_t rtt_dev = (rt_spi_sc1161y_device_t) rt_malloc(sizeof(struct spi_sc1161y_device));
	
    /* SPI configure */
    rtt_dev->rt_spi_device = (struct rt_spi_device *) rt_device_find(RT_SPI5_DEVICE_NAME);
	
    if (rtt_dev->rt_spi_device == NULL)
    {
        SCL161Y_PRINTF("spi5 bus device spi50 not found! sc1161y init failed \r\n"); 
		
        return(RT_ERROR);
    }
		
    cfg.mode = RT_SPI_MODE_MASK;
    cfg.max_hz = 0 * 1000 * 1000;
    cfg.data_width = 8;	
    rt_spi_configure(rtt_dev->rt_spi_device, &cfg);	
	
    /* initialize lock */
    rt_mutex_init(&(rtt_dev->lock), "sc1161y", RT_IPC_FLAG_FIFO);	
	
    /* register device */
    rtt_dev->sc1161y_device.type = RT_Device_Class_SPIDevice;
    rtt_dev->sc1161y_device.init = RT_NULL;
    rtt_dev->sc1161y_device.open = RT_NULL;
    rtt_dev->sc1161y_device.close = RT_NULL;
    rtt_dev->sc1161y_device.read = rt_hw_sc1161y_read;
    rtt_dev->sc1161y_device.write = rt_hw_sc1161y_write;
    rtt_dev->sc1161y_device.control = RT_NULL;	
    /* no private */
    rtt_dev->sc1161y_device.user_data = RT_NULL;
	
    rt_device_register(&(rtt_dev->sc1161y_device), RT_SPI_ENCRYPTION_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
	
    return(RT_EOK);
}
INIT_PREV_EXPORT(rt_hw_sc1161y_spi_init)


/**
  * @brief : read one byte through SPI2.
  * @param : none.
  * @return: Return the reading data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
unsigned char sc1161y_ReadByte(void)
{		 			 
    unsigned char Rxdata = 0;
    rt_device_read(device_sc1161y, NULL, &Rxdata, 1);
    return (unsigned char)Rxdata; // 返回收到的数据		
}

/**
  * @brief : write one byte through SPI2.
  * @param : [TxData] writing data.
  * @return: none.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void sc1161y_WriteByte(unsigned char TxData)
{
    rt_device_write(device_sc1161y, NULL, &TxData, 1);  
}

/**
  * @brief : read Write through SPI5.
  * @param : none.
  * @return: Return the reading data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void sc1161y_ReadWrite(unsigned char *TxData , unsigned char *RxData)
{		 			 
    unsigned int i = 0;
    unsigned int length = 0;
    unsigned int timer = 0;
	
//    while(1)
    {
        length = 0;
        length |= TxData[5] << 8;
        length |= TxData[6];
        if (length > 0x400) // 长度超限不发送
        { 
            return;
        }
	
        rt_thread_delay(1);
        sc1161y_cs_select();
        rt_thread_delay(1);
	
        rt_enter_critical();
				
        for (i = 0; i < (length +2 +5 +1); i++)
        {
            sc1161y_WriteByte(TxData[i]);
        }
        rt_exit_critical();
    
        rt_thread_delay(1);
        sc1161y_cs_deselect();
        rt_thread_delay(1);	

        rt_thread_delay(1);
        sc1161y_cs_select();
        rt_thread_delay(1);
	
        while (timer < 40)
        {
            timer++;
            if (timer > 10)
            {
                rt_thread_delay(100);
            }
            if (sc1161y_ReadByte() == 0x55)
            {
                break;
            }
        }
	
        rt_enter_critical();	
				
        for (i = 0; i < 4; i++)
        {
            RxData[i] = sc1161y_ReadByte();
        }
        rt_exit_critical();
	
        length = 0;
        length |= RxData[2]<<8;
        length |= RxData[3];
        if (length > 0x400)//长度超限不接受
        {
            return;
        }
	
        rt_enter_critical();
        for (i = 0; i < length + 1; i++)
        {
            RxData[4 + i] = sc1161y_ReadByte();
        }
        rt_exit_critical();
    
        rt_thread_delay(1);
        sc1161y_cs_deselect();
        rt_thread_delay(1);	
    }
}

/**
  * @brief  sc1161y device initialization.
  * @param  None.
  * @retval None.
  */
int rt_hw_sc1161y_init(void)
{       
    device_sc1161y = rt_device_find(RT_SPI_ENCRYPTION_NAME);
    if (device_sc1161y == NULL)
    {
        SCL161Y_PRINTF("sc1161y is not found! \r\n"); 	
        return(1);        
    }   
    rt_device_open(device_sc1161y, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);

    sc1161y_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);	
    if (sc1161y_pin == RT_NULL)
    {
        SCL161Y_PRINTF("sc1161y pin device is not found!!! ");         
        return(1);
    }
		
    return(0);
}

INIT_ENV_EXPORT(rt_hw_sc1161y_init);


/* END OF FILE ---------------------------------------------------------------*/

