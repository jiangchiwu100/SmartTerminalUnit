/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      snapshoot.h
* @brief:     保存快照信息
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018-09-06
* @update:
*/
#ifndef __SNAPSHOOT_H__
#define __SNAPSHOOT_H__

#include "distribution_def.h"

extern SwitchSnapshoot* SnapshootSwitchProperty(StationTopology* st);
extern void SwitchSnapshoot_Destory(SwitchSnapshoot* ss);
extern ErrorCode Station_Snapshoot(StationTopology* pTopology);
extern ErrorCode Station_DeleteSnapshoot(StationTopology* pTopology);

extern bool Snapshoot_StartUse(SwitchSnapshoot* ss);
extern void Snapshoot_StopUse(SwitchSnapshoot* ss);
#endif
