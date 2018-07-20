/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      extern_mutal.h
* @brief:
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018-06-14
* @update:
*/
#ifndef __EXTERN_MUTAL_H__
#define __EXTERN_MUTAL_H__

#include "distribution_def.h"





extern void ExecuteFunctioncode(FrameRtu* pRtu, SimulationStationServer* server);
extern uint8_t StationCommunicationServer(StationPoint* point);
extern void StationExecuteFunctioncode(StationPoint* point);

#endif
