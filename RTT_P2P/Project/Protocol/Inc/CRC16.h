/* 
 * File:   CRC16.h
 * Author: ZFreeGo
 *
 * Created on 2014年9月17日, 上午9:56
 */

#ifndef CRC16_H
#define	CRC16_H
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif



extern uint16_t CRC16(uint8_t *puchMsg, uint8_t usDataLen);

#ifdef	__cplusplus
}
#endif

#endif	/* CRC16_H */

