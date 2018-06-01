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

<<<<<<< HEAD
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
    ME_UART_COM,        // UART通讯参数
    ME_NET_COM,         // NET通讯参数
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

/**
 * @note  注意：所有名称为字符串数组格式，且长度为30字节
 */
/**
 * @brief 点表配置转Json结构体定义
 */
typedef struct tagConfigurationSetDatabaseToJson
{
    char name[20];      //数据名称
    int dataLen;        //标明该数据的长度
    float value[200];   //存储数据的数组
}ConfigurationSetDatabaseToJson;


/**
=======
/**
 * @note  注意：所有名称为字符串数组格式，且长度为30字节
 */
/**
 * @brief 点表配置转Json结构体定义
 */
typedef struct tagConfigurationSetDatabaseToJson
{
    char name[20];      //数据名称
    int dataLen;        //标明该数据的长度
    float value[200];   //存储数据的数组
}ConfigurationSetDatabaseToJson;


/**
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
 * @brief 遥信配置结构体
 */
struct tagTelesignalCfg
{
<<<<<<< HEAD
	unsigned char enable;   // 使能
    char *pName;            // 名称
    unsigned short *pAddr;  // 点号
    unsigned char *pVal;    // 值	
	char *pContentYx[2];    // 数据显示内容	
	char *pContentSoe[2];   // 数据显示内容		
=======
	unsigned char enable;  // 使能
    char *pName;        // 名称
    unsigned char *pVal;   // 值	
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
};

/* 遥测配置结构 */
struct tagTelemetryCfg
{
	unsigned char enable;  // 使能
<<<<<<< HEAD
	unsigned char menuNum; // 菜单编号	
    char *pName;           // 面板名称
    char *pNameUp;         // 上位机名称
    unsigned short *pAddr;  // 点号
=======
    char *pName;        // 名称
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
    float *pVal;           // 值	
    char *pUnit;        // 单位
    float *pCalifactor;    // 校准系数		
	float *pZerodrift;     // 零漂    
	float *pDeadzone;      // 死区	
};
	
/* 遥控配置结构 */
struct tagTelecontrolCfg
{
	unsigned char enable;   // 使能
    char *pName;            // 名称
	unsigned char dataType; // 数据类型
	char *pContentSoe[13];   // 数据显示内容	
};

/* 校准系数配置结构 */
struct tagCalibrateFactor
{
	unsigned char enable;  // 使能
    char *pName;        // 名称
    float calibrateVal;    // 校准值
    unsigned short *pAddr;  // 点号
    float *factorVal;      // 系数值	
    float factorMax;	   // 系数上限值	
    float factorMin;       // 系数下限值
    float factorDefault;   // 系数缺省值	
};

/* 定值配置结构 */
struct tagValueParaCfg
{
<<<<<<< HEAD
	unsigned char enable;  // 使能
	unsigned char menuNum; // 菜单编号
    char *pName;           // 名称
    float *pVal;           // 值	
    char *pUnit;           // 单位
    float valMax;	       // 上限值	
    float valMin;          // 下限值
    float defaultVal;      // 缺省值	
	unsigned char dataType;// 数据类型
    unsigned char decimals;// 小数点位数
	char *pContent[5];        // 数据显示内容
	char *pNote;           // 备注
=======
	unsigned char enable;      // 使能
	unsigned char menuNum;     // 菜单编号
    char *pName;            // 名称
    float *pVal;               // 值	
    char *pUnit;            // 单位
    float valMax;	           // 上限值	
    float valMin;              // 下限值
    float defaultVal;          // 缺省值	
	unsigned char dataType;    // 数据类型
	char *pContent[5];      // 数据显示内容
	char *pNote;            // 备注
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
};

/* 固有参数配置结构 */
struct tagInherentParaCfg
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    char *pVal;            // 值	
};
    
extern struct tagTelesignalCfg TelesignalCfg[];
extern struct tagTelemetryCfg TelemetryCfg[];
extern struct tagCalibrateFactor CalibrateFactorCfg[];
extern struct tagValueParaCfg ParameterCfg[];
extern struct tagValueParaCfg FixedValueCfg1[];
extern struct tagValueParaCfg FixedValueCfg2[];
<<<<<<< HEAD
extern struct tagTelecontrolCfg TelecontrolCfg[];
extern struct tagInherentParaCfg InherentParaCfg[];
=======
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290
extern ConfigurationSetDatabaseToJson SetDatabaseCfg[];

extern unsigned int g_FixedValueCfg1_Len;
extern unsigned int g_FixedValueCfg2_Len;
extern unsigned int g_ParameterCfg_Len;
extern unsigned int g_CalibrateFactorCfg_Len;
extern unsigned int g_TelemetryCfg_Len;
extern unsigned int g_TelesignalCfg_Len;
<<<<<<< HEAD
extern unsigned int g_TelecontrolCfg_Len;
extern unsigned int g_InherentParaCfg_Len;
=======
extern unsigned int g_SetDatabaseCfg_Len;
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290

#endif /* __POINT_TABLE_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
