/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_test_case.h
  * @brief:     相关测试用例
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
#ifndef __DISTRIBUTION_TEST_CASE_H__
#define __DISTRIBUTION_TEST_CASE_H__
#include "topology.h"
#include <stdint.h>
//初始化
extern uint8_t ListDataInit(void);

extern void TopologyTest(uint8_t data[], uint8_t len);
extern void CalTest(void);
extern void TestPath(void);
extern void TestPathAll(void);
extern void SetLocalMessage(uint8_t data[], uint8_t len);
extern void SetAimMessage(uint8_t data[], uint8_t len);
extern void DeleteTopologyNode(uint8_t data[], uint8_t len);
extern void AddStationPoint(uint8_t data[], uint8_t len);



#endif
