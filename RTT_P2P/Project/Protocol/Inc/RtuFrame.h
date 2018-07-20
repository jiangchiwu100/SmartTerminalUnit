/* 
 * File:   RtuFrame.h
 * Author: LiDehai
 *
 * Created on 2014年12月4日, 上午7:27
 */

#ifndef RTUFRAME_H
#define	RTUFRAME_H
#include <stdint.h>
#include "buffer.h"
#include "distribution_def.h"


#ifdef	__cplusplus
extern "C" {
#endif





extern ErrorCode MakePacketMessage(PointUint8* packet, uint16_t addres, uint16_t sourceAddr);
extern uint8_t DataProtocolAnylast(ProtocolAnylast* anylast);
extern ErrorCode ProtocolAnylastInit(ProtocolAnylast* anylast, uint16_t address);
#ifdef	__cplusplus
}
#endif

#endif	/* RTUFRAME_H */

