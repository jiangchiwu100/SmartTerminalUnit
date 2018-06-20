/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_wdg.c
  * @brief:     The driver of WDG.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-11-24
  * @updata:    [2018-01-23][Lexun][make the code cleanup]
  */


/* INCLUDES FILES ------------------------------------------------------------*/
#include "drv_wdg.h"
#include "rtthread.h"
#include "drv_gpio.h"
#include "common_data.h"

/* PRIVATE VARIABLES ---------------------------------------------------------*/
#if USING_SOFTWARE_IWDG
static IWDG_HandleTypeDef IWDG_Handler; 
#else 
static struct rt_device_pin *wdg_deal_pin; 
#endif /* USING_SOFTWARE_IWDG */

#if USING_SOFTWARE_IWDG

/**
  * @brief : software watchdog initialization  Tout=((4*2^prer)*rlr)/32 (ms).
  * @param : [prer] - Frequency division(IWDG_PRESCALER_4~IWDG_PRESCALER_256).
  * @param : [rlr] - Automatic reload value(0~0XFFF).
  * @return: None. 
  */
static void rt_hw_iwdg_init(rt_uint8_t prer, rt_uint16_t rlr)
{
    IWDG_Handler.Instance = IWDG;
    IWDG_Handler.Init.Prescaler = prer;	//设置IWDG分频系数
    IWDG_Handler.Init.Reload = rlr;		//重装载值
    HAL_IWDG_Init(&IWDG_Handler);		//初始化IWDG  
	
    HAL_IWDG_Start(&IWDG_Handler);	//开启独立看门狗
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : hardware watchdog initialization.
  * @param : None.
  * @return: None. 
  */
int rt_hw_wdg_deal_init(void)
{               
    rt_hw_iwdg_init(IWDG_PRESCALER_64, 1000); // 2s
	HAL_IWDG_Refresh(&IWDG_Handler); 
	
	return 0;
}

INIT_PREV_EXPORT(rt_hw_wdg_deal_init);


#else /* USING_SOFTWARE_IWDG */

/**
  * @brief : hardware watchdog initialization.
  * @param : None.
  * @return: None. 
  */
int rt_hw_wdg_deal_init(void)
{               
    wdg_deal_pin = (struct rt_device_pin *)rt_device_find(RT_PIN_NAME);
	
    if (wdg_deal_pin == RT_NULL)
    {
        WDG_PRINTF("watchdog pin device is not found!!! "); 
    }
	
    rt_hw_wdg_deal_task();
		
    return(RT_EOK);
}
//INIT_PREV_EXPORT(rt_hw_wdg_deal_init);

#endif /* USING_SOFTWARE_IWDG */
/**
  * @brief : feed watchdog function.
  * @param : None.
  * @return: None. 
  */  
int rt_hw_wdg_deal_task(void)
{
  #if USING_SOFTWARE_IWDG	
	HAL_IWDG_Refresh(&IWDG_Handler); 	
  #else	 
    pin_status[INDEX_WDG_FEED].status = !pin_status[INDEX_WDG_FEED].status;    
    rt_device_write(&wdg_deal_pin->parent, 0, &pin_status[INDEX_WDG_FEED], sizeof(struct rt_device_pin_status));       
  #endif /* USING_SOFTWARE_IWDG */	
    return(RT_EOK);
}


/* END OF FILE ---------------------------------------------------------------*/

