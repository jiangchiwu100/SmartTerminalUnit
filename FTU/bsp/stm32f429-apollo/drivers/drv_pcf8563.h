/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : pcf8563.h
  * @brief   : The driver of pcf8563.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2018-01-14
  * @updata  : [2017-12-21][Sunxr][newly increased]
  */

#ifndef __DRV_PCF8563_H__
#define __DRV_PCF8563_H__


/* INCLUDES FILES ------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/
#ifdef  DEBUG 
#define PCF8563_PRINTF(...)    rt_kprintf(__VA_ARGS__)
#else
#define PCF8563_PRINTF(...)
#endif /* DEBUG */

#define PCF8563_R_ADDR    (0xA3 >> 1)
#define PCF8563_W_ADDR    (0xA2 >> 1)

/* ENUM ----------------------------------------------------------------------*/
enum
{
    SD2405_CFG,
    SD2405_CALENDER_INIT,
    SD2405_TEST,
};


/* STRUCT --------------------------------------------------------------------*/	


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_pcf8563_init(void);


#endif /* __DRV_SD2405_H__ */


/* END OF FILE ---------------------------------------------------------------*/
