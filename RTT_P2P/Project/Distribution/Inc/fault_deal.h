/**
*             Copyright (C) TOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      fault_deal.c
* @brief:     故障处理，独立故障移除中的函数
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-12
* @update:
*/
#ifndef __FAULT_DEAL_H__
#define __FAULT_DEAL_H__
#include "common_def.h"

#include "distribution_def.h"
#include <stdint.h>
//#define ALIGN(n)                    __attribute__((aligned(n)))

extern uint8_t FaultDealStateCenter(FaultDealHandle* handle);



#endif
