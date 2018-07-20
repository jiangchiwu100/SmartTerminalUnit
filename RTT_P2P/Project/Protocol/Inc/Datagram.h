/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Datagram.c
* @brief:     数据包协议
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018-07-19
* @update:    
*/

#ifndef _DATAGRAM_H
#define	_DATAGRAM_H
#include <stdint.h>
#include "buffer.h"
#include "distribution_def.h"



#ifdef	__cplusplus
extern "C" {
#endif

extern ErrorCode ProtocolAnylastDatagramInit(ProtocolAnylastDatagram* anylast, DatagramTransferNode* pTransferNode,
		uint16_t address);
extern ErrorCode Datagram_CopyToPacket(uint8_t* pdata, uint16_t size, PointUint8* pPacket);
extern ErrorCode Datagram_Destory(DatagramFrame*  pDatagram);
#ifdef	__cplusplus
}
#endif

#endif	/* RTUFRAME_H */

