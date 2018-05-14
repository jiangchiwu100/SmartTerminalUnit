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

#ifndef __INTER_FACE_S2J_
#define __INTER_FACE_S2J_

#include <s2j.h>

#include "point_table_config.h"

typedef struct tagProductID
{
    char* ProductSerialNumber;
    
}ProductID;

extern cJSON *TelesignalCfg_StructToJson(void* struct_obj);         //实现遥测结构体的struct转json
extern void *TelesignalCfg_JsonToStruct(cJSON* json_obj);           //实现遥测结构体的json转struct
extern cJSON *TelemetryCfg_StructToJson(void* struct_obj);          //实现遥测结构体的struct转json
extern void *TelemetryCfg_JsonToStruct(cJSON* json_obj);            //实现遥测结构体的json转struct
extern cJSON *CalibrateFactorCfg_StructToJson(void* struct_obj);    //实现校准系数结构体的struct转json
extern void *CalibrateFactorCfg_JsonToStruct(cJSON* json_obj);      //实现校准系数结构体的json转struct
extern cJSON *ParameterCfg_StructToJson(void* struct_obj);          //实现定值0区结构体的struct转json
extern void *ParameterCfg_JsonToStruct(cJSON* json_obj);            //实现定值0区结构体的json转struct
extern cJSON *FixedValueCfg1_StructToJson(void* struct_obj);        //实现定值一区结构体的struct转json
extern void *FixedValueCfg1_JsonToStruct(cJSON* json_obj);          //实现定值一区结构体的json转struct
extern cJSON *FixedValueCfg2_StructToJson(void* struct_obj);        //实现定值二区结构体的struct转json
extern void *FixedValueCfg2_JsonToStruct(cJSON* json_obj);          //实现定值二区结构体的json转struct
extern cJSON *SetDatabaseCfg_StructToJson(void* struct_obj);        //实现配置信息结构体的json转struct
extern void *SetDatabaseCfg_JsonToStruct(cJSON* json_obj);      //实现配置信息结构体的struct转json

extern cJSON *ProductID_StructToJson(void);     //实现产品ID号的转换

extern cJSON* rt_Get_cJSON(const char *value);  //从字符串中获得json
extern char* rt_Print_cJSON(cJSON *item);       //将json打印到字符串中

extern ProductID g_ProductID;

#endif
