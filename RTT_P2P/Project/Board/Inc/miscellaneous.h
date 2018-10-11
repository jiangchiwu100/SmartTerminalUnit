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
#include <stdbool.h>


extern void SystemReset(void);



extern void StopWatchInit(void);
extern void StopWatchStart(void);
extern uint32_t StopWatchStop(void);

extern uint8_t* System_getGooseConfigName(uint8_t id);
extern uint8_t* System_getConfigName(uint8_t id);

extern uint8_t* System_getGooseConfigFullName(uint8_t id);
extern uint8_t* System_getConfigFullName(uint8_t id);

extern bool System_getConfigFile(uint8_t id);
#endif

