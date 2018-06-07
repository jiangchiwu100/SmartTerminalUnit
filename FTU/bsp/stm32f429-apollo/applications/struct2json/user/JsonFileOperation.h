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

enum
{
    _CFG_PARAMTER = 0,      //定值0区
    _CFG_FIXED_VALUE_1,     //定值1区
    _CFG_CALIBRATE_FACTOR,  //校准系数
    _CFG_TELE_METRY,        //遥测
    _CFG_TELE_SIGNAL,       //遥信
    _CFG_TELE_CONTROL,      //遥控
    _CFG_TELE_INHERENT,     //固有参数
    _CFG_ALL_NUM,           //总数
};

extern rt_err_t rt_s2j_init(void);  //初始化struct2json
extern uint8_t Create_JsonFile(void);

#endif

