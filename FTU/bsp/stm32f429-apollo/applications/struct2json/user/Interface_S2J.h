/**
 * @file Interface_S2J.h
 * @brief 实现struct2json函数对外接口
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.05.10
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */


#include <s2j.h>

#include <rthw.h>
#include <rtthread.h>
#include "point_table_config.h"


extern rt_err_t rt_s2j_init(void);  //初始化struct2json
extern cJSON *TelesignalCfg_StructToJson(struct tagTelesignalCfg* struct_obj);         //实现遥测结构体的struct转json
extern struct tagTelesignalCfg *TelesignalCfg_JsonToStruct(cJSON* json_obj);           //实现遥测结构体的json转struct
extern cJSON *TelemetryCfg_StructToJson(struct tagTelemetryCfg* struct_obj);          //实现遥测结构体的struct转json
extern struct tagTelemetryCfg *TelemetryCfg_JsonToStruct(cJSON* json_obj);            //实现遥测结构体的json转struct
extern cJSON *CalibrateFactorCfg_StructToJson(struct tagCalibrateFactor* struct_obj);    //实现校准系数结构体的struct转json
extern struct tagCalibrateFactor *CalibrateFactorCfg_JsonToStruct(cJSON* json_obj);      //实现校准系数结构体的json转struct
extern cJSON *ParameterCfg_StructToJson(struct tagValueParaCfg* struct_obj);          //实现定值0区结构体的struct转json
extern struct tagValueParaCfg *ParameterCfg_JsonToStruct(cJSON* json_obj);            //实现定值0区结构体的json转struct
extern cJSON *FixedValueCfg1_StructToJson(struct tagValueParaCfg* struct_obj);        //实现定值一区结构体的struct转json
extern struct tagValueParaCfg *FixedValueCfg1_JsonToStruct(cJSON* json_obj);          //实现定值一区结构体的json转struct
extern cJSON *FixedValueCfg2_StructToJson(struct tagValueParaCfg* struct_obj);        //实现定值二区结构体的struct转json
extern struct tagValueParaCfg *FixedValueCfg2_JsonToStruct(cJSON* json_obj);          //实现定值二区结构体的json转struct
extern cJSON *SetDatabaseCfg_StructToJson(ConfigurationSetDatabaseToJson* struct_obj);  //实现配置信息结构体的json转struct
extern ConfigurationSetDatabaseToJson *SetDatabaseCfg_JsonToStruct(cJSON* json_obj);    //实现配置信息结构体的struct转json

extern cJSON* rt_Get_cJSON(const char *value);  //从字符串中获得json
extern char* rt_Print_cJSON(cJSON *item);       //将json打印到字符串中

#define FixedValueCfg1_LEN  (sizeof FixedValueCfg1/(sizeof tagValueParaCfg))
