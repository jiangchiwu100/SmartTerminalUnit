/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      commmon_data.c
  * @brief:     The commmon data of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sun
  * @date:      2017-09-05
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
	
	
/* INCLUDES FILES ------------------------------------------------------------*/
#include "string.h"
#include "math.h"
#include "drv_di.h"
#include "drv_do.h"
#include "drv_led.h"
#include "drv_fm25vxx.h"
#include "calculator.h"
#include "common_data.h"
#include "file_operate.h"
#include "breaker_interface.h"
#include "load_switch_interface.h"
#include ".\MultiThread\multi_thread.h"
#include "common_config.h"


/* PUBLIC VARIABLES ---------------------------------------------------------*/
uint16_t                            g_ThreadRunSta;
uint16_t                            g_AddrCount;
uint16_t                            g_StartWave;
struct ConfigurationSetDatabase     *g_ConfigurationSetDB; // 系统配置结构
struct SD2405Time                   g_SystemTime; // 系统时间
//TelesignalDatabase                  g_TelesignalDB; // 遥信缓存
uint8_t								g_TelesignalDB[TELESIGNAL_TOTAL_NUM];						
 /* 新遥信点表映射 */
List                                g_NewListTelesignal[TELESIGNAL_TOTAL_NUM];
rt_uint16_t                         g_NewMaxNumTelesignal;
rt_uint16_t                         g_NewToOldTelesignal[299 + TELESIGNAL_TOTAL_NUM]; // 新点表映射，填原点表数组下标

/* 遥测缓存 */
float                               g_TelemetryDB[TELEMETRY_TOTAL_NUM];
float                               g_TelemetryLastDB[TELEMETRY_TOTAL_NUM];
float g_secondHarmonicIa, g_secondHarmonicIb, g_secondHarmonicIc;

#if RT_USING_TELEMETRY_SET
float                   g_TelemetrySetEnable[TELEMETRY_TOTAL_NUM];
float                   g_TelemetrySetValue[TELEMETRY_TOTAL_NUM];
#endif /* RT_USING_TELEMETRY_SET */

/* 新遥测点表映射 */
rt_uint16_t                         g_NewPropertyTelemetry[TELEMETRY_TOTAL_NUM];//低6位类型
float                               g_NewMultipleRateTelemetry[TELEMETRY_TOTAL_NUM];//倍率
rt_uint16_t                         g_NewAddTelemetry[TELEMETRY_TOTAL_NUM];//写入对应新地址

rt_uint16_t                         g_NewMaxNumTelemetry; // 新点表个数
rt_uint16_t                         g_NewToOldTelemetry[TELEMETRY_TOTAL_NUM]; // 新点表映射，填原点表数组下标

/* 新遥控点表映射 */
rt_uint16_t                         g_NewToOldRemote[REMOTE_TOTAL_NUM]; // 新点表映射，填原点表数组下标
rt_uint16_t                         g_NewToOldPropertyRemote[REMOTE_TOTAL_NUM];//新点表属性//低6位类型，变化，取反

rt_uint32_t                         g_CommunicatFlag[COM_MAX]; // 通讯互锁标志

/* 定值和参数缓存 */
struct Inherent                 g_InherentPara = RT_SYS_CONFIG_DEFAULT;

float                          *g_pFixedValue;
float                           g_FixedValue1[FIXED_VALUE_NUM];
float                           g_FixedValue2[FIXED_VALUE_NUM];
float                           g_Parameter[RUN_PARAMETER_NUM];
float                           g_CalibrateFactor[CALIFACTOR_NUM];

struct tagValueParaCfg         *g_pFixedValueCfg;

/* 定值操作信息 */
struct ValueParameterOperate        g_ValueParaOperateInfo;

/* 定值和参数预置缓存 */
struct ValueParaPreset              g_ValueParaPresetDB;

/* 需存储记忆数据 */
struct MEMORY_FLAG_Str              g_FlagDB;

/* COS缓存 */
struct COS_Str                      g_COSDB[COS_MAX_NUM];

/* SOE缓存 */
struct SOE_Str                      g_SOEDB[SOE_MAX_NUM];

/* SOE缓存 */
struct SOE_Str                      g_SOENewDB[SOE_NEW_MAX_NUM];

/* CO缓存 */
struct CO_Str                       g_CoDB[CO_MAX_NUM];

/* FEVENT缓存 */
struct FEVENT_Str                   g_FeventDB[FEVENT_MAX_NUM];

/* ULOG缓存 */
struct ULOG_Str                     g_UlogDB[ULOG_MAX_NUM];

/* NVA缓存 */
struct NVA_Str                      g_NVADB[NVA_MAX_NUM];

/* COS缓存参数 */
rt_uint32_t                         g_COSDBIn;
rt_uint32_t                         g_COSDBOut[DEV_MAX_NUM];

/* NVA缓存参数 */
rt_uint32_t                         g_NVADBIn;
rt_uint32_t                         g_NVADBOut[DEV_MAX_NUM];

/* 通讯信息 */
struct lwip_dev                     g_EthW5500; // w5500 information
struct lwip_dev                     g_EthDP83848; // dp83848 

/* 通讯灯状态 */
rt_uint32_t                         g_ComLed = 0;

/* 采样队列 */
rt_int8_t                           g_SampleQueueIn;  // 入指针
Queue                               *g_SampleQueueBuf;// __attribute__((at(SDRAM_ADDR_WAVE))); // 数据空间
uint16_t                            g_StartWave;

/* 缓存后的数据，防止计算被打断 */
double                              g_IaSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };  
double                              g_IbSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_IcSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_I0SampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_UaSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_UbSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_UcSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_U0SampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_UASampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_UCSampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_Udc1SampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };
double                              g_Udc2SampleBuf[ADC_SAMPLE_NUM * 2] = { 0 };


/* PRIVATE VARIABLES --------------------------------------------------------*/
static float RatedValue[] = {0, RATED_VALUE_I, RATED_VALUE_I, RATED_VALUE_I, RATED_VALUE_I0,
                                RATED_VALUE_U, RATED_VALUE_U, RATED_VALUE_U, RATED_VALUE_U0,
                                RATED_VALUE_U, RATED_VALUE_U,
                                RATED_VALUE_P, RATED_VALUE_Q, RATED_VALUE_S, RATED_VALUE_PF};

static rt_device_t device_fram = RT_NULL;                             
static rt_device_t device_sd2405 = RT_NULL;   

																
/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : 二进制机器码转换为浮点数
  * @param :  bin-bin类型数据
  * @return: *(float*)(&bin)-float类型数据
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
float BinToFloat(int bin)
{
    return (*(float*)(&bin));
}

/**
  * @brief: 浮点数转换为二进制机器码
  * @param:  fl-float类型数据
  * @return: *(int*)(&fl)-bin类型数据
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
int FloatToBin(float fl)
{
    return (*(int*)(&fl));
}

/**
  * @Description: BCD码转十进制数据
  * @param:  bcd-待转换的BCD码
  * @return: dec-转换后的十进制数据
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t BCDToDecimal(uint8_t bcd)
{
    uint8_t dec = 0;

    dec = ((bcd >> 4) & 0x0F)*10 + (bcd & 0x0F);

    return (dec);
}

/**
  * @Description: 十进制数据转BCD码
  * @param:  dec-待转换的十进制码
  * @return: bcd-转换后的BCD数据
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DecimalToBCD(uint8_t dec)
{
    uint8_t bcd;

    bcd = (((dec / 10)<<4) & 0xF0) + ((dec % 10) & 0x0F);

    return (bcd);
}

static uint8_t s_update_sta;
/**
  * @brief: 写系统更新状态
  * @param:  sta-状态值
  * @return: [none]
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_write_update_state(const uint8_t sta)
{
    s_update_sta &= ~(SYS_HAVE_UPDATE | SYS_UPDATE_FAILED);
	s_update_sta |= sta;
}

/**
  * @brief: 读取系统更新状态
  * @param:  [none]
  * @return: [s_update_sta]-系统更新状态值
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t rt_multi_common_read_update_state(void)
{
    return s_update_sta;
}
/**
  * @brief: 获取定值缓存地址和偏移量
  * @param:  addr-定值地址
  * @param:  offset-偏移量
  * @param:  pInfo-定值信息的地址
  * @return: array-定值缓冲区地址
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
float* GetValueArray(uint16_t addr, uint8_t sn)
{
    float* array;
    uint32_t offset;
	float   *pFixedValue;

    
    (sn == 2) ? (pFixedValue = g_FixedValue2) : (pFixedValue = g_FixedValue1);

    if ((INTRIPARAME_START_ADDR <= addr)&&(addr < INTRIPARAME_START_ADDR + INHERENT_PARAMETER_NUM))
    {
        // intrinisic  para block 固有参数
        array = (float *)g_InherentPara.terminalType;
        offset = (addr - INTRIPARAME_START_ADDR) * 24 / 4;

    }
    else if((RUNPARAMETER_START_ADDR <= addr) && (addr < (RUNPARAMETER_START_ADDR + RUN_PARAMETER_NUM)))
    {
        // running para block 运行参数
        array = g_Parameter;
        offset = addr - RUNPARAMETER_START_ADDR;
    }
    else if((CALIBRATE_FACTOR_START_ADDR <= addr) && ( addr < (CALIBRATE_FACTOR_START_ADDR + g_CalibrateFactorCfg_Len)))
    {
        // calibration para block 校准系数
        array = g_CalibrateFactor;
        offset = addr - CALIBRATE_FACTOR_START_ADDR;
    }
    else if (addr >= CALIBRATE_VALUE_START_ADDR && (addr <= (CALIBRATE_VALUE_START_ADDR + g_CalibrateFactorCfg_Len)))
    {
        // 校准值 不固化
        array = NULL;
        offset = 0;
    }
    else if((FIXED_VALUE_START_ADDR <= addr) && ( addr < (FIXED_VALUE_START_ADDR + FIXED_VALUE_NUM)))
    {
        // public para block 定值公共信息
        array = pFixedValue;
        offset = addr - FIXED_VALUE_START_ADDR;
    }
    #if RT_USING_TELEMETRY_SET
    else if((TELEMETRY_SETENABLE_START_ADDR <= addr) && ( addr < (TELEMETRY_SETENABLE_START_ADDR + TELEMETRY_TOTAL_NUM)))
    {
        // 强制遥测地址
        array = g_TelemetrySetEnable;
        offset = addr - TELEMETRY_SETENABLE_START_ADDR;
    }
    else if((TELEMETRY_SETVALUE_START_ADDR <= addr) && ( addr < (TELEMETRY_SETVALUE_START_ADDR + TELEMETRY_TOTAL_NUM)))
    {
        // 强制遥测值
        array = g_TelemetrySetValue;
        offset = addr - TELEMETRY_SETVALUE_START_ADDR;
    }
    #endif
    else
    {
        // error
        array = NULL;
        offset = 0;
    }
    
    return (array + offset);
}

/**
  * @brief: 参数监测.
  * @param:  [none]
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void ParameterCheck(void)
{
    uint32_t i;

	for (i = 0; i < g_CalibrateFactorCfg_Len; i++)
	{
		if (*(CalibrateFactorCfg[i].factorVal) > CalibrateFactorCfg[i].factorMax || *(CalibrateFactorCfg[i].factorVal) < CalibrateFactorCfg[i].factorMin)
		{
		    *(CalibrateFactorCfg[i].factorVal) = CalibrateFactorCfg[i].factorDefault; 
		}
	}
	
	for (i = 0; i < g_FixedValueCfg1_Len; i++)
	{
		if (*g_pFixedValueCfg[i].pVal > g_pFixedValueCfg[i].valMax || *g_pFixedValueCfg[i].pVal < g_pFixedValueCfg[i].valMin)
		{
		    *g_pFixedValueCfg[i].pVal = g_pFixedValueCfg[i].defaultVal; 
		}
	}	

	for (i = 0; i < g_ParameterCfg_Len; i++)
	{
		if (*ParameterCfg[i].pVal > ParameterCfg[i].valMax || *ParameterCfg[i].pVal < ParameterCfg[i].valMin)
		{
		    *ParameterCfg[i].pVal = ParameterCfg[i].defaultVal; 
		}
	}	
}

/**
  * @brief: 写定值/参数.
  * @param:  pData-预置数据指针,直接将报文中定值参数部分数据写入此处包括地址、Tag类型、长度、值
  * @param:  pInfo-定值操作信息
  * @return: TRUE-写入成功  FALSE-写入错误
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DBWriteValue(uint8_t *pData, struct CommonInfo *pInfo)
{
    uint8_t i;
    uint8_t data[256];
    float *pValueTemp;
    static uint8_t sn;

    switch(pInfo->operateType)
    {
    case DB_VALUE_PRESET:
        memcpy(&data, pData, 256);
        sn = pInfo->sn;

		if (g_ValueParaOperateInfo.num >= VALUE_PARAMETER_TOTAL_NUM)
		{
		    memset(&g_ValueParaPresetDB, 0, sizeof(struct ValueParaPreset));
			memset(&g_ValueParaOperateInfo, 0, sizeof(struct ValueParameterOperate ));
			return FALSE;
		}
        for (i = 0; i < pInfo->num; i++)
        {
            memcpy(&g_ValueParaPresetDB.property[i + g_ValueParaOperateInfo.num], data, sizeof(ValueParameterStr)); // 记录定值信息
            memcpy(&g_ValueParaPresetDB.value[g_ValueParaOperateInfo.len], &data[sizeof(ValueParameterStr)], data[3]); // 记录定值        
            g_ValueParaOperateInfo.len += data[3]/sizeof(float);
            memcpy(&data, &data[sizeof(ValueParameterStr) + data[3]], 256);

            if (g_ValueParaPresetDB.property[i].addr >= CALIBRATE_VALUE_START_ADDR && (g_ValueParaPresetDB.property[i].addr <= (CALIBRATE_VALUE_START_ADDR + g_CalibrateFactorCfg_Len)))
            {
                g_ValueParaOperateInfo.calibratFlag = 1;
            }
        }
        g_ValueParaOperateInfo.num += pInfo->num; // 记录预置定值数量
		g_ValueParaOperateInfo.num %= VALUE_PARAMETER_TOTAL_NUM;
        break;
    case DB_VALUE_SOLID:
        for (i = 0; i < g_ValueParaOperateInfo.num; i++)
        {
            /* 获取定值 */
            pValueTemp = GetValueArray(g_ValueParaPresetDB.property[i].addr, sn);

            if (pValueTemp == NULL)
            {
                memcpy(&g_ValueParaPresetDB.value, &g_ValueParaPresetDB.value[g_ValueParaPresetDB.property[i].length/sizeof(float)],\
                    VALUE_PARAMETER_TOTAL_NUM * 4 + 24 - g_ValueParaPresetDB.property[i].length/sizeof(float));				
                break;	// 地址错误
            }
            else
            {
                /* 固化到数据库	*/
                memcpy(pValueTemp, &g_ValueParaPresetDB.value, g_ValueParaPresetDB.property[i].length); // 记录定值
                memcpy(&g_ValueParaPresetDB.value, &g_ValueParaPresetDB.value[g_ValueParaPresetDB.property[i].length/sizeof(float)],\
                    VALUE_PARAMETER_TOTAL_NUM * 4 + 24 - g_ValueParaPresetDB.property[i].length/sizeof(float));
            }
        }

        /* 固化到FRAM */
        ParameterCheck();
        rt_multi_common_data_save_value_to_fram(sn);

        g_ValueParaOperateInfo.num = 0;
        g_ValueParaOperateInfo.len = 0;
        memset(&g_ValueParaPresetDB, 0, sizeof(g_ValueParaPresetDB));
        break;
    case DB_VALUE_REVOKE:
        g_ValueParaOperateInfo.num = 0;
	    g_ValueParaOperateInfo.len = 0;
        memset(&g_ValueParaPresetDB, 0, sizeof(g_ValueParaPresetDB));
        break;
    default:
        break;
    }
    return TRUE;
}

/**
  * @brief: 从数据缓存读出定值/参数.
  * @param:  pAddr-读取数据的地址
  * @param:  pData-返回所读取的定值
  * @param:  num-所读取的定值个数
  * @param:  length-返回的数据字节数
  * @return: TRUE-读取成功  FALSE-读取错误
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DBReadValue(uint16_t *pAddr, uint32_t num, uint8_t *pData, uint8_t sn, uint32_t *length)
{
    uint8_t i;
    uint16_t addr;
    uint32_t len = 0;
    float *pValueTemp;
    ValueParameterStr PropertyTemp;

    for (i = 0; i < num; i++)
    {
        addr = *(pAddr + i);
        /* 获取定值属性 */
        if (addr >= RUNPARAMETER_START_ADDR)
        {
            PropertyTemp.addr = addr;
            PropertyTemp.length = 4;
            PropertyTemp.tag = 38;
        }
        else if (addr >= INTRIPARAME_START_ADDR && addr <= INTRIPARAME_START_ADDR + INHERENT_PARAMETER_NUM)// 对固有参数特殊处理
        {
            PropertyTemp.addr = addr;
            PropertyTemp.length = 24;
            PropertyTemp.tag = 4;
        }
        /* 获取定值 */
        pValueTemp = GetValueArray(addr, sn);

        if (pValueTemp == NULL)
        {
            break;
        }
        else
        {
            memcpy(pData + len, &PropertyTemp, sizeof(ValueParameterStr));
            memcpy(pData + len + sizeof(ValueParameterStr), pValueTemp, PropertyTemp.length);
            len += (sizeof(ValueParameterStr) + PropertyTemp.length);
        }
    }
	
    *length = len;
    return TRUE;
}

/**
  * @brief: 读定值区号
  * @param:  无
  * @return: 当前定值区号
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DBReadValueAreaSN(void)
{
    return g_ValueParaOperateInfo.currentSN;
}

/**
  * @brief: 切换定值区
  * @param:  sn-要切换的定值区号
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DBSwitchValueArea(uint8_t sn)
{
    if (sn == g_ValueParaOperateInfo.currentSN || sn == 0)
    {
        return FALSE;
    }
    else
    {
        (sn == 1) ? (g_pFixedValue = g_FixedValue1, g_pFixedValueCfg = FixedValueCfg1) : (g_pFixedValue = g_FixedValue2, g_pFixedValueCfg = FixedValueCfg2);
        g_ValueParaOperateInfo.currentSN = sn;
        /* 存储当前定值区号 */
		rt_multi_common_data_fram_record_write(CURRENT_SN, &g_ValueParaOperateInfo.currentSN, 1);
        return TRUE;
    }
}

/**
  * @brief: 校准系数计算
  * @param:  num-参数数量
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void CalibrationFactorCal(uint8_t num)
{
    uint8_t i, offset;
    static uint8_t complate = 0;
    static float telemetry[AVERAGE_TIMER];
    static uint8_t counter[TELEMETRY_TOTAL_NUM];

    if (g_ValueParaOperateInfo.calibratFlag)
    {
        for (i = 0; i < num; i++)
        {
            if (g_ValueParaPresetDB.property[i].addr >= CALIBRATE_VALUE_START_ADDR && (g_ValueParaPresetDB.property[i].addr <= (CALIBRATE_VALUE_START_ADDR + g_CalibrateFactorCfg_Len)))
            {
                offset = g_ValueParaPresetDB.property[i].addr - CALIBRATE_VALUE_START_ADDR;
                telemetry[offset] += (*CalibrateFactorCfg[offset].telemetry);
                counter[offset]++;

                if (counter[offset] >= AVERAGE_TIMER)
                {
                    counter[offset] = 0;
                    telemetry[offset] /= (float)AVERAGE_TIMER;

                    if (g_ValueParaPresetDB.property[i].addr >= CALIBRATE_VALUE_START_ADDR + g_CalibrateFactorCfg_Len - sizeof(g_Alpha)/sizeof(float))
                    {
						*CalibrateFactorCfg[offset].factorVal -= (g_ValueParaPresetDB.value[i] - g_Alpha[sizeof(g_Alpha)/sizeof(float) - (CALIBRATE_VALUE_START_ADDR + g_CalibrateFactorCfg_Len - g_ValueParaPresetDB.property[i].addr)]);
                    }
                    else
                    {
                        *CalibrateFactorCfg[offset].factorVal *= (g_ValueParaPresetDB.value[i] / telemetry[offset]);                      
                    }
					
                    g_ValueParaOperateInfo.num = 0;
                    g_ValueParaOperateInfo.len = 0;
                    telemetry[offset] = 0;
                    g_ValueParaPresetDB.property[i].addr = 0;
                    g_ValueParaPresetDB.value[i] = 0;
                    complate = 1;
                }
            }
        }

        if (complate)
        {
            complate = 0;
            g_ValueParaOperateInfo.calibratFlag = 0;
            ParameterCheck();
            /* 固化到FRAM */            
            rt_multi_common_data_save_value_to_fram(DB_CALI);
            
            memset(&g_ValueParaPresetDB, 0, sizeof(g_ValueParaPresetDB));
        }
    }
}

/**
  * @brief: 将SOE写入数据缓存.
  * @param:  addr-地址
  * @param:  state-值
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
rt_uint8_t DBWriteSOE(uint16_t addr, rt_uint8_t state)
{
    rt_uint32_t i;
    rt_uint16_t value,valuetemp;
    ListElmt *element;
    rt_uint8_t Property;
    uint16_t newaddr;
    
    if (state == g_TelesignalDB[addr])
    {
        return FALSE;
    }
    g_TelesignalDB[addr] = state;
    g_SOEDB[g_FlagDB.queue_soe.in].addr = addr + TELESIGNAL_START_ADDR;		

    g_SOEDB[g_FlagDB.queue_soe.in].value = state;
    g_SOEDB[g_FlagDB.queue_soe.in].time.year = g_SystemTime.year;
    g_SOEDB[g_FlagDB.queue_soe.in].time.month = g_SystemTime.month;
    g_SOEDB[g_FlagDB.queue_soe.in].time.dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
    g_SOEDB[g_FlagDB.queue_soe.in].time.hour = g_SystemTime.hour;
    g_SOEDB[g_FlagDB.queue_soe.in].time.minute = g_SystemTime.minute;
    g_SOEDB[g_FlagDB.queue_soe.in].time.msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
    g_SOEDB[g_FlagDB.queue_soe.in].time.msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);

    if (++(g_FlagDB.queue_soe.in) >= SOE_MAX_NUM)
    {
        g_FlagDB.queue_soe.in = 0;
        g_FlagDB.queue_soe.full = FULL;
    }
    
    for (i = 0; i < DEV_MAX_NUM; i++)
    {
        if ((g_FlagDB.queue_soe.in) == g_FlagDB.queue_soe.out[i])
        if (++(g_FlagDB.queue_soe.out[i]) >= SOE_MAX_NUM)
        {
            g_FlagDB.queue_soe.out[i] = 0;
        }
    }

    //*MemoryCounter.soe = DB_COUNTER_EN;	
    
    if (addr < TELESIGNAL_TOTAL_NUM)
    {
        if(g_NewListTelesignal[addr].size != 0)//链表不为空
        {
            element = g_NewListTelesignal[addr].head;
            do//遍历链表
            {
                for(value=0,i=0;i<((((rt_uint16_t*)(element->data))[1])>>NEWONEYX_NUM);i++)
                {
                    valuetemp = g_TelesignalDB[(((((rt_uint16_t*)(element->data))[2+i])>>NEWONEYX_ADDR)&NEWJUDG_ADDR)] - 1;//单点
                    if((((rt_uint16_t*)(element->data))[2+i]>>NEWONEYX_CAL>>NEWCAL_NEG)&NEWPROPERTY_JUDG)
                    {
                        valuetemp = (~valuetemp)&0x01;                
                    }
                    if(((((rt_uint16_t*)(element->data))[2+i]>>NEWONEYX_CAL>>NEWCAL_AND)&NEWPROPERTY_JUDG) == NEWJUDG_AND)
                    {
                        value &= valuetemp;                 
                    }  
                    else
                    {
                        value |= valuetemp;  
                    }      
                }
                
                value = value + 1;//双点
                
                if(value == ((((rt_uint16_t*)(element->data))[0])>>NEWONEYX_VAULE))//如果组合遥信未发生变化
                {
                    element = element->next;
                    continue;
                }
                
                (((rt_uint16_t*)(element->data))[0]) = (value<<NEWONEYX_VAULE)|((((rt_uint16_t*)(element->data))[0])&(NEWJUDG_ADDR<<NEWONEYX_ADDR));
                
                newaddr = ((((rt_uint16_t*)(element->data))[0])>>NEWONEYX_ADDR)&NEWJUDG_ADDR;
                
                Property = (((rt_uint16_t*)(element->data))[1])>>NEWONEYX_PROPERTY;
                
                if((Property>>NEWPROPERTY_SOE)&NEWPROPERTY_JUDG)//上送SOE
                {
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].addr = newaddr;           
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].value = value;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.year = g_SystemTime.year;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.month = g_SystemTime.month;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.hour = g_SystemTime.hour;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.minute = g_SystemTime.minute;
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
                    g_SOENewDB[g_FlagDB.queue_soe_new.in].time.msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
                    
                    if (++(g_FlagDB.queue_soe_new.in) >= SOE_NEW_MAX_NUM)
                    {
                        g_FlagDB.queue_soe_new.in = 0;
                        g_FlagDB.queue_soe_new.full = FULL;
                    }
                    
                    if(g_FlagDB.queue_soe_new.in != g_FlagDB.queue_soe_new.out[SLAVE104_ID1])
                    {
                        g_FlagDB.queue_soe_new.in = g_FlagDB.queue_soe_new.in;
                    }
                    
                    for (i = 0; i < DEV_MAX_NUM; i++)
                    {
                        if ((g_FlagDB.queue_soe_new.in) == g_FlagDB.queue_soe_new.out[i])
                        if (++(g_FlagDB.queue_soe_new.out[i]) >= SOE_NEW_MAX_NUM)
                        {
                            g_FlagDB.queue_soe_new.out[i] = 0;
                        }
                    }
                }
                
                if((Property>>NEWPROPERTY_COS)&NEWPROPERTY_JUDG)//上送COS    
                {
                    g_COSDB[g_COSDBIn].addr = newaddr;
                    g_COSDB[g_COSDBIn].value = value;

                    if (++g_COSDBIn >= COS_MAX_NUM)
                    {
                        g_COSDBIn = 0;
                    }
                }
                
                element = element->next;
            }
            while(element != NULL); 
        }
    }	
    return TRUE;
}

/**
  * @brief: 将操作记录写入数据缓存.
  * @param:  addr-地址
  * @param:  state-值
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
rt_uint8_t DBWriteCO(uint16_t addr, rt_uint8_t state)
{
    rt_uint32_t i;

    if (addr >= ADDR_LOGIC_ACT)
    {
        return FALSE;
    }
    
    g_CoDB[g_FlagDB.queue_co.in].addr = addr + REMOTE_START_ADDR;
    g_CoDB[g_FlagDB.queue_co.in].value = state;
    g_CoDB[g_FlagDB.queue_co.in].time.year = g_SystemTime.year;
    g_CoDB[g_FlagDB.queue_co.in].time.month = g_SystemTime.month;
    g_CoDB[g_FlagDB.queue_co.in].time.dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
    g_CoDB[g_FlagDB.queue_co.in].time.hour = g_SystemTime.hour;
    g_CoDB[g_FlagDB.queue_co.in].time.minute = g_SystemTime.minute;
    g_CoDB[g_FlagDB.queue_co.in].time.msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
    g_CoDB[g_FlagDB.queue_co.in].time.msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);

    if (++(g_FlagDB.queue_co.in) >= CO_MAX_NUM)
    {
        g_FlagDB.queue_co.in = 0;
        g_FlagDB.queue_co.full = FULL;
    }
    
    for (i = 0; i < DEV_MAX_NUM; i++)
    {
        if((g_FlagDB.queue_co.in) == g_FlagDB.queue_co.out[i])
        if (++(g_FlagDB.queue_co.out[i]) >= CO_MAX_NUM)
        {
            g_FlagDB.queue_co.out[i] = 0;
        }
    }
	
    return TRUE;
}

/**
  * @brief: 将操作记录写入数据缓存.
  * @param:  addr-地址
  * @param:  state-值
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
rt_uint8_t DBWriteFEVENT(rt_uint16_t yx_addr, rt_uint16_t *yc_addr, rt_uint16_t yc_num)
{
    rt_uint32_t i;
    
    memset(&g_FeventDB[g_FlagDB.queue_fevent.in],0,sizeof(g_FeventDB[g_FlagDB.queue_fevent.in]));

    g_FeventDB[g_FlagDB.queue_fevent.in].yx_type = COMMON_DATA_M_DP_NA_1;
    g_FeventDB[g_FlagDB.queue_fevent.in].yx_num = 1;
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].addr = yx_addr + TELESIGNAL_START_ADDR; 
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].value = g_TelesignalDB[yx_addr];
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.year = g_SystemTime.year;
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.month = g_SystemTime.month;    
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.hour = g_SystemTime.hour; 
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.minute = g_SystemTime.minute; 
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond); 
    g_FeventDB[g_FlagDB.queue_fevent.in].yx[0].time.msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
    g_FeventDB[g_FlagDB.queue_fevent.in].yc_type = COMMON_DATA_M_ME_NC_1;
    g_FeventDB[g_FlagDB.queue_fevent.in].yc_num = yc_num;
        
    for (i = 0; i < yc_num; i++)
    {
        g_FeventDB[g_FlagDB.queue_fevent.in].yc[i].addr = yc_addr[i] + TELEMETRY_START_ADDR;
        g_FeventDB[g_FlagDB.queue_fevent.in].yc[i].value = g_TelemetryDB[yc_addr[i] - TELEMETRY_START_ADDR];         
    }

    if (++g_FlagDB.queue_fevent.in >= FEVENT_MAX_NUM)
    {
        g_FlagDB.queue_fevent.in = 0;
        g_FlagDB.queue_fevent.full = FULL;
    }
    
    for (i = 0; i < DEV_MAX_NUM; i++)
    {
        if((g_FlagDB.queue_fevent.in) == g_FlagDB.queue_fevent.out[i])
        if (++(g_FlagDB.queue_fevent.out[i]) >= FEVENT_MAX_NUM)
        {
            g_FlagDB.queue_fevent.out[i] = 0;
        }
    }
	
    return TRUE;
}

/**
  * @brief: 将变化遥测写入数据缓存.
  * @param:  pWriteData-写入数据指针
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
rt_uint8_t DBWriteNVA(struct NVA_Str *pWriteData, uint8_t num)
{
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        g_NVADB[g_NVADBIn] = *pWriteData;

        if (++g_NVADBIn >= NVA_MAX_NUM)
        {
            g_NVADBIn = 0;
        }
    }
    return TRUE;
}

/**
  * @brief: 清除变化遥测.
  * @param:  
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void DBClearNVA(void)
{
    uint8_t i;
    
    for (i = 1; i <= ((float *)&(g_Parameter[DEADZONE_DC1])) - ((float *)&(g_Parameter[DEADZONE_F])); i++)
    {
        g_TelemetryLastDB[i] = g_TelemetryDB[i];  
    }
}

/**
  * @brief: 检查是否有变化遥测.
  * @param: 无.
  * @return: TRUE-有  FALSE-无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
uint8_t DB_NVA_Check(void)
{
    uint8_t i;
    uint8_t rlt = FALSE;
    static uint8_t s_Counter;

    s_Counter++;
    if (s_Counter > 200)
    {
        s_Counter = 0;
//        ParameterCheck(DEADZONE);

        for (i = 1; i <= ((float *)&(g_Parameter[DEADZONE_DC1])) - ((float *)&(g_Parameter[DEADZONE_F])); i++)
        {
            if (fabsf((float)g_TelemetryDB[i] - (float)g_TelemetryLastDB[i]) > 
                *((float *)&(g_Parameter[DEADZONE_F]) + i) / 100.0f * RatedValue[i])
//                g_ParameterDB.Data.runPara.buf[DEAD_ZONE_INDEX + i] / 100.0f * RatedValue[i])
            {
                g_NVADB[g_NVADBIn].addr = TELEMETRY_START_ADDR + i;
                g_NVADB[g_NVADBIn].value = g_TelemetryDB[i];

                if (++g_NVADBIn >= NVA_MAX_NUM)
                {
                    g_NVADBIn = 0;
                }
                g_NVADB[g_NVADBIn].addr = TELEMETRY_START_ADDR + i + (&g_TelemetryDB[ADDR_Ia_ONCE] - &g_TelemetryDB[ADDR_IA]);
                g_NVADB[g_NVADBIn].value = g_TelemetryDB[i + (&g_TelemetryDB[ADDR_Ia_ONCE] - &g_TelemetryDB[ADDR_IA])];

                if (++g_NVADBIn >= NVA_MAX_NUM)
                {
                    g_NVADBIn = 0;
                }
                
                g_TelemetryLastDB[i] = g_TelemetryDB[i];                
            }
        }
    }

    return rlt;
}

/**
  * @brief: 读取系统时间
  * @param:  pTime-读出的系统时间结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void DBReadSystemTime(struct CP56Time2a_t *pTime)
{
    pTime->year = g_SystemTime.year;
    pTime->month = g_SystemTime.month;
    pTime->dayofWeek = g_SystemTime.day | g_SystemTime.week << 5;
    pTime->hour	= g_SystemTime.hour;
    pTime->minute = g_SystemTime.minute;
    pTime->msecondH = HIBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
    pTime->msecondL = LOBYTE(g_SystemTime.second * 1000 + g_SystemTime.msecond);
}

/**
  * @brief: 修改系统时间
  * @param:  time-读出的系统时间结构体
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void DBWriteSystemTime(struct CP56Time2a_t *pTime)
{   
    g_SystemTime.year = pTime->year;
    g_SystemTime.month = pTime->month;
    g_SystemTime.day = pTime->dayofWeek & 0x1F;
    g_SystemTime.week = (pTime->dayofWeek >> 5) & 0x1F;
    g_SystemTime.hour = pTime->hour;
    g_SystemTime.minute = pTime->minute;
    g_SystemTime.second = MAKEWORD(pTime->msecondL, pTime->msecondH) / 1000;
    g_SystemTime.msecond = MAKEWORD(pTime->msecondL, pTime->msecondH) % 1000;

    rt_device_write(device_sd2405, 0x00, (uint8_t *)&g_SystemTime, sizeof(struct SD2405Time));
}

/**
  * @brief : 复归
  * @param : act-动作源 REMOTE/LOCAL/
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void DBRevert(uint16_t addr)
{
    if (g_TelesignalDB[ADDR_DEVICE_FAULT] == ON)
    {
        DBWriteSOE(ADDR_DEVICE_FAULT, OFF);
    }

    BreakerCtrlReset(BRE_DEV0);
    LoadSwitchCtrlReset(BRE_DEV0);

    rt_hw_alarm_led_operate(OFF);	// 底部告警灯灭

    DBWriteCO(addr, ON);
    
    DBWriteSOE(ADDR_BATTERY_FAULT_ALARM, OFF); // 电池故障复归    
}

/**
  * @brief : 清除记录
  * @param : act-清除记录 REMOTE/LOCAL/
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
static void DBClear(uint16_t addr)
{
    rt_device_control(device_fram, FM_CLEAR_RECORD, RT_NULL);       
    g_CommunicatFlag[COM_FILE] |= COMMUNICATLOCKCLEAR; 
	
	DBWriteCO(addr, ON);
}

/**
  * @brief : 遥控操作（包括远程与本地操作）
  * @param : [addr]-遥控地址
  * @param : [operate_type]-操作类型（包括DO_CLOSE/DO_CLOSE_RECOVERY/DO_OPEN/DO_OPEN_RECOVERY/DO_COIL_ENERGY_STORAGE/DO_ALARM_LED/DO_BATTERY_ACTIVE/
                           DO_BATTERY_ACTIVE_RECOVERY/DO_BATTERY_ACTIVE_END/DO_BATTERY_ACTIVE_END_RECOVERY/DO_BATTERY_DROP_OUT/DO_BATTERY_DROP_OUT_RECOVERY）
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_telecontrl_operate(uint16_t addr, uint8_t operate_type)
{
    switch (addr)     
	{
		case ADDR_REMOTE_OPERATE:
	    case ADDR_REMOTE_ACTIVE:
        case ADDR_LOCAL_OPERATE:		
		case ADDR_HANDHELD_OPER:			
			rt_hw_do_operate(addr, operate_type);
			break;
		case ADDR_REMOTE_RESET:		
		case ADDR_LOCAL_RESET:
			DBRevert(addr);
			break;
		case ADDR_REMOTE_CLEAR:
		case ADDR_LOCAL_CLEAR:
			DBClear(addr);
			break;	   		
	}
}
/* FRAM ------------------------------------------------------------------*/
/**
  * @Description: 向Fram存储定值
  * @param:  sn-定值区号
  * @return: rlt: TRUE-写入成功 FALSE-写入失败
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_data_save_value_to_fram(uint8_t sn)
{
    uint32_t addr;
    uint32_t len;
    float *pInfo;
	
    switch (sn)
    {
    case DB_SN0:
        len = sizeof(g_Parameter);
        addr = ADDR_FRAM_AREA0;
        pInfo = GetValueArray(RUNPARAMETER_START_ADDR, sn);
        break;
    case DB_SN1:
        len = sizeof(g_FixedValue1);
        addr = ADDR_FRAM_AREA1;
        pInfo = GetValueArray(FIXED_VALUE_START_ADDR, sn);
        break;
    case DB_SN2:
        len = sizeof(g_FixedValue2);
        addr = ADDR_FRAM_AREA2;
        pInfo = GetValueArray(FIXED_VALUE_START_ADDR, sn);
        break;
    case DB_CALI:
        len = sizeof(g_CalibrateFactor);
        addr = ADDR_FRAM_CALI_FACTOR;
        pInfo = GetValueArray(CALIBRATE_FACTOR_START_ADDR, sn);
        break;	
    default:
        break;
    }
	
    rt_device_write(device_fram, addr, (uint8_t *)pInfo, len);
}

/**
  * @Description: 从Frame中读取定值属性
  * @param:  sn-定值区号
  * @return: rlt: TRUE-读取成功 FALSE-读取失败
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_data_get_value_from_fram(uint8_t sn)
{
    uint32_t addr;
    uint32_t len;
    float *pInfo;

    switch (sn)
    {
    case DB_SN0:
        len = sizeof(g_Parameter);
        addr = ADDR_FRAM_AREA0;
        pInfo = GetValueArray(RUNPARAMETER_START_ADDR, sn);
        break;
    case DB_SN1: 
        len = sizeof(g_FixedValue1);
        addr = ADDR_FRAM_AREA1;
        pInfo = GetValueArray(FIXED_VALUE_START_ADDR, sn);
        break;
    case DB_SN2:
        len = sizeof(g_FixedValue2);
        addr = ADDR_FRAM_AREA2;
        pInfo = GetValueArray(FIXED_VALUE_START_ADDR, sn);
        break;
    default:
        break;
    }
    rt_device_read(device_fram, addr, (uint8_t *)pInfo, len);
}

/**
  * @brief:  第一次上电向Fram存储缺省定值
  * @param:  [none]
  * @return: [none]
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_common_data_save_value_default_to_fram(void)
{
	uint32_t i;
	
    for (i = 0; i < g_ParameterCfg_Len; i++)
	{
	    *ParameterCfg[i].pVal = ParameterCfg[i].defaultVal;
	}
	
    for (i = 0; i < g_CalibrateFactorCfg_Len; i++)
	{
	    *CalibrateFactorCfg[i].factorVal = CalibrateFactorCfg[i].factorDefault;
	}

    for (i = 0; i < g_FixedValueCfg1_Len; i++)
	{
	    *FixedValueCfg1[i].pVal = g_pFixedValueCfg[i].defaultVal;
	}	

	for (i = 0; i < g_FixedValueCfg2_Len; i++)
	{
	    *FixedValueCfg2[i].pVal = g_pFixedValueCfg[i].defaultVal;
	}
	
	for (i = 0; i < 4; i++)
	{
	    rt_multi_common_data_save_value_to_fram(i);
	}   	
}

/**
  * @Description: FRAM数据记录写入
  * @param:  type-数据类型
  * @param:  pBuf-写入的数据
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
rt_uint16_t rt_multi_common_data_fram_record_write(uint8_t type, uint8_t *pBuf, uint32_t len)
{
    switch(type)
    {        
        case TELESIGNAL:
            rt_device_write(device_fram, ADDR_FRAM_TELISIGNAL, pBuf, len);
            break;

        case SOE_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_SOE, pBuf, len);
            break;
        
        case SOE_NEW_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_SOE_NEW, pBuf, len);
            break;
        
        case FEVENT_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_FEVENT, pBuf, len);
            break;

        case CO_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_CO, pBuf, len);			
            break;

        case LOG_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_LOG, pBuf, len);			
            break;

        case MEMORY_FLAG:
            rt_device_write(device_fram, ADDR_FRAM_MEMORY, pBuf, len);							
            break;
		
        case CFG_RECODE:
            rt_device_write(device_fram, ADDR_FRAM_CONFIG, pBuf, len);	
            break;
        case CURRENT_SN: // 当前定值区号	
            rt_device_write(device_fram, ADDR_FRAM_CURRENT_SN, pBuf, len);            						
            break;		
        default:
            break;
    }

    return 0;
}

/**
  * @brief: FRAM在线更新状态写入
  * @param:  type-状态类型
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_data_fram_update_state_write(uint8_t type)
{
	uint8_t update_state;
	
    switch (type)
	{
	    case UPDATE_START:
			update_state = 0x01;
		    break;
	    case UPDATE_COMPLATE:
			update_state = 0x02;
		    break;
	    case UPDATE_BACK:
			update_state = 0x20;
		    break;
	    case RUN_NOMAL:
			update_state = 0x08;
		    break;
	    default:			
		    break;		
	}
	rt_device_write(device_fram, (ADDR_FRAM_UPDATE + 1), &update_state, 1);
}

/**
  * @Description: FRAM数据记录读取
  * @param:  type-数据类型
  * @param:  pBuf-读取的数据
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_data_fram_record_read(uint8_t type, uint8_t *pBuf)
{
    switch(type)
    {        
        case TELESIGNAL:
            rt_device_read(device_fram, ADDR_FRAM_TELISIGNAL, pBuf, TELESIGNAL_TOTAL_NUM);
            break;

        case SOE_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_SOE, pBuf, sizeof(g_SOEDB));
            break;

        case SOE_NEW_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_SOE_NEW, pBuf, sizeof(g_SOENewDB));
            break;
        
        case FEVENT_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_FEVENT, pBuf, sizeof(g_FeventDB));
            break;

        case CO_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_CO, pBuf, sizeof(g_CoDB));		
            break;

        case LOG_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_LOG, pBuf, sizeof(g_UlogDB));				
            break;

        case MEMORY_FLAG:
            rt_device_read(device_fram, ADDR_FRAM_MEMORY, pBuf, sizeof(g_FlagDB));						
            break;
		
        case CFG_RECODE:
            rt_device_read(device_fram, ADDR_FRAM_CONFIG, pBuf, sizeof(struct ConfigurationSetDatabase));									
            break;
       
        default:
            break;
    }
}

/**
  * @Description: 掉电存储
  * @param:  无
  * @return: 无
  * @updata: [YYYY-MM-DD] [更改人姓名][变更描述]
  */
void rt_multi_common_data_powerdown_storage(void)
{
    rt_multi_common_data_fram_record_write(TELESIGNAL, (uint8_t *)&g_TelesignalDB, sizeof(g_TelesignalDB));
    rt_multi_common_data_fram_record_write(SOE_RECODE, (uint8_t *)&g_SOEDB, sizeof(g_SOEDB));
    rt_multi_common_data_fram_record_write(SOE_NEW_RECODE, (uint8_t *)&g_SOENewDB, sizeof(g_SOENewDB));
    rt_multi_common_data_fram_record_write(FEVENT_RECODE, (uint8_t *)&g_FeventDB, sizeof(g_FeventDB));
    rt_multi_common_data_fram_record_write(CO_RECODE, (uint8_t *)&g_CoDB, sizeof(g_CoDB));
    rt_multi_common_data_fram_record_write(LOG_RECODE, (uint8_t *)&g_UlogDB, sizeof(g_UlogDB));
    rt_multi_common_data_fram_record_write(MEMORY_FLAG, (uint8_t *)&g_FlagDB, sizeof(g_FlagDB));
}

/* INIT ------------------------------------------------------------------*/

/**
  * @brief : configure default.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][J.Lee][Make the code cleanup]
  */
void rt_multi_common_data_configure_default(void)
{
    uint16_t i;
    
    g_ConfigurationSetDB->YXSetNum = TELESIGNAL_TOTAL_NUM;
    
    for(i=0; i<TELESIGNAL_TOTAL_NUM;i++)
    {
        g_ConfigurationSetDB->YXSet[2*i] = (1<<NEWONEYX_NUM)|(COMMON_DATA_M_SP_NA_1<<NEWPROPERTY_TI)|(1<<NEWPROPERTY_SOE)|(1<<NEWPROPERTY_COS);
        g_ConfigurationSetDB->YXSet[2*i + 1] = (0<<NEWONEYX_CAL)|((TELESIGNAL_START_ADDR + i)<<NEWONEYX_ADDR);
    }
    
    for(i=0;i<TELEMETRY_TOTAL_NUM;i++)
    {
        g_ConfigurationSetDB->YCAddr[i] = TELEMETRY_START_ADDR + i;
        g_ConfigurationSetDB->YCProperty[i] = 2<<NEWPROPERTY_TI;
        g_ConfigurationSetDB->YCMultipleRate[i] = 1;
    }
    
    for(i=0;i<sizeof(g_ConfigurationSetDB->YKAddr)/sizeof(uint16_t);i++)
    {
        g_ConfigurationSetDB->YKAddr[i] = REMOTE_START_ADDR + i;
        g_ConfigurationSetDB->YKProperty[i] = 0;
    }

    memcpy(&g_ConfigurationSetDB->ID_Value, (char *)&("F31xxxxxxxxx20171211xxxx"), sizeof("F31xxxxxxxxx20171211xxxx"));   
        
    rt_multi_common_data_fram_record_write(CFG_RECODE, (uint8_t *)g_ConfigurationSetDB, sizeof(struct ConfigurationSetDatabase));    
}

/**
  * @brief : common data read from fram.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][J.Lee][Make the code cleanup]
  */
void rt_multi_common_data_read_config_from_fram(void)
{
    uint32_t i,j,temp1;
    uint8_t sn =0,configureFault=0;

    /* 读取遥信 */
	rt_multi_common_data_fram_record_read(TELESIGNAL, (uint8_t *)&g_TelesignalDB);
    
    for (i = 0; i < DI_NUM; i++)
    {
        g_DiCollect.stateLast |= g_TelesignalDB[i];   	
    }

    /* 读取SOE */
    //FM25VxxReadData(ADDR_FRAM_SOE, NULL, (uint8_t *)g_SOEDB, sizeof(g_SOEDB));
    rt_multi_common_data_fram_record_read(SOE_RECODE, (uint8_t *)&g_SOEDB);
    rt_multi_common_data_fram_record_read(SOE_NEW_RECODE, (uint8_t *)&g_SOENewDB);
		
    /* 读取故障事件 */
    //FM25VxxReadData(ADDR_FRAM_FEVENT, NULL, (uint8_t *)g_FeventDB, sizeof(g_FeventDB));
    rt_multi_common_data_fram_record_read(FEVENT_RECODE, (uint8_t *)&g_FeventDB);
		
    /* 读取操作记录 */
	//FM25VxxReadData(ADDR_FRAM_CO, NULL, (uint8_t *)g_CoDB, sizeof(g_CoDB));	
    rt_multi_common_data_fram_record_read(CO_RECODE, (uint8_t *)&g_CoDB);	
		
    /* 读取日志 */
    //FM25VxxReadData(ADDR_FRAM_LOG, NULL, (uint8_t *)g_UlogDB, sizeof(g_UlogDB));			
    rt_multi_common_data_fram_record_read(LOG_RECODE, (uint8_t *)&g_UlogDB);
		
    /* 读取记忆数据 */
    //FM25VxxReadData(ADDR_FRAM_MEMORY, NULL, (uint8_t *)&g_FlagDB, sizeof(g_FlagDB));	
    rt_multi_common_data_fram_record_read(MEMORY_FLAG, (uint8_t *)&g_FlagDB);
		
    /* 读取配置文件 */
    //FM25VxxReadData(ADDR_FRAM_CONFIG, NULL, (uint8_t *)&, sizeof(struct ConfigurationSetDatabase));
    rt_multi_common_data_fram_record_read(CFG_RECODE, (uint8_t *)g_ConfigurationSetDB);
    
    for(i=0,temp1=0;((i<g_ConfigurationSetDB->YXSetNum)&&(configureFault == 0));i++)//检查遥信
    {
        for(j=0;j<(g_ConfigurationSetDB->YXSet[temp1]>>NEWONEYX_NUM);j++)
        {
            if(!((((g_ConfigurationSetDB->YXSet[temp1+1+j]>>NEWONEYX_ADDR)&NEWJUDG_ADDR)>=0)&&(((g_ConfigurationSetDB->YXSet[temp1+1+j]>>NEWONEYX_ADDR)&NEWJUDG_ADDR) < TELEMETRY_TOTAL_NUM)))
            {
                configureFault = 1;
                break;
            }
        }
        temp1 += (g_ConfigurationSetDB->YXSet[temp1]>>NEWONEYX_NUM) + 1;
    }
    for(;temp1<sizeof(g_ConfigurationSetDB->YXSet)/sizeof(uint16_t);temp1++)
    {
        g_ConfigurationSetDB->YXSet[temp1] = 0;
    }
    
    for(i=0;((i<sizeof(g_ConfigurationSetDB->YCAddr)/sizeof(uint16_t))&&(configureFault == 0));i++)//检查遥测
    {
        if(!((g_ConfigurationSetDB->YCAddr[i]>=TELEMETRY_START_ADDR)&&(g_ConfigurationSetDB->YCAddr[i]<TELEMETRY_START_ADDR+TELEMETRY_TOTAL_NUM)))
        {
            if(g_ConfigurationSetDB->YCAddr[i] != 0)
            {
                configureFault = 1;
                break;
            }
        }
    }
 
    for(i=0;((i<sizeof(g_ConfigurationSetDB->YKAddr)/sizeof(uint16_t))&&(configureFault == 0));i++)//检查遥控
    {
        if(!((g_ConfigurationSetDB->YKAddr[i]>=REMOTE_START_ADDR)&&(g_ConfigurationSetDB->YKAddr[i]<REMOTE_START_ADDR+REMOTE_TOTAL_NUM)))
        {
            if(g_ConfigurationSetDB->YKAddr[i] != 0)
            {
                configureFault = 1;
                break;
            }
        }
    }
    
    if(configureFault == 1)
    {
        rt_multi_common_data_configure_default();
    }
    
    
		
    /* 读取定值区 */
    //FM25VxxReadData(ADDR_FRAM_CURRENT_SN, NULL, &sn, 1);
    rt_multi_common_data_fram_record_read(CURRENT_SN, (uint8_t *)&sn);
		
    if (sn != 1 || sn != 2)
    {
        sn = 1; // 如果没有存储定值区  默认为1区
    }
    g_ValueParaOperateInfo.currentSN = sn;
    
    //初始化2区
    for(i=0;i<g_FixedValueCfg2_Len;i++)
    {
        FixedValueCfg2[i] = FixedValueCfg1[i];
        FixedValueCfg2[i].pVal = &g_FixedValue2[FixedValueCfg1[i].pVal - g_FixedValue1];
    }
    
    /* 读取当前定值区号 */
    if (g_ValueParaOperateInfo.currentSN == 2)
    {
        //g_pFixedValue = &g_FixedValueDB2;
		g_pFixedValue = g_FixedValue2; 
		g_pFixedValueCfg = FixedValueCfg2;
    }
    else // 默认1区
    {
        g_ValueParaOperateInfo.currentSN = 1;
        //g_pFixedValue = &g_FixedValueDB1;
		g_pFixedValue = g_FixedValue1;
		g_pFixedValueCfg = FixedValueCfg1;
    } 
    
    
    /* 读取定值 */
    for (i = 0; i < 4; i++)
    {
        rt_multi_common_data_get_value_from_fram(i); // 读取定值参数
    }
	
	ParameterCheck();
}

/**
  * @brief : FTU system fixed parameters initialization.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][Make the code cleanup]
  */
void rt_multi_common_data_para_init(void)
{
//    g_EthDP83848 = (struct lwip_dev)RT_ETH1_CONFIG_DEFAULT;
//    g_EthW5500 = (struct lwip_dev)RT_ETH2_CONFIG_DEFAULT;
	
    //不同
    g_EthDP83848.ip[0] = (uint8_t)g_Parameter[NET_IP1_0];
    g_EthDP83848.ip[1] = (uint8_t)g_Parameter[NET_IP1_1];
    g_EthDP83848.ip[2] = (uint8_t)g_Parameter[NET_IP1_2];
    g_EthDP83848.ip[3] = (uint8_t)g_Parameter[NET_IP1_3];
    
    g_EthW5500.ip[0] = (uint8_t)g_Parameter[NET_IP2_0];
    g_EthW5500.ip[1] = (uint8_t)g_Parameter[NET_IP2_1];
    g_EthW5500.ip[2] = (uint8_t)g_Parameter[NET_IP2_2];
    g_EthW5500.ip[3] = (uint8_t)g_Parameter[NET_IP2_3];
    
    g_EthDP83848.mac[0] = 0x00;
    g_EthDP83848.mac[1] = 0x80;
    g_EthDP83848.mac[2] = 0xE1;
    g_EthDP83848.mac[3] = *(rt_uint8_t*)(UID_BASE + 4);
    g_EthDP83848.mac[4] = *(rt_uint8_t*)(UID_BASE + 2);
    g_EthDP83848.mac[5] = *(rt_uint8_t*)(UID_BASE + 0);
	
    g_EthW5500.mac[0] = 0x00;
    g_EthW5500.mac[1] = 0x80;
    g_EthW5500.mac[2] = 0xE1;
    g_EthW5500.mac[3] = *(rt_uint8_t*)(UID_BASE + 0);
    g_EthW5500.mac[4] = *(rt_uint8_t*)(UID_BASE + 2);
    g_EthW5500.mac[5] = *(rt_uint8_t*)(UID_BASE + 4);
    
    //相同
    g_EthDP83848.netmask[0] = (uint8_t)g_Parameter[NET_NETMASK_0];
    g_EthDP83848.netmask[1] = (uint8_t)g_Parameter[NET_NETMASK_1];
    g_EthDP83848.netmask[2] = (uint8_t)g_Parameter[NET_NETMASK_2];
    g_EthDP83848.netmask[3] = (uint8_t)g_Parameter[NET_NETMASK_3];
    g_EthDP83848.gateway[0] = (uint8_t)g_Parameter[NET_GATEWAY_0];
    g_EthDP83848.gateway[1] = (uint8_t)g_Parameter[NET_GATEWAY_1];
    g_EthDP83848.gateway[2] = (uint8_t)g_Parameter[NET_GATEWAY_2];
    g_EthDP83848.gateway[3] = (uint8_t)g_Parameter[NET_GATEWAY_3];
    g_EthDP83848.dns[0] = (uint8_t)g_Parameter[NET_DNS_0];
    g_EthDP83848.dns[1] = (uint8_t)g_Parameter[NET_DNS_1];
    g_EthDP83848.dns[2] = (uint8_t)g_Parameter[NET_DNS_2];
    g_EthDP83848.dns[3] = (uint8_t)g_Parameter[NET_DNS_3];
    g_EthDP83848.remoteip[0] = 192;
    g_EthDP83848.remoteip[1] = 168;
    g_EthDP83848.remoteip[2] = 60;
    g_EthDP83848.remoteip[3] = 115;
    g_EthDP83848.dhcpstatus = 0;
    
    g_EthW5500.netmask[0] = (uint8_t)g_Parameter[NET_NETMASK_0];
    g_EthW5500.netmask[1] = (uint8_t)g_Parameter[NET_NETMASK_1];
    g_EthW5500.netmask[2] = (uint8_t)g_Parameter[NET_NETMASK_2];
    g_EthW5500.netmask[3] = (uint8_t)g_Parameter[NET_NETMASK_3];
    g_EthW5500.gateway[0] = (uint8_t)g_Parameter[NET_GATEWAY_0];
    g_EthW5500.gateway[1] = (uint8_t)g_Parameter[NET_GATEWAY_1];
    g_EthW5500.gateway[2] = (uint8_t)g_Parameter[NET_GATEWAY_2];
    g_EthW5500.gateway[3] = (uint8_t)g_Parameter[NET_GATEWAY_3];
    g_EthW5500.dns[0] = (uint8_t)g_Parameter[NET_DNS_0];
    g_EthW5500.dns[1] = (uint8_t)g_Parameter[NET_DNS_1];
    g_EthW5500.dns[2] = (uint8_t)g_Parameter[NET_DNS_2];
    g_EthW5500.dns[3] = (uint8_t)g_Parameter[NET_DNS_3];

    g_EthW5500.remoteip[0] = 192;
    g_EthW5500.remoteip[1] = 168;
    g_EthW5500.remoteip[2] = 60;
    g_EthW5500.remoteip[3] = 115;
    g_EthW5500.dhcpstatus = 0;
}

/**
  * @brief : common data read from fram.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][J.Lee][Make the code cleanup]
  */
void rt_multi_common_data_read_config(void)
{
    rt_multi_common_data_read_config_from_fram();
    
//    rt_multi_common_data_configure_default();//临时
//    #warning "please cancle after test!" 
          
    rt_multi_common_data_para_init();
}
    
/**  
  * @brief : common data config.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][J.Lee][Make the code cleanup]
  */
void rt_multi_common_data_config(void)
{    
    rt_uint32_t i,j,temp1,temp2;
    rt_uint8_t addr,value;
    char buf[24];
    /* 北京双杰汉字编码 */
    char buf1[8] = {0xB1, 0xB1, 0xBE, 0xA9, 0xCB, 0xAB, 0xBD, 0xDC};
    char tempstr[24];
	
//    g_InherentPara = RT_SYS_CONFIG_DEFAULT;	  

    sprintf(tempstr,"%02X",(g_EthDP83848.mac[0]));
    strcpy(buf,tempstr);
    strcat(buf,".");
    sprintf(tempstr,"%02X",(g_EthDP83848.mac[1]));
    strcat(buf,tempstr);
    strcat(buf,".");
    sprintf(tempstr,"%02X",(g_EthDP83848.mac[2]));
    strcat(buf,tempstr);
    strcat(buf,".");
    sprintf(tempstr,"%02X",(g_EthDP83848.mac[3]));
    strcat(buf,tempstr);
    strcat(buf,".");
    sprintf(tempstr,"%02X",(g_EthDP83848.mac[4]));
    strcat(buf,tempstr);
    strcat(buf,".");
    sprintf(tempstr,"%02X",(g_EthDP83848.mac[5]));
    strcat(buf,tempstr);

    memcpy(&g_InherentPara.terminalID, &g_ConfigurationSetDB->ID_Value, sizeof(g_ConfigurationSetDB->ID_Value));   
    memcpy(&g_InherentPara.terminalMAC, &buf, 17);
    memcpy(&g_InherentPara.terminalManufacturer, &buf1, 8);  
    
    memset(g_CommunicatFlag,0,sizeof(g_CommunicatFlag));//清除通讯互锁标志
    
    /* 遥信变双点 */
    for (i = 0; i < TELESIGNAL_TOTAL_NUM; i++)
    {
        if (g_TelesignalDB[i] != ON && g_TelesignalDB[i] != OFF)
        {
            g_TelesignalDB[i] = OFF;
        }
    }	
    
    //遥信配置  
    g_NewMaxNumTelesignal = g_ConfigurationSetDB->YXSetNum;//新点表总数
    for(i=0,temp1=0,temp2=0,value=0;i<g_NewMaxNumTelesignal;i++)//原点表内添加地址和初始遥信
    {
        for(j=0,value=0;j<(g_ConfigurationSetDB->YXSet[temp1]>>NEWONEYX_NUM);j++)
        {
            value |= (g_TelesignalDB[((g_ConfigurationSetDB->YXSet[temp1+1+j]>>NEWONEYX_ADDR)&NEWJUDG_ADDR)]-1);//单点
        }
        value = value + 1;//双点
        g_NewToOldTelesignal[temp2] = (value<<NEWONEYX_VAULE)|((i+TELESIGNAL_START_ADDR)<<NEWONEYX_ADDR);
        memcpy(&g_NewToOldTelesignal[temp2+1],&g_ConfigurationSetDB->YXSet[temp1],((g_ConfigurationSetDB->YXSet[temp1]>>NEWONEYX_NUM)+1)*sizeof(uint16_t));
        temp1 += (g_ConfigurationSetDB->YXSet[temp1]>>NEWONEYX_NUM) + 1;
        temp2 += (g_NewToOldTelesignal[temp2+1]>>NEWONEYX_NUM) + 1 + 1;
    }
       
    for(i=0;i<TELESIGNAL_TOTAL_NUM;i++)//建立空链表
    {
        list_init(&g_NewListTelesignal[i]);
    }
    
    for(i=0,temp1=0;i<g_NewMaxNumTelesignal;i++)//填充链表内容
    {//值&点号/数量&属性/运算&点号(N)
        if((g_NewToOldTelesignal[temp1+1]>>NEWONEYX_NUM) != 0)
        {
            for(j=0;j<(g_NewToOldTelesignal[temp1+1]>>NEWONEYX_NUM);j++)
            {
                addr = (g_NewToOldTelesignal[temp1+1+j+1]>>NEWONEYX_ADDR)&NEWJUDG_ADDR;
                list_ins_next(&g_NewListTelesignal[addr],NULL,&g_NewToOldTelesignal[temp1]);
                THREAD_PRINTF("%d -> %d\n",addr,(g_NewToOldTelesignal[temp1]>>NEWONEYX_ADDR)&NEWJUDG_ADDR);
            }            
        } 
        temp1 += (g_NewToOldTelesignal[temp1+1]>>NEWONEYX_NUM) + 1 + 1;        
    }
	
    //遥测配置
    for(i=0;i<sizeof(g_ConfigurationSetDB->YCAddr)/sizeof(uint16_t);i++)
    {
        if(g_ConfigurationSetDB->YCAddr[i] != 0)
        {
            g_NewMaxNumTelemetry = i+1;
            g_NewToOldTelemetry[i] = g_ConfigurationSetDB->YCAddr[i];
        }
    }
    
    for(i=0;i<g_NewMaxNumTelemetry;i++)
    {
        if(g_ConfigurationSetDB->YCAddr[i] != 0)
        {
            g_NewAddTelemetry[g_ConfigurationSetDB->YCAddr[i] - TELEMETRY_START_ADDR] = TELEMETRY_START_ADDR + i;
            g_NewPropertyTelemetry[g_ConfigurationSetDB->YCAddr[i] - TELEMETRY_START_ADDR] = g_ConfigurationSetDB->YCProperty[i];
            g_NewMultipleRateTelemetry[g_ConfigurationSetDB->YCAddr[i] - TELEMETRY_START_ADDR] = g_ConfigurationSetDB->YCMultipleRate[i];
            THREAD_PRINTF("%d -> %d\n",g_ConfigurationSetDB->YCAddr[i],TELEMETRY_START_ADDR + i);
        }
    }
    
    //遥控配置
    for(i=0;i<sizeof(g_ConfigurationSetDB->YKAddr)/sizeof(uint16_t);i++)
    {
        g_NewToOldRemote[i] = g_ConfigurationSetDB->YKAddr[i];
        g_NewToOldPropertyRemote[i] = g_ConfigurationSetDB->YKProperty[i];
    }
    
    //清除存储标志
    g_COSDBIn = 0;
    for(i=0;i<DEV_MAX_NUM;i++)
    {
        g_COSDBOut[i] = g_COSDBIn;
    }
    
    g_NVADBIn = 0;
    for(i=0;i<DEV_MAX_NUM;i++)
    {
        g_NVADBOut[i] = g_NVADBIn;
    }  
		
    memset(&g_TelemetryDB, 0, sizeof(g_TelemetryDB));
    memset(&g_TelemetryLastDB, 0, sizeof(g_TelemetryLastDB));
}

/**
  * @brief : FTU system protocol parameters initialization.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][J.Lee][Make the code cleanup]
  */
int rt_multi_common_data_init(void)
{
    g_ConfigurationSetDB = rt_malloc(sizeof(struct ConfigurationSetDatabase));
    
    device_fram = rt_device_find(RT_SPI_FRAM_NAME);
	
    if (device_fram == NULL)
    {
        THREAD_PRINTF("fram is not found! \r\n"); 		
    }
    else
    {
        rt_multi_common_data_read_config();         		
        rt_multi_common_data_config();
    }	
	
    device_sd2405 = rt_device_find(RT_I2C_SD2405_NAME);
	
    if (device_sd2405 == NULL)
    {
        THREAD_PRINTF("sd2405 is not found! \r\n"); 		
    }
    
    return(RT_EOK);
}

INIT_COMPONENT_EXPORT(rt_multi_common_data_init)


/* END OF FILE ---------------------------------------------------------------*/


