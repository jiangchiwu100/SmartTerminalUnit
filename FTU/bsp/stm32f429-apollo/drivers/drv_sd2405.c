/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : sd2405.c
  * @brief   : The driver of sd2405.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2018-01-14
  * @updata  : [2017-12-21][Sunxr][newly increased]
  */
	
/* INCLUDES FILES ------------------------------------------------------------*/
#include <board.h>
#include <finsh.h>
#include <rtdevice.h>
#include "drv_iic.h"
#include "drv_gpio.h"
#include "drv_sd2405.h"
#include "common_data.h"
#include ".\MultiThread\multi_thread.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct rt_device sd2405_device;
static struct rt_i2c_bus_device *i2c_bus_device = RT_NULL ;


/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PRVIATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : the Interrupt service of sd2405
  * @param : [args]-entrance parameter pointer
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_sd2405_irq_service(void *args)
{
    if (!(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)))
    { 
        g_SystemTime.msecond = 0;	     
        
        if (++g_SystemTime.second > 59)
        {
            g_SystemTime.second = 0;

            if (++g_SystemTime.minute > 59)
            {	
                rt_event_send(&sd2405_event, EVENT_RUN);               
            }  									
        }            							
    }    
}
/**
  * @brief : read time from sd2405
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_size_t rt_hw_sd2405_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size) 
{
    rt_uint8_t time[7] = {0};

    rt_i2c_master_recv(i2c_bus_device, SD2405_DEV_ADDR, RT_I2C_WR, time, sizeof(time));
	
    g_SystemTime.second = BCDToDecimal(time[0]);
    g_SystemTime.minute = BCDToDecimal(time[1]);
    if(time[2]&0x80)
    {
        g_SystemTime.hour   = BCDToDecimal(time[2]&0x7f);
    }
    else
    {
        g_SystemTime.hour   = BCDToDecimal(time[2]&0x1f);
        if(g_SystemTime.hour == 12)
        {
            g_SystemTime.hour = 0;
        }
        if(time[2]&0x20)
        {
            g_SystemTime.hour += 12;
        }   
    }
    g_SystemTime.week   = BCDToDecimal(time[3]);
    g_SystemTime.day    = BCDToDecimal(time[4]);
    g_SystemTime.month  = BCDToDecimal(time[5]);
    g_SystemTime.year   = BCDToDecimal(time[6]);

    g_SystemTime.week = g_SystemTime.week == 0 ? 7 : g_SystemTime.week;
	
	return RT_EOK;
}

/**
  * @brief : write time to sd2405
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [time] the pointer of write time.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_size_t rt_hw_sd2405_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) 
{
	/* sec, min, hour, week, day, mon, year */
    rt_uint8_t calendar[8] = {0};
    struct SD2405Time *time = (struct SD2405Time *)buffer;

    calendar[1] = DecimalToBCD(time->second);
    calendar[2] = DecimalToBCD(time->minute);
    calendar[3] = DecimalToBCD(time->hour) | 0x80;
    calendar[4] = DecimalToBCD(time->week);
    calendar[5] = DecimalToBCD(time->day);
    calendar[6] = DecimalToBCD(time->month);
    calendar[7] = DecimalToBCD(time->year);

    rt_i2c_master_send(i2c_bus_device, SD2405_DEV_ADDR, RT_I2C_WR, calendar, 8);	
	
	return RT_EOK;	
}

/**
  * @brief : set default time to sd2405
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_sd2405_calender_init(void)
{
    rt_uint8_t flag = 0;
    rt_uint8_t tx_data[2] = {0x14,0xAA};
    rt_uint8_t calendar[8] = {0x00,0x00,0x08,0x08,0x01,0x01,0x01,0x18}; /* sec, min, hour, week, day, mon, year */

    struct rt_i2c_msg msg[2];

	msg[0].addr  = SD2405_DEV_ADDR;
	msg[0].flags = RT_I2C_WR;
	msg[0].len   = 1;
	msg[0].buf   = tx_data;

	msg[1].addr  = SD2405_DEV_ADDR;
	msg[1].flags = RT_I2C_RD ;
	msg[1].len   = 1;
	msg[1].buf   = &flag;
	
	rt_i2c_transfer(i2c_bus_device, msg, 2);  
	
    if (flag == 0xAA)
    {        
    }
    else
    {
		msg[0].addr  = SD2405_DEV_ADDR;
		msg[0].flags = RT_I2C_WR;
		msg[0].len   = 2;
		msg[0].buf   = tx_data;

		msg[1].addr  = SD2405_DEV_ADDR;
		msg[1].flags = RT_I2C_WR ;
		msg[1].len   = 8;
		msg[1].buf   = calendar;
		
		rt_i2c_transfer(i2c_bus_device, msg, 2);
    }
}

/**
  * @brief : config the work mode of sd2405
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_sd2405_config(void)
{
    rt_uint8_t i;
    rt_uint8_t data[2];
    rt_uint8_t sd2405Data[4] = {0xA9, 0x84, 0x8f, 0x01};
    rt_uint8_t sd2405MemoryAddr[4] = {0x10, 0x0f, 0x11, 0x14};
//    struct rt_i2c_msg msg;
	
    for (i = 0; i < 3; i++)
    {
        data[0] = sd2405MemoryAddr[i];	
        data[1] = sd2405Data[i];	
        rt_i2c_master_send(i2c_bus_device, SD2405_DEV_ADDR, RT_I2C_WR, data, 2);		
    }
}

/**
  * @brief : check the sd2405
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_uint8_t rt_hw_sd2405_test(void)
{
    rt_uint8_t flag = 0x14;
	struct rt_i2c_msg msg[2];

	msg[0].addr  = SD2405_DEV_ADDR;
	msg[0].flags = RT_I2C_WR;
	msg[0].len   = 1;
	msg[0].buf   = &flag;

	msg[1].addr  = SD2405_DEV_ADDR;
	msg[1].flags = RT_I2C_RD ;
	msg[1].len   = 1;
	msg[1].buf   = &flag;
	
	rt_i2c_transfer(i2c_bus_device, msg, 2);
	
    if (flag == 0xAA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
FINSH_FUNCTION_EXPORT(rt_hw_sd2405_test, test the sd2405);
FINSH_FUNCTION_EXPORT_ALIAS(rt_hw_sd2405_test, sd2405_test, test the sd2405);
/**
  * @brief : fm25vxx contrl function
  * @param : [dev] the device.
  * @param : [cmd] the commond.
  * @param : [args] the args.
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_err_t rt_hw_sd2405_control(rt_device_t dev, int cmd, void *args) 
{
    RT_ASSERT(dev != RT_NULL);
	
	switch (cmd)
	{
	    case SD2405_CFG: 
			rt_hw_sd2405_config();
		    break;
		case SD2405_CALENDER_INIT: 
			rt_hw_sd2405_calender_init();
			break;
		case SD2405_TEST: 
			rt_hw_sd2405_test();
			break;		
		default:
			break;
	}
	
	return RT_EOK;
}

/**
  * @brief : sd2405 init function
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
int rt_hw_sd2405_init(void)
{
    static rt_device_t device;
    static struct rt_device_pin *sd2405_irq_pin;

    sd2405_irq_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);
	
    if (sd2405_irq_pin == RT_NULL)
    {
        SD2405_PRINTF("pin device is not found!!!/r/n "); 
    }
    else
    {
        sd2405_irq_pin->ops->pin_attach_irq(device, 165, PIN_IRQ_MODE_FALLING, rt_hw_sd2405_irq_service, RT_NULL);  
        sd2405_irq_pin->ops->pin_irq_enable(device, 165, PIN_IRQ_ENABLE, INT_EXTI9_5_PRIO); 
    }
	
    i2c_bus_device = (struct rt_i2c_bus_device *)rt_device_find(RT_I2C0_NAME);
	
    if (i2c_bus_device == RT_NULL)
    {
        SD2405_PRINTF("i2c_bus is not found !!!"); 
		
        return -RT_ENOSYS; 
    }
    else
    {
        rt_device_open(&i2c_bus_device->parent, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
        SD2405_PRINTF("i2c_bus is open ~~~"); 
    }
	
    /* register device */
    sd2405_device.type = RT_Device_Class_I2CBUS;
    sd2405_device.init = RT_NULL;
    sd2405_device.open = RT_NULL;
    sd2405_device.close = RT_NULL;
    sd2405_device.read = rt_hw_sd2405_read;
    sd2405_device.write = rt_hw_sd2405_write;
    sd2405_device.control = rt_hw_sd2405_control;	
		
    /* no private */
    sd2405_device.user_data = RT_NULL;
	
    rt_device_register(&sd2405_device, RT_I2C_SD2405_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
    
    device = rt_device_find(RT_I2C_SD2405_NAME);
	
    if (device == NULL)
    {
        SD2405_PRINTF("sd2405 is not found! \r\n"); 		
    }
    else	
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	  

        rt_hw_sd2405_config();
        rt_hw_sd2405_calender_init();	
				
        rt_device_read(device, 0, RT_NULL, 0);	
    }
	
    return(0);
}
INIT_DEVICE_EXPORT(rt_hw_sd2405_init);


/* END OF FILE ---------------------------------------------------------------*/
