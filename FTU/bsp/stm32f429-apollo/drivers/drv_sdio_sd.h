/**
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      drv_sdio_sd.h
  * @brief:     The driver of SD carf,this file is part of RT-Thread RTOS.
  * @version:   V1.0.0(the first version)
  * @author:    Lexun
  * @date:      2009-01-05
  * @update:    [2018-02-02][Lexun][make the code clean up]
  */
#ifndef _SDMMC_SDCARD_H_
#define _SDMMC_SDCARD_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <board.h>
#include "stm32f4xx.h"
#include <rtdevice.h>


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define SD_PRINTF(...)    rt_kprintf(__VA_ARGS__)
#else
#define SD_PRINTF(...)   
#endif /* DEBUG */


#define SD_TIMEOUT             ((uint32_t)100000000) // 超时时间
#define SD_DMA_MODE            0 // 1：DMA模式，0：查询模式   


/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern SD_HandleTypeDef        SDCARD_Handler; // SD卡句柄
extern HAL_SD_CardInfoTypedef  SDCardInfo; // SD卡信息结构体


/* PUBLIC FUNCTION -----------------------------------------------------------*/
rt_uint8_t SD_Init(void);
rt_uint8_t SD_GetCardInfo(HAL_SD_CardInfoTypedef *cardinfo);


#endif /* _SDMMC_SDCARD_H_ */


/* END OF FILE ---------------------------------------------------------------*/

