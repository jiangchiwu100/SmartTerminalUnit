﻿/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_insulate.h
  * @brief:     故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-07
  * @update:    
  */
#ifndef __FAULT_INSULATE_H__
#define __FAULT_INSULATE_H__
#include "common_def.h"

#include "distribution_def.h"
#include <stdint.h>
//#define ALIGN(n)                    __attribute__((aligned(n)))

extern StateResult FaultInsulate_Deal(FaultDealHandle* handle);


#endif
