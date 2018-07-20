/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Datagram.c
* @brief:     数据包协议
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018-07-19
* @update:    
*/
#include "RtuFrame.h"
#include "helper.h"
#include "CRC16.h"
#include "log.h"
#include <string.h>

#include "Datagram.h"

#include "station_manager.h"
#include "topology.h"
#include "extern_interface.h"

#include "distribution_enum.h"



static bool DataProtocolAnylastDatagram_Check(uint8_t* pData, uint16_t size);
static ErrorCode DataProtocolAnylastDatagram(ProtocolAnylastDatagram* anylast);


/**
* @brief : 校验数据是否满足帧格式要求，不对输入数据进行校验
* @param : ProtocolAnylast* anylast 解析处理器
* @return: 剩余的未处理数据包数量
* @update: [2018-07-19][张宇飞][创建]
*/
static bool DataProtocolAnylastDatagram_Check(uint8_t* pData, uint16_t size)
{
	uint16_t index = 0;
	uint16_t dataLen = 0, len1 = 0, len2 = 2;
	//校验最小长度
	if (size < FRAME_MIN_LEN)
	{
		return false;
	}

	if (pData[index++] != START_CHAR1)
	{
		return false;
	}
	if (pData[index++] != START_CHAR2)
	{
		return false;
	}
	len1 = pData[index++];
	len2 = pData[index++];


	dataLen = COMBINE_UINT16(len2, len1);

	if (pData[index++] != START_CHAR3)
	{
		return false;
	}
	if (pData[index++] != START_CHAR4)
	{
		return false;
	}
	if (size != FRAME_MIN_LEN + dataLen)
	{
		return false;
	}

	if (END_CHAR != pData[size - 1])
	{
		return false;
	}
	//校验CRC
	uint16_t crc = CRC16(pData, size - 3);
	uint16_t crc1 = COMBINE_UINT16(pData[size - 2], pData[size - 3]);
	if (crc != crc1)
	{
		return false;
	}


	return true;

}
/**
* @brief : 自定义数据解析处理
* @param : ProtocolAnylast* anylast 解析处理器
* @return: 剩余的未处理数据包数量
* @update: [2018-07-19][张宇飞][创建]
*/
static ErrorCode DataProtocolAnylastDatagram(ProtocolAnylastDatagram* anylast)
{
	CHECK_POINT_RETURN_LOG(anylast, NULL, ERROR_NULL_PTR, 0);
	CHECK_POINT_RETURN_LOG(anylast->pTransferNode, NULL, ERROR_NULL_PTR, 0);

	ErrorCode error;
	uint8_t data = 0;
	uint8_t result = 0;
	FrameRtu* pReciveFrame = &(anylast->recvRtu);
	RingQueue* pReciveRing = &(anylast->pTransferNode->reciveRing);
	bool state;
	CHECK_EQUAL_RETURN(pReciveRing->count, 0, ERROR_OK_NULL);

	pReciveFrame->completeFlag = false;
	if (anylast->pReiveDatagram)
	{
		error = Datagram_Destory(anylast->pReiveDatagram);
		if (error)
		{
			perror("Datagram_Destory Error:0x%X", error);
			return error;
		}
		anylast->pReiveDatagram = NULL;
	}
	state = pReciveRing->Read(pReciveRing, (void**)&(anylast->pReiveDatagram));
	if (!state)
	{
		perror("pReciveRing->Read:0x%X", error);
		return ERROR_UNKONOW;
	}
	uint8_t* pData = anylast->pReiveDatagram->pData;
	state = DataProtocolAnylastDatagram_Check(pData,
		anylast->pReiveDatagram->size);
	//开始校验
	if (!state)
	{
		error = Datagram_Destory(anylast->pReiveDatagram);
		if (error)
		{
			perror("Datagram_Destory Error:0x%X", error);
			return error;
		}
		anylast->pReiveDatagram = NULL;
		return ERROR_OK_NULL;
	}
	//赋值
	pReciveFrame->address = COMBINE_UINT16(pData[FRAME_SOURCE_INDEX + 1], pData[FRAME_SOURCE_INDEX]);
	pReciveFrame->destAddress = COMBINE_UINT16(pData[FRAME_DEST_INDEX + 1], pData[FRAME_DEST_INDEX]);
	pReciveFrame->funcode = pData[FRAME_FUNCODE_INDEX];
	pReciveFrame->pValidData = pData + FRAME_VALID_INDEX;
	pReciveFrame->pData = pData;
	pReciveFrame->datalen = COMBINE_UINT16(pData[FRAME_DATALEN_INDEX + 1], pData[FRAME_DATALEN_INDEX]);
	pReciveFrame->completeFlag = true;
	return ERROR_OK_NULL;
}

/**
*
* @param  ProtocolAnylastDatagram* anylast 分析器数据
* @param  uint16_t address
* @return ErrorCode
* @update: [2018-07-19][张宇飞][创建]
*/
ErrorCode ProtocolAnylastDatagramInit(ProtocolAnylastDatagram* anylast, DatagramTransferNode* pTransferNode,
	uint16_t address)
{
	CHECK_POINT_RETURN(pTransferNode, NULL, ERROR_NULL_PTR);
	anylast->address = address;
	anylast->ProtocolAnylastDeal = DataProtocolAnylastDatagram;
	anylast->sendRtu.address = address;
	anylast->pTransferNode = pTransferNode;
	return ERROR_OK_NULL;
}

/**
*
* @param  数据拷贝，有动态内存分配
* @param  uint8_t* pdata
* @param  uint16_t size
* @param  PointUint8* pPacket
* @return ErrorCode
* @update: [2018-07-19][张宇飞][创建]
*/
ErrorCode Datagram_CopyToPacket(uint8_t* pdata, uint16_t size, PointUint8* pPacket)
{
	CHECK_POINT_RETURN(pdata, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(pPacket, NULL, ERROR_NULL_PTR);

	pPacket->len = size;
	pPacket->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * size);
	CHECK_POINT_RETURN(pPacket->pData, NULL, ERROR_MALLOC);
	MEMCPY(pPacket->pData, pdata, size);
	return ERROR_OK_NULL;
}
/**
* @brief  : 数据报路由器写数据,不进行参数检测
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列
* @param  ：const DatagramFrame* const pDatagram 数据包数据
* @return: 0-正常，非空--异常
* @update: [2018-07-19][张宇飞][]
*/
ErrorCode Datagram_Destory(DatagramFrame*  pDatagram)
{
	CHECK_POINT_RETURN_LOG(pDatagram, NULL, ERROR_NULL_PTR, 0);

	if (pDatagram->pData)
	{
		SafeFree(pDatagram->pData);
	}
	FREE(pDatagram);
	return ERROR_OK_NULL;
}