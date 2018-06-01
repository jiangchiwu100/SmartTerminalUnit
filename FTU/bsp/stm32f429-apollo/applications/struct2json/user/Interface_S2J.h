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
    char* pointTableType;       //标注该文件时遥信还是遥测(点表类型)
    char* productSerialNumber;  //产品ID号
<<<<<<< HEAD
    char  md5[20];

}ProductID;

extern cJSON *InherentParaCfg_StructToJson(void* struct_obj);         //实现固有参数结构体的struct转json
extern cJSON *TelecontrolCfg_StructToJson(void* struct_obj);         //实现遥控结构体的struct转json
extern cJSON *TelesignalCfg_StructToJson(void* struct_obj);         //实现遥信结构体的struct转json
extern cJSON *TelemetryCfg_StructToJson(void* struct_obj);          //实现遥测结构体的struct转json
extern cJSON *CalibrateFactorCfg_StructToJson(void* struct_obj);    //实现校准系数结构体的struct转json
extern cJSON *ParameterCfg_StructToJson(void* struct_obj);          //实现定值0区结构体的struct转json
extern cJSON *FixedValueCfg1_StructToJson(void* struct_obj);        //实现定值1区结构体的struct转json

extern cJSON *ProductID_StructToJson(void);     //实现产品ID号的转换
extern char *ProductID_JsonToStruct(cJSON* json_obj);   //获取MD5校验码
=======
    
}ProductID;

extern cJSON *TelesignalCfg_StructToJson(void* struct_obj);         //实现遥测结构体的struct转json
extern cJSON *TelemetryCfg_StructToJson(void* struct_obj);          //实现遥测结构体的struct转json
extern cJSON *CalibrateFactorCfg_StructToJson(void* struct_obj);    //实现校准系数结构体的struct转json
extern cJSON *ParameterCfg_StructToJson(void* struct_obj);          //实现定值0区结构体的struct转json
extern cJSON *FixedValueCfg1_StructToJson(void* struct_obj);        //实现定值一区结构体的struct转json
extern cJSON *FixedValueCfg2_StructToJson(void* struct_obj);        //实现定值二区结构体的struct转json


extern cJSON *SetDatabaseCfg_StructToJson(void* struct_obj);        //实现配置信息结构体的json转struct
extern ConfigurationSetDatabaseToJson *SetDatabaseCfg_JsonToStruct(cJSON* json_obj);      //实现配置信息结构体的struct转json

extern cJSON *ProductID_StructToJson(void);     //实现产品ID号的转换
>>>>>>> a1f225a8622d705e09cc0146f1e90b4c456e6290

extern cJSON* rt_Get_cJSON(const char *value);  //从字符串中获得json
extern char* rt_Print_cJSON(cJSON *item);       //将json打印到字符串中

extern ProductID g_ProductID;

#endif
