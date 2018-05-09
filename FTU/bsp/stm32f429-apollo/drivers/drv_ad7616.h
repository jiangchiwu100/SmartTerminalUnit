/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      ad7616_fsmc.c
  * @brief:     The driver of AD7616_FSMC.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @updata  : [2018-02-27][Lexun][make code cleanup]
  */
#ifndef _DRV_AD7616_H_
#define _DRV_AD7616_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "rtconfig.h"
#include "stm32f4xx_hal.h"


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define AD7616_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define AD7616_PRINTF(...)
#endif /* DEBUG */

#define AD7616_DATA_ADDR   0x64000000

#define AD_RESET_H()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);
#define AD_RESET_L()       HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);

#define AD7606_BUSY_PIN    GPIO_PIN_12


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_hw_adc_init(void);


#endif /* END _DRV_LED_H_ */


/* END OF FILE ---------------------------------------------------------------*/

