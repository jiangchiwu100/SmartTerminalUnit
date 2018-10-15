/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      config
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-10-12
  * @update:    
  */
#ifndef __DISTRIBUTION_CONTROL_H__
#define __DISTRIBUTION_CONTROL_H__
#include "distribution_def.h"



#ifdef	__cplusplus
extern "C"
{
#endif

 
extern void  Station_ControlDistribution(SwitchProperty* switchProperty, StationPoint* point);
extern void EnterExitAndUpdate(SwitchProperty* local, StationPoint* point);
extern void EnterResetAndUpdate(SwitchProperty* local, StationPoint* point);
    
#ifdef	__cplusplus
}
#endif

#endif
