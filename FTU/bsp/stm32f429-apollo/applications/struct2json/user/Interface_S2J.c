/**
 * @file Interface_S2J.c
 * @brief 实现struct2json函数对外接口
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.05.10
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 
#include "Interface_S2J.h"

ProductID g_ProductID;

/**
 * @fn ProductID_StructToJson
 * @brief 实现产品ID号的struct转json
 * @return 返回指向转换后的json指针
 *
 */
cJSON *ProductID_StructToJson(void)
{
	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, &g_ProductID, string, pointTableType);				//标注该文件时遥信还是遥测
	s2j_json_set_basic_element(json_obj, &g_ProductID, string, ProductSerialNumber);		//名称

	/* return Student JSON object pointer */
	return json_obj;
}


/**
 * @fn TelesignalCfg_StructToJson
 * @brief 实现遥测结构体的struct转json
 * @param struct_obj 指向结构体的指针
 * @return 返回指向转换后的json指针
 *
 */
cJSON *TelesignalCfg_StructToJson(void* struct_obj)
{
	struct tagTelesignalCfg *fixedValueCfg1 = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, int, enable);			//使能
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pName);		//名称
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pVal, 1);		//值    使用数组思想

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn TelesignalCfg_JsonToStruct
* @brief 实现遥测结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *TelesignalCfg_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagTelesignalCfg);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);		//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);		//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, pVal);		//值
	
	/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn TelemetryCfg_StructToJson
* @brief 实现遥测结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *TelemetryCfg_StructToJson(void* struct_obj)
{
	struct tagTelemetryCfg *fixedValueCfg1 = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, int, enable);				//使能
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pName);			//名称
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pVal, 1);			//值    使用数组思想
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pUnit);			//单位
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pCalifactor, 1);	//校准系数
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pZerodrift, 1);	//零漂
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pDeadzone, 1);		//死区

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn TelemetryCfg_JsonToStruct
* @brief 实现遥测结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *TelemetryCfg_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagTelemetryCfg);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);			//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);			//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, pVal);			//值
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pUnit);			//单位
	s2j_struct_get_array_element(struct_obj, json_obj, double, pCalifactor);	//校准系数
	s2j_struct_get_array_element(struct_obj, json_obj, double, pZerodrift);		//零漂
	s2j_struct_get_array_element(struct_obj, json_obj, double, pDeadzone);		//死区

	/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn CalibrateFactorCfg_StructToJson
* @brief 实现校准系数结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *CalibrateFactorCfg_StructToJson(void* struct_obj)
{
	struct tagCalibrateFactor *calibrateFactorCfg = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, int, enable);		    //使能
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, string, pName);	    //名称
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, double, calibrateVal);		//校准值
	s2j_json_set_array_element(json_obj, calibrateFactorCfg, double, telemetry, 1);		//遥测
	s2j_json_set_array_element(json_obj, calibrateFactorCfg, double, factorVal, 1);		//系数值
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, double, factorMax);		//系数上限值
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, double, factorMin);		//系数下限值
	s2j_json_set_basic_element(json_obj, calibrateFactorCfg, double, factorDefault);	//系数缺省值

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn CalibrateFactorCfg_JsonToStruct
* @brief 实现校准系数结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *CalibrateFactorCfg_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagCalibrateFactor);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);			//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);			//名称
	s2j_struct_get_basic_element(struct_obj, json_obj, double, calibrateVal);	//校准值
	s2j_struct_get_array_element(struct_obj, json_obj, double, telemetry);		//遥测
	s2j_struct_get_array_element(struct_obj, json_obj, double, factorVal);		//系数值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, factorMax);		//系数上限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, factorMin);		//系数下限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, factorDefault);	//系数缺省值

	/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn ParameterCfg_StructToJson
* @brief 实现定值0区结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *ParameterCfg_StructToJson(void* struct_obj)
{
	struct tagValueParaCfg *ParameterCfg = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, ParameterCfg, int, enable);		//使能
	s2j_json_set_basic_element(json_obj, ParameterCfg, int, menuNum);		//菜单编号
	s2j_json_set_basic_element(json_obj, ParameterCfg, string, pName);		//名称
	s2j_json_set_array_element(json_obj, ParameterCfg, double, pVal, 1);	//值    使用数组思想
	s2j_json_set_basic_element(json_obj, ParameterCfg, string, pUnit);		//单位
	s2j_json_set_basic_element(json_obj, ParameterCfg, double, valMax);		//上限值
	s2j_json_set_basic_element(json_obj, ParameterCfg, double, valMin);		//下限值
	s2j_json_set_basic_element(json_obj, ParameterCfg, double, defaultVal);	//缺省值
	s2j_json_set_basic_element(json_obj, ParameterCfg, int, dataType);		//数据类型
	s2j_json_set_array_element(json_obj, ParameterCfg, string, pContent, ParameterCfg->dataType);	//数据显示内容
	s2j_json_set_basic_element(json_obj, ParameterCfg, string, pNote);		//备注

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn ParameterCfg_JsonToStruct
* @brief 实现定值0区结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *ParameterCfg_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagValueParaCfg);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);		//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, int, menuNum);		//菜单编号
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);		//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, pVal);		//值
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pUnit);		//单位
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMax);		//上限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMin);		//下限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, defaultVal);	//缺省值
	s2j_struct_get_basic_element(struct_obj, json_obj, int, dataType);		//数据类型
	s2j_struct_get_array_element(struct_obj, json_obj, string, pContent);	//数据显示内容
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pNote);		//备注

	/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn FixedValueCfg1_StructToJson
* @brief 实现定值一区结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *FixedValueCfg1_StructToJson(void* struct_obj)
{
	struct tagValueParaCfg *fixedValueCfg1 = struct_obj;
    
	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, int, enable);			//使能
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, int, menuNum);			//菜单编号
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pName);		//名称
	s2j_json_set_array_element(json_obj, fixedValueCfg1, double, pVal, 1);		//值    使用数组思想
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pUnit);		//单位
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, double, valMax);		//上限值
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, double, valMin);		//下限值
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, double, defaultVal);	//缺省值
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, int, dataType);		//数据类型
	s2j_json_set_array_element(json_obj, fixedValueCfg1, string, pContent, fixedValueCfg1->dataType);	//数据显示内容
	s2j_json_set_basic_element(json_obj, fixedValueCfg1, string, pNote);		//备注

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn FixedValueCfg1_JsonToStruct
* @brief 实现定值一区结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *FixedValueCfg1_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagValueParaCfg);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);		//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, int, menuNum);		//菜单编号
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);		//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, pVal);		//值
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pUnit);		//单位
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMax);		//上限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMin);		//下限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, defaultVal);	//缺省值
	s2j_struct_get_basic_element(struct_obj, json_obj, int, dataType);		//数据类型
	s2j_struct_get_array_element(struct_obj, json_obj, string, pContent);	//数据显示内容
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pNote);		//备注

																			/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn FixedValueCfg2_StructToJson
* @brief 实现定值二区结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *FixedValueCfg2_StructToJson(void* struct_obj)
{
	struct tagValueParaCfg *fixedValueCfg2 = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, int, enable);			//使能
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, int, menuNum);			//菜单编号
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, string, pName);		//名称
	s2j_json_set_array_element(json_obj, fixedValueCfg2, double, pVal, 1);		//值    使用数组思想
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, string, pUnit);		//单位
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, double, valMax);		//上限值
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, double, valMin);		//下限值
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, double, defaultVal);	//缺省值
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, int, dataType);		//数据类型
	s2j_json_set_array_element(json_obj, fixedValueCfg2, string, pContent, fixedValueCfg2->dataType);	//数据显示内容
	s2j_json_set_basic_element(json_obj, fixedValueCfg2, string, pNote);		//备注

	/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn FixedValueCfg2_JsonToStruct
* @brief 实现定值二区结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *FixedValueCfg2_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, struct tagValueParaCfg);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, enable);		//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, int, menuNum);		//菜单编号
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pName);		//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, pVal);		//值
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pUnit);		//单位
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMax);		//上限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, valMin);		//下限值
	s2j_struct_get_basic_element(struct_obj, json_obj, double, defaultVal);	//缺省值
	s2j_struct_get_basic_element(struct_obj, json_obj, int, dataType);		//数据类型
	s2j_struct_get_array_element(struct_obj, json_obj, string, pContent);	//数据显示内容
	s2j_struct_get_basic_element(struct_obj, json_obj, string, pNote);		//备注

	/* return Student structure object pointer */
	return struct_obj;
}

/**
* @fn FixedValueCfg2_StructToJson
* @brief 实现定值二区结构体的struct转json
* @param struct_obj 指向结构体的指针
* @return 返回指向转换后的json指针
*
*/
cJSON *SetDatabaseCfg_StructToJson(void* struct_obj)
{
	ConfigurationSetDatabaseToJson *setDatabaseCfg = struct_obj;

	/* create Student JSON object */
	s2j_create_json_obj(json_obj);

	/* serialize data to Student JSON object. */
	s2j_json_set_basic_element(json_obj, setDatabaseCfg, int, dataLen);			//菜单编号
	s2j_json_set_basic_element(json_obj, setDatabaseCfg, string, name);		//名称
	s2j_json_set_array_element(json_obj, setDatabaseCfg, double, value, setDatabaseCfg->dataLen);		//值    使用数组思想
																				/* return Student JSON object pointer */
	return json_obj;
}

/**
* @fn FixedValueCfg2_JsonToStruct
* @brief 实现定值二区结构体的json转struct
* @param json_obj 指向json的指针
* @return 返回指向转换后的结构体指针
*
*/
void *SetDatabaseCfg_JsonToStruct(cJSON* json_obj)
{
	/* create Student structure object */
	s2j_create_struct_obj(struct_obj, ConfigurationSetDatabaseToJson);

	/* deserialize data to Student structure object. */
	s2j_struct_get_basic_element(struct_obj, json_obj, int, dataLen);		//使能
	s2j_struct_get_basic_element(struct_obj, json_obj, string, name);		//名称
	s2j_struct_get_array_element(struct_obj, json_obj, double, value);		//值

																			/* return Student structure object pointer */
	return struct_obj;
}

/**
 * @fn rt_Get_cJSON
 * @brief 获取json，重新封装cJSON_Parse函数
 * @param value 指向要转换为json结构的字符串
 * @return 返回指向获取到的json指针
 * 
 */
inline cJSON* rt_Get_cJSON(const char *value)
{
    return cJSON_Parse(value);
}


/**
 * @fn rt_Print_cJSON
 * @brief 打印json，重新封装cJSON_Print函数
 * @param item 指向要打印的json结构体的指针
 * @return 返回指向打印的json结构体生成的字符串指针
 * 
 */
inline char* rt_Print_cJSON(cJSON *item)
{
    return cJSON_Print(item);
}


