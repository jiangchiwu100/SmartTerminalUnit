/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_handheld.c
  * @brief:     The driver of handheld.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */

/* INCLUDE FILES -------------------------------------------------------------*/	
#include "drv_do.h"
#include "drv_gpio.h"
#include "drv_handheld.h"
#include "common_data.h"


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t rt_hand_dev;
static struct HandheldRemote Handheld;


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : handheld GPIO initialize
  * @param : [none].
  * @return: [0] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */   
int rt_hw_hand_init(void)
{
    rt_hand_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_hand_dev == RT_NULL)
    {
        HANDHELD_PRINTF("hand pin device is not found !!!"); 
    }
    else
    {
        HANDHELD_PRINTF("hand pin device is found ~~~"); 
    }
	
    return(0);
}
INIT_DEVICE_EXPORT(rt_hw_hand_init);

/**
  * @brief : handheld task
  * @param : [clock]-call cycle.
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
void rt_hw_handheld_remote_task(rt_uint8_t clock)
{
    static uint32_t s_open_shake_timer = 0;
    static uint32_t s_close_shake_timer = 0;
    static uint32_t s_reserve_time;

    if (g_TelesignalDB[g_TelesignalAddr.remote] == OFF) // Local mode does not allow operation
    {
        return;
    }

    rt_device_read(rt_hand_dev, 0, &pin_status[INDEX_HANDHELD_OPEN_PRESET], sizeof(struct rt_device_pin_status));
    rt_device_read(rt_hand_dev, 0, &pin_status[INDEX_HANDHELD_OPEN_EXECUTE], sizeof(struct rt_device_pin_status));
    rt_device_read(rt_hand_dev, 0, &pin_status[INDEX_HANDHELD_CLOSE_PRESET], sizeof(struct rt_device_pin_status));
    rt_device_read(rt_hand_dev, 0, &pin_status[INDEX_HANDHELD_CLOSE_EXECUTE], sizeof(struct rt_device_pin_status));	
    
	/* preset */
    if (pin_status[INDEX_HANDHELD_OPEN_PRESET].status)
    {
        s_close_shake_timer = 0;
        s_open_shake_timer / clock >= g_Parameter[DI_SHAKING_TIME] ? s_open_shake_timer = 0, Handheld.openPresetFlag = 1 : s_open_shake_timer++;
    }
    else if (pin_status[INDEX_HANDHELD_CLOSE_PRESET].status)
    {
        s_open_shake_timer = 0;
        s_close_shake_timer / clock >= g_Parameter[DI_SHAKING_TIME] ? s_close_shake_timer = 0, Handheld.closePresetFlag = 1 : s_close_shake_timer++;
    }

    /* open execute */
    if (Handheld.openPresetFlag)
    {
        // Preset timeout count
        Handheld.openPresetTimeOut++;
        s_reserve_time = g_Parameter[REVERSE_TIME] > 0 ? FloatToBin(g_Parameter[REVERSE_TIME]) : 3000;

        if (Handheld.openPresetTimeOut >= s_reserve_time / clock)
        {
            Handheld.openPresetFlag = 0;
        }
        else // Not timed out
        {
            if (pin_status[INDEX_HANDHELD_OPEN_EXECUTE].status)
            {
                s_open_shake_timer / clock >= g_Parameter[DI_SHAKING_TIME] ? s_open_shake_timer = 0, rt_hw_do_operate(ADDR_HANDHELD_OPER, DO_OPEN) : s_open_shake_timer++;
            }
        }
    }
    else
    {
        Handheld.openPresetTimeOut = 0;
    }

    /* close execute */
    if (Handheld.closePresetFlag)
    {
        // Preset timeout count
        Handheld.closePresetTimeOut++;
        s_reserve_time = g_Parameter[REVERSE_TIME] > 0 ? FloatToBin(g_Parameter[REVERSE_TIME]) : 3000;

        if (Handheld.closePresetTimeOut >= s_reserve_time / clock)
        {
            Handheld.closePresetFlag = 0;
        }
        else // Not timed out
        {
            if (pin_status[INDEX_HANDHELD_CLOSE_EXECUTE].status)
            {
                s_close_shake_timer / clock >= g_Parameter[DI_SHAKING_TIME] ? s_close_shake_timer = 0, rt_hw_do_operate(ADDR_HANDHELD_OPER, DO_CLOSE) : s_close_shake_timer++;
            }
        }
    }
    else
    {
        Handheld.closePresetTimeOut = 0;
    }
}

/* END OF FILE ---------------------------------------------------------------*/
