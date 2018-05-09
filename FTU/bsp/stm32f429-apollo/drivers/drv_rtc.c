/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_rtc.c
  * @brief:     The driver of RTC,this file is part of RT-Thread RTOS
  * @version:   V1.0.0(the first version for STM32F429)
  * @author:    Bernard
  * @date:      2009-01-05
  * @update:    [2018-02-02][Lexun][make the code clean up]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include "board.h"
#include "drv_rtc.h"
#include <rtdevice.h>


#if defined(RT_USING_RTC)

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static struct rt_device rtc;
static RTC_HandleTypeDef RTC_Handler;  //RTC句柄


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief : get the RTC time stamp.
  * @param : None.
  * @return: return the new time stamp.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
static time_t GetRTCTimeStamp(void)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    struct tm tm_new;
    
    HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
    tm_new.tm_sec  = RTC_TimeStruct.Seconds; 
    tm_new.tm_min  = RTC_TimeStruct.Minutes; 
    tm_new.tm_hour = RTC_TimeStruct.Hours;
    tm_new.tm_mday = RTC_DateStruct.Date;
    tm_new.tm_mon  = RTC_DateStruct.Month-1; 
    tm_new.tm_year = RTC_DateStruct.Year+100;
	
    return mktime(&tm_new);
}

/**
  * @brief : set the RTC time stamp.
  * @param : [time_stamp] the time stamp.
  * @return: [RT_ERROR] error;[RT_EOK] success.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
static rt_err_t SetRTCTimeStamp(time_t time_stamp)
{
    RTC_TimeTypeDef RTC_TimeStruct;
    RTC_DateTypeDef RTC_DateStruct;
    struct tm *p_tm;
    p_tm = localtime(&time_stamp);
    if (p_tm->tm_year < 100)
    {
        return(RT_ERROR);
    }
    RTC_TimeStruct.Seconds = p_tm->tm_sec ; 
    RTC_TimeStruct.Minutes = p_tm->tm_min ; 
    RTC_TimeStruct.Hours   = p_tm->tm_hour;
    RTC_DateStruct.Date    = p_tm->tm_mday;
    RTC_DateStruct.Month   = p_tm->tm_mon+1 ; 
    RTC_DateStruct.Year    = p_tm->tm_year-100;
    RTC_DateStruct.WeekDay = p_tm->tm_wday+1;
    HAL_RTC_SetTime(&RTC_Handler,&RTC_TimeStruct,RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&RTC_Handler,&RTC_DateStruct,RTC_FORMAT_BIN);
  
    return(RT_EOK);
}

/**
  * @brief : open the rtc device.
  * @param : [dev] the device pointer.
  * @param : [oflag] the flag.
  * @return: [RT_EOK] success.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
static rt_err_t _rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }

    return(RT_EOK);
}

/**
  * @brief : read the rtc device.
  * @param : [dev] the device pointer.
  * @param : [pos] the position.
  * @param : [buffer] the buffer.
  * @param : [size] the size of buffer.
  * @return: [RT_EOK] success.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
static rt_size_t _rtc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    return(0);
}

/**
  * @brief : Configure the rtc device.
  * @param : [dev] Pointer to device descriptor.
  * @param : [cmd] RTC control command.
  * @param : [args] Arguments.
  * @return: return the result.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
static rt_err_t _rtc_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result;
    RT_ASSERT(dev != RT_NULL);
	
    switch (cmd)
    {
        case RT_DEVICE_CTRL_RTC_GET_TIME:
            *(rt_uint32_t *)args = GetRTCTimeStamp();
            RTC_PRINTF("RTC: get rtc_time %x\n", *(rt_uint32_t *)args());
            break;

        case RT_DEVICE_CTRL_RTC_SET_TIME:
            result = SetRTCTimeStamp(*(rt_uint32_t *)args);
            RTC_PRINTF("RTC: set rtc_time %x\n", *(rt_uint32_t *)args);
            /* Reset counter */
            break;
    }

    return(result);
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : RTC initialization function.
  * @param : None.
  * @return: [0] success;[2]failure.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
rt_uint8_t RTC_Init(void)
{      
    RTC_Handler.Instance = RTC;
    RTC_Handler.Init.HourFormat = RTC_HOURFORMAT_24; // RTC设置为24小时格式 
    RTC_Handler.Init.AsynchPrediv = 0X7F; // RTC异步分频系数(1~0X7F)
    RTC_Handler.Init.SynchPrediv = 0XFF; // RTC同步分频系数(0~7FFF)   
    RTC_Handler.Init.OutPut = RTC_OUTPUT_DISABLE;     
    RTC_Handler.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RTC_Handler.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if (HAL_RTC_Init(&RTC_Handler) != HAL_OK)
    {
        return(2);
    }
      
    if (HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0) != 0X5050) // 是否第一次配置
    { 
//        RTC_Set_Time(23,59,56,RTC_HOURFORMAT12_PM); // 设置时间 ,根据实际时间修改
//        RTC_Set_Date(15,12,27,7); // 设置日期
        HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,0X5050); // 标记已经初始化过了
    }
		
    return(0);
}

/**
  * @brief : RTC msp initialization function.
  * @param : [hrtc] the RTC handler.
  * @return: [0] success;[2]failure.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE(); // 使能电源时钟PWR
    HAL_PWR_EnableBkUpAccess(); // 取消备份区域写保护
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE; // LSE配置
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON; // RTC使用LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC; // 外设为RTC
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; // RTC时钟源为LSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        
    __HAL_RCC_RTC_ENABLE(); // RTC时钟使能
}

/**
  * @brief : Set the alarm clock (by a week of alarm, 24 hours).
  * @param : [week] the week parameter.
  * @param : [hour] the hour parameter.
  * @param : [min] the minute parameter.
  * @param : [sec] the second parameter.
  * @return: None.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void RTC_Set_AlarmA(rt_uint8_t week,rt_uint8_t hour,rt_uint8_t min,rt_uint8_t sec)
{ 
    RTC_AlarmTypeDef RTC_AlarmSturuct;
    
    RTC_AlarmSturuct.AlarmTime.Hours=hour;  //小时
    RTC_AlarmSturuct.AlarmTime.Minutes=min; //分钟
    RTC_AlarmSturuct.AlarmTime.Seconds=sec; //秒
    RTC_AlarmSturuct.AlarmTime.SubSeconds=0;
    RTC_AlarmSturuct.AlarmTime.TimeFormat=RTC_HOURFORMAT12_AM;
    
    RTC_AlarmSturuct.AlarmMask=RTC_ALARMMASK_NONE;//精确匹配星期，时分秒
    RTC_AlarmSturuct.AlarmSubSecondMask=RTC_ALARMSUBSECONDMASK_NONE;
    RTC_AlarmSturuct.AlarmDateWeekDaySel=RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;//按星期
    RTC_AlarmSturuct.AlarmDateWeekDay=week; //星期
    RTC_AlarmSturuct.Alarm=RTC_ALARM_A;     //闹钟A
    HAL_RTC_SetAlarm_IT(&RTC_Handler,&RTC_AlarmSturuct,RTC_FORMAT_BIN);
    
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, (INT_RTC_ALARM_PRIO >> 4) & 0x0F, INT_RTC_ALARM_PRIO & 0x0F);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
  * @brief : Periodic wake-up timer setting.
  * @param : [wksel] RTCEx_Wakeup_Timer_Definitions.
  * @param : [cnt] auto reloading value, reduced to 0 to produce interrupts.
  * @return: None.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void RTC_Set_WakeUp(rt_uint32_t wksel, rt_uint16_t cnt)
{ 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler, RTC_FLAG_WUTF); // 清除RTC WAKE UP的标志
	
    HAL_RTCEx_SetWakeUpTimer_IT(&RTC_Handler,cnt,wksel); // 设置重装载值和时钟 
	
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, (INT_RTC_WKUP_PRIO >> 4) & 0x0F, INT_RTC_WKUP_PRIO & 0x0F);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}

/**
  * @brief : RTC Alarm IRQHandler.
  * @param : None.
  * @return: None.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void RTC_Alarm_IRQHandler(void)
{
    HAL_RTC_AlarmIRQHandler(&RTC_Handler);
}
    
/**
  * @brief : RTC AlarmA IRQHandler callback function.
  * @param : [hrtc] the handler pointer.
  * @return: None.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    RTC_PRINTF("ALARM A!\r\n");
}

/**
  * @brief : RTC WKUP IRQHandler function.
  * @param : None.
  * @return: None.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
void RTC_WKUP_IRQHandler(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RTC_Handler); 
}

/**
  * @brief : Register RTC device.
  * @param : [device] Pointer to device descriptor.
  * @param : [name] Device name.
  * @param : [flag] Configuration flags.
  * @return: Error code.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
rt_err_t rt_hw_rtc_register(rt_device_t device,	const char *name,	rt_uint32_t flag)
{
    RT_ASSERT(device != RT_NULL);

    device->type        = RT_Device_Class_RTC;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;
    device->init        = RT_NULL;
    device->open        = _rtc_open;
    device->close       = RT_NULL;
    device->read        = _rtc_read;
    device->write       = RT_NULL;
    device->control     = _rtc_control;
    device->user_data   = RT_NULL;

    /* register a character device */
    return rt_device_register(device, name, RT_DEVICE_FLAG_RDWR | flag);
}

/**
  * @brief : Initialize all RTC module related hardware and register RTC device to kernel.
  * @param : None.
  * @return: [RT_EOK] success.
  * @update: [2017-12-07][Lexun][make code cleanup]
  */  
int rt_hw_rtc_init(void)
{
    RTC_Init();
	
    /* register rtc device */
    rt_hw_rtc_register(&rtc, RT_RTC_NAME, 0);
	
    return(RT_EOK);
}
//INIT_BOARD_EXPORT(rt_hw_rtc_init);

#endif /* RT_USING_RTC */


/* END OF FILE ---------------------------------------------------------------*/

