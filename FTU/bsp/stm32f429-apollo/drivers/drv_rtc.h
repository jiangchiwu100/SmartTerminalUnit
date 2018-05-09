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
#ifndef __DRV_RTC_H__
#define __DRV_RTC_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <time.h>
#include <string.h>


/* DEFINES -------------------------------------------------------------------*/
#ifdef DEBUG
#define RTC_PRINTF(format,args...) 			rt_kprintf(format, ##args)
#else
#define RTC_PRINTF(format,args...)
#endif


/* PUBLIC FUNCTION -----------------------------------------------------------*/
int rt_hw_rtc_init(void);

#endif /* __DRV_RTC_H__ */


/* END OF FILE ---------------------------------------------------------------*/

