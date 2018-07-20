/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_simulation.h
  * @brief:     解析执行
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
#ifndef __LOGICAL_SIMULATION_H__
#define __LOGICAL_SIMULATION_H__

#include <stdint.h>

//初始化

#include "distribution_def.h"



extern uint8_t SwitchRunStateSimulation(SimulationStation* station);

extern ErrorCode SimulationStationServerInit(SimulationStationServer* server);
extern ErrorCode SimulationStationServerAddMember(SimulationStationServer* server, uint32_t id, SwitchProperty** );
extern ErrorCode UpdateBindSwitchState(SimulationStation* station);
extern SimulationStation* FindSimulationStationById(const  List* list, uint32_t id);
extern ErrorCode SimulationSwitchControlOperate(SimulationStation* station, SwitchControlOperate operate);

#endif
