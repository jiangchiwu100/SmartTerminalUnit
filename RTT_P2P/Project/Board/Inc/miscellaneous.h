/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      miscellaneous.h
  * @brief:     用于不好分类的其他项信息
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-26 
  * @update:    
  */
#ifndef __MISCELLANEOUS_H
#define __MISCELLANEOUS_H

#include <stdint.h>

void SystemReset(void);

extern void StopWatchInit(void);
extern void StopWatchStart(void);
extern uint32_t StopWatchStop(void);
#endif

