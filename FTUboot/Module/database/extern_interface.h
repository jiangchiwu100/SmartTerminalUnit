/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution.h
* @brief:     外部接口，主要针对常用，有根据情况变化的情况，如malloc，利于程序移植
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-26
* @update:
*/

#ifndef  __EXTERN_INTERFACE_H
#define  __EXTERN_INTERFACE_H

#include <stdint.h>
//#define DLL_MODE

#ifndef DLL_MODE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//函数替换,根据实验环境
#define MALLOC malloc
#define FREE free
#define SafeFree(ptr)  {FREE(ptr); ptr = 0;}
#define MEMSET memset
#define MEMCPY  memcpy

#define CALLOC  calloc
#define REALLOC realloc
#define GetTime()  0

#undef  perror

#ifdef MSVC
#define perror(arg)  {rt_kprintf("error:%s, line: %d:\n", __func__ , __LINE__ );  rt_kprintf( arg);}
#else
extern void udp_debug_printf(const char *fmt, ...);
#define  rt_kprintf   DebugPrintf
#define perror(arg...)  {rt_kprintf("error:%s, line: %d:\n", __func__ , __LINE__ );  rt_kprintf( arg);}
#endif


#else

#include <stdlib.h>
#define MALLOC malloc
#define FREE free
#define SafeFree(ptr)  {FREE(ptr); ptr = 0;}
#define MEMSET memset
#define MEMCPY  memcpy

#define CALLOC  calloc
#define REALLOC realloc
#define GetTime() (0)
#define perror(args)  {printf("error:%s, line: %d:\n", __FUNCDNAME__, __LINE__);  printf(args);}
#endif


/**
*将两个8bit融合成16bit
*/
#define COMBINE_UINT16(byteH, byteL) (uint16_t)(((uint16_t)(uint8_t)(byteH)<<8) | ((uint8_t)(byteL)))

typedef struct TagPointUint8
{
	uint8_t* pData; //数据指针
	uint16_t len; //指向数据长度
}PointUint8;

/**
*获取N字节,n=0,1,2,3
*/
#define GET_N_BYTE(data, n) ((uint8_t)( (uint32_t)(data>>(n<<3))))
/**
* 错误代码定义
*/
typedef enum TagErrorCode
{
	ERROR_OK_NULL = 0,
	ERROR_NULL_PTR = 0x11,
	ERROR_PARAMTER = 0x12,
	ERROR_MALLOC = 0x13,
	ERROR_OVER_LIMIT = 0x14, //超过范围
	ERROR_UNFIND = 0x15,
	ERROR_CHECK = 0x16, //校验错误
	ERROR_LEN = 0x17,  //长度错误
	ERROR_INSERT_LIST = 0x18,  //插入校验
	ERROR_VOLUM = 0x19,  //容量不足
	ERROR_EXIST = 0x1A,  //已存在   
	EROR_INIT = 0x1B, //初始化
	ERROR_ENCODE = 0x1C, //编码错误
	ERROR_DECODE = 0x1D, //解码错误

	ERROR_UNKONOW = 0x30,
}ErrorCode;

#endif // ! __DISTRIBUTION_H
