/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_handheld.h
  * @brief:     The driver of handheld.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */

#ifndef __DRV_HANDHELD_H__
#define __DRV_HANDHELD_H__


/* INCLUDE FILES -------------------------------------------------------------*/	
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/	
#ifdef  DEBUG
#define HANDHELD_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define HANDHELD_PRINTF(...)
#endif /* DEBUG */


/* STRUCT --------------------------------------------------------------------*/	
struct HandheldRemote
{
    uint8_t openPresetFlag;
    uint8_t closePresetFlag;	  
    uint32_t openPresetTimeOut;
    uint32_t closePresetTimeOut;
};


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_hand_init(void);
void rt_hw_handheld_remote_task(uint8_t clock);


#endif /* __DRV_HANDHELD_H__ */


/* END OF FILE ---------------------------------------------------------------*/
