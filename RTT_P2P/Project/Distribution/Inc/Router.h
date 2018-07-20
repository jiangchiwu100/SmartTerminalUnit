/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      topology.c
  * @brief:     
  * @version:   V1.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
#ifndef __ROUTER_H__
#define __ROUTER_H__

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




#define GET_NODEFIFO_ELEMENT(express) ((NodeFifo*)(express->data))

#ifdef	__cplusplus
extern "C" {
#endif

extern ErrorCode RouteInit(Router* router);
extern uint8_t BuildNodeFifo(uint32_t id, uint16_t capacity, NodeFifo** node);
extern ErrorCode AddNodeFifo(Router* router, uint32_t id, uint16_t capacity, NodeFifo** addNde);
extern uint8_t RouterNodeSendData(const Router* router, uint32_t fromID, uint32_t toID, uint8_t* data, uint16_t len);
extern uint8_t RouterNodeReciveData(Router* router, uint32_t fromID, uint32_t* toID, uint8_t* data, uint16_t *reciveLen);
extern uint8_t RouterBroadcast(Router* router);

#ifdef	__cplusplus
}
#endif


#endif
