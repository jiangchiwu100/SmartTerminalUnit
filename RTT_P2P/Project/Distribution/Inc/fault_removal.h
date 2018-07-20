/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_treatment.h
  * @brief:     故障处理，故障判据，故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
#ifndef __FAULT_REMOVAL_H__
#define __FAULT_REMOVAL_H__
#include "common_def.h"

#include "distribution_def.h"
#include <stdint.h>
//#define ALIGN(n)                    __attribute__((aligned(n)))

extern StateResult RemovalState_Start(FaultDealHandle* handle);
extern StateResult RemovalState_Gather(FaultDealHandle* handle);
extern StateResult RemovalState_DelayGather(FaultDealHandle* handle);
extern StateResult RemovalState_Gather(FaultDealHandle* handle);
extern StateResult RemovalState_DelayGather(FaultDealHandle* handle);
extern StateResult RemovalState_Treatment(FaultDealHandle* handle);
extern StateResult RemovalState_CenterDectect(FaultDealHandle* handle);
extern StateResult RemovalState_Backup(FaultDealHandle* handle);
extern StateResult RemovalState_BackupDectect(FaultDealHandle* handle);
extern StateResult RemovalState_OverTime(FaultDealHandle* handle);


#endif
