/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_treatment.h
  * @brief:     故障处理，故障判据，故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
#ifndef __FAULT_TREATMENT_H__
#define __FAULT_TREATMENT_H__

#include "distribution_def.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern ErrorCode JudgeFaultArea(DistributionStation* distribution, bool);


#ifdef	__cplusplus
}
#endif
#endif
