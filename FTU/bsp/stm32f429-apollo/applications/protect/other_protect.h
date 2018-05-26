/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      other_protect.h
  * @brief:     The driver of other_protect.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017.11.28
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */
	
#ifndef __OTHER_PROTECT_H
#define __OTHER_PROTECT_H

/* INCLUDES ------------------------------------------------------------------*/	
#include "rtthread.h"
#include "common_data.h"
#include "stm32f4xx_hal.h"

/* DEFINE --------------------------------------------------------------------*/
#define DB_COUNTER_EN                 0x80000000  // 计数器使能
#define DB_COUNTER                    0x7fffffff  // 计数器


#define CHARGE_TIME           5000
#define MEMORY_TIME           5000

#define MONI_FRAM             0x01
#define MONI_AD7616           0x02
#define MONI_DO               0x04

/* ENUM ----------------------------------------------------------------------*/	
enum
{
    DB_DEV0,				// 设备数量
    DB_DEV_MAX_NUM
};

/* 遥测越限结构体 */
struct OverLimit
{
    float *funSwitch;
    float *telemetry;
    float *uplimit;
    float *uplimitFactor;
    float *downlimit;
    float *downlimitFactor;
    float *delay;
    rt_uint16_t soeAddrUp;
    rt_uint16_t soeAddrDown;
    rt_uint8_t *stateUp;
    rt_uint8_t *stateDown;
    rt_uint32_t *counterUp;
    rt_uint32_t *counterDown;
    rt_uint32_t *counterUpReverse;
    rt_uint32_t *counterDownReverse;    
};

/* 遥测超限 */
struct OverVoltage
{
    float *funSwitch;
    float *telemetry;
    float *value;
    float *factor;
    float *delay;
    rt_uint16_t soeAddr;
    rt_uint8_t *state;
    rt_uint32_t *counter;
    rt_uint32_t *counterReverse;	
};

/* 过负荷/重过载 */
struct OverLoad
{
    float *funSwitch;
    float *Ia;
    float *Ib;
    float *Ic;
    float *value;
    float *factor;
    float *delay;
    rt_uint16_t soeAddr;
    rt_uint8_t *state;
    rt_uint32_t *counter;
    rt_uint32_t *counterReverse;	
};

struct MemoryCounterStr
{
    rt_uint32_t *counterSave; 
    rt_uint32_t *counterCharge;    
};

struct CommunicatLockStr
{
    rt_uint32_t *flag[COM_MAX];
    rt_uint32_t *counter[COM_MAX];
    float *timeout;
};

/* 系统自检 */
struct MonitorStr
{
    rt_uint32_t ad7616_Counter;
	rt_uint32_t dcDeviceCounter;
};

/* 直流量采集结构体 */
struct DCData
{
    uint8_t buf[15];
    uint8_t sta;

    float dcVol;
    float temp;
    float dcVolLast;
    float tempLast;
    float testCount;
};

extern struct DCData DcData;
extern struct MonitorStr Monitor;

/* PUBLIC FUNCTION  ----------------------------------------------------------*/
void DC_GatherInit(void);
void DC_GatherTask(uint8_t clock);
void other_protect_init(void);
void other_protect_clock(void);
#endif 

/* END OF FILE ---------------------------------------------------------------*/
