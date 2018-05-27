/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      other_protect.c
  * @brief:     The driver of other_protect.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-11-28
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <math.h>
//#include <arm_math.h>
//#include "drv_usart.h"
//#include "drv_fm25vxx.h"
#include "string.h"
#include "drv_do.h"
#include "drv_timer.h"
#include "drv_sd2405.h"
#include "common_data.h"
#include "other_protect.h"
#include "drv_fm25vxx.h"

/* PRIVATE VARIABLES ---------------------------------------------------------*/
static rt_device_t device_fram = RT_NULL; 
static List s_ListTimers[DB_DEV_MAX_NUM];//定时器链表链表//0x80000000使能位

static struct OverLimit OverLimitUab;
static struct OverLimit OverLimitUbc;
static struct OverLimit OverLimitUac;
static struct OverLimit OverLimitUAB;
static struct OverLimit OverLimitUBC;
static struct OverLimit OverLimitIa;
static struct OverLimit OverLimitIb;
static struct OverLimit OverLimitIc;
static struct OverLimit OverLimitU0;
static struct OverLimit OverLimitI0;
static struct OverLimit OverLimitDC_U;
static struct OverLimit OverLimitDC_I;

static struct OverLoad OverLoad;           // 过负荷
static struct OverLoad HeavyOverLoad;      // 重过载
static struct OverVoltage OverVoltageUab;  // 过电压
static struct OverVoltage OverVoltageUBC;
static struct OverVoltage DownVoltageUab;  // 低电压
static struct OverVoltage DownVoltageUBC;

static struct OverVoltage OverFrequency;  // 过频
static struct OverVoltage DownFrequency;  // 低频

/* 各种记录存储计数器 */
static struct MemoryCounterStr MemoryCounter;

static struct CommunicatLockStr CommunicatLock; // 通讯互锁

/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @Description: 系统运行监测任务
  * @param:  clock-调用周期
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void SysMonitorTask(void)
{	
	rt_uint8_t a_all_sta = 0;
    rt_uint8_t s_rlt = 0;
	
    rt_device_control(device_fram, FM_MONITOR, &s_rlt); 
	 	 
    if (s_rlt == 1) // FRAM监测  定值区
    {
        a_all_sta |= MONI_FRAM;
    }
    else
    {
        a_all_sta &= ~MONI_FRAM;
    }

//	if (g_TelesignalDB.Str.devicePowerDown == ON)
//    {
//        a_all_sta |= MONI_DO;
//    }
//    else
//    {
//        a_all_sta &= ~MONI_DO;
//    }
		
	if (a_all_sta == 0)
	{
	    DBWriteSOE(ADDR_SELF_CHECK_ABNOMAL, OFF);
	}
	else
	{
	    DBWriteSOE(ADDR_SELF_CHECK_ABNOMAL, ON);
	}
}

/**
  * @Description: 申请系统计数器.
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void ApplyForCounter(rt_uint8_t pdrv, rt_uint32_t **dev)
{
    rt_uint32_t *s_timer;
        
    s_timer = rt_malloc(sizeof(rt_uint32_t));
    
    *dev = s_timer;
    *s_timer = 0;
    
    list_ins_next(&s_ListTimers[pdrv],NULL,s_timer); 
}

/**
  * @Description: 遥测越限判断函数
  * @param:  overlimit-越限结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void TelemetryOverLimit(struct OverLimit *overlimit)
{
    if (!g_pFixedValue[OVERLIMIT_SWITCH] || !*overlimit->funSwitch)
    {
		DBWriteSOE(overlimit->soeAddrUp, OFF);
		DBWriteSOE(overlimit->soeAddrDown, OFF);
        return;
    }

    /* 上限 */
    if (*overlimit->telemetry > *overlimit->uplimit)
    {
        if (*overlimit->stateUp == OFF)
        {
            if(!(*overlimit->counterUp & DB_COUNTER_EN))
            {
                *overlimit->counterUp = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterUp & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterUp = 0;
                DBWriteSOE(overlimit->soeAddrUp, ON);
            }
        }
    }
    else
    {
        *overlimit->counterUp = 0;
        if (*overlimit->stateUp == ON && *overlimit->telemetry <= (*overlimit->uplimit * (*overlimit->uplimitFactor)))
        {
            if(!(*overlimit->counterUpReverse & DB_COUNTER_EN))
            {
                *overlimit->counterUpReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterUpReverse & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterUpReverse = 0;
                DBWriteSOE(overlimit->soeAddrUp, OFF);
            }			
        }
    }

    /* 下限 */
    if (*overlimit->telemetry < *overlimit->downlimit)
    {
		*overlimit->counterDownReverse = 0;
        if (*overlimit->stateDown == OFF)
        {
            if(!(*overlimit->counterDown & DB_COUNTER_EN))
            {
                *overlimit->counterDown = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterDown & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterDown = 0;
                DBWriteSOE(overlimit->soeAddrDown, ON);
            }
        }
    }
    else
    {
        *overlimit->counterDown = 0;
        if (*overlimit->stateDown == ON && *overlimit->telemetry >= (*overlimit->downlimit * (*overlimit->downlimitFactor)))
        {
            if(!(*overlimit->counterDownReverse & DB_COUNTER_EN))
            {
                *overlimit->counterDownReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*overlimit->counterDownReverse & DB_COUNTER) > (rt_uint32_t)((*overlimit->delay)*1000))
            {
                *overlimit->counterDownReverse = 0;
                DBWriteSOE(overlimit->soeAddrDown, OFF);
            }			
        }
    }
}

/**
  * @Description: 遥测超限监测（过压）
  * @param:  over-遥测超限结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void OverVoltageCheck(struct OverVoltage *over)
{
    if (!(*over->funSwitch))
    {
		DBWriteSOE(over->soeAddr, OFF);

        return;
    }

    if (*over->telemetry > *over->value)
    {
		*over->counterReverse = 0;
        if (*over->state == OFF)
        {
            if (!(*over->counter & DB_COUNTER_EN))
            {
                *over->counter = DB_COUNTER_EN;//启动时钟
            }

            if ((*over->counter & DB_COUNTER) > (rt_uint32_t)((*over->delay)*1000))
            {
                *over->counter = 0;
                DBWriteSOE(over->soeAddr, ON);

				if (*over->funSwitch == 1 && g_TelesignalDB[ADDR_FUNCTION_HARDSTRAP] == ON)
				{
				    rt_hw_do_operate(DO_OPEN, 0);
				}
            }
        }
    }
    else
    {
        *over->counter = 0;
        if (*over->state == ON && *over->telemetry < (*over->value * (*over->factor)))
        {
            if (!(*over->counterReverse & DB_COUNTER_EN))
            {
                *over->counterReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*over->counterReverse & DB_COUNTER) > (rt_uint32_t)((*over->delay)*1000))
            {
                *over->counterReverse = 0;
                DBWriteSOE(over->soeAddr, OFF);
            }			
        }
    }
}

/**
  * @brief:  遥测超限监测（低压）
  * @param:  over-遥测超限结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void DownVoltageCheck(struct OverVoltage *down)
{
    if (!(*down->funSwitch))
    {
		DBWriteSOE(down->soeAddr, OFF);
        return;
    }

    if (*down->telemetry < *down->value)
    {
		*down->counterReverse = 0;
        if (*down->state == OFF)
        {
            if (!(*down->counter & DB_COUNTER_EN))
            {
                *down->counter = DB_COUNTER_EN;//启动时钟
            }

            if ((*down->counter & DB_COUNTER) > (rt_uint32_t)((*down->delay)*1000))
            {
                *down->counter = 0;
                DBWriteSOE(down->soeAddr, ON);
				
				if (*down->funSwitch == 1 && g_TelesignalDB[ADDR_FUNCTION_HARDSTRAP] == ON)
				{
				    rt_hw_do_operate(DO_OPEN, 0);
				}
            }
        }
    }
    else
    {
        *down->counter = 0;
        if (*down->state == ON && *down->telemetry > (*down->value * (*down->factor)))
        {
            if (!(*down->counterReverse & DB_COUNTER_EN))
            {
                *down->counterReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*down->counterReverse & DB_COUNTER) > (rt_uint32_t)((*down->delay)*1000))
            {
                *down->counterReverse = 0;
                DBWriteSOE(down->soeAddr, OFF);
            }			
        }
    }
}

/**
  * @Description: 过负荷
  * @param:  over-过负荷结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void OverLoadCheck(struct OverLoad *over)
{
    if (!(*over->funSwitch))
    {
		DBWriteSOE(over->soeAddr, OFF);
        return;
    }

    if ((*over->Ia > *over->value || *over->Ib > *over->value || *over->Ic > *over->value))
    {
		*over->counterReverse = 0;
        if (*over->state == OFF)
        {
            if (!(*over->counter & DB_COUNTER_EN))
            {
                *over->counter = DB_COUNTER_EN;//启动时钟
            }

            if ((*over->counter & DB_COUNTER) > (rt_uint32_t)((*over->delay)*1000))
            {
                *over->counter = 0;
                DBWriteSOE(over->soeAddr, ON);
            }
        }
    }
    else
    {
        *over->counter = 0;
        if (*over->state == ON && *over->Ia <= *over->value * (*over->factor) && *over->Ib <= *over->value * (*over->factor) && *over->Ic <= *over->value * (*over->factor))
        {
            if (!(*over->counterReverse & DB_COUNTER_EN))
            {
                *over->counterReverse = DB_COUNTER_EN;//启动时钟
            }

            if ((*over->counterReverse & DB_COUNTER) > (rt_uint32_t)((*over->delay)*1000))
            {
                *over->counterReverse = 0;
                DBWriteSOE(over->soeAddr, OFF);
            }			
        }
    }
}

/**
  * @Description: 遥测异常监测
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void TelemetryAbnormalCheck(void)
{
    /* 遥测越限 */
    TelemetryOverLimit(&OverLimitUab);
    TelemetryOverLimit(&OverLimitUbc);
    TelemetryOverLimit(&OverLimitUAB);	
    TelemetryOverLimit(&OverLimitUBC);
    TelemetryOverLimit(&OverLimitUac);
    TelemetryOverLimit(&OverLimitU0);
    TelemetryOverLimit(&OverLimitI0);
    TelemetryOverLimit(&OverLimitIa);
    TelemetryOverLimit(&OverLimitIb);
    TelemetryOverLimit(&OverLimitIc);
    TelemetryOverLimit(&OverLimitDC_U);
    TelemetryOverLimit(&OverLimitDC_I);

    /* 重过载 */
    OverLoadCheck(&HeavyOverLoad);
    /* 过负荷 */
    OverLoadCheck(&OverLoad);
    /* 过电压 */
    OverVoltageCheck(&OverVoltageUab);
    OverVoltageCheck(&OverVoltageUBC);
    OverVoltageCheck(&OverFrequency);
	
	if (g_TelemetryDB[ADDR_Uab] >= 10.0f)
	{
	    DownVoltageCheck(&DownVoltageUab);
	}

	if (g_TelemetryDB[ADDR_UCB] >= 10.0f)
	{
	    DownVoltageCheck(&DownVoltageUBC);
	}
	
	if (g_TelemetryDB[ADDR_F] >= 10.0f)
	{
	    DownVoltageCheck(&DownFrequency);
	}
    	
    /* 电池欠压 */
    if (g_pFixedValue[BATTERY_LOWVOLTAGE_ALARM_SWITCH])
    {
        if (g_TelemetryDB[ADDR_DC1] < g_pFixedValue[BATTERY_LOWVOLTAGE_VALUE] && g_TelesignalDB[ADDR_BATTERY_UNDERVOLTAGE_ALARM] == OFF)
        {
            DBWriteSOE(ADDR_BATTERY_UNDERVOLTAGE_ALARM, ON);
        }
        else if (g_TelemetryDB[ADDR_DC1] >= g_pFixedValue[BATTERY_LOWVOLTAGE_VALUE] * g_pFixedValue[BATTERY_LOWVOLTAGE_FACTOR])
        {
            if (g_TelesignalDB[ADDR_BATTERY_UNDERVOLTAGE_ALARM] == ON)
            {
                DBWriteSOE(ADDR_BATTERY_UNDERVOLTAGE_ALARM, OFF);
            }
        }
    }
    /* 永磁机构电容欠压 */
    if (g_Parameter[OPERATING_MECHANISM] == MAGNET) // 永磁机构电容欠压
    {
        /* 合闸状态 */
        if (g_TelesignalDB[ADDR_CLOSE] == ON &&  g_TelesignalDB[ADDR_OPEN] == OFF && g_TelemetryDB[ADDR_DC2] < g_Parameter[CAP_UNDERVOLTAGE_CLOSING])
        {
            DBWriteSOE(ADDR_OPERATING_MECHANISM, OFF);
        }
        else if (g_TelemetryDB[ADDR_DC2] >= g_Parameter[CAP_UNDERVOLTAGE_CLOSING] * g_Parameter[CAP_UNDERVOLTAGE_FACTOR])
        {
            DBWriteSOE(ADDR_OPERATING_MECHANISM, ON);
        }

        /* 分闸状态 */
        if (g_TelesignalDB[ADDR_CLOSE] == OFF &&  g_TelesignalDB[ADDR_OPEN] == ON && g_TelemetryDB[ADDR_DC2] < g_Parameter[CAP_UNDERVOLTAGE_OPENING])
        {
            DBWriteSOE(ADDR_OPERATING_MECHANISM, OFF);
        }
        else if (g_TelemetryDB[ADDR_DC2] >= g_Parameter[CAP_UNDERVOLTAGE_OPENING] * g_Parameter[CAP_UNDERVOLTAGE_FACTOR])
        {
            DBWriteSOE(ADDR_OPERATING_MECHANISM, ON);
        }
    }
}

/**
  * @Description: 通讯互锁操作
  * @param:  clock-调用周期
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void ComunicatLock(void)
{
    rt_uint8_t i;
    
    for (i = 0; i < COM_MAX; i++)
    {
        if (*(CommunicatLock.flag[i])&COMMUNICATLOCKCLRCON)//计时清零
        {
            *(CommunicatLock.counter[i]) = 0; 
            *(CommunicatLock.flag[i]) &= ~COMMUNICATLOCKCLRCON;
        } 
        
        if (*(CommunicatLock.flag[i])&COMMUNICATLOCKUSERCON)//计时启动
        {
            if(!(*(CommunicatLock.counter[i])&DB_COUNTER_EN))
            {
                *(CommunicatLock.counter[i]) = DB_COUNTER_EN;   
            }  
            if((*(CommunicatLock.counter[i])&DB_COUNTER)>((rt_uint32_t)*(CommunicatLock.timeout) > 5000 ? (rt_uint32_t)*(CommunicatLock.timeout) : 5000))//超时复位
            {
                    *(CommunicatLock.flag[i]) &= COMMUNICATLOCKFUC;
                    *(CommunicatLock.counter[i]) = 0; 
            }            
        }
        else
        {
            *(CommunicatLock.counter[i]) = 0; 
        }
    }
		
    if ((*(CommunicatLock.flag[COM_FILE])&COMMUNICATLOCKRESET)&&(!(*(CommunicatLock.flag[COM_FILE])&COMMUNICATLOCKUSERSTA)))//复位
    {
        rt_multi_common_data_powerdown_storage(); 
        NVIC_SystemReset(); //软复位
    }
}

/**
  * @biief : other_protect init
  * @param : none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void other_protect_init(void)
{
    rt_uint8_t pdrv = 0;
	
    list_init(&s_ListTimers[pdrv]);
    
    ApplyForCounter(pdrv, &OverLimitUab.counterUp);
    ApplyForCounter(pdrv, &OverLimitUab.counterDown);
    ApplyForCounter(pdrv, &OverLimitUab.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUab.counterDownReverse);	
    OverLimitUab.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitUab.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_Uab_UP];
    OverLimitUab.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_Uab_DOWN];
    OverLimitUab.soeAddrUp = ADDR_OVERLIMIT_Uab_UP;
    OverLimitUab.soeAddrDown = ADDR_OVERLIMIT_Uab_DOWN;
    OverLimitUab.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U];
    OverLimitUab.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_U];
    OverLimitUab.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_Uab];
    OverLimitUab.telemetry = &g_TelemetryDB[ADDR_Uab];
    OverLimitUab.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitUab.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

	ApplyForCounter(pdrv, &OverLimitUbc.counterUp);
    ApplyForCounter(pdrv, &OverLimitUbc.counterDown);
    ApplyForCounter(pdrv, &OverLimitUbc.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUbc.counterDownReverse);	
    OverLimitUbc.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitUbc.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_Ubc_UP];
    OverLimitUbc.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_Ubc_DOWN];
    OverLimitUbc.soeAddrUp = ADDR_OVERLIMIT_Ubc_UP;
    OverLimitUbc.soeAddrDown = ADDR_OVERLIMIT_Ubc_DOWN;
    OverLimitUbc.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U];
    OverLimitUbc.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_U];
    OverLimitUbc.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_Ubc];
    OverLimitUbc.telemetry = &g_TelemetryDB[ADDR_Ubc];
    OverLimitUbc.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitUbc.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitUAB.counterUp);
    ApplyForCounter(pdrv, &OverLimitUAB.counterDown);
    ApplyForCounter(pdrv, &OverLimitUAB.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUAB.counterDownReverse);	
    OverLimitUAB.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitUAB.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_UAB_UP];
    OverLimitUAB.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_UAB_DOWN];
    OverLimitUAB.soeAddrUp = ADDR_OVERLIMIT_UAB_UP;
    OverLimitUAB.soeAddrDown = ADDR_OVERLIMIT_UAB_DOWN;
    OverLimitUAB.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U];
    OverLimitUAB.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_U];
    OverLimitUAB.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_UAB];
    OverLimitUAB.telemetry = &g_TelemetryDB[ADDR_UAB];
    OverLimitUAB.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitUAB.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];
	
    ApplyForCounter(pdrv, &OverLimitUBC.counterUp);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDown);
    ApplyForCounter(pdrv, &OverLimitUBC.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDownReverse);	
    OverLimitUBC.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitUBC.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_UBC_UP];
    OverLimitUBC.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_UBC_DOWN];
    OverLimitUBC.soeAddrUp = ADDR_OVERLIMIT_UBC_UP;
    OverLimitUBC.soeAddrDown = ADDR_OVERLIMIT_UBC_DOWN;
    OverLimitUBC.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U];
    OverLimitUBC.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_U];
    OverLimitUBC.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_UBC];
    OverLimitUBC.telemetry = &g_TelemetryDB[ADDR_UCB];
    OverLimitUBC.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitUBC.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitUac.counterUp);
    ApplyForCounter(pdrv, &OverLimitUac.counterDown);
    ApplyForCounter(pdrv, &OverLimitUac.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUac.counterDownReverse);	
    OverLimitUac.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitUac.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_Uca_UP];
    OverLimitUac.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_Uca_DOWN];
    OverLimitUac.soeAddrUp = ADDR_OVERLIMIT_Uca_UP;
    OverLimitUac.soeAddrDown = ADDR_OVERLIMIT_Uca_DOWN;
    OverLimitUac.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U];
    OverLimitUac.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_U];
    OverLimitUac.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_Uca];
    OverLimitUac.telemetry = &g_TelemetryDB[ADDR_Uac];
    OverLimitUac.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitUac.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIa.counterUp);
    ApplyForCounter(pdrv, &OverLimitIa.counterDown);
    ApplyForCounter(pdrv, &OverLimitIa.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIa.counterDownReverse);		
    OverLimitIa.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitIa.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_IA_UP];
    OverLimitIa.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_IA_DOWN];
    OverLimitIa.soeAddrUp = ADDR_OVERLIMIT_IA_UP;
    OverLimitIa.soeAddrDown = ADDR_OVERLIMIT_IA_DOWN;
    OverLimitIa.uplimit = &g_pFixedValue[UPLIMIT_CURRENT_I];
    OverLimitIa.downlimit = &g_pFixedValue[DOWNLIMIT_CURRENT_I];
    OverLimitIa.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_IA];
    OverLimitIa.telemetry = &g_TelemetryDB[ADDR_IA];
    OverLimitIa.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitIa.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIb.counterUp);
    ApplyForCounter(pdrv, &OverLimitIb.counterDown);
    ApplyForCounter(pdrv, &OverLimitIb.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIb.counterDownReverse);	
    OverLimitIb.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitIb.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_IB_UP];
    OverLimitIb.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_IB_DOWN];
    OverLimitIb.soeAddrUp = ADDR_OVERLIMIT_IB_UP;
    OverLimitIb.soeAddrDown = ADDR_OVERLIMIT_IB_DOWN;
    OverLimitIb.uplimit = &g_pFixedValue[UPLIMIT_CURRENT_I];
    OverLimitIb.downlimit = &g_pFixedValue[DOWNLIMIT_CURRENT_I];
    OverLimitIb.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_IB];
    OverLimitIb.telemetry = &g_TelemetryDB[ADDR_IB];
    OverLimitIb.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitIb.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitIc.counterUp);
    ApplyForCounter(pdrv, &OverLimitIc.counterDown);
    ApplyForCounter(pdrv, &OverLimitIc.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIc.counterDownReverse);	
    OverLimitIc.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitIc.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_IC_UP];
    OverLimitIc.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_IC_DOWN];
    OverLimitIc.soeAddrUp = ADDR_OVERLIMIT_IC_UP;
    OverLimitIc.soeAddrDown = ADDR_OVERLIMIT_IC_DOWN;
    OverLimitIc.uplimit = &g_pFixedValue[UPLIMIT_CURRENT_I];
    OverLimitIc.downlimit = &g_pFixedValue[DOWNLIMIT_CURRENT_I];
    OverLimitIc.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_IC];
    OverLimitIc.telemetry = &g_TelemetryDB[ADDR_IC];
    OverLimitIc.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitIc.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitU0.counterUp);
    ApplyForCounter(pdrv, &OverLimitU0.counterDown);
    ApplyForCounter(pdrv, &OverLimitU0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitU0.counterDownReverse);	
    OverLimitU0.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitU0.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_U0_UP];
    OverLimitU0.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_U0_DOWN];
    OverLimitU0.soeAddrUp = ADDR_OVERLIMIT_U0_UP;
    OverLimitU0.soeAddrDown = ADDR_OVERLIMIT_U0_DOWN;
    OverLimitU0.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U0];
    OverLimitU0.downlimit = &g_pFixedValue[UPLIMIT_VOLTAGE_U0];
    OverLimitU0.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_U0];
    OverLimitU0.telemetry = &g_TelemetryDB[ADDR_U0];
    OverLimitU0.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitU0.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitI0.counterUp);
    ApplyForCounter(pdrv, &OverLimitI0.counterDown);
    ApplyForCounter(pdrv, &OverLimitI0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitI0.counterDownReverse);		
    OverLimitI0.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitI0.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_I0_UP];
    OverLimitI0.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_I0_DOWN];
    OverLimitI0.soeAddrUp = ADDR_OVERLIMIT_I0_UP;
    OverLimitI0.soeAddrDown = ADDR_OVERLIMIT_I0_DOWN;
    OverLimitI0.uplimit = &g_pFixedValue[UPLIMIT_VOLTAGE_I0];
    OverLimitI0.downlimit = &g_pFixedValue[DOWNLIMIT_VOLTAGE_I0];
    OverLimitI0.funSwitch = &g_pFixedValue[OVERLIMIT_ALARM_SWITCH_I0];
    OverLimitI0.telemetry = &g_TelemetryDB[ADDR_I0];
    OverLimitI0.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitI0.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitDC_U.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDownReverse);			
    OverLimitDC_U.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitDC_U.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_DC_U_UP];
    OverLimitDC_U.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_DC_U_DOWN];
    OverLimitDC_U.soeAddrUp = ADDR_OVERLIMIT_DC_U_UP;
    OverLimitDC_U.soeAddrDown = ADDR_OVERLIMIT_DC_U_DOWN;
    OverLimitDC_U.uplimit = &g_pFixedValue[UPLIMIT_DC_VOLTAGE_U];
    OverLimitDC_U.downlimit = &g_pFixedValue[DOWNLIMIT_DC_VOLTAGE_U];
    OverLimitDC_U.funSwitch = &g_pFixedValue[OVERLIMIT_ALARMSWITCH_DC_U];
    OverLimitDC_U.telemetry = &g_TelemetryDB[ADDR_DC1];
    OverLimitDC_U.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitDC_U.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &OverLimitDC_I.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDownReverse);	
    OverLimitDC_I.delay = &g_pFixedValue[OVERLIMIT_TIME];
    OverLimitDC_I.stateUp = &g_TelesignalDB[ADDR_OVERLIMIT_DC_U_UP];
    OverLimitDC_I.stateDown = &g_TelesignalDB[ADDR_OVERLIMIT_DC_U_DOWN];
    OverLimitDC_I.soeAddrUp = ADDR_OVERLIMIT_DC_U_UP;
    OverLimitDC_I.soeAddrDown = ADDR_OVERLIMIT_DC_U_DOWN;
    OverLimitDC_I.uplimit = &g_pFixedValue[UPLIMIT_DC_CURRENT_I];
    OverLimitDC_I.downlimit = &g_pFixedValue[DOWNLIMIT_DC_CURRENT_I];
    OverLimitDC_I.funSwitch = &g_pFixedValue[OVERLIMIT_ALARMSWITCH_DC_I];
    OverLimitDC_I.telemetry = &g_TelemetryDB[ADDR_DC2];
    OverLimitDC_I.uplimitFactor = &g_pFixedValue[UPLIMIT_FACTOR];
    OverLimitDC_I.downlimitFactor = &g_pFixedValue[DOWNLIMIT_FACTOR];

    ApplyForCounter(pdrv, &HeavyOverLoad.counter);
    ApplyForCounter(pdrv, &HeavyOverLoad.counterReverse);	
    HeavyOverLoad.delay = &g_pFixedValue[HEAVY_OVERLOAD_TIME];
    HeavyOverLoad.state = &g_TelesignalDB[ADDR_HAVEY_OVERLOAD_EVENT];
    HeavyOverLoad.soeAddr = ADDR_HAVEY_OVERLOAD_EVENT;
    HeavyOverLoad.value = &g_pFixedValue[HEAVY_OVERLOAD_VALUE];
    HeavyOverLoad.funSwitch = &g_pFixedValue[HEAVY_OVERLOAD_SWITCH];
    HeavyOverLoad.Ia = &g_TelemetryDB[ADDR_IA];
    HeavyOverLoad.Ib = &g_TelemetryDB[ADDR_IB];
    HeavyOverLoad.Ic = &g_TelemetryDB[ADDR_IC];
    HeavyOverLoad.factor = &g_pFixedValue[HEAVY_OVERLOAD_FACTOR];

    ApplyForCounter(pdrv, &OverLoad.counter);
    ApplyForCounter(pdrv, &OverLoad.counterReverse);	
    OverLoad.delay = &g_pFixedValue[OVERLOAD_TIME];
    OverLoad.state = &g_TelesignalDB[ADDR_OVERLOAD_EVENT];
    OverLoad.soeAddr = ADDR_OVERLOAD_EVENT;
    OverLoad.value = &g_pFixedValue[OVERLOAD_VALUE];
    OverLoad.funSwitch = &g_pFixedValue[OVERLOAD_SWITCH];
    OverLoad.Ia = &g_TelemetryDB[ADDR_IA];
    OverLoad.Ib = &g_TelemetryDB[ADDR_IB];
    OverLoad.Ic = &g_TelemetryDB[ADDR_IC];
    OverLoad.factor = &g_pFixedValue[OVERLOAD_FACTOR];

    ApplyForCounter(pdrv, &OverVoltageUab.counter);
    ApplyForCounter(pdrv, &OverVoltageUab.counterReverse);	
    OverVoltageUab.delay = &g_pFixedValue[OVERVOLTAGE_TIME];
    OverVoltageUab.state = &g_TelesignalDB[ADDR_OVER_VOLTAGE_PROTECTION];
    OverVoltageUab.soeAddr = ADDR_OVER_VOLTAGE_PROTECTION;
    OverVoltageUab.value = &g_pFixedValue[OVERVOLTAGE_VALUE];
    OverVoltageUab.funSwitch = &g_pFixedValue[OVERVOLTAGE_SWITCH];
    OverVoltageUab.telemetry = &g_TelemetryDB[ADDR_Uab];
    OverVoltageUab.factor = &g_pFixedValue[OVERVOLTAGE_FACTOR];

    ApplyForCounter(pdrv, &DownVoltageUab.counter);
    ApplyForCounter(pdrv, &DownVoltageUab.counterReverse);	
    DownVoltageUab.delay = &g_pFixedValue[DOWNVOLTAGE_VALUE];
    DownVoltageUab.state = &g_TelesignalDB[ADDR_DOWN_VOLTAGE_PROTECTION];
    DownVoltageUab.soeAddr = ADDR_DOWN_VOLTAGE_PROTECTION;
    DownVoltageUab.value = &g_pFixedValue[DOWNVOLTAGE_VALUE];
    DownVoltageUab.funSwitch = &g_pFixedValue[DOWNVOLTAGE_SWITCH];
    DownVoltageUab.telemetry = &g_TelemetryDB[ADDR_Uab];
    DownVoltageUab.factor = &g_pFixedValue[DOWNVOLTAGE_FACTOR];
	
    ApplyForCounter(pdrv, &OverVoltageUBC.counter);
    ApplyForCounter(pdrv, &OverVoltageUBC.counterReverse);
    OverVoltageUBC.delay = &g_pFixedValue[OVERVOLTAGE_TIME];
    OverVoltageUBC.state = &g_TelesignalDB[ADDR_OVER_VOLTAGE_PROTECTION];
    OverVoltageUBC.soeAddr = ADDR_OVER_VOLTAGE_PROTECTION;
    OverVoltageUBC.value = &g_pFixedValue[OVERVOLTAGE_VALUE];
    OverVoltageUBC.funSwitch = &g_pFixedValue[OVERVOLTAGE_SWITCH];
    OverVoltageUBC.telemetry = &g_TelemetryDB[ADDR_UCB];
    OverVoltageUBC.factor = &g_pFixedValue[OVERVOLTAGE_FACTOR];

    ApplyForCounter(pdrv, &DownVoltageUBC.counter);
    ApplyForCounter(pdrv, &DownVoltageUBC.counterReverse);
    DownVoltageUBC.delay = &g_pFixedValue[OVERVOLTAGE_TIME];
    DownVoltageUBC.state = &g_TelesignalDB[ADDR_DOWN_VOLTAGE_PROTECTION];
    DownVoltageUBC.soeAddr = ADDR_DOWN_VOLTAGE_PROTECTION;
    DownVoltageUBC.value = &g_pFixedValue[OVERVOLTAGE_VALUE];
    DownVoltageUBC.funSwitch = &g_pFixedValue[OVERVOLTAGE_SWITCH];
    DownVoltageUBC.telemetry = &g_TelemetryDB[ADDR_UCB];
    DownVoltageUBC.factor = &g_pFixedValue[DOWNVOLTAGE_FACTOR];

    ApplyForCounter(pdrv, &OverFrequency.counter);
    ApplyForCounter(pdrv, &OverFrequency.counterReverse);
    OverFrequency.delay = &g_pFixedValue[OVERFREQUENCY_VALUE];
    OverFrequency.state = &g_TelesignalDB[ADDR_OVER_FREQUEBNCY_PROTECTION];
    OverFrequency.soeAddr = ADDR_OVER_FREQUEBNCY_PROTECTION;
    OverFrequency.value = &g_pFixedValue[OVERFREQUENCY_VALUE];
    OverFrequency.funSwitch = &g_pFixedValue[OVERFREQUENCY_SWITCH];
    OverFrequency.telemetry = &g_FreGather[FRE_Uab].freValueProtect;
    OverFrequency.factor = &g_pFixedValue[OVERFREQUENCY_FACTOR];

    ApplyForCounter(pdrv, &DownFrequency.counter);
    ApplyForCounter(pdrv, &DownFrequency.counterReverse);
    DownFrequency.delay = &g_pFixedValue[DOWNFREQUENCY_TIME];
    DownFrequency.state = &g_TelesignalDB[ADDR_DOWN_FREQUEBNCY_PROTECTION];
    DownFrequency.soeAddr = ADDR_DOWN_FREQUEBNCY_PROTECTION;
    DownFrequency.value = &g_pFixedValue[DOWNFREQUENCY_VALUE];
    DownFrequency.funSwitch = &g_pFixedValue[DOWNFREQUENCY_SWITCH];
    DownFrequency.telemetry = &g_FreGather[FRE_Uab].freValueProtect;
    DownFrequency.factor = &g_pFixedValue[DOWNFREQUENCY_FACTOR];

    ApplyForCounter(pdrv, &MemoryCounter.counterSave);
    ApplyForCounter(pdrv, &MemoryCounter.counterCharge);    
    CommunicatLock.timeout = &g_Parameter[REVERSE_TIME];
    CommunicatLock.flag[COM_YK] = &g_CommunicatFlag[COM_YK];
    CommunicatLock.flag[COM_PARA] = &g_CommunicatFlag[COM_PARA];
    CommunicatLock.flag[COM_FILE] = &g_CommunicatFlag[COM_FILE];
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_YK]);
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_PARA]);
    ApplyForCounter(pdrv, &CommunicatLock.counter[COM_FILE]);
	
    device_fram = rt_device_find(RT_SPI_FRAM_NAME);
    if (device_fram == NULL)
    {
        
    }	
}	

/**
  * @brief: Recoed Memory
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void RecoedMemory(void)
{	
    /* 需存储记忆数据 */
    static struct MEMORY_FLAG_Str s_Flag_DB1;   
    static struct MEMORY_FLAG_Str s_Flag_DB2; 
    static rt_uint8_t state=0;
    		
    /* 装置掉电 */
    if (g_TelesignalDB[ADDR_DEVICE_FAULT] == ON)
    {
        *MemoryCounter.counterCharge = DB_COUNTER_EN;
        if(state==1)
        {
            state = 0;
            rt_multi_common_data_powerdown_storage();
        }
    }
    else
    {
        state = 1;
        if ((memcmp(&s_Flag_DB2, &g_FlagDB, sizeof(g_FlagDB))))
        {
            *MemoryCounter.counterSave = DB_COUNTER_EN;
        }
        if ((memcmp(&g_FlagDB, &s_Flag_DB1, sizeof(g_FlagDB))))
        {
            s_Flag_DB2 = g_FlagDB;
            if(!(*MemoryCounter.counterSave&DB_COUNTER_EN))
            {
                *MemoryCounter.counterSave = DB_COUNTER_EN;
            }
            if (((*MemoryCounter.counterSave) & DB_COUNTER) >= MEMORY_TIME)
            {
                *MemoryCounter.counterSave = 0;
                s_Flag_DB1 = g_FlagDB;
                rt_multi_common_data_powerdown_storage();
                (*CommunicatLock.flag[COM_FILE]) |= COMMUNICATLOCKRECORD;                    
            }
        } 
        
        if (((*MemoryCounter.counterCharge) & DB_COUNTER) >= CHARGE_TIME)
        {
           *MemoryCounter.counterCharge = CHARGE_TIME;
           (*CommunicatLock.flag[COM_FILE]) &= ~COMMUNICATLOCKLOSEPOWER; //掉电恢复
        }
        else
        { 
            if(!(*MemoryCounter.counterCharge&DB_COUNTER_EN))
            {
                *MemoryCounter.counterCharge = DB_COUNTER_EN;
            }
            (*CommunicatLock.flag[COM_FILE]) |= COMMUNICATLOCKLOSEPOWER;//掉电保护
        }
    }
}

/**
  * @brief: the counter of system
  * @param:  none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
static void SystemCounterTask(rt_uint8_t pdrv)
{
    ListElmt *element;
    
    element = s_ListTimers[pdrv].head;
    while(element != NULL)
    {
        if((*((uint32_t *)(element->data)))&DB_COUNTER_EN)
        {
            (*((uint32_t *)(element->data)))++;
        }
        element = element->next;
    }
}


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : other_protect_clock init
  * @param : none
  * @return: none
  * @updata: [YYYY-MM-DD][NAME][BRIEF]
  */
void other_protect_clock(void)
{
    /* 系统计数器 */
    SystemCounterTask(DB_DEV0);	

    /* 记录存储处理 */
    RecoedMemory();	
	
    /* 遥测异常监测 */
    TelemetryAbnormalCheck();

    /* 通讯互锁 */
    ComunicatLock();   
 
    /* 系统自检 */
    SysMonitorTask();	
}

/* END OF FILE ---------------------------------------------------------------*/

