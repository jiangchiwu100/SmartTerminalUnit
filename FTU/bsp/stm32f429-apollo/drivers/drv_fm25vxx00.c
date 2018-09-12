/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : fm25vxx.c
  * @brief   : The driver of FM25Vxx10.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2017-12-21
  * @updata  : [2017-12-21][Sunxr][newly increased]
  */
  

/* INCLUDES FILES --------------------------------------------------------------*/
#include <board.h>
#include <finsh.h>
#include "string.h"
#include "spi_flash.h"
#include "drv_spi.h"
#include "drv_io_spi.h"
#include "drv_fm25vxx.h"
#include "common_data.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct spi_flash_device fm25vxx;

/**
  * @brief : fm25vxx read data
  * @param : [cmd] the command.
  * @return: [none]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static void rt_hw_fm25vxx_cmd(rt_uint8_t cmd)
{
    rt_spi_transfer(fm25vxx.rt_spi_device, &cmd, NULL, 1);    
}

/**
  * @brief : fm25vxx sleep function
  * @param : [none].
  * @return: [none]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
void rt_hw_fram_sleep(void)
{	
    rt_hw_fm25vxx_cmd(FRAM_WREN); // write enabled
    rt_hw_fm25vxx_cmd(FRAM_SLEEP); // write disabled
}

/**
  * @brief : fm25vxx read SR
  * @param : [cmd] operation command. 
  * @return: [data] The value of the register read
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
rt_uint8_t rt_hw_fram_read_register(rt_uint8_t cmd)
{
    rt_uint8_t data = 0;

    rt_hw_fm25vxx_cmd(FRAM_WREN); 
	
    rt_spi_send_then_recv(fm25vxx.rt_spi_device, &cmd, 1, &data, 1);
	
    return (data);
}

/**
  * @brief : fm25vxx first power-on process
  * @param : [none]
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static int rt_hw_fram_first_clear(void)
{
//    rt_uint32_t i = 0;
    rt_uint8_t flag = 0;
    rt_device_t device = RT_NULL;
		
    device = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device == NULL)
    {
        FRAM_PRINTF("fram is not found! \r\n"); 
		
        return(RT_ERROR); 
    }

    rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);		
    
    rt_device_read(device, ADDR_FRAM_UPDATE, &flag, 1); 
		
//    if (flag != FRAM_HWFLAG1)
//    {
//		rt_device_read(device, ADDR_FRAM_UPDATE, &flag, 1);
//		
//		if (flag != FRAM_HWFLAG1)
//		{
////			flag = 0;

////			for (i = 0; i < FM25V10_MAX_ADDR; i++)
////			{
////				rt_device_write(device, 0x01 + i, &flag, 1); 
////			}          
//            
//			flag = FRAM_HWFLAG1;

//			rt_device_write(device, ADDR_FRAM_UPDATE, &flag, 1);  
//			
//			FRAM_PRINTF("fram is powered on firstly! \r\n"); 		
//		}
//    }
	
    return RT_EOK; 
}

/**
  * @brief : read fm25vxx id
  * @param : [none]
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_uint8_t rt_hw_fram_read_id(void)
{
	rt_uint8_t tx_data[10] = {FRAM_RIID, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    rt_uint8_t rx_data[10] = {0};
		
    rt_spi_transfer(fm25vxx.rt_spi_device, tx_data, rx_data, 10);    
	
    if (rx_data[7] == 0xC2 && rx_data[8] == 0x24)
    {
        return RT_EOK;
    }
    else
    {
        return RT_ERROR;
    }
}

/**
  * @brief : fm25vxx test function
  * @param : [param] the parameter.
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
void rt_hw_fm25vxx_test(void)
{
//    rt_uint8_t i;
//    rt_uint8_t buf1[10] = {1,2,3,4,5,6,7,8,9};
    rt_uint8_t buf2[10] = {0};
    rt_device_t device = RT_NULL;

    FRAM_PRINTF("fram test start! \r\n"); 	
	
    device = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device == NULL)
    {
        FRAM_PRINTF("fram is not found! \r\n"); 		
    }
    else
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	  

		rt_device_read(device, ADDR_FRAM_START_NUM, &buf2, 1);  
		
		FRAM_PRINTF("开机次数 : %d", buf2[0]);
		FRAM_PRINTF("\r\n");
//        rt_device_write(device, 0x01, &buf1, 10);
//        FRAM_PRINTF("write_buf = {");
//        for (i = 0; i < sizeof(buf1); i++)
//        {
//            FRAM_PRINTF("%d,", buf1[i]);			
//        }
//        FRAM_PRINTF("}\r\n");
//		
//        FRAM_PRINTF("read_buf = {");
//        rt_device_read(device, 0x01, &buf2, 10);  
//        for (i = 0; i < sizeof(buf2); i++)
//        {
//            FRAM_PRINTF("%d,", buf2[i]);	
//            if (buf2[i] != buf1[i])
//            {
//                FRAM_PRINTF("fram is abnormal !!!");		
//            }				
//        }
//        FRAM_PRINTF("}\r\n");		
		
    }
    FRAM_PRINTF("fram test finish! \r\n");	
}

FINSH_FUNCTION_EXPORT(rt_hw_fm25vxx_test, test the fram);
FINSH_FUNCTION_EXPORT_ALIAS(rt_hw_fm25vxx_test, fram_test, test the fram);

/**
  * @brief : fm25vxx monitor function
  * @param : [param] the parameter.
  * @return: [1]-abnormal; [0]-normal 
  * @updata: [2018-03-26][Sunxr][newly increased]
  */
rt_uint8_t rt_hw_fm25vxx_monitor(void)
{
    static rt_uint8_t data1 = 0xAA;
    rt_uint8_t data2 = 0;
    rt_device_t device = RT_NULL;	
	
    device = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device == NULL)
    {
        return 1;		
    }
    else
    {
		rt_device_write(device, ADDR_FRAM_MONITOR, &data1, 1);  
		
		rt_device_read(device, ADDR_FRAM_MONITOR, &data2, 1);  

        if (data2 == data1)
		{
		    return 0;
		}
        else
		{
		    return 1;
		}			
    }	
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
static rt_size_t rt_hw_fm25vxx_read(rt_device_t dev, rt_off_t addr, void* buffer, rt_size_t size) 
{
    rt_uint8_t cmd[4];
	
    if ((addr + size) > FM25V10_MAX_ADDR)
    {
        return RT_ERROR;
    }

    cmd[0] = FRAM_READ;
    cmd[1] = addr >> 16 & 0xff;
    cmd[2] = addr >> 8  & 0xff;
    cmd[3] = addr       & 0xff;

    rt_spi_send_then_recv(fm25vxx.rt_spi_device, cmd, sizeof(cmd), buffer, size);

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
static rt_size_t rt_hw_fm25vxx_write(rt_device_t dev, rt_off_t addr, const void* buffer, rt_size_t size) 
{
    rt_uint8_t cmd[4];
	
    if ((addr + size) > FM25V10_MAX_ADDR)
    {
        return RT_ERROR;
    }

    rt_hw_fm25vxx_cmd(FRAM_WREN); // write enabled
	
    cmd[0] = FRAM_WRITE;
    cmd[1] = addr >> 16 & 0xff;
    cmd[2] = addr >> 8  & 0xff;
    cmd[3] = addr       & 0xff;

    rt_spi_send_then_send(fm25vxx.rt_spi_device, cmd, sizeof(cmd), buffer, size);

    rt_hw_fm25vxx_cmd(FRAM_WRDI); // write disabled
    
    return RT_EOK;	
}

/**
  * @brief : fm25vxx clear record
  * @param : [none]
  * @return: [none]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static void rt_hw_fm25vxx_clear_record(void)
{
    rt_uint32_t i;
    rt_uint8_t flag = 0;
    	
    for (i = 0; i < sizeof(g_SOEDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_SOE + i, &flag, 1);
    }
    
    for (i = 0; i < sizeof(g_SOENewDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_SOE_NEW + i, &flag, 1);
    } 
    
    for (i = 0; i < sizeof(g_FeventDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_FEVENT + i, &flag, 1);
    }      

    for (i = 0; i < sizeof(g_CoDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_CO + i, &flag, 1);
    }   
    
    for (i = 0; i < sizeof(g_UlogDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_LOG + i, &flag, 1);
    }   		

    for (i = 0; i < sizeof(g_FlagDB); i++)
    {
        rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_MEMORY + i, &flag, 1);
    }   		

	rt_device_write(&fm25vxx.flash_device, ADDR_FRAM_START_NUM, &flag, 1);
	
    memset(&g_SOEDB, 0, sizeof(g_SOEDB));
    memset(&g_SOENewDB, 0, sizeof(g_SOENewDB));
    memset(&g_FeventDB, 0, sizeof(g_FeventDB));	
    memset(&g_CoDB, 0, sizeof(g_CoDB));
    memset(&g_UlogDB, 0, sizeof(g_UlogDB));		
    memset(&g_FlagDB.queue_soe, 0, sizeof(g_FlagDB.queue_soe));
    memset(&g_FlagDB.queue_fevent, 0, sizeof(g_FlagDB.queue_fevent));
    memset(&g_FlagDB.queue_co, 0, sizeof(g_FlagDB.queue_co));
    memset(&g_FlagDB.queue_ulog, 0, sizeof(g_FlagDB.queue_ulog));	
}

/**
  * @brief : fm25vxx contrl function
  * @param : [dev] the device.
  * @param : [cmd] the commond.
  * @param : [args] the args.
  * @return: [none]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static rt_err_t rt_hw_fm25vxx_control(rt_device_t dev, int cmd, void *args) 
{
    RT_ASSERT(dev != RT_NULL);
	
    switch (cmd)
    {
        case FM_CLEAR_RECORD: 
            rt_hw_fm25vxx_clear_record();
            break;
				
        case FM_POWER_DOWN_DEAL: 
            break;
		
        case FM_SLEEP:
            rt_hw_fram_sleep();
            break;
				
        case FM_READ_SR:
            if (args != RT_NULL)
            {
                rt_hw_fram_read_register(*(rt_uint8_t *)args);
            }
            break;
						
        case FM_READ_ID:
            rt_hw_fram_read_id();
            break;	
				
        case FM_TEST:
            rt_hw_fm25vxx_test();
            break;		

        case FM_MONITOR:
            *(rt_uint8_t *)args = rt_hw_fm25vxx_monitor();
            break;			
		
        default:
            break;
    }
	
    return(RT_EOK);
}

#ifdef RT_USING_IO_SPI1
/**
  * @brief : IO_SPI1 initialization 
  * @param : [none]
  * @return: [RT_ERROR] error;[RT_EOK] successful.
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static int rt_hw_io_spi1_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        rt_err_t result;

        IO_SPI1_CLK_RCC_ENABLE();
        IO_SPI1_MOSI_RCC_ENABLE();
        IO_SPI1_MISO_RCC_ENABLE();

        GPIO_InitStructure.Pin = IO_SPI1_CLK_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(IO_SPI1_CLK_GPIO_PORT, &GPIO_InitStructure);
			
        GPIO_InitStructure.Pin = IO_SPI1_MOSI_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(IO_SPI1_MOSI_GPIO_PROT, &GPIO_InitStructure);

        GPIO_InitStructure.Pin = IO_SPI1_MISO_GPIO_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(IO_SPI1_MISO_GPIO_PORT, &GPIO_InitStructure);

//        result = stm32_io_spi_bus_register(IO_SPI1, 0, "io_spi1");
        result = stm32_io_spi_bus_register(IO_SPI1, 0, RT_IO_SPI1_BUS_NAME);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
    /* attach cs */
    {
        rt_err_t result;
        static struct stm32_spi_cs  spi_cs;		
        static struct rt_spi_device spi_device;
		
        /* add by sun */
        fm25vxx.rt_spi_device = &spi_device;
		
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

        spi_cs.GPIOx = IO_SPI1_CS_GPIO_PORT;
        spi_cs.GPIO_Pin = IO_SPI1_CS_GPIO_PIN;
        IO_SPI1_CS_RCC_ENABLE();

        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);

//        result = rt_spi_bus_attach_device(&spi_device, "io_spi10", "io_spi1", (void*)&spi_cs);
        result = rt_spi_bus_attach_device(&spi_device, RT_IO_SPI1_DEVICE_NAME, RT_IO_SPI1_BUS_NAME, (void*)&spi_cs);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
		
    return(RT_EOK);
}
INIT_BOARD_EXPORT(rt_hw_io_spi1_init);

/**
  * @brief : fm25vxx hardware init
  * @param : [param] the parameter.
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
int rt_hw_fm25vxx_init(void)
{
    struct rt_spi_configuration cfg;	
    rt_spi_flash_device_t rtt_dev = (rt_spi_flash_device_t) rt_malloc(sizeof(struct spi_flash_device));
    rt_device_t device = RT_NULL;
	
    /* SPI configure */
    rtt_dev->rt_spi_device = (struct rt_spi_device *) rt_device_find(RT_IO_SPI1_DEVICE_NAME);
	
    if (rtt_dev->rt_spi_device == NULL)
    {
        FRAM_PRINTF("io_spi1 bus device io_spi10 not found! fm25vxx init failed \r\n"); 
		
        return(RT_ERROR);
    }
    cfg.mode = RT_SPI_MODE_MASK;
    cfg.max_hz = 50 * 1000 * 1000;
    cfg.data_width = 8;	
    rt_spi_configure(rtt_dev->rt_spi_device, &cfg);	
	
    /* initialize lock */
    rt_mutex_init(&(rtt_dev->lock), "fram0", RT_IPC_FLAG_FIFO);	
	
    /* register device */
    fm25vxx.flash_device.type = RT_Device_Class_SPIDevice;
    fm25vxx.flash_device.init = RT_NULL;
    fm25vxx.flash_device.open = RT_NULL;
    fm25vxx.flash_device.close = RT_NULL;
    fm25vxx.flash_device.read = rt_hw_fm25vxx_read;
    fm25vxx.flash_device.write = rt_hw_fm25vxx_write;
    fm25vxx.flash_device.control = rt_hw_fm25vxx_control;	
    /* no private */
    fm25vxx.user_data = RT_NULL;
	
    rt_device_register(&fm25vxx.flash_device, RT_SPI_FRAM_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	

    device = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device == NULL)
    {
        FRAM_PRINTF("fram is not found! \r\n"); 		
    }
    else	
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	  

        rt_device_control(&fm25vxx.flash_device, FM_READ_ID, RT_NULL); 
//	    rt_device_control(&fm25vxx.flash_device, FM_TEST, RT_NULL);		
    }	

    rt_hw_fram_first_clear();
	
    return(RT_EOK);
}
INIT_PREV_EXPORT(rt_hw_fm25vxx_init)

#else 

/**
  * @brief : spi4 init 
  * @param : [none]
  * @return: [result]
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
static int rt_hw_spi5_init(void)
{
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        rt_err_t result;

        __HAL_RCC_GPIOF_CLK_ENABLE();

        GPIO_InitStructure.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStructure.Alternate  = GPIO_AF5_SPI5;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

//        result = stm32_spi_bus_register(SPI4, 0, "spi4");
        result = stm32_spi_bus_register(SPI5, 0, RT_SPI5_BUS_NAME);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
    /* attach cs */
    {
        rt_err_t result;
        static struct stm32_spi_cs  spi_cs;		
        static struct rt_spi_device spi_device;
		
        /* add by sun */
        fm25vxx.rt_spi_device = &spi_device;
		
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull  = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

        spi_cs.GPIOx = GPIOF;
        spi_cs.GPIO_Pin = GPIO_PIN_6;
        __HAL_RCC_GPIOF_CLK_ENABLE();

        GPIO_InitStructure.Pin = spi_cs.GPIO_Pin;
        HAL_GPIO_WritePin(spi_cs.GPIOx, spi_cs.GPIO_Pin, GPIO_PIN_SET);
        HAL_GPIO_Init(spi_cs.GPIOx, &GPIO_InitStructure);

//        result = rt_spi_bus_attach_device(&spi_device, "spi40", "spi4", (void*)&spi_cs);
        result = rt_spi_bus_attach_device(&spi_device, RT_SPI5_DEVICE_NAME, RT_SPI5_BUS_NAME, (void*)&spi_cs);
        if (result != RT_EOK)
        {
            return(result);
        }
    }
		
    return(RT_EOK);
}
INIT_BOARD_EXPORT(rt_hw_spi5_init);

/**
  * @brief : fm25vxx hardware init
  * @param : [param] the parameter.
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */
int rt_hw_fm25vxx_init(void)
{
    struct rt_spi_configuration cfg;	
    rt_spi_flash_device_t rtt_dev = (rt_spi_flash_device_t) rt_malloc(sizeof(struct spi_flash_device));
    rt_device_t device = RT_NULL;
	
    /* SPI configure */
    rtt_dev->rt_spi_device = (struct rt_spi_device *) rt_device_find(RT_SPI5_DEVICE_NAME);
    
    if (rtt_dev->rt_spi_device == NULL)
    {
        FRAM_PRINTF("spi5 bus device spi50 not found! fm25vxx init failed \r\n"); 
		
        return RT_ERROR;
    }
	
    cfg.mode = RT_SPI_MODE_MASK;
    cfg.max_hz = 50 * 1000 * 1000;
    cfg.data_width = 8;	
    rt_spi_configure(rtt_dev->rt_spi_device, &cfg);	

    /* initialize lock */
    rt_mutex_init(&(rtt_dev->lock), "fram0", RT_IPC_FLAG_FIFO);	
	
    /* register device */
    fm25vxx.flash_device.type = RT_Device_Class_SPIDevice;
    fm25vxx.flash_device.init = RT_NULL;
    fm25vxx.flash_device.open = RT_NULL;
    fm25vxx.flash_device.close = RT_NULL;
    fm25vxx.flash_device.read = rt_hw_fm25vxx_read;
    fm25vxx.flash_device.write = rt_hw_fm25vxx_write;
    fm25vxx.flash_device.control = rt_hw_fm25vxx_control;
		
    /* no private */
    fm25vxx.user_data = RT_NULL;
	
    rt_device_register(&fm25vxx.flash_device, RT_SPI_FRAM_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
	
    device = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device == NULL)
    {
        FRAM_PRINTF("fram is not found! \r\n"); 		
    }
    else	
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	  

        rt_device_control(&fm25vxx.flash_device, FM_READ_ID, RT_NULL); 
//	    rt_device_control(&fm25vxx.flash_device, FM_TEST, RT_NULL);		
    }

    rt_hw_fram_first_clear();
	
    return RT_EOK;
}
INIT_PREV_EXPORT(rt_hw_fm25vxx_init)

#endif /* RT_USING_IO_SPI1 */

/* END OF FILE ---------------------------------------------------------------*/
