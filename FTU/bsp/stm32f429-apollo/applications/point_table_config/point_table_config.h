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


/**
 * @brief 点表配置转Json结构体定义
 */
typedef struct tagConfigurationSetDatabaseToJson
{
    char* name;         //数据名称
    int dataLen;        //标明该数据的长度
    float value[100];   //存储数据的数组
}ConfigurationSetDatabaseToJson;


/**
 * @brief 遥信配置结构体
 */
struct tagTelesignalCfg
{
	unsigned char enable;  // 使能
    char *pName;           // 名称
    unsigned char *pVal;   // 值	
};

struct tagTelemetryCfg
{
	unsigned char enable;  // 使能
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
extern ConfigurationSetDatabaseToJson SetDatabaseCfg[];

extern unsigned int g_FixedValueCfg1_Len;
extern unsigned int g_FixedValueCfg2_Len;
extern unsigned int g_ParameterCfg_Len;
extern unsigned int g_CalibrateFactorCfg_Len;
extern unsigned int g_TelemetryCfg_Len;
extern unsigned int g_TelesignalCfg_Len;
extern unsigned int g_SetDatabaseCfg_Len;

#endif /* __POINT_TABLE_CONFIG_H__ */

/* END OF FILE ---------------------------------------------------------------*/
