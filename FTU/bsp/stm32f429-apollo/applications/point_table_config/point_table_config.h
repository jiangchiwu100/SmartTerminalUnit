/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      point_table_config.h
  * @brief:     The point_table_config of the project.
  * @version:   ver 1.1
  * @author:    Mr.Sxr
  * @date:      2018-05-09
  * @update:    2018-05-09 [][]
  */
	
#ifndef __POINT_TABLE_CONFIG_H__
#define __POINT_TABLE_CONFIG_H__

enum TELEMETRY_MENU
{
    SECONDRY = 1,    // 二次遥测
    ONCE,            // 一次遥测
    HARMONIC         // 谐波分量
};

enum PARAMETER_MENU
{
    ME_BASIC_SET = 1,   // 基本设置
    ME_ZERODRIFT,       // 零漂
    ME_DEADEZONE,       // 死区
};

enum FIXED_VALUE_MENU
{
    INTERGHASE = 1,  // 相间故障
    ZERO_SEQUE,      // 零序故障
	OTHER_PROTEC,    // 其他保护
	LOGICAL_FUN,     // 逻辑功能
	LIMITATION,      // 越限告警
	HEAVY_LOAD,      // 重过载
	OVERLOAD,        // 过负荷
	LIMIT_V_F,       // 越压越频
	BATTERY_SET,     // 电池设置
	AUTO_RESET,      // 自动复归
	LOOP_CLOSE,      // 合环
	FAULT_SWITCH,    // 故障投退
};  

struct tagTelesignalCfg
{
	unsigned char enable;   // 使能
    char *pName;            // 名称
    unsigned char *pVal;    // 值	
	char *pContentYx[2];    // 数据显示内容	
	char *pContentSoe[2];   // 数据显示内容		
};

struct tagTelemetryCfg
{
	unsigned char enable;  // 使能
	unsigned char menuNum; // 菜单编号	
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float *pCalifactor;    // 校准系数		
	float *pZerodrift;     // 零漂    
	float *pDeadzone;      // 死区	
};
	
struct tagCalibrateFactor
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    float calibrateVal;    // 校准值
    float *telemetry;	   // 遥测
    float *factorVal;      // 系数值	
    float factorMax;	   // 系数上限值	
    float factorMin;       // 系数下限值
    float factorDefault;   // 系数缺省值	
};

struct tagValueParaCfg
{
	unsigned char enable;  // 使能
	unsigned char menuNum; // 菜单编号
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float valMax;	       // 上限值	
    float valMin;          // 下限值
    float defaultVal;      // 缺省值	
	unsigned char dataType;// 数据类型
	char *pContent[5];        // 数据显示内容
	char *pNote;           // 备注
};

extern struct tagTelesignalCfg TelesignalCfg[];
extern struct tagTelemetryCfg TelemetryCfg[];
extern struct tagCalibrateFactor CalibrateFactorCfg[];
extern struct tagValueParaCfg ParameterCfg[];
extern struct tagValueParaCfg FixedValueCfg1[];
extern struct tagValueParaCfg FixedValueCfg2[];

extern unsigned int g_FixedValueCfg1_Len;
extern unsigned int g_FixedValueCfg2_Len;
extern unsigned int g_ParameterCfg_Len;
extern unsigned int g_CalibrateFactorCfg_Len;
extern unsigned int g_TelemetryCfg_Len;
extern unsigned int g_TelesignalCfg_Len;


#endif /* __POINT_TABLE_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
