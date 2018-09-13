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

void UpdateLocalPublicRef(ServerModelManager* manager);

void GooseCheckAdd(GooseSubscriber subscriber, RingQueue* ring);
#endif 
