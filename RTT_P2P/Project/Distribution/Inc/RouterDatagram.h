/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      topology.c
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
#ifndef __ROUTER_DATAGRAM_H__
#define __ROUTER_DATAGRAM_H__

#include <stdint.h>
#include "buffer.h"
#include "list.h"  

#define DEBUG_TOPOLOGY  1

#if DEBUG_TOPOLOGY
#define DEBUG_PRINTF(...) rt_kprintf(...)

#else
#define DEBUG_PRINTF(express)  
    
#endif
#include "distribution_def.h"






#ifdef	__cplusplus
extern "C" {
#endif

ErrorCode RouterDatagram_NewTransferNode(uint32_t id, uint16_t capacity, DatagramTransferNode* node);
ErrorCode RouterDatagram_TransmissionCenter(ListDouble* stationPointList);
ErrorCode RouterDatagram_WritePacket(RingQueue* ring, PointUint8* pPacket);
#ifdef	__cplusplus
}
#endif


#endif
