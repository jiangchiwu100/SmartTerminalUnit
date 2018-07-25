/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      station.h
  * @brief:     转移创建
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-24
  * @update:    
  */
#ifndef __STATION_H__
#define __STATION_H__

#include "distribution_def.h"
#include "common_def.h"
#include <stdint.h>


#ifdef	__cplusplus
extern "C" {
#endif

extern ErrorCode PacketEncodeStationMessage(uint32_t* pdata, uint8_t len, PointUint8* packet, uint16_t addLen, uint16_t offset);
extern ErrorCode PacketDecodeStationMessage(AreaID* area, uint8_t* data, uint16_t len);
extern ErrorCode TransmitEncodeStationMessage_All(StationPoint* point, uint16_t destAddress);
extern ErrorCode ParseNanopb(StationPoint*, uint16_t sourceAddress, uint8_t* pdata, uint8_t len);
extern ErrorCode PacketEncodeStationMessage_All(const StationPoint* const point, PointUint8* packet, uint16_t addLen, uint16_t offset);
extern void  ManagerAddStationByStationMessage(uint8_t data[], uint8_t len, StationManger* manger);
#ifdef	__cplusplus
}
#endif

#endif
