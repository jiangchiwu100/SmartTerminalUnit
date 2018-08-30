/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      parse_action.h
  * @brief:     解析执行
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
#ifndef __PARSE_IMPLEMENT_H__
#define __PARSE_IMPLEMENT_H__
#include "topology.h"
#include "station_manager.h"

#include <stdint.h>
//初始化

extern uint8_t StationInit(StationTopology* station,  uint32_t id);
extern void StationAddTopologyByArrayData(uint8_t data[], uint8_t len, StationTopology* station );
extern void StationCalDistributionPowerArea(StationTopology* station);
extern void StationTestPath(StationTopology* station);
extern void StationTestPathAll(StationTopology* station);
extern void StationSetLocalMessage(uint8_t data[], uint8_t len, StationPoint* point );
extern void StationSetAimMessage(uint8_t data[], uint8_t len, StationTopology* station);
extern void StationDeleteTopologyNode(uint8_t data[], uint8_t len, StationTopology* station);
extern void StationReciveReplyMessage(uint8_t data[], uint8_t len, StationTopology* station);
extern void StationReciveGetMessage(FrameRtu* pRtu, StationPoint* point);
extern void StationReciveRemovalMessage(uint8_t data[], uint8_t len, StationTopology* station);
extern void  StationReciveInsulateMessage(uint8_t data[], uint8_t len, StationTopology* station);
extern uint8_t MakeSingleStatusMessage(uint32_t id, FaultState state, SwitchState position,
    OperateType operateType, OverTimeType overTime,
    PointUint8* packet);
extern uint8_t MakeSingleLoopStatusMessage(uint32_t id, FaultState state, SwitchState position,
    OperateType operateType, OverTimeType overTime,
    PointUint8* packet);

extern ErrorCode MakeSimpleMessage(FuncionCode,  uint32_t id, uint8_t data, PointUint8* packet);
extern ErrorCode MakeCapacityMessage(TransferCode code, uint32_t id, uint32_t data, PointUint8* packet);
extern ErrorCode MakeRemovalMessage(const SwitchProperty* const switchProperty, ResultType type, PointUint8* packet);
extern ErrorCode MakeTransferSetCancerMessage(TransferCode code, uint32_t id, PointUint8* packet);
extern ErrorCode MakeSingleGetMessage(uint32_t id, GetMessageCmd cmd, PointUint8* packet);
extern ErrorCode MakeInsulateMessage(uint32_t id, ResultType result, PointUint8* packet);
extern ErrorCode MakeSingleReplyTopologyMessage(TopologyMessage* topology, PointUint8* packet);
extern ErrorCode SendLogMessage(uint16_t source, uint16_t dest);
extern void  ManagerAddStation(uint8_t data[], uint8_t len, StationManger* manger);

extern void  StationSendStatusMessage(StationPoint* station);
extern void  StationUpdateStatusMessage(uint8_t data[], uint8_t len, StationPoint* point);
extern void  StationUpdateLoopStatusMessage(uint8_t data[], uint8_t len, StationPoint* point);
extern void  StationOperateSwitch(uint8_t data[], uint8_t len, SimulationStationServer* server);
extern void  StationSetConnectPath(uint8_t data[], uint8_t len, StationTopology* topology);
#endif
