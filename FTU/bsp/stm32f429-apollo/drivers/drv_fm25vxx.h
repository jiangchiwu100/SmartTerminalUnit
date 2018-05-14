/**
  *            Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file    : fm25vxx.h
  * @brief   : The driver of FM25Vxx10.
  * @version : V03.001
  * @author  : Sun
  * @date    : 2017-12-21
  * @updata  : [2017-12-21][Sunxr][newly increased]
  */

#ifndef __DRV_FM25VXX_H__
#define __DRV_FM25VXX_H__


/* INCLUDE FILES -------------------------------------------------------------*/	
#include <stm32f4xx.h>
#include <rtthread.h>


/* DEFINE --------------------------------------------------------------------*/
#define DEBUG

#ifdef DEBUG
#define FRAM_PRINTF(...)    rt_kprintf(__VA_ARGS__)
#else
#define FRAM_PRINTF(...)   
#endif /* DEBUG */

#define FM25V10_MAX_ADDR                0x1ffff   

#define FRAM_WREN                       0x06     // Set write enable latch
#define FRAM_WRDI                       0x04     // Write disable
#define FRAM_RDSR                       0x05     // Read status register
#define FRAM_WRSR                       0x01     // Write status register
#define FRAM_READ                       0x03     // Read memory data
#define FRAM_FSTRD                      0x0B     // Fast read memory data
#define FRAM_WRITE                      0x02     // Write memory data
#define FRAM_SLEEP                      0xB9     // Enter sleep mode
#define FRAM_RIID                       0x9F     // Read device ID
#define FRAM_SNR                        0xC3     // Read S/N

#define FRAM_HWFLAG1                    0x55
#define FRAM_HWFLAG2                    0xAA


/* ENUM ----------------------------------------------------------------------*/	
enum FramOperate
{
    FM_CLEAR_RECORD,   
    FM_POWER_DOWN_DEAL,
    FM_SLEEP,
    FM_READ_SR,
    FM_READ_ID,
    FM_TEST,
	FM_MONITOR,
};


/* PUBLIC FUNCTION -----------------------------------------------------------*/
int rt_hw_fm25vxx_init(void);


#endif /* __DRV_FM25VXX_H__ */


/* END OF FILE ---------------------------------------------------------------*/

