/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_di.h
  * @brief:     The driver of di.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
	
#ifndef __DRV_DI_H__
#define __DRV_DI_H__


/* INCLUDE FILES -------------------------------------------------------------*/	
#include "rtthread.h"
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/	
#ifdef  DEBUG
#define DI_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define DI_PRINTF(...)
#endif /* DEBUG */

#define DI_NUM                 8


/* STRUCT -------------------------------------------------------------------*/	
typedef struct TagDiStr
{  
    rt_uint32_t counter[DI_NUM];
    rt_uint32_t state;	
    rt_uint32_t stateLast;	
    rt_uint32_t doubleCounter;
    rt_uint32_t deviceFalutCounter;
    rt_uint32_t disconnectCounter;	
    rt_uint8_t doubleState;
    rt_uint8_t doubleStateLast;   	
}DiStr;

/* DEFINE --------------------------------------------------------------------*/	

#define ReadDI2()              HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)     // SwitchOpen
#define ReadDI1()              HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7)      // SwitchClose  
#define ReadDI3()              HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8)      // EnergyStorage
#define ReadDI4()              HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9)      // 低气压
#define ReadDI5()              HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)      // 电池失电告警
#define ReadDI6()              HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_13)     // 电池活化
#define ReadDI7()              HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_14)     // 电池欠压
#define ReadDI8()              HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_15)     // 电源故障告警 


// 开入顺序需严格按照点表顺序 从低位到高位排列 
#define ReadDI()		(ReadDI1()|(ReadDI2()<<1)|((!ReadDI3()&0x01)<<2)|(ReadDI4()<<3)|(ReadDI8()<<4)|(ReadDI7()<<5 |(ReadDI6()<<6)|((!ReadDI5()&0x01)<<7)))


/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern DiStr g_DiCollect;


/* PUBLIC FUNCTION  ----------------------------------------------------------*/
extern int rt_hw_di_init(void);
extern void rt_hw_di_check_task(rt_uint8_t clock);


#endif /* __DRV_DI_H__ */


/* END OF FILE ---------------------------------------------------------------*/

