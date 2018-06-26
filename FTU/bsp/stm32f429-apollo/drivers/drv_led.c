/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_led.c
  * @brief:     The driver of led.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtdevice.h>
#include "drv_led.h"
#include "drv_gpio.h"
#include "common_data.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t rt_led_dev;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : led GPIO initialize
  * @param : [none].
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
int rt_hw_led_init(void)
{	
    GPIO_InitTypeDef GPIO_InitStruct;
 
    /* Enable the GPIO_LED clock */
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    rt_led_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_led_dev == RT_NULL)
    {
        LED_PRINTF("led pin device is not found !!!"); 
    }
    else
    {
        LED_PRINTF("led pin device is found ~~~"); 
    }
    
    return (RT_EOK);
}
INIT_DEVICE_EXPORT(rt_hw_led_init);


/* PUBLIC FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : bottom alarm led operate
  * @param : [act]-ON/OFF.
  * @return: [RT_EOK] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
void rt_hw_alarm_led_operate(rt_uint8_t act)
{
    static rt_uint32_t s_alarm_led_flag;
    static rt_uint32_t s_alarm_led_counter;

    if (act == ON)
    {
        if (g_TelesignalDB[g_TelesignalAddr.shortCircuitFault] == ON || g_TelesignalDB[g_TelesignalAddr.earthingFault] == ON)
        {
            if(!s_alarm_led_flag)
            {
                ALARM_LED_ON();
                s_alarm_led_flag = 1;
                s_alarm_led_counter = 0;
            }
        }
        else
        {
            s_alarm_led_flag = 0;
        }

        if (s_alarm_led_flag && s_alarm_led_counter < 6)
        {
            if (++s_alarm_led_counter > 5)
            {
                s_alarm_led_counter = 10;
                ALARM_LED_RECOVER();
            }
        }
    }
    else if (act == OFF)
    {
        ALARM_LED_OFF();
        s_alarm_led_flag = 0;
    }
}

void rt_led_fast_task()
{
    static rt_uint32_t s_run_counter;
    
	while(1)
	{
		if (s_run_counter == 100)
		{
			pin_status[INDEX_BOARD_RUN_LED].status = !pin_status[INDEX_BOARD_RUN_LED].status;
			//pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
			rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
			//rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
		}
		else if (s_run_counter == 200)
		{
			s_run_counter = 0;
			pin_status[INDEX_BOARD_RUN_LED].status = !pin_status[INDEX_BOARD_RUN_LED].status;
			//pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
			rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
			//rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
		}    

		s_run_counter++;	
	}
}
/**
  * @brief : led task
  * @param : [none]
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
void rt_hw_led_task(void)
{
    static rt_uint32_t s_run_counter;

    if (s_run_counter == 500)
    {
        pin_status[INDEX_BOARD_RUN_LED].status = !pin_status[INDEX_BOARD_RUN_LED].status;
        //pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
        //rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
    }
    else if (s_run_counter == 1000)
    {
        s_run_counter = 0;
        pin_status[INDEX_BOARD_RUN_LED].status = !pin_status[INDEX_BOARD_RUN_LED].status;
        //pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
        //rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
    }    

    s_run_counter++;
    
    if(g_TelesignalDB[g_TelesignalAddr.switchClose] == ON)
    {
        pin_status[INDEX_BOTTOM_CLOSE_LED].status = GPIO_PIN_SET;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_CLOSE_LED], sizeof(struct rt_device_pin_status));
    }
    else
    {
        pin_status[INDEX_BOTTOM_CLOSE_LED].status = GPIO_PIN_RESET;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_CLOSE_LED], sizeof(struct rt_device_pin_status));
    }
    
    if(g_TelesignalDB[g_TelesignalAddr.switchOpen] == ON)
    {
        pin_status[INDEX_BOTTOM_OPEN_LED].status = GPIO_PIN_SET;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_OPEN_LED], sizeof(struct rt_device_pin_status));
    }
    else
    {
        pin_status[INDEX_BOTTOM_OPEN_LED].status = GPIO_PIN_RESET;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_OPEN_LED], sizeof(struct rt_device_pin_status));
    }
    
    if (g_TelesignalDB[g_TelesignalAddr.breakingCurrent] == ON || g_TelesignalDB[g_TelesignalAddr.overloadEvent] == ON || g_TelesignalDB[g_TelesignalAddr.haveyOverload] == ON || 
        g_TelesignalDB[g_TelesignalAddr.protectionAct] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitI0Down] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitI0Up] == ON ||
        g_TelesignalDB[g_TelesignalAddr.overLimitIaDown] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitIaUp]== ON || g_TelesignalDB[g_TelesignalAddr.overLimitIbDown] == ON || 
        g_TelesignalDB[g_TelesignalAddr.overLimitIbUp] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitIcDown] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitIcUp] == ON ||
        g_TelesignalDB[g_TelesignalAddr.overLimitUabDown] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUabUp] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUBCUp] == ON ||
        g_TelesignalDB[g_TelesignalAddr.overLimitUBCDown] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUcaUp] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUcaDown] == ON ||
        g_TelesignalDB[g_TelesignalAddr.overLimitUbcDown] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUbcUp] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitUABDown] == ON ||
        g_TelesignalDB[g_TelesignalAddr.overLimitUABUp] == ON ||g_TelesignalDB[g_TelesignalAddr.overLimitU0Up] == ON || g_TelesignalDB[g_TelesignalAddr.overLimitU0Down] == ON)
    {
        rt_hw_alarm_led_operate(ON);
    }
}

/* END OF FILE ---------------------------------------------------------------*/


