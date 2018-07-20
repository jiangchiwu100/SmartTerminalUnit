/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      RingQueue.h
* @brief:     环形队列,由队列改型而来 TODO：高速无锁环形队列
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-27
* @update
*/

#ifndef __RINGQUEUE_H
#define __RINGQUEUE_H




#include "common_def.h"

#ifdef	__cplusplus
extern "C" {
#endif


extern bool RingQueueInit(RingQueue* ring, uint16_t capacity);
extern bool RingQueueDestory(RingQueue* ring);
#ifdef	__cplusplus
}
#endif

#endif

