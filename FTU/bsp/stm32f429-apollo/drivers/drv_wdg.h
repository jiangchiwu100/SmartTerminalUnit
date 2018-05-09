/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_wdg.h
  * @brief:     The driver of WDG.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-11-24
  * @updata:    [2018-01-23][Lexun][make the code cleanup]
  */
#ifndef __FTU_WDG_H__
#define __FTU_WDG_H__


/* INCLUDES FILES ------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* INCLUDES FILES ------------------------------------------------------------*/
#ifdef  WDG_DEBUG
#define WDG_PRINTF(...)        rt_kprintf(__VA_ARGS__)
#else
#define WDG_PRINTF(...)
#endif /* WDG_DEBUG */

#define USING_SOFTWARE_IWDG        1

/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_wdg_deal_task(void);


#endif /* __FTU_WDG_H__ */


/* END OF FILE ---------------------------------------------------------------*/

