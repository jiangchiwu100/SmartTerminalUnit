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
 
#ifndef __JSON_FILE_OPERATION_
#define __JSON_FILE_OPERATION_

#include "stdint.h"
#include <s2j.h>

#include <rthw.h>
#include <rtthread.h>



#define _CFG_SET_DATA_BASE       0    //配置信息
#define _CFG_PARAMTER            1    //定值0区
#define _CFG_FIXED_VALUE_1       2    //定值1区
#define _CFG_FIXED_VALUE_2       3    //定值2区
#define _CFG_CALIBRATE_FACTOR    4    //校准系数
#define _CFG_TELE_METRY          5    //遥测
#define _CFG_TELE_SIGNAL         6    //遥信

extern rt_err_t rt_s2j_init(void);  //初始化struct2json
extern void Create_JsonFile(char* fileName, uint16_t length, uint8_t name);
extern uint8_t GetJsonForFile(char* fileName, uint8_t name);

#endif

