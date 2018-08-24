/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      status_update.h
  * @brief:     状态更新与关联
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  * @update:    
  */
#ifndef __UPDATE_STATUS_H__
#define __UPDATE_STATUS_H__


#include "distribution_def.h"
#include "coordinator_def.h"

extern void DataArributeToLocalProperty(SwitchProperty* sw, DeviceIndicate* di);
extern void LocalPropertyToDataArribute(const SwitchProperty* const sw, DeviceIndicate* di);
extern void GoosePublishSwitchStatus(const SwitchProperty*const sw, DeviceIndicate* di);

extern void GooseSubscriberUpdateSwitchStatus(DeviceIndicate* di);
#endif
