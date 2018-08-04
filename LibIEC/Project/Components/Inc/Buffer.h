/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      buffer.h
* @brief:     buffer
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-27
* @update
*/


#ifndef __buffer_H
#define __buffer_H

#include <stdint.h>

typedef struct TagFIFO
{
    uint16_t capacity;//FIFO总容量
    uint16_t count;//运算个数
    uint16_t head;//队尾索引，入队索引
    uint16_t end;  //队头索引，出队索引
    uint8_t* pData;
} FifoInformation;

typedef struct TagPointUint8
{
	uint8_t* pData; //数据指针
	uint16_t len; //指向数据长度
}PointUint8;


typedef struct TagFifoHandle
{
    FifoInformation fifo;
    uint8_t(*Enqueue)(struct TagFifoHandle *const fifohandle, uint8_t const indata);
    uint8_t(*Dequeue)(struct TagFifoHandle *const fifohandle, uint8_t*  dedata);
    uint8_t(*Peek)(struct TagFifoHandle *const fifohandle, uint8_t  *peekdata, uint8_t len);
} FifoHandle;





void FifoInit(FifoHandle *handle, PointUint8 *pdata);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif

