/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      drv_do.h
  * @brief:     The driver of do.
  * @version:   V03.001
  * @author:    Mr.Sun
  * @date:      2017.09.05
  * @updata  : [2018-01-16][Sunxr][modify the driver]
  */
#ifndef __DRV_DO_H__
#define __DRV_DO_H__

/* INCLUDES FILSE ------------------------------------------------------------*/
#include "rtthread.h"


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define DO_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define DO_PRINTF(...)
#endif /* DEBUG */

#define DO_SET             GPIO_PIN_RESET 
#define DO_CLR             GPIO_PIN_SET

#define WRITEDO1_ON()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET) 
#define WRITEDO1_OFF()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET)

#define WRITEDO2_ON()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET) 
#define WRITEDO2_OFF()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET) 

#define WRITEDO3_ON()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET) 
#define WRITEDO3_OFF()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_SET) 

#define WRITEDO4_ON()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET) 
#define WRITEDO4_OFF()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET) 

#define WRITEDO5_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)   
#define WRITEDO5_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)     

#define WRITEDO6_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)   
#define WRITEDO6_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

#define WRITEDO7_ON()      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_10, GPIO_PIN_RESET)   
#define WRITEDO7_OFF()     HAL_GPIO_WritePin(GPIOI, GPIO_PIN_10, GPIO_PIN_SET)

#define WRITEDO8_ON()      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_RESET) 
#define WRITEDO8_OFF()     HAL_GPIO_WritePin(GPIOI, GPIO_PIN_11, GPIO_PIN_SET)

#define FZ_SET()           WRITEDO2_ON()  // 断路器分闸
#define FZ_CLR()           WRITEDO2_OFF()

#define HZ_SET()           WRITEDO1_ON()  // 断路器合闸
#define HZ_CLR()           WRITEDO1_OFF()

#define CN_SET()           WRITEDO3_ON()  // 储能
#define CN_CLR()           WRITEDO3_OFF() // 储能

#define YZ_SET()           WRITEDO4_ON()  // 预置
#define YZ_CLR()           WRITEDO4_OFF() // 预置

#define DC_TC_SET()        WRITEDO6_ON()  // 电池退出
#define DC_TC_CLR()        WRITEDO6_OFF() 

#define HH_TC_SET()        WRITEDO7_ON()  // 活化退出
#define HH_TC_CLR()        WRITEDO7_OFF() 

#define HH_QD_SET()        WRITEDO8_ON()  // 活化启动
#define HH_QD_CLR()        WRITEDO8_OFF() 

#define BF_OUT_SET()       WRITEDO5_ON()  // 备份输出
#define BF_OUT_CLR()       WRITEDO5_OFF() //

#define OPENBUSY            DoFlag.openBusy   // 分闸状态
#define OPENREADY           DoFlag.openReady  // 分闸准备就绪
#define CLOSEBUSY           DoFlag.closeBusy  // 合闸状态
#define CLOSEREADY          DoFlag.closeReady // 合闸准备就绪
#define OPENING             DoFlag.opening    // 正在分闸
#define CLOSING             DoFlag.closing    // 正在合闸
#define ENERGY_STORAGEING   DoFlag.energyStorageing  // 正在储能
#define OPEN_CMD_FLG        DoFlag.openCmdflg // 分闸命令标志位
#define CLOSE_CMD_FLG       DoFlag.openCmdflg // 合闸命令标志位
#define RELAY_OPEN          DoFlag.relayOpen  // 继电器分闸标志
#define RELAY_CLOSE         DoFlag.relayClose // 继电器合闸标志
         

/* STRUCTS -------------------------------------------------------------------*/
typedef struct TagDoFlag
{	
	unsigned opening:1;
	unsigned closing:1;
	unsigned openReady:1;
	unsigned openBusy:1;
	unsigned closeBusy:1;
	unsigned closeReady:1;	
	unsigned relayOpen:1;
	unsigned relayClose:1;
	unsigned energyStorageing:1;	
}DoFlagBit;

typedef struct TagDoSrtuct
{
    rt_uint16_t actSource; // 动作来源 遥控/本地/逻辑
    float openingDelay;
    float closingDelay;	
}DoSrtuct;
	
/* PUBLIC FUNCTION  ----------------------------------------------------------*/
int rt_hw_do_init(void);
rt_uint8_t rt_hw_do_operate(rt_uint16_t addr, rt_uint8_t operate_type);
void rt_hw_battery_activation(rt_uint8_t clock);


#endif /* __DRV_DO_H__ */


/* END OF FILE ---------------------------------------------------------------*/
