/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_interface.h
  * @brief:     故障处理，故障判据，故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
#ifndef __DISTRIBUTION_INTERFACE_H__
#define __DISTRIBUTION_INTERFACE_H__


#include "distribution_def.h"

extern bool SystemIsOverTime(uint32_t startTime, uint32_t limitTime);
extern bool RemovalHandleInit(FaultDealHandle* handle, SwitchProperty* switchProperty,
	DatagramTransferNode* pTransferNode);
extern ErrorCode RemovalHandleCheckself(FaultDealHandle* handle);
extern uint8_t RemovalHandleUpdateSwitchProperty(FaultDealHandle* handle, SwitchProperty* switchProperty);

extern ErrorCode TransmitMessageExtern(const SwitchProperty* const switchProperty, DatagramTransferNode* pTransferNode, FuncionCode code, uint16_t destAddress);
extern ErrorCode TransmitTransferMessageExtern(DatagramTransferNode* pTransferNode, TransferCode code, uint32_t objectID, uint16_t destAddress);
extern bool FaultRemovalReset(FaultDealHandle* handle);
#endif
