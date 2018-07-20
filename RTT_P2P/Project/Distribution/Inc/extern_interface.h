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

//#define DLL_MODE

#ifndef DLL_MODE
#include "rtthread.h"

//函数替换,根据实验环境
#define MALLOC rt_malloc
#define FREE rt_free
#define SafeFree(ptr)  {FREE(ptr); ptr = 0;}
#define MEMSET rt_memset
#define MEMCPY  rt_memcpy

#define CALLOC  rt_calloc
#define REALLOC rt_realloc
#define GetTime()  rt_tick_get()

#undef  perror

#ifdef MSVC
#define perror(arg)  {rt_kprintf("error:%s, line: %d:\n", __func__ , __LINE__ );  rt_kprintf( arg);}
#else
#define perror(arg...)  {rt_kprintf("error:%s, line: %d:\n", __func__ , __LINE__ );  rt_kprintf( arg);}
#endif

#define printf(args)  rt_kprintf(args)
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

#endif // ! __DISTRIBUTION_H
