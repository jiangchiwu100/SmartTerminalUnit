/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_online.h
* @brief:     用于分布式在线与离线处理
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-31
* @update:
*/
#ifndef __DISTRIBUTION_ONLINE_H__
#define __DISTRIBUTION_ONLINE_H__

#include "distribution_def.h"


void  StationUpdateOnlineStatusMessage(uint8_t data[], uint8_t len, StationPoint* point);
ErrorCode CheckIsOnlineStatus(StationPoint* pPoint);


#endif
