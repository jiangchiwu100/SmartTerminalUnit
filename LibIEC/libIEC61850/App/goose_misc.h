/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      goose_misc.h
  * @brief:     61850����ӳ��
  * @version:   V0.0.0
  * @author:    Zhang Yufei
  * @date:      2018-08-24
  * @update:
  */

#ifndef __GOOSE_MSIC_H_
#define __GOOSE_MSIC_H_

#include "Coordinator_def.h"
#include "RingQueue.h"

#define LIVE_TIME_NAME				"timeAllowToLive"
#define LIVE_TIME_TICK 				2*5000						/* 报文允许生存时间,需要2*5000ms */
#define LIVE_TIME_FLAG				RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER	/* 单次定时，软件定时 */

void UpdateLocalPublicRef(ServerModelManager* manager);
void FtuIdleHook(void);
void GooseCheckAdd(GooseSubscriber subscriber, RingQueue* ring);
#endif 
