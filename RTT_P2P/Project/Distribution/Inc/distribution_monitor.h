/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_monitor.h
* @brief:     用于分布式监控相关，维护常用状态
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-28
* @update:
*/
#ifndef __DISTRIBUTION_MONITOR_H__
#define __DISTRIBUTION_MONITOR_H__

#include "distribution_def.h"

#ifdef	__cplusplus
extern "C" {
#endif

ErrorCode CheckMessageValid(StationPoint* station);

#ifdef	__cplusplus
}
#endif

#endif
