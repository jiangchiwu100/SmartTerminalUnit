/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : sd2405.h
  * @brief   : The driver of sd2405.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2018-01-14
  * @updata  : [2017-12-21][Sunxr][newly increased]
  */

#ifndef __DRV_SD2405_H__
#define __DRV_SD2405_H__


/* INCLUDES FILES ------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/
#ifdef  DEBUG 
#define SD2405_PRINTF(...)    rt_kprintf(__VA_ARGS__)
#else
#define SD2405_PRINTF(...)
#endif /* DEBUG */

#define SD2405_DEV_ADDR    (0x64 >> 1)


/* ENUM ----------------------------------------------------------------------*/
enum
{
    SD2405_CFG,
    SD2405_CALENDER_INIT,
    SD2405_TEST,
};


/* STRUCT --------------------------------------------------------------------*/	


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_sd2405_init(void);


#endif /* __DRV_SD2405_H__ */


/* END OF FILE ---------------------------------------------------------------*/
