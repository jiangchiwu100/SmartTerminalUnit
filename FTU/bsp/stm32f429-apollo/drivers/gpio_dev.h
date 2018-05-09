/*
** ------------------------------- Copyright (c) -------------------------------                                     
**
**                           http://www.sojoline.com
**
** note:    Copyright SOJO. ALL RIGHTS RESERVED.
**          This software is provided under license and contains proprietary and
**          confidential material which is the property of Company Name tech. 
**    
** -------------------------------- FILE INFO ----------------------------------
** File name:               gpio_dev.h
** Descriptions:            The configration of GPIO. 
**
** -----------------------------------------------------------------------------
** Author:                  Mr.Lee
** Created date:            2017.07.12
** System:                  The new FTU system.
** Version:                 V1.01
** Descriptions:            The original version
**
** -----------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
** ---------------------------------------------------------------------------*/
#ifndef _GPIO_DEV_H_
#define _GPIO_DEV_H_


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#ifdef NORMAL_SWC

/* DEFINES -------------------------------------------------------------------*/
#define OUT_SET                   (GPIO_PIN_SET)
#define OUT_RESET                 (GPIO_PIN_RESET)

#define OUT_POS_24V               (0)
#define OUT_POS_CLOSE             (1)
#define OUT_POS_BACK              (2)
#define OUT_POS_CUT               (3)
#define OUT_POS_EXT_ALRAM         (4)
#define OUT_POS_LOCAL_CTRL        (5)
#define OUT_POS_VOL_ACTIVE        (6)

#define IN_NORMAL                 (GPIO_PIN_RESET)
#define IN_ERR                    (GPIO_PIN_SET)

#define IN_POS_HAND_CLOSE         (7)  // 手动合闸信号输入位
#define IN_POS_HAND_OPEN          (8)  // 手动分闸信号输入位
#define IN_POS_HAND_MODE          (9)  // 手动/就地控制信号输入位
#define IN_POS_ST_CLOSE           (10) // 开关合闸位置输入
#define IN_POS_ST_CUT             (11) // 开关分闸位置输入
#define IN_POS_ST_LOCAL_CTRL      (12) // 远方/就地控制信号输入位
#define IN_POS_ST_VOL_ACTIVE      (13) // 电池激活信号输入位
#define IN_POS_ST_VOL_220         (14) // 交流供电信号输入位
#define IN_POS_ST_VOL_LACK        (15) // 失压信号输入位

#define LED_ON                    (GPIO_PIN_RESET)
#define LED_OFF                   (GPIO_PIN_SET)

#define LED_RUN_POS               (0) // 运行
#define LED_FA_POS                (1) // error
#define LED_COM_POS               (2) // 101 通信
#define LED_NET_POS               (3) // 104 通信
#define LED_CUT_POS               (4) // 速断
#define LED_ERT_POS               (5) // 零序
#define LED_OVI_POS               (6) // 过流
#define LED_RECOL_POS             (7) // 重合闸

#define DIAL_GROUP_A              (0) // 零序电流定值
#define DIAL_GROUP_B              (1) // 零序延时
#define DIAL_GROUP_C              (2) // 过流定值
#define DIAL_GROUP_D              (3) // 过流延时
#define DIAL_GROUP_E              (4) // 速断电流定值
#define DIAL_GROUP_F              (5) // 时间间隔(low bit) / 重合闸次数(high bit)
#define DIAL_GROUP_G              (6)
#define DIAL_GROUP_H              (7)

#else /* not define NORMAL SWC */

#define OUT_SET                   (GPIO_PIN_SET)
#define OUT_RESET                 (GPIO_PIN_RESET)

#define IN_NORMAL                 (GPIO_PIN_RESET)
#define IN_ERR                    (GPIO_PIN_SET)

#define IN_POS_HAND_CLOSE         (7)  // 外部输入手动合信号
#define IN_POS_HAND_OPEN          (8)  // 外部输入手动分信号

#define IN_POS_ST_CLOSE           (10) // 外部输入开关合状态
#define IN_POS_ST_CUT             (11) // 外部输入开关分状态
#define IN_POS_HAND_MODE          (0)
#define IN_POS_ST_LOCAL_CTRL      (0)

#define OUT_POS_CLOSE             (1)  // 自动控制合输出
#define OUT_POS_CUT               (2)  // 自动控制分输出

#define OUT_POS_RELAY_CLOSE       (3)  // 继电器外部 合闸状态--led
#define OUT_POS_RELAY_CUT         (4)  // 继电器外部 分闸状态--led
#define OUT_POS_RELAY_ALARM       (5)  // 继电器外部 告警状态--led

#define LED_ON                    (OUT_SET)   // 外部LED
#define LED_OFF                   (OUT_RESET) // 外部LED

#define LED_CLOSE                 (0x10)
#define LED_CUT                   (0x08)
#define LED_OVER_C                (0x04)
#define LED_RECLOSE               (0x02)
#define LED_ERROR                 (0x01)

#define LED_RUN_POS               (0)           // 运行
#define LED_FA_POS                (LED_ERROR)   // error
#define LED_COM_POS               (0)           // 101通信
#define LED_NET_POS               (0)           // 104通信
#define LED_CUT_POS               (LED_OVER_C)  // 速断
#define LED_ZERO_POS              (LED_OVER_C)  // 零序
#define LED_OVI_POS               (LED_OVER_C)  // 过流
#define LED_RECOL_POS             (LED_RECLOSE) // 重合闸


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
//void EX_Led_On(int led);
//void EX_Led_Off(int led);


#endif /* END NORMAL_SWC */


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
//extern void Led_On(unsigned int led);
//extern void Led_Off(unsigned int led);
unsigned int rt_get_utc(void);
void rt_get_time(unsigned char *time);
void rt_set_time(unsigned char *time);


#endif /* END _GPIO_DEV_H_ */


/* END OF FILE ---------------------------------------------------------------*/

