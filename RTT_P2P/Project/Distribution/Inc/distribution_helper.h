/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_test_case.h
  * @brief:     辅助方法
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
#ifndef __DISTRIBUTION_HELPER_H__
#define __DISTRIBUTION_HELPER_H__

#include "distribution_def.h"
#define 	SIMPLE_PRINT

#ifdef	__cplusplus
extern "C" {
#endif



extern void PrintTopologyMessage(const TopologyMessage*  topologyMessage);
extern void PrintSwitchMessage(const SwitchProperty*  switchNode);
extern void PrintBFSHelper(const BFSHelper* helper);
extern void PrintBFSHelperSimple(const BFSHelper* helper);
extern void PrintMemoryUsed(void);
extern void PrintPowerAreaMessage(List* areaList, uint8_t len);
extern void PrintIDTipsTick(uint32_t id, uint8_t* tips);
extern void PrintSwitchList(List* list);
#ifdef	__cplusplus
}
#endif
#endif
