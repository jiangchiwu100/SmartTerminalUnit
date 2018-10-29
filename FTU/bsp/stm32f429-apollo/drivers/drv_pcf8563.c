/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : pcf8563.c
  * @brief   : The driver of pcf8563.
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
#include "drv_pcf8563.h"
#include "common_data.h"
#include ".\MultiThread\multi_thread.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct rt_device pcf8563_device;
static struct rt_i2c_bus_device *i2c_bus_device = RT_NULL ;


/* PUBLIC VARIABLES ----------------------------------------------------------*/


/* PRVIATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : the Interrupt service of pcf8563
  * @param : [args]-entrance parameter pointer
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_pcf8563_irq_service(void *args)
{
    if (!(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_14)))
    { 
        g_SystemTime.msecond = 0;	     
		
        if (++g_SystemTime.second > 59)
        {
            g_SystemTime.second = 0;

            if (++g_SystemTime.minute > 59)
            {	
                rt_event_send(&pcf8563_event, EVENT_RUN);               
            }  									
        }            							
    }    
}
/**
  * @brief : read time from pcf8563
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [buffer] the pointer of data.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_size_t rt_hw_pcf8563_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size) 
{
	rt_uint8_t rx_addr = 0x02;
    rt_uint8_t time[16] = {0};
    struct rt_i2c_msg msg[2];

	msg[0].addr  = PCF8563_W_ADDR;
	msg[0].flags = RT_I2C_WR;
	msg[0].len   = 1;
	msg[0].buf   = &rx_addr;

	msg[1].addr  = PCF8563_R_ADDR;
	msg[1].flags = RT_I2C_RD ;
	msg[1].len   = 7;
	msg[1].buf   = time;
	
	rt_i2c_transfer(i2c_bus_device, msg, 2);  
		
    g_SystemTime.second = BCDToDecimal(time[0] & 0x7f);
    g_SystemTime.minute = BCDToDecimal(time[1] & 0x7f);
    g_SystemTime.hour   = BCDToDecimal(time[2] & 0x3f);
    g_SystemTime.day    = BCDToDecimal(time[3] & 0x3f);	
    g_SystemTime.week   = time[4] & 0x07;  // 星期不以BCD码表示
    g_SystemTime.month  = BCDToDecimal(time[5] & 0x1f);
    g_SystemTime.year   = BCDToDecimal(time[6]);

    g_SystemTime.week = g_SystemTime.week == 0 ? 7 : g_SystemTime.week;
	
	return RT_EOK;
}

/**
  * @brief : write time to pcf8563
  * @param : [dev] the device.
  * @param : [pos] the addr.
  * @param : [time] the pointer of write time.
  * @param : [size] the size of data.
  * @return: [RT_EOK]/[RT_ERROR]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_size_t rt_hw_pcf8563_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size) 
{
    struct tm tm_pcf8563;	
    rt_device_t rtc_dev; 	
	/* sec, min, hour, week, day, mon, year */
    rt_uint8_t calendar[8] = {0x02};
	rt_uint8_t *time = (rt_uint8_t *)buffer;
	time_t time_stamp;
	
    calendar[1] = DecimalToBCD(*(time + 2));
    calendar[2] = DecimalToBCD(*(time + 3));
    calendar[3] = DecimalToBCD(*(time + 4));
    calendar[4] = DecimalToBCD(*(time + 6));
    calendar[5] = *(time + 5);
    calendar[6] = DecimalToBCD(*(time + 7));
    calendar[7] = DecimalToBCD(*(time + 8));

    rt_i2c_master_send(i2c_bus_device, PCF8563_W_ADDR, RT_I2C_WR, calendar, 8);	
	
    rtc_dev = rt_device_find(RT_RTC_NAME);		
	
    if (rtc_dev == RT_NULL)
    {
        PCF8563_PRINTF("rtc device is not found in pcf8563 write!!!/r/n "); 
    }
    else
    {
		tm_pcf8563.tm_sec  = *(time + 2); 
		tm_pcf8563.tm_min  = *(time + 3); 
		tm_pcf8563.tm_hour = *(time + 4);
		tm_pcf8563.tm_mday = *(time + 6);
		tm_pcf8563.tm_wday = *(time + 5);	
		tm_pcf8563.tm_mon  = *(time + 7) - 1; 
		tm_pcf8563.tm_year = *(time + 8) + 100;
		
        time_stamp = mktime(&tm_pcf8563);
	 	
        rt_device_control(rtc_dev, RT_DEVICE_CTRL_RTC_SET_TIME, &time_stamp); 
    }
	
	return RT_EOK;	
}

/**
  * @brief : set default time to pcf8563
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_pcf8563_calender_test(void)
{
    rt_uint8_t rx_data[17] = {0x02};
    rt_uint8_t calendar[8] = {0x02,0x00,0x08,0x08,0x01,0x01,0x01,0x18}; /* sec, min, hour, week, day, mon, year */

    struct rt_i2c_msg msg[3];

	msg[0].addr  = PCF8563_W_ADDR;
	msg[0].flags = RT_I2C_WR;
	msg[0].len   = 8;
	msg[0].buf   = calendar;

	msg[1].addr  = PCF8563_W_ADDR;
	msg[1].flags = RT_I2C_WR ;
	msg[1].len   = 1;
	msg[1].buf   = rx_data;
	
	msg[2].addr  = PCF8563_R_ADDR;
	msg[2].flags = RT_I2C_RD ;
	msg[2].len   = 7;
	msg[2].buf   = rx_data;
	
	rt_i2c_transfer(i2c_bus_device, msg, sizeof(calendar));  
	
    if (rx_data[6] == 0x18)
    {      
        rt_kprintf("pcf8563 is normal");		
    }
    else
    {
        rt_kprintf("pcf8563 is abnormal");
    }
}
FINSH_FUNCTION_EXPORT(rt_hw_pcf8563_calender_test, test the pcf8563);
FINSH_FUNCTION_EXPORT_ALIAS(rt_hw_pcf8563_calender_test, pcf8563_test, test the pcf8563);
/**
  * @brief : config the work mode of pcf8563
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static void rt_hw_pcf8563_config(void)
{
    rt_uint8_t i;
    rt_uint8_t data[2];
    rt_uint8_t pcf8563Data[]       = {0x00, 0x00};
    rt_uint8_t pcf8563MemoryAddr[] = {0x00, 0x01};	
//    rt_uint8_t pcf8563Data[]       = {0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x80, 0x81, 0x40};
//    rt_uint8_t pcf8563MemoryAddr[] = {0x00, 0x01, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
//    struct rt_i2c_msg msg;
	
    for (i = 0; i < sizeof(pcf8563MemoryAddr); i++)
    {
        data[0] = pcf8563MemoryAddr[i];	
        data[1] = pcf8563Data[i];	
        rt_i2c_master_send(i2c_bus_device, PCF8563_W_ADDR, RT_I2C_WR, data, 2);		
    }
}

/**
  * @brief : fm25vxx contrl function
  * @param : [dev] the device.
  * @param : [cmd] the commond.
  * @param : [args] the args.
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
static rt_err_t rt_hw_pcf8563_control(rt_device_t dev, int cmd, void *args) 
{
    RT_ASSERT(dev != RT_NULL);
	
	switch (cmd)
	{
	    case SD2405_CFG: 
			rt_hw_pcf8563_config();
		    break;
		case SD2405_CALENDER_INIT: 
			
			break;
		case SD2405_TEST: 
			rt_hw_pcf8563_calender_test();
			break;		
		default:
			break;
	}
	
	return RT_EOK;
}

/**
  * @brief : pcf8563 init function
  * @param : [none]
  * @return: [none]
  * @updata: [2018-01-14][Sunxr][newly increased]
  */
int rt_hw_pcf8563_init(void)
{    
    static rt_device_t device;
//    static struct rt_device_pin *pcf8563_irq_pin;

//    pcf8563_irq_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);
//	
//    if (pcf8563_irq_pin == RT_NULL)
//    {
//        PCF8563_PRINTF("pin device is not found!!!/r/n "); 
//    }
//    else
//    {
//        pcf8563_irq_pin->ops->pin_attach_irq(device, 129, PIN_IRQ_MODE_FALLING, rt_hw_pcf8563_irq_service, RT_NULL);  
//        pcf8563_irq_pin->ops->pin_irq_enable(device, 129, PIN_IRQ_ENABLE, INT_EXTI9_5_PRIO); 
//    }
	
    i2c_bus_device = (struct rt_i2c_bus_device *)rt_device_find(RT_I2C0_NAME);
	
    if (i2c_bus_device == RT_NULL)
    {
        PCF8563_PRINTF("i2c_bus is not found !!!"); 
		
        return -RT_ENOSYS; 
    }
    else
    {
        rt_device_open(&i2c_bus_device->parent, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);
        PCF8563_PRINTF("i2c_bus is open ~~~"); 
    }
	
    /* register device */
    pcf8563_device.type = RT_Device_Class_I2CBUS;
    pcf8563_device.init = RT_NULL;
    pcf8563_device.open = RT_NULL;
    pcf8563_device.close = RT_NULL;
    pcf8563_device.read = rt_hw_pcf8563_read;
    pcf8563_device.write = rt_hw_pcf8563_write;
    pcf8563_device.control = rt_hw_pcf8563_control;	
		
    /* no private */
    pcf8563_device.user_data = RT_NULL;
	
    rt_device_register(&pcf8563_device, RT_I2C_PCF8563_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	
    
    device = rt_device_find(RT_I2C_PCF8563_NAME);
	
    if (device == NULL)
    {
        PCF8563_PRINTF("pcf8563 is not found! \r\n"); 		
    }
    else	
    {
        rt_device_open(device, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);	  

        rt_hw_pcf8563_config();
//        rt_hw_pcf8563_calender_test();	
				
        rt_device_read(device, 0, RT_NULL, 0);	
    }
	
    return(0);
}
INIT_DEVICE_EXPORT(rt_hw_pcf8563_init);


/* END OF FILE ---------------------------------------------------------------*/
