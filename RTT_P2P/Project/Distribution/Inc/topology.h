/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      topology.h
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-25
  * @update:    
  */
#ifndef __TOPOLOGY_H__
#define __TOPOLOGY_H__


#include "distribution_def.h"

#define DEBUG_TOPOLOGY  1

#if DEBUG_TOPOLOGY
#define DEBUG_PRINTF(...) rt_kprintf(...)

#else
#define DEBUG_PRINTF(express)  
    
#endif




/**
* 获取拓扑链表内容 express--链表元素  输出类型 TopologyMessage*
*/
#define GET_TOPOLOGY_ELEMENT(express) ((TopologyMessage*)(express->data))
#define GET_SWITCH_ELEMENT(express) ((SwitchProperty*)(express->data))


#ifdef	__cplusplus
extern "C" {
#endif
//更新配电区域故障状态
extern ErrorCode UpdatePowerAreaFaultState(SwitchProperty* switchProperty);

//增添
extern ErrorCode  AddTopologyMember(const uint8_t data[], uint8_t len,  ListDouble* topologyList);
extern ErrorCode  AddMemberByTopology(TopologyMessage*  topologyMessage, ListDouble* topologyList);
extern void DeleteTopologyListNodeById(ListDouble* topologyList, uint32_t id);


//查找相关
extern ErrorCode FindSwitchNodeByID(const ListDouble* listNeighboorSwitch, uint32_t id, SwitchProperty** find);
extern ErrorCode FindTopologyNodeByID(ListDouble* listNeighboorSwitch, uint32_t id, TopologyMessage** find);
extern ErrorCode ReserializeCollectTopology(const uint8_t* sourceArray, uint16_t startIndex, TopologyMessage** topology);
extern ErrorCode SerializeCollectTopology(const TopologyMessage* topology, PointUint8* packet, uint16_t addLen, uint16_t offset);
//内存释放相关
extern void FreeTopologyMemory(TopologyMessage** topology);
extern void FreeBFSHelper(BFSHelper** helper);
extern void DestoryBFSHelper(void* pHelper);

//转换相关
extern ErrorCode GetNeighboorSwitch(ListDouble* neighbourSwitchList, TopologyMessage* local, const  ListDouble* listNeighboor, ListDouble* listNeighboorSwitch);
extern ErrorCode GetSwitchList(const  ListDouble* listTopology, ListDouble* listSwitch);


//路经有关
extern ErrorCode BreadthFirstPath(const ListDouble* switchList, const SwitchProperty* start, BFSHelper** helper);
extern ErrorCode PathTo(const ListDouble* switchList, const BFSHelper* helper, const SwitchProperty* start, const SwitchProperty* end);
extern ErrorCode FindPath(const ListDouble* switchList, const SwitchProperty* start, const SwitchProperty* end, BFSHelper** helper);

extern ErrorCode GetAllTopologyByMutual(StationPoint* point);
extern ErrorCode CheckAllTopologyCompleted(StationPoint* point, bool* result);

//配电区域相关
extern ErrorCode GetPowerDistributionArea(TopologyMessage* local,ListDouble* listNeighboorSwitch, ListDouble* listArea);
extern ErrorCode ExtractPowerAreaFromList(SwitchProperty* switchRef, DistributionStation* distriStation, uint8_t len);
extern void FreeAreaList(ListDouble* areaList, uint8_t len);

extern ErrorCode CheckNeighboorTopologyCompleted(SwitchProperty* curent, ListDouble* list, bool* result);
extern ErrorCode GetNeighboorTopologyByMutual(StationPoint* point);
extern ErrorCode SendGetTopologyMessage(uint32_t dest, DatagramTransferNode* pTransferNode);
extern ErrorCode UpdateDistributionPowerArea(StationTopology* topology);

//联络开关相关
extern void StationCalConnectPathAndJudge(StationTopology* station);
extern bool CheckIsPermitTransferpower(ConnectSwitch* topology);
extern ErrorCode ConnectedSwitchJuadgeAPP(StationPoint* point);
extern ErrorCode SetConnectPath(uint32_t id, PathConnected isSet, uint8_t hops, StationTopology* toplogy);

extern ErrorCode ConnectPath_ResetUpdateFlag(ListDouble* list);
extern ErrorCode ConnectPath_CheckIsUpdateComplete(ListDouble* list, bool* result);
extern ErrorCode ConnectPath_MeetCondition(ListDouble* list, ConnectPath** pcp);
extern bool CheckIsLockConnectJudge(StationTopology* topology);
#ifdef	__cplusplus
}
#endif

#endif
