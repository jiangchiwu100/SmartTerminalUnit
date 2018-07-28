/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      station_manager.h
  * @brief:     站点服务器
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-11
  * @update:    
  */
#ifndef __STATION_MANGER_H__
#define __STATION_MANGER_H__

#include <stdint.h>

#include "distribution_def.h"




extern  void  GainMangerNeighbourMembers(StationManger* manager);
extern  ErrorCode GetNeighboorRunState(StationPoint* point);
extern ErrorCode GetAllTopologyRunState(StationPoint* point);

extern StationManger g_StationManger;
extern DatagramTransferNode g_VirtualNode;

extern uint8_t  StationMangerInit(StationManger* manger);
extern uint8_t StationMangerAddMember(StationManger* manger, uint32_t id);
extern ErrorCode StationManagerAddMemberByTopology(TopologyMessage*  topologyMessage, StationManger* manger);

extern uint8_t StationServerInit(StationServer* server);
extern ErrorCode StationServerAddPoint(StationServer* server, 
                           TopologyMessage*  topologyMessage, StationPoint** pstation);
extern StationPoint* FindStationPointById(const  ListDouble* list, uint32_t id);

extern void MantaiceFrameDeal(uint8_t* pData, uint8_t len);
extern void StationPointFrameDeal(uint8_t* pData, uint8_t len);

#endif
