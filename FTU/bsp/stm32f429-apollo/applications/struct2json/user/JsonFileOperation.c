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
static void Json_To_Struct(int index, uint8_t name, cJSON *struct_json);
static void Get_JSON(cJSON * root, uint8_t name);
static uint8_t Get_ID_For_Json(char* fileName, char *jsonFileName);

/**
 * @fn rt_s2j_init
 * @brief 初始化struct2json
 * @return  RT_EOK    初始化成功
 *          其他      失败
 */
rt_err_t rt_s2j_init(void)
{
	g_ProductID.productSerialNumber = TERMINAL_PRODUCT_SERIAL_NUMBER;

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
    
	memset(Json_DirName,0,sizeof(Json_DirName));
	strcpy(Json_DirName,"/sojo");
	strcat(Json_DirName,"/HISTORY/Config");//建立JSON文件目录
	mkdir(Json_DirName,0);//建立目录
	
    strcpy(Json_FileName,"/sojo");
    strcat(Json_FileName,"/HISTORY/Config/");
    strcat(Json_FileName, fileName);	
    strcat(Json_FileName, ".json");	

    unlink(Json_FileName);  //删除文件

    if(Get_ID_For_Json(Json_FileName, fileName) == 0)   //ID号能对应上则退出
    {
        return;
    }
     
    g_ProductID.pointTableType = fileName;    //json内同时写入文件名称以作区分

    cJSON *struct_json = ProductID_StructToJson();
    
    string = rt_Print_cJSON(struct_json);
    
    Json_MyFile = open(Json_FileName,  O_RDWR | O_CREAT, 0);  //创建一个可读写文件
    
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
        
        if(i < (length - 1))
        {
            write(file, ",\n", 2);  //依照标准格式进行写入
        }
        else
        {
            write(file, "\n", 1);  //依照标准格式进行写入
        }
    }

}
ConfigurationSetDatabaseToJson SetDatabaseCfg_1[30];
/**
 * @fn Struct_To_Json
 * @brief 将获取到的字符串转换为相应的结构体
 * @note 不能单独使用该函数，需要配合使用
 * @param value    指向要转换的字符串的指针
 * @return 返回转换后的结构体指针
 * 
 */
static void Json_To_Struct(int index, uint8_t name, cJSON *struct_json)
{
    switch(name)
    {
        case _CFG_SET_DATA_BASE:
        {
            SetDatabaseCfg[index] = *SetDatabaseCfg_JsonToStruct(struct_json);
            break;
        }
        case _CFG_PARAMTER:
        case _CFG_FIXED_VALUE_1:
        case _CFG_FIXED_VALUE_2:
        case _CFG_CALIBRATE_FACTOR:
        case _CFG_TELE_METRY:
        case _CFG_TELE_SIGNAL:
        default :
        {
            break;
        }
    }
}

/**
 * @fn Get_JSON
 * @brief 遍历查找json的最内层键值对
 * @note 不能单独使用该函数，需要配合使用
 * @param root    指向从字符串获取的json对象
 * @param name    要转换的结构体的名字
 * @return 返回转换后的结构体指针
 * 
 */
void Get_JSON(cJSON * root, uint8_t name)
{
	cJSON * item;
	for (int i = 0; i < cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
	{
		item = cJSON_GetArrayItem(root, i);
		if (cJSON_Object != item->type)		//值不为json对象则查找child是否为空，为空即不包含json
		{
			if (item->child != NULL)
			{
                int a = cJSON_GetArraySize(item);
				for (int j = 0; j < a; j++)   //遍历外层json键值对
				{
					cJSON * _item = cJSON_GetArrayItem(item, j);
					if (cJSON_Object == _item->type)    //如果类型为cJSON_Object则转换
					{
						/* deserialize Student structure object */
                        Json_To_Struct(j, name, _item);    //转换
					}
				}
			}
		}
	}
}

/**
 * @fn Create_JsonFile
 * @brief 结构体转换为json并写入到文件
 * @param fileName    要写入的文件名称
 * @param name    需要转换的结构体名字
 * @return 正确    0
 *         文件打开错误    1
 *         json格式错误    2
 */
uint8_t GetJsonForFile(char* fileName, uint8_t name)
{
    //TERMINAL_PRODUCT_SERIAL_NUMBER
    static char* _string;
    static char data;
    static cJSON *readJson;

    _string = rt_malloc(1024*1024);     //分配1M的内存
    
	memset(Json_DirName,0,sizeof(Json_DirName));
	strcpy(Json_DirName,"/sojo");
	strcat(Json_DirName,"/HISTORY/Config");//建立JSON文件目录
	mkdir(Json_DirName,0);//建立目录
	
    strcpy(Json_FileName,"/sojo");
    strcat(Json_FileName,"/HISTORY/Config/");
    strcat(Json_FileName, fileName);	
    strcat(Json_FileName, ".json");	

    Json_MyFile = open(Json_FileName,  O_RDONLY, 0);  //打开文件
    if(Json_MyFile < 0)
    {
        return 1;
    }

    for (uint32_t i = 0; (read(Json_MyFile, &data, 1)); i++)
	{
        _string[i] = data;
	}

	close(Json_MyFile);

    readJson = rt_Get_cJSON(_string);
    if(readJson == 0)   //未能能正确转换json ，首先检查是否写错
    {
        rt_free(_string);    //释放动态内存
        return 2;
    }
    //增加动态内存获取，大小为结构体大小
    //获取文件内容
    //转换，根据电脑端demo
    Get_JSON(readJson, name); //获取文件内的json数据并转换为结构体数据

    rt_free(_string);    //释放动态内存
    return 0;
}

/**
 * @fn Get_ID_For_Json
 * @brief 从json文件中获取ID号
 * @param fileName 指向要读取的文件
 * @return ID正确   0
 *         不正确   1
 *         没有“productSerialNumber”字段    2
 *         文件名称不正确    3
 *         文件打开不正确    4
 *         字符串位置不正确    5
 * 
 */
static uint8_t Get_ID_For_Json(char* fileName, char *jsonFileName)
{
    char* string;
    static char data;
    string = rt_malloc(512);     //分配512字节的内存
    int myFile_;
    uint8_t count = 0;  //计数，计读到的“，”标号的数量
    uint8_t res = 1;    //要返回的结果
    char* _string;
    
    myFile_ = open(fileName,  O_RDONLY, 0);  //打开文件
    if(myFile_ < 0)
    {
        return 4;
    }

    for (uint32_t i = 0; (read(myFile_, &data, 1)); i++)
	{
        if(data == ',')
        {
            count++;
            if(count >= 2)
            {
                string[i] = '}';
                break;  //跳出循环
            }
        }
        string[i] = data;
	}
    close(myFile_);
    
    _string = strstr(string, "pointTableType");
    if(string - _string > 4)  //此处硬编码
    {
        res = 5;
    }
    if (strstr(string, jsonFileName))   //string字符串中是否包含有该json文件名称
    {
        if (strstr(string, "productSerialNumber"))   //判断字符串ProductSerialNumber在字符串1中首次出现的位置
        {
            if(strstr(string, TERMINAL_PRODUCT_SERIAL_NUMBER))    //判断读取出的ID与软件中是否一致
            {
                res = 0;
            }
            else
            {
                res = 1;
            }
        }
        else
        {
            res = 2;
        }
    }
    else
    {
        res = 3;
    }
    
    rt_free(string);    //释放动态内存
    
    return res;
}


