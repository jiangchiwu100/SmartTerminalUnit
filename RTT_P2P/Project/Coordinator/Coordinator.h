/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator.h
* @brief:     协调器文件，本文件主要用于补充分布式的集中控制性
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-23
* @update:
*/

#ifndef __COORDINATOR_H
#define __COORDINATOR_H


#include "coordinator_def.h"
#include <stdbool.h>

bool DeviceIndicate_getBooleanStatus(DeviceIndicate* self, DeviceStatusCode code);
void DeviceIndicate_setBooleanStatus(DeviceIndicate* self, DeviceStatusCode code, bool state);
extern DeviceIndicate* DeviceIndicate_crate(uint16_t daCount);
extern void MmsDatasetToDataAtrributeSet(const MmsValue* self, DeviceIndicate* deviceIndicate);
#endif
