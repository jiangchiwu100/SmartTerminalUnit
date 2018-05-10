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
static rt_uint32_t s_Counter[DB_DEV_MAX_NUM][COUNTER_MAX];   //0x80000000使能位

static struct OverLimit OverLimitUab;
static struct OverLimit OverLimitUBC;
static struct OverLimit OverLimitUac;
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
static rt_uint8_t ApplyForCounter(rt_uint8_t pdrv, rt_uint32_t **dev)
{
    rt_uint8_t res = FALSE;

    static rt_uint8_t i = 0;

    if (i < COUNTER_MAX)
    {
        *dev = &s_Counter[pdrv][i++];
        s_Counter[pdrv][i] = 0;
        res = TRUE;
    }
    return(res);
}

/**
  * @Description: 遥测越限判断函数
  * @param:  overlimit-越限结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void TelemetryOverLimit(struct OverLimit *overlimit)
{
    if (!g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitch || !*overlimit->funSwitch)
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

				if (*over->funSwitch == 1)
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
				
				if (*down->funSwitch == 1)
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
	
	if (g_TelemetryDB.Str.Uab >= 10.0f)
	{
	    DownVoltageCheck(&DownVoltageUab);
	}

	if (g_TelemetryDB.Str.UBC >= 10.0f)
	{
	    DownVoltageCheck(&DownVoltageUBC);
	}
	
	if (g_TelemetryDB.Str.frequency >= 10)
	{
	    DownVoltageCheck(&DownFrequency);
	}
    	
    /* 电池欠压 */
    if (g_pFixedValue->Data.publicValue.Str.batteryLowVoltageAlarmSwitch)
    {
        if (g_TelemetryDB.Str.DCVoltage < g_pFixedValue->Data.publicValue.Str.batteryLowVoltageValue && g_TelesignalDB.Str.batteryUnderVoltageAlarm == OFF)
        {
            DBWriteSOE(ADDR_BATTERY_UNDERVOLTAGE_ALARM, ON);
        }
        else if (g_TelemetryDB.Str.DCVoltage >= g_pFixedValue->Data.publicValue.Str.batteryLowVoltageValue * g_pFixedValue->Data.publicValue.Str.batteryLowVoltageFactor)
        {
            if (g_TelesignalDB.Str.batteryUnderVoltageAlarm == ON)
            {
                DBWriteSOE(ADDR_BATTERY_UNDERVOLTAGE_ALARM, OFF);
            }
        }
    }
    /* 永磁机构电容欠压 */
    if (g_ParameterDB.Data.runPara.Str.operatingMechanism == MAGNET) // 永磁机构电容欠压
    {
        /* 合闸状态 */
        if (g_TelesignalDB.Str.switchClose == ON &&  g_TelesignalDB.Str.switchOpen == OFF
                && g_TelemetryDB.Str.DCSpare < g_ParameterDB.Data.runPara.Str.capUnderVoltageClosing)
        {
            DBWriteSOE(ADDR_CAP_UNDER_VOLTAGE, OFF);
        }
        else if (g_TelemetryDB.Str.DCSpare >= g_ParameterDB.Data.runPara.Str.capUnderVoltageClosing * g_ParameterDB.Data.runPara.Str.capUnderVoltageFactor)
        {
            DBWriteSOE(ADDR_CAP_UNDER_VOLTAGE, ON);
        }

        /* 分闸状态 */
        if (g_TelesignalDB.Str.switchClose == OFF &&  g_TelesignalDB.Str.switchOpen == ON
            && g_TelemetryDB.Str.DCSpare < g_ParameterDB.Data.runPara.Str.capUnderVoltageOpening)
        {
            DBWriteSOE(ADDR_CAP_UNDER_VOLTAGE, OFF);
        }
        else if (g_TelemetryDB.Str.DCSpare >= g_ParameterDB.Data.runPara.Str.capUnderVoltageOpening * g_ParameterDB.Data.runPara.Str.capUnderVoltageFactor)
        {
            DBWriteSOE(ADDR_CAP_UNDER_VOLTAGE, ON);
        }
    }
}

/**
  * @Description: 电池活化
  * @param:  clock-调用周期
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void BatteryActivation(rt_uint8_t clock)
{
    static rt_uint32_t s_CycleCounter;
    static rt_uint32_t s_ActCounter;
    static rt_uint32_t s_FaultCounter;
    static rt_uint32_t s_RecoverCounter;
    
    if (g_TelesignalDB.Str.batteryActivationStatus == OFF)
    {
        if (++s_RecoverCounter > 400)
        {
            s_RecoverCounter = 0;
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END_RECOVERY, 0);
        }         
        
        if (g_pFixedValue->Data.publicValue.Str.batteryActiveSwitch)
        {                       
            if (s_CycleCounter >= g_pFixedValue->Data.publicValue.Str.batteryActiveCycle * 24 * 3600 * 1000 / clock) // 换算为小时
            {
                s_CycleCounter = 0;
                rt_hw_do_operate(DO_BATTERY_ACTIVE, 0);
            }
            else
            {
                s_CycleCounter++;
            }                
        }
    }
    else
    {
        s_CycleCounter = 0;
        s_RecoverCounter = 0;        
    }

    if (g_TelesignalDB.Str.batteryActivationStatus == ON)
    {
        if (s_FaultCounter <= g_pFixedValue->Data.publicValue.Str.batteryActiveFaultTime + 1)
        {
            s_FaultCounter++;
        }
        
        if (s_ActCounter > 500)
        {
            rt_hw_do_operate(DO_BATTERY_ACTIVE_RECOVERY, 0);
        }

        if ((g_TelemetryDB.Str.Uab < g_pFixedValue->Data.publicValue.Str.downlimitVoltageU
            && g_TelemetryDB.Str.UBC < g_pFixedValue->Data.publicValue.Str.downlimitVoltageU))
        {
            /* 交流电消失，停止活化 */
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);                
        }
            
        if (g_pFixedValue->Data.publicValue.Str.batteryActiveSwitch && 
            s_ActCounter >= g_pFixedValue->Data.publicValue.Str.batteryActiveTime * 60 * 1000)
        {
            s_ActCounter = 0;

            /* 停止活化 */
            rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);
        }
        else
        {
            s_ActCounter++;
            
            /* 活化过程中电池电压低于故障电压，并上报SOE*/           
            if (s_FaultCounter <= g_pFixedValue->Data.publicValue.Str.batteryActiveFaultTime)
            {                
                if (g_TelemetryDB.Str.DCVoltage < g_pFixedValue->Data.publicValue.Str.batteryActiveFaultVoltage)
                {
                    s_FaultCounter = 0;
                    /* 停止活化 */
                    rt_hw_do_operate(DO_BATTERY_ACTIVE_END, 0);
                    DBWriteSOE(ADDR_BATTERY_FAULT_ALARM, ON); // 电池故障
                }
            }           
        }
    }
    else
    {
        s_ActCounter = 0;
        s_FaultCounter = 0;
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
	
    ApplyForCounter(pdrv, &OverLimitUab.counterUp);
    ApplyForCounter(pdrv, &OverLimitUab.counterDown);
    ApplyForCounter(pdrv, &OverLimitUab.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUab.counterDownReverse);	
    OverLimitUab.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitUab.stateUp = &g_TelesignalDB.Str.overLimitUaUp;
    OverLimitUab.stateDown = &g_TelesignalDB.Str.overLimitUaDown;
    OverLimitUab.soeAddrUp = ADDR_OVER_LIMIT_Uab_UP;
    OverLimitUab.soeAddrDown = ADDR_OVER_LIMIT_Uab_DOWN;
    OverLimitUab.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitVoltageU;
    OverLimitUab.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitVoltageU;
    OverLimitUab.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchUab;
    OverLimitUab.telemetry = &g_TelemetryDB.Str.Uab;
    OverLimitUab.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitUab.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitUBC.counterUp);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDown);
    ApplyForCounter(pdrv, &OverLimitUBC.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUBC.counterDownReverse);	
    OverLimitUBC.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitUBC.stateUp = &g_TelesignalDB.Str.overLimitUcUp;
    OverLimitUBC.stateDown = &g_TelesignalDB.Str.overLimitUcDown;
    OverLimitUBC.soeAddrUp = ADDR_OVER_LIMIT_UBC_UP;
    OverLimitUBC.soeAddrDown = ADDR_OVER_LIMIT_UBC_DOWN;
    OverLimitUBC.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitVoltageU;
    OverLimitUBC.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitVoltageU;
    OverLimitUBC.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchUbc;
    OverLimitUBC.telemetry = &g_TelemetryDB.Str.UBC;
    OverLimitUBC.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitUBC.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitUac.counterUp);
    ApplyForCounter(pdrv, &OverLimitUac.counterDown);
    ApplyForCounter(pdrv, &OverLimitUac.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitUac.counterDownReverse);	
    OverLimitUac.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitUac.stateUp = &g_TelesignalDB.Str.overLimitUbUp;
    OverLimitUac.stateDown = &g_TelesignalDB.Str.overLimitUbDown;
    OverLimitUac.soeAddrUp = ADDR_OVER_LIMIT_Uca_UP;
    OverLimitUac.soeAddrDown = ADDR_OVER_LIMIT_Uca_DOWN;
    OverLimitUac.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitVoltageU;
    OverLimitUac.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitVoltageU;
    OverLimitUac.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchUac;
    OverLimitUac.telemetry = &g_TelemetryDB.Str.Uac;
    OverLimitUac.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitUac.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitIa.counterUp);
    ApplyForCounter(pdrv, &OverLimitIa.counterDown);
    ApplyForCounter(pdrv, &OverLimitIa.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIa.counterDownReverse);		
    OverLimitIa.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitIa.stateUp = &g_TelesignalDB.Str.overLimitIaUp;
    OverLimitIa.stateDown = &g_TelesignalDB.Str.overLimitIaDown;
    OverLimitIa.soeAddrUp = ADDR_OVER_LIMIT_IA_UP;
    OverLimitIa.soeAddrDown = ADDR_OVER_LIMIT_IA_DOWN;
    OverLimitIa.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitCurrentI;
    OverLimitIa.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitCurrentI;
    OverLimitIa.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchIa;
    OverLimitIa.telemetry = &g_TelemetryDB.Str.Ia;
    OverLimitIa.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitIa.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitIb.counterUp);
    ApplyForCounter(pdrv, &OverLimitIb.counterDown);
    ApplyForCounter(pdrv, &OverLimitIb.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIb.counterDownReverse);	
    OverLimitIb.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitIb.stateUp = &g_TelesignalDB.Str.overLimitIbUp;
    OverLimitIb.stateDown = &g_TelesignalDB.Str.overLimitIbDown;
    OverLimitIb.soeAddrUp = ADDR_OVER_LIMIT_IB_UP;
    OverLimitIb.soeAddrDown = ADDR_OVER_LIMIT_IB_DOWN;
    OverLimitIb.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitCurrentI;
    OverLimitIb.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitCurrentI;
    OverLimitIb.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchIb;
    OverLimitIb.telemetry = &g_TelemetryDB.Str.Ib;
    OverLimitIb.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitIb.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitIc.counterUp);
    ApplyForCounter(pdrv, &OverLimitIc.counterDown);
    ApplyForCounter(pdrv, &OverLimitIc.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitIc.counterDownReverse);	
    OverLimitIc.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitIc.stateUp = &g_TelesignalDB.Str.overLimitIcUp;
    OverLimitIc.stateDown = &g_TelesignalDB.Str.overLimitIcDown;
    OverLimitIc.soeAddrUp = ADDR_OVER_LIMIT_IC_UP;
    OverLimitIc.soeAddrDown = ADDR_OVER_LIMIT_IC_DOWN;
    OverLimitIc.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitCurrentI;
    OverLimitIc.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitCurrentI;
    OverLimitIc.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchIc;
    OverLimitIc.telemetry = &g_TelemetryDB.Str.Ic;
    OverLimitIc.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitIc.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitU0.counterUp);
    ApplyForCounter(pdrv, &OverLimitU0.counterDown);
    ApplyForCounter(pdrv, &OverLimitU0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitU0.counterDownReverse);	
    OverLimitU0.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitU0.stateUp = &g_TelesignalDB.Str.overLimitU0Up;
    OverLimitU0.stateDown = &g_TelesignalDB.Str.overLimitU0Down;
    OverLimitU0.soeAddrUp = ADDR_OVER_LIMIT_U0_UP;
    OverLimitU0.soeAddrDown = ADDR_OVER_LIMIT_U0_DOWN;
    OverLimitU0.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitVoltageU0;
    OverLimitU0.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitVoltageU0;
    OverLimitU0.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchU0;
    OverLimitU0.telemetry = &g_TelemetryDB.Str.U0;
    OverLimitU0.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitU0.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitI0.counterUp);
    ApplyForCounter(pdrv, &OverLimitI0.counterDown);
    ApplyForCounter(pdrv, &OverLimitI0.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitI0.counterDownReverse);		
    OverLimitI0.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitI0.stateUp = &g_TelesignalDB.Str.overLimitI0Up;
    OverLimitI0.stateDown = &g_TelesignalDB.Str.overLimitI0Down;
    OverLimitI0.soeAddrUp = ADDR_OVER_LIMIT_I0_UP;
    OverLimitI0.soeAddrDown = ADDR_OVER_LIMIT_I0_DOWN;
    OverLimitI0.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitCurrentI0;
    OverLimitI0.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitCurrentI0;
    OverLimitI0.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchI0;
    OverLimitI0.telemetry = &g_TelemetryDB.Str.I0;
    OverLimitI0.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitI0.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitDC_U.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_U.counterDownReverse);			
    OverLimitDC_U.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitDC_U.stateUp = &g_TelesignalDB.Str.overLimitDC_U_Up;
    OverLimitDC_U.stateDown = &g_TelesignalDB.Str.overLimitDC_U_Down;
    OverLimitDC_U.soeAddrUp = ADDR_OVER_LIMIT_DC_U_UP;
    OverLimitDC_U.soeAddrDown = ADDR_OVER_LIMIT_DC_U_DOWN;
    OverLimitDC_U.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitDCVoltageU;
    OverLimitDC_U.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitDCVoltageU;
    OverLimitDC_U.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchDC_U;
    OverLimitDC_U.telemetry = &g_TelemetryDB.Str.DCVoltage;
    OverLimitDC_U.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitDC_U.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &OverLimitDC_I.counterUp);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDown);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterUpReverse);
    ApplyForCounter(pdrv, &OverLimitDC_I.counterDownReverse);	
    OverLimitDC_I.delay = &g_pFixedValue->Data.publicValue.Str.overlimitTime;
    OverLimitDC_I.stateUp = &g_TelesignalDB.Str.overLimitDC_I_Up;
    OverLimitDC_I.stateDown = &g_TelesignalDB.Str.overLimitDC_I_Up;
    OverLimitDC_I.soeAddrUp = ADDR_OVER_LIMIT_DC_U_UP;
    OverLimitDC_I.soeAddrDown = ADDR_OVER_LIMIT_DC_U_DOWN;
    OverLimitDC_I.uplimit = &g_pFixedValue->Data.publicValue.Str.uplimitDCCurrentI;
    OverLimitDC_I.downlimit = &g_pFixedValue->Data.publicValue.Str.downlimitDCCurrentI;
    OverLimitDC_I.funSwitch = &g_pFixedValue->Data.publicValue.Str.overlimitAlarmSwitchDC_I;
    OverLimitDC_I.telemetry = &g_TelemetryDB.Str.DCSpare;
    OverLimitDC_I.uplimitFactor = &g_pFixedValue->Data.publicValue.Str.uplimitFactor;
    OverLimitDC_I.downlimitFactor = &g_pFixedValue->Data.publicValue.Str.downlimitFactor;

    ApplyForCounter(pdrv, &HeavyOverLoad.counter);
    ApplyForCounter(pdrv, &HeavyOverLoad.counterReverse);	
    HeavyOverLoad.delay = &g_pFixedValue->Data.publicValue.Str.heavyOverloadTime;
    HeavyOverLoad.state = &g_TelesignalDB.Str.haveyOverload;
    HeavyOverLoad.soeAddr = ADDR_HAVEY_OVERLOAD_EVENT;
    HeavyOverLoad.value = &g_pFixedValue->Data.publicValue.Str.heavyOverloadValue;
    HeavyOverLoad.funSwitch = &g_pFixedValue->Data.publicValue.Str.heavyOverloadSwitch;
    HeavyOverLoad.Ia = &g_TelemetryDB.Str.Ia;
    HeavyOverLoad.Ib = &g_TelemetryDB.Str.Ib;
    HeavyOverLoad.Ic = &g_TelemetryDB.Str.Ic;
    HeavyOverLoad.factor = &g_pFixedValue->Data.publicValue.Str.heavyOverloadFactor;

    ApplyForCounter(pdrv, &OverLoad.counter);
    ApplyForCounter(pdrv, &OverLoad.counterReverse);	
    OverLoad.delay = &g_pFixedValue->Data.publicValue.Str.overloadTime;
    OverLoad.state = &g_TelesignalDB.Str.overloadEvent;
    OverLoad.soeAddr = ADDR_OVERLOAD_EVENT;
    OverLoad.value = &g_pFixedValue->Data.publicValue.Str.overloadValue;
    OverLoad.funSwitch = &g_pFixedValue->Data.publicValue.Str.overloadSwitch;
    OverLoad.Ia = &g_TelemetryDB.Str.Ia;
    OverLoad.Ib = &g_TelemetryDB.Str.Ib;
    OverLoad.Ic = &g_TelemetryDB.Str.Ic;
    OverLoad.factor = &g_pFixedValue->Data.publicValue.Str.overloadFactor;

    ApplyForCounter(pdrv, &OverVoltageUab.counter);
    ApplyForCounter(pdrv, &OverVoltageUab.counterReverse);	
    OverVoltageUab.delay = &g_pFixedValue->Data.publicValue.Str.overVoltageTime;
    OverVoltageUab.state = &g_TelesignalDB.Str.overVoltageProtection;
    OverVoltageUab.soeAddr = ADDR_OVER_VOLTAGE_PROTECTION;
    OverVoltageUab.value = &g_pFixedValue->Data.publicValue.Str.overVoltageValue;
    OverVoltageUab.funSwitch = &g_pFixedValue->Data.publicValue.Str.overVoltageSwitch;
    OverVoltageUab.telemetry = &g_TelemetryDB.Str.Uab;
    OverVoltageUab.factor = &g_pFixedValue->Data.publicValue.Str.overVoltageFactor;

    ApplyForCounter(pdrv, &DownVoltageUab.counter);
    ApplyForCounter(pdrv, &DownVoltageUab.counterReverse);	
    DownVoltageUab.delay = &g_pFixedValue->Data.publicValue.Str.downVoltageTime;
    DownVoltageUab.state = &g_TelesignalDB.Str.downVoltageProtection;
    DownVoltageUab.soeAddr = ADDR_DOWN_VOLTAGE_PROTECTION;
    DownVoltageUab.value = &g_pFixedValue->Data.publicValue.Str.downVoltageValue;
    DownVoltageUab.funSwitch = &g_pFixedValue->Data.publicValue.Str.downVoltageSwitch;
    DownVoltageUab.telemetry = &g_TelemetryDB.Str.Uab;
    DownVoltageUab.factor = &g_pFixedValue->Data.publicValue.Str.downVoltageFactor;
	
//    ApplyForCounter(pdrv, &OverVoltageUBC.counter);
//    ApplyForCounter(pdrv, &OverVoltageUBC.counterReverse);
//    OverVoltageUBC.delay = &g_pFixedValue->Data.publicValue.Str.overVoltageTime;
//    OverVoltageUBC.state = &g_TelesignalDB.Str.overVoltageProtection;
//    OverVoltageUBC.soeAddr = ADDR_OVER_VOLTAGE_PROTECTION;
//    OverVoltageUBC.value = &g_pFixedValue->Data.publicValue.Str.overVoltageValue;
//    OverVoltageUBC.funSwitch = &g_pFixedValue->Data.publicValue.Str.overVoltageSwitch;
//    OverVoltageUBC.telemetry = &g_TelemetryDB.Str.UBC;
//    OverVoltageUBC.factor = &g_pFixedValue->Data.publicValue.Str.overVoltageFactor;

//    ApplyForCounter(pdrv, &DownVoltageUBC.counter);
//    ApplyForCounter(pdrv, &DownVoltageUBC.counterReverse);
//    DownVoltageUBC.delay = &g_pFixedValue->Data.publicValue.Str.downVoltageTime;
//    DownVoltageUBC.state = &g_TelesignalDB.Str.downVoltageProtection;
//    DownVoltageUBC.soeAddr = ADDR_DOWN_VOLTAGE_PROTECTION;
//    DownVoltageUBC.value = &g_pFixedValue->Data.publicValue.Str.downVoltageValue;
//    DownVoltageUBC.funSwitch = &g_pFixedValue->Data.publicValue.Str.downVoltageSwitch;
//    DownVoltageUBC.telemetry = &g_TelemetryDB.Str.UBC;
//    DownVoltageUBC.factor = &g_pFixedValue->Data.publicValue.Str.downVoltageFactor;

    ApplyForCounter(pdrv, &OverFrequency.counter);
    ApplyForCounter(pdrv, &OverFrequency.counterReverse);
    OverFrequency.delay = &g_pFixedValue->Data.publicValue.Str.overFrequencyTime;
    OverFrequency.state = &g_TelesignalDB.Str.overFrequencyProtection;
    OverFrequency.soeAddr = ADDR_OVER_FREQUEBNCY_PROTECTION;
    OverFrequency.value = &g_pFixedValue->Data.publicValue.Str.overFrequencyValue;
    OverFrequency.funSwitch = &g_pFixedValue->Data.publicValue.Str.overFrequencySwitch;
    OverFrequency.telemetry = &g_FreGather.freValueProtect;
    OverFrequency.factor = &g_pFixedValue->Data.publicValue.Str.overFrequencyFactor;

    ApplyForCounter(pdrv, &DownFrequency.counter);
    ApplyForCounter(pdrv, &DownFrequency.counterReverse);
    DownFrequency.delay = &g_pFixedValue->Data.publicValue.Str.downFrequencyTime;
    DownFrequency.state = &g_TelesignalDB.Str.downFrequencyProtection;
    DownFrequency.soeAddr = ADDR_DOWN_FREQUEBNCY_PROTECTION;
    DownFrequency.value = &g_pFixedValue->Data.publicValue.Str.downFrequencyValue;
    DownFrequency.funSwitch = &g_pFixedValue->Data.publicValue.Str.downFrequencySwitch;
    DownFrequency.telemetry = &g_FreGather.freValueProtect;
    DownFrequency.factor = &g_pFixedValue->Data.publicValue.Str.downFrequencyFactor;

    ApplyForCounter(pdrv, &MemoryCounter.counterSave);
    ApplyForCounter(pdrv, &MemoryCounter.counterCharge);    
    CommunicatLock.timeout = &g_ParameterDB.Data.runPara.Str.reverseTime;
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
    if (g_TelesignalDB.Str.devicePowerDown == ON)
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
static void SystemCounterTask(void)
{
    rt_uint8_t i;
    rt_uint8_t pdrv = 0;

    for (i = 0; i < COUNTER_MAX; i++)//定时增加
    {
        if (s_Counter[pdrv][i] & DB_COUNTER_EN)
        {
            s_Counter[pdrv][i]++;
        }
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
    SystemCounterTask();	

    /* 记录存储处理 */
    RecoedMemory();	
	
    /* 遥测异常监测 */
    TelemetryAbnormalCheck();

    /* 电池活化 */
    BatteryActivation(1);

    /* 通讯互锁 */
    ComunicatLock();   
 
    /* 系统自检 */
    SysMonitorTask();	
}

/* END OF FILE ---------------------------------------------------------------*/

