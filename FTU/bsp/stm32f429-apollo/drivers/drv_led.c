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
        if (g_TelesignalDB[ADDR_SHORT_CIRCUIT_FAULT] == ON || g_TelesignalDB[ADDR_EARTHING_FAULT] == ON)
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
        pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
    }
    else if (s_run_counter == 1000)
    {
        s_run_counter = 0;
        pin_status[INDEX_BOARD_RUN_LED].status = !pin_status[INDEX_BOARD_RUN_LED].status;
        pin_status[INDEX_BOTTOM_RUN_LED].status = !pin_status[INDEX_BOTTOM_RUN_LED].status;
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOARD_RUN_LED], sizeof(struct rt_device_pin_status));
        rt_device_write(rt_led_dev, 0, &pin_status[INDEX_BOTTOM_RUN_LED], sizeof(struct rt_device_pin_status));
    }    

    s_run_counter++;
    
    if (g_TelesignalDB[ADDR_BREAKINGCURRENT] == ON || g_TelesignalDB[ADDR_OVERLOAD_EVENT] == ON || g_TelesignalDB[ADDR_HAVEY_OVERLOAD_EVENT] == ON || 
        g_TelesignalDB[ADDR_PROTECTION_ACT] == ON || g_TelesignalDB[ADDR_OVERLIMIT_I0_DOWN] == ON || g_TelesignalDB[ADDR_OVERLIMIT_I0_UP] == ON ||
        g_TelesignalDB[ADDR_OVERLIMIT_IA_DOWN] == ON || g_TelesignalDB[ADDR_OVERLIMIT_IA_UP]== ON || g_TelesignalDB[ADDR_OVERLIMIT_IB_DOWN] == ON || 
        g_TelesignalDB[ADDR_OVERLIMIT_IB_UP] == ON || g_TelesignalDB[ADDR_OVERLIMIT_IC_DOWN] == ON || g_TelesignalDB[ADDR_OVERLIMIT_IC_UP] == ON ||
        g_TelesignalDB[ADDR_OVERLIMIT_Uab_DOWN] == ON || g_TelesignalDB[ADDR_OVERLIMIT_Uab_UP] == ON || g_TelesignalDB[ADDR_OVERLIMIT_UBC_UP] == ON ||
        g_TelesignalDB[ADDR_OVERLIMIT_UBC_DOWN] == ON || g_TelesignalDB[ADDR_OVERLIMIT_Uca_UP] == ON || g_TelesignalDB[ADDR_OVERLIMIT_Uca_DOWN] == ON ||
        g_TelesignalDB[ADDR_OVERLIMIT_U0_UP] == ON || g_TelesignalDB[ADDR_OVERLIMIT_U0_DOWN] == ON)
    {
        rt_hw_alarm_led_operate(ON);
    }
}

/* END OF FILE ---------------------------------------------------------------*/


