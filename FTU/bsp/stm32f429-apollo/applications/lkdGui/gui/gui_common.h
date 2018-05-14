#ifndef __COMMON_H__
#define __COMMON_H__

#include <rtthread.h>


uint32_t GetTimer1Tick(void);/* 获取Timer1当前计数*/
/* 获取Timer1相对于输入参数的间隔计数*/
uint32_t GetTimer1IntervalTick(uint32_t beginTick);

void time_static_init(void); /* Timer1初始化*/

#endif
