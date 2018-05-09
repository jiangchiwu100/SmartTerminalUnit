/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_di.c
  * @brief:     The driver of di.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
	
	
/* INCLUDES FILSE ------------------------------------------------------------*/
#include "drv_di.h"
#include "drv_gpio.h"
#include "string.h"
#include "common_data.h"


/* PUBLIC VARIABLES ----------------------------------------------------------*/
DiStr g_DiCollect;


/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t rt_di_dev;


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : di GPIO initialize
  * @param : [none].
  * @return: [0] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */  
int rt_hw_di_init(void)
{
    rt_di_dev = rt_device_find(RT_PIN_NAME);	
    
    if (rt_di_dev == RT_NULL)
    {
        DI_PRINTF("di pin device is not found !!!"); 
    }
    else
    {
        DI_PRINTF("di pin device is found ~~~"); 
    }
		
    return (0);
}
INIT_DEVICE_EXPORT(rt_hw_di_init);

/**
  * @brief : double-point detection
  * @param : [shakingTime]-eliminate shaking time.
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
static void rt_hw_double_point_check(float shaking_time)
{
    g_DiCollect.doubleState = g_DiCollect.state & 0x03;

    if ((g_DiCollect.doubleState == 0x00 || g_DiCollect.doubleState == 0x03))
    {
        if (g_pFixedValue->Data.publicValue.Str.controlloopAnomalyEnable == SWITCH_OFF)
        {
            return;
        }
            
        if (g_TelesignalDB.Str.deviceFault == OFF && g_DiCollect.doubleState == 0x00)
        {
            /* 进入装置故障判断 */
            g_DiCollect.deviceFalutCounter++;
            if (g_DiCollect.deviceFalutCounter > DEVICE_FAULT_TIME)
            {
                g_DiCollect.deviceFalutCounter = 0;

                DBWriteSOE(ADDR_DEVICE_FAULT, ON);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }

        if (g_DiCollect.doubleState == 0x03 && g_TelesignalDB.Str.controlLoopDisconnection == OFF)
        {
            /* 控制回路断线 */
            g_DiCollect.disconnectCounter++;

            if (g_DiCollect.disconnectCounter > LOOP_DISCONNECT_TIME)
            {
                g_DiCollect.disconnectCounter = 0;

                DBWriteSOE(ADDR_CONTROL_LOOP_DISCONNECTION, ON);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }
    }
    else if (g_DiCollect.doubleState == 0x01 || g_DiCollect.doubleState == 0x02)
    {
        if (g_TelesignalDB.Str.controlLoopDisconnection == ON)
        {
            DBWriteSOE(ADDR_CONTROL_LOOP_DISCONNECTION, OFF);
        }

        g_TelesignalDB.Str.controlLoopDisconnection = OFF;

        if (g_DiCollect.doubleState != g_DiCollect.doubleStateLast)
        {
            g_DiCollect.doubleCounter++;
            if (g_DiCollect.doubleCounter > shaking_time)
            {
                g_DiCollect.doubleCounter = 0;

                DBWriteSOE(ADDR_DOUBLE_SWITCH, ~g_DiCollect.doubleState & 0x03);
                g_DiCollect.doubleStateLast = g_DiCollect.doubleState;
            }
        }
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : di detection
  * @param : [clock]-call cycle
  * @return: [none] 
  * @updata: [2017-12-21][Sunxr][newly increased]
  */ 
void rt_hw_di_check_task(rt_uint8_t clock)
{
    rt_uint8_t i;
    static float s_shaking_time = 0;
	
    for (i = 0; i < DI_NUM; i++)
    {
        rt_device_read(rt_di_dev, 0, &pin_status[INDEX_SWITCH_CLOSE_DI + i], sizeof(struct rt_device_pin_status));	
    }
	
    g_DiCollect.state = pin_status[INDEX_SWITCH_CLOSE_DI].status | pin_status[INDEX_SWITCH_OPEN_DI].status << 1 | (!pin_status[INDEX_ENERGY_STORAGE_DI].status) << 2 | \
                        pin_status[INDEX_LOW_PRESSURE_DI].status << 3 | pin_status[INDEX_POWER_FAILURE_ALARM_DI].status << 4 | pin_status[INDEX_BETTERY_UNDERVOLTAGE_DI].status << 5 | \
                        pin_status[INDEX_BATTERYA_CTIVATE_DI].status << 6 | (!pin_status[INDEX_BATTERY_LOSS_ALARM_DI].status) << 7;
	
    s_shaking_time = (uint32_t)g_ParameterDB.Data.runPara.Str.diShakingTime / clock;

    for (i = 0; i < DI_NUM; i++)
    {
        if ((g_DiCollect.state & (0x01 << i)) == (g_DiCollect.stateLast & (0x01 << i)))
        {
            g_DiCollect.counter[i] = 0;
            g_TelesignalDB.buf[i] = (g_DiCollect.state >> i) & 0x01 ? OFF : ON;
        }
        else
        {
            g_DiCollect.counter[i]++;

            if (g_DiCollect.counter[i] >= s_shaking_time)
            {
                g_DiCollect.counter[i] = 0;

                DBWriteSOE(TELESIGNAL_START_ADDR + i, ((g_DiCollect.state >> i) & 0x01) ? OFF : ON);				
				
                g_DiCollect.stateLast ^= (0x01 << i);
            }
        }
    }	

    rt_hw_double_point_check(s_shaking_time);
    
    rt_device_read(rt_di_dev, 0, &pin_status[INDEX_MCU_POWER_ALARM_DI], sizeof(struct rt_device_pin_status));
    DBWriteSOE(ADDR_DEVICE_POWER_DOWN, (pin_status[INDEX_MCU_POWER_ALARM_DI].status) ? ON : OFF);
}

/* END OF FILE ---------------------------------------------------------------*/
