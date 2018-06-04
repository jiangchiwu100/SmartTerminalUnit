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
#include "md5.h"
	
	
static int Json_MyFile; /* File object */
static char Json_FileName[255]; /* File Name */

static void Struct_To_Json(int file);
static uint8_t Get_MD5ID_For_Json(const char* fileName);

/**
 * @fn rt_s2j_malloc_fn
 * @brief 消除warning
 * @return  申请空间指针
 */
void *rt_s2j_malloc_fn(size_t sz)
{
    return(rt_malloc(sz));
}

/**
 * @fn rt_s2j_init
 * @brief 初始化struct2json
 * @return  RT_EOK    初始化成功
 *          其他      失败
 */
rt_err_t rt_s2j_init(void)
{
	g_ProductID.productSerialNumber = TERMINAL_PRODUCT_SERIAL_NUMBER;

    s2jHook.malloc_fn = rt_s2j_malloc_fn;  //初始化内存申请函数
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
uint8_t Create_JsonFile(void)
{
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    char* string;
    char md5_buffer[16];

    strcpy(Json_FileName,"/sojo");
	strcat(Json_FileName,"/AllJsonCfg.json");

//unlink(Json_FileName);  //删除文件
//Test
    Json_MyFile = open(Json_FileName,  O_RDWR | O_CREAT, 0);  //创建一个可读写文件
    write(Json_MyFile, string, (strlen(string) - 2));  //将硬件版本号写入到config文件中
    write(Json_MyFile, ",\n", 2);  //写入文件
	close(Json_MyFile);

    uint8_t res = Get_MD5ID_For_Json(Json_FileName);
//	uint8_t res = 1;
    if(res == 0)   //ID号能对应上则退出
    {
        return 0;
    }
     
    g_ProductID.pointTableType = "AllJsonCfg.json";    //json内同时写入文件名称以作区分

    cJSON *struct_json = ProductID_StructToJson();
    
    string = rt_Print_cJSON(struct_json);
    
    Json_MyFile = open(Json_FileName,  O_RDWR | O_CREAT, 0);  //创建一个可读写文件
    
    if (Json_MyFile == -1)
    {
        res = 1;
    }
    
    write(Json_MyFile, string, (strlen(string) - 2));  //将硬件版本号写入到config文件中
    write(Json_MyFile, ",\n", 2);  //写入文件
    
    Struct_To_Json(Json_MyFile);   //结构体转换并写入
    
    write(Json_MyFile, "}\n", 2);  //依照标准格式进行写入
    
	close(Json_MyFile);

    getFileMD5(Json_FileName, md5_buffer);
    rt_multi_common_data_fram_record_write(JSON_MD5, (uint8_t *)md5_buffer, sizeof(md5_buffer));   //将MD5校验码写入到FRAM中
    //TODO:应该在读取一遍，校验一次
    return res;
}

/**
 * @fn Struct_To_Json
 * @brief 结构体转换为json并写入到文件
 * @note 不能单独使用该函数，需要配合使用
 * @param length    转换的长度
 * @param name    需要转换的结构体
 * 
 */
static void Struct_To_Json(int file)
{
    char* string;
    cJSON *struct_json;
    uint16_t length;

    for(uint16_t i = 0; i < _CFG_ALL_NUM; i++)
    {    
        switch(i)
        {
            case _CFG_PARAMTER:
            {
                write(Json_MyFile, " \"FixedValueAreaZero\":[  \n", sizeof(" \"FixedValueAreaZero\":[ \n") );  //依照标准格式进行写入
                length = g_ParameterCfg_Len;
                break;
            }
            case _CFG_FIXED_VALUE_1:
            {
                write(Json_MyFile, " \"FixedValueAreaOne\":[  \n", sizeof(" \"FixedValueAreaOne\":[ \n") );  //依照标准格式进行写入
                length = g_FixedValueCfg1_Len;
                break;
            }
            case _CFG_CALIBRATE_FACTOR:
            {
                write(Json_MyFile, " \"CalibrateCoefficient\":[  \n", sizeof(" \"CalibrateCoefficient\":[ \n") );  //依照标准格式进行写入
                length = g_CalibrateFactorCfg_Len;
                break;
            }
            case _CFG_TELE_METRY:
            {
                write(Json_MyFile, " \"TelemeteringForUpLoad\":[  \n", sizeof(" \"TelemeteringForUpLoad\":[ \n") );  //依照标准格式进行写入
                length = g_TelemetryCfg_Len;
                break;
            }
            case _CFG_TELE_SIGNAL:
            {
                write(Json_MyFile, " \"TelesignalForUpLoad\":[  \n", sizeof(" \"TelesignalForUpLoad\":[ \n") );  //依照标准格式进行写入
                length = g_TelesignalCfg_Len;
                break;
            }
            case _CFG_TELE_CONTROL:
            {
                write(Json_MyFile, " \"TelecontrolForUpLoad\":[  \n", sizeof(" \"TelecontrolForUpLoad\":[ \n") );  //依照标准格式进行写入
                length = g_TelecontrolCfg_Len;
                break;
            }
            case _CFG_TELE_INHERENT:
            {
                write(Json_MyFile, " \"InherentParameter\":[  \n", sizeof(" \"InherentParameter\":[ \n") );  //依照标准格式进行写入
                length = g_InherentParaCfg_Len;
                break;
            }
            default :
            {
                break;
            }
        }
        
        for(uint16_t j = 0; j < length; j++)
        {
            switch(i)
            {
                case _CFG_PARAMTER:
                {
                    struct_json = ParameterCfg_StructToJson(&ParameterCfg[j]);
                    break;
                }
                case _CFG_FIXED_VALUE_1:
                {
                    struct_json = FixedValueCfg1_StructToJson(&FixedValueCfg1[j]);
                    break;
                }
                case _CFG_CALIBRATE_FACTOR:
                {
                    struct_json = CalibrateFactorCfg_StructToJson(&CalibrateFactorCfg[j]);
                    break;
                }
                case _CFG_TELE_METRY:
                {
                    struct_json = TelemetryCfg_StructToJson(&TelemetryCfg[j]);
                    break;
                }
                case _CFG_TELE_SIGNAL:
                {
                    struct_json = TelesignalCfg_StructToJson(&TelesignalCfg[j]);
                    break;
                }
                case _CFG_TELE_CONTROL:
                {
                    struct_json = TelecontrolCfg_StructToJson(&TelecontrolCfg[j]);
                    break;
                }
                case _CFG_TELE_INHERENT:
                {
                    struct_json = InherentParaCfg_StructToJson(&InherentParaCfg[j]);
                    break;
                }
                default :
                {
                    break;
                }
            }
            
            string = rt_Print_cJSON(struct_json);

            write(file, string, strlen(string));  //写入文件
            
            if(j < (length - 1))
            {
                write(file, ",\n", 2);  //依照标准格式进行写入
            }
            else
            {
                write(file, "\n", 1);  //依照标准格式进行写入
            }
        }
        write(Json_MyFile, "]\n", 2);  //依照标准格式进行写入
        if(i < (_CFG_ALL_NUM - 1))
        {
            write(file, ",\n", 2);  //依照标准格式进行写入
        }
    }
}

/**
 * @fn Get_MD5ID_For_Json
 * @brief 从json文件中获取MD5校验码
 * @param fileName 指向要读取的文件
 * @return 0    md5校验正确
 *         1    md5校验不正确
 *         2    不存在MD5校验码
 *         3    字符串位置不正确
 *         4    获取文件MD5校验码失败
 * 
 */
static uint8_t Get_MD5ID_For_Json(const char* fileName)
{
    uint8_t res = 1;    //要返回的结果
    char md5_buffer[16];    //保存md5校验码的数组
    char md5Fram[16];   //保存在FRAM中的MD5校验码

    rt_multi_common_data_fram_record_read(JSON_MD5, (uint8_t *)md5Fram);   //读取

    //此处直接判断MD5
    if(getFileMD5(fileName, md5_buffer) == 0)
    {
        if(strstr(md5Fram, md5_buffer))    //比较MD5校验码是否一致
        {
            return 0;    //md5校验码正确
        }
        else
        {
            res = 1;    //md5校验码不正确
        }
    }
    else
    {
        res = 2;    //获取文件MD5校验码失败
    }

    return res;
}


