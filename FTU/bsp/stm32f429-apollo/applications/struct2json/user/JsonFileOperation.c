/**
 * @file JsonFileOperation.c
 * @brief 实现Json文件操作
 * @copyright Copyright (c) 2017 Beijing SOJO Electric CO., LTD.
 * @company SOJO
 * @date 2018.05.14
 *
 * @author Zhangxiaomou
 * @version ver 1.0
 */
 

#include <dfs_posix.h>
#include "string.h"
#include "common_data.h"
#include "point_table_config.h"
#include "Interface_S2J.h"
#include "common_config.h"
#include "JsonFileOperation.h"
	
	
static int Json_MyFile; /* File object */
static char Json_FileName[255]; /* File Name */
static char Json_DirName[255]; /* Dir Name */

static void Struct_To_Json(uint16_t length, uint8_t name, int file);
static void *Json_To_Struct(char *value, uint8_t name);


/**
 * @fn rt_s2j_init
 * @brief 初始化struct2json
 * @return  RT_EOK    初始化成功
 *          其他      失败
 */
rt_err_t rt_s2j_init(void)
{
	g_ProductID.ProductSerialNumber = TERMINAL_PRODUCT_SERIAL_NUMBER;

    s2jHook.malloc_fn = rt_malloc;  //初始化内存申请函数
    s2jHook.free_fn = rt_free;

    s2j_init(&s2jHook);   //初始化struct2json的内存申请

    return RT_EOK;
}

/**
 * @fn Create_JsonFile
 * @brief 结构体转换为json并写入到文件
 * @param fileName    要写入的文件名称
 * @param length    转换的长度
 * @param name    需要转换的结构体
 * 
 */
void Create_JsonFile(char* fileName, uint16_t length, uint8_t name)
{
    
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    char* string;
    
    char readBuffer[256];
    
	memset(Json_DirName,0,sizeof(Json_DirName));
	strcpy(Json_DirName,"/sojo");
	strcat(Json_DirName,"/HISTORY/Config");//建立JSON文件目录
	mkdir(Json_DirName,0);//建立目录
	
    strcpy(Json_FileName,"/sojo");
    strcat(Json_FileName,"/HISTORY/Config/");
    strcat(Json_FileName, fileName);	
    strcat(Json_FileName, ".json");	

    g_ProductID.name = fileName;    //json内同时写入文件名称以作区分

    unlink(Json_FileName);   //删除该文件
     
    cJSON *struct_json = ProductID_StructToJson();
    
    string = rt_Print_cJSON(struct_json);
    
    s2j_delete_json_obj(struct_json);       //删除该json
    
    Json_MyFile = open(Json_FileName,  O_RDWR | O_CREAT, 0);  //创建一个可读写文件
    
    read(Json_MyFile, readBuffer, (sizeof(string) + 3));    //读取文件头，检查是否写过
    
    if((strstr(readBuffer, string)) != NULL)    //两个字符串相等
    {
        close(Json_MyFile);
        return;
    }
    
    write(Json_MyFile, string, (strlen(string) - 2));  //将硬件版本号写入到config文件中
    write(Json_MyFile, ",\n", 2);  //写入文件
    
    write(Json_MyFile, " \"PointList\":[  \n", sizeof(" \"PointList\":[ \n") );  //依照标准格式进行写入

    Struct_To_Json(length, name, Json_MyFile);   //结构体转换并写入
    
    write(Json_MyFile, "]\n", 2);  //依照标准格式进行写入

    write(Json_MyFile, "}\n", 2);  //依照标准格式进行写入
    
	close(Json_MyFile);
}

/**
 * @fn Struct_To_Json
 * @brief 结构体转换为json并写入到文件
 * @note 不能单独使用该函数，需要配合使用
 * @param length    转换的长度
 * @param name    需要转换的结构体
 * 
 */
static void Struct_To_Json(uint16_t length, uint8_t name, int file)
{
    char* string;
    cJSON *struct_json;

    for(uint16_t i = 0; i < length; i++)
    {
        
        switch(name)
        {
            case _CFG_SET_DATA_BASE:
            {
                struct_json = SetDatabaseCfg_StructToJson(&SetDatabaseCfg[i]);
                break;
            }
            case _CFG_PARAMTER:
            {
                struct_json = ParameterCfg_StructToJson(&ParameterCfg[i]);
                break;
            }
            case _CFG_FIXED_VALUE_1:
            {
                struct_json = FixedValueCfg1_StructToJson(&FixedValueCfg1[i]);
                break;
            }
            case _CFG_FIXED_VALUE_2:
            {
                struct_json = FixedValueCfg2_StructToJson(&FixedValueCfg2[i]);
                break;
            }
            case _CFG_CALIBRATE_FACTOR:
            {
                struct_json = CalibrateFactorCfg_StructToJson(&CalibrateFactorCfg[i]);
                break;
            }
            case _CFG_TELE_METRY:
            {
                struct_json = TelemetryCfg_StructToJson(&TelemetryCfg[i]);
                break;
            }
            case _CFG_TELE_SIGNAL:
            {
                struct_json = TelesignalCfg_StructToJson(&TelesignalCfg[i]);
                break;
            }
            default :
            {
                break;
            }
        }
        
        string = rt_Print_cJSON(struct_json);

        write(file, string, strlen(string));  //写入文件
        
        if(i < (g_FixedValueCfg2_Len - 1))
        {
            write(file, ",\n", 2);  //依照标准格式进行写入
        }
        else
        {
            write(file, "\n", 1);  //依照标准格式进行写入
        }
        
        s2j_delete_json_obj(struct_json);       //删除该json
    }

}



/**
 * @fn Struct_To_Json
 * @brief 将获取到的字符串转换为相应的结构体
 * @note 不能单独使用该函数，需要配合使用
 * @param value    指向要转换的字符串的指针
 * @return 返回转换后的结构体指针
 * 
 */
static void *Json_To_Struct(char *value, uint8_t name)
{
    cJSON *struct_json = rt_Get_cJSON(value);   //将获取到的字符串转换为json
    switch(name)
    {
        case _CFG_SET_DATA_BASE:
        {
            return SetDatabaseCfg_JsonToStruct(struct_json);
        }
        case _CFG_PARAMTER:
        {
            return ParameterCfg_JsonToStruct(struct_json);
        }
        case _CFG_FIXED_VALUE_1:
        {
            return FixedValueCfg1_JsonToStruct(struct_json);
        }
        case _CFG_FIXED_VALUE_2:
        {
            return FixedValueCfg2_JsonToStruct(struct_json);
        }
        case _CFG_CALIBRATE_FACTOR:
        {
            return CalibrateFactorCfg_JsonToStruct(struct_json);
        }
        case _CFG_TELE_METRY:
        {
            return TelemetryCfg_JsonToStruct(struct_json);
        }
        case _CFG_TELE_SIGNAL:
        {
            return TelesignalCfg_JsonToStruct(struct_json);
        }
        default :
        {
            break;
        }
    }
    return  NULL;   //此时发生错误
}




