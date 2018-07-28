/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      RouterDatagram.c
  * @brief:     用于数据报路由
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-18
  * @update:    
  */
  
 
#include "common_def.h"
#include "helper.h"
#include "ringqueue.h"



#include "RouterDatagram.h"
#include "log.h"


#include "Datagram.h"
#include "distribution_config.h"
#include "extern_interface.h"

static ErrorCode RouterDatagram_WriteDatagram(RingQueue* ring,  DatagramFrame*  pPacket);

static ErrorCode RouterDatagram_CopyFrame(const DatagramFrame*  frame, DatagramFrame**  pNewframe);
static ErrorCode RouterDatagram_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket);

static void VirtualMonitorRecive(DatagramTransferNode* pTransferNode, DatagramFrame* frame);
static void VirtualMonitorSend(DatagramTransferNode* pTransferNode, ListDouble* stationPointList);





/**
* @brief  : 数据报路由器写数据,不进行参数检测
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列
* @param  ：const DatagramFrame* const pDatagram 数据包数据
* @return: 0-正常，非空--异常
* @update: [2018-07-18][张宇飞][]
*/
static ErrorCode RouterDatagram_WriteDatagram(RingQueue* ring,  DatagramFrame*  pDatagram)
{
	/* CHECK_POINT_RETURN(ring, NULL, ERROR_NULL_PTR);
	 CHECK_POINT_RETURN(pDatagram, NULL, ERROR_NULL_PTR);
	 CHECK_POINT_RETURN(pDatagram->pData, NULL, ERROR_NULL_PTR);*/

	//检查是否已满
    if (ring->capacity <= ring->count)
    {
        return ERROR_VOLUM;
    }
	bool state = ring->Write(ring, pDatagram);
	if (! state)
	{
		return ERROR_UNKONOW;
	}

    return ERROR_OK_NULL;
}

/**
* @brief  : 数据报路由器发送数据，有动态分配
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列 
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-18][张宇飞][]
*/


ErrorCode RouterDatagram_WritePacket(RingQueue* ring, PointUint8* pPacket)
{
	CHECK_POINT_RETURN(ring, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(pPacket, NULL, ERROR_NULL_PTR);
	DatagramFrame*  pFrame = (DatagramFrame*)CALLOC(1, sizeof(DatagramFrame)); 
	CHECK_POINT_RETURN(pFrame, NULL, ERROR_MALLOC);
	
	//重新提取目的和源地址
	pFrame->destAddress = COMBINE_UINT16(pPacket->pData[FRAME_DEST_INDEX + 1], pPacket->pData[FRAME_DEST_INDEX]);
	pFrame->sourceAddress = COMBINE_UINT16(pPacket->pData[FRAME_SOURCE_INDEX + 1], pPacket->pData[FRAME_SOURCE_INDEX]);
	pFrame->size = pPacket->len;
	pFrame->pData = pPacket->pData;

	ErrorCode error = RouterDatagram_WriteDatagram(ring, pFrame);
    
    
	if (error == ERROR_VOLUM)
	{
		FREE(pFrame);
	}
	return error;
}




/**
* @brief  : ，有动态分配
* @param  : 队列句柄
* @param  ：RingQueue* ring 环形队列
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-18][张宇飞][]
*/
#if !UDP_SEND
static ErrorCode RouterDatagram_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket)
{
	CHECK_POINT_RETURN(node, NULL, ERROR_NULL_PTR);
	return RouterDatagram_WritePacket(&(node->sendRing), pPacket);
}
#else
static ErrorCode RouterDatagram_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket)
{
    extern ErrorCode Udp_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket);
	return Udp_SendPacketNode(node, pPacket);
}

#endif

/**
* @brief  : 新建节点
* @param  :uint32_t id
* @param  ：uint16_t capacity
* @param  ：DatagramTransferNode* node 指定已分配好的节点
* @return: error 
* @update: [2018-07-18][张宇飞][]
*/
ErrorCode  RouterDatagram_NewTransferNode(uint32_t id, uint16_t capacity, DatagramTransferNode* node)
{
	bool state;
	CHECK_POINT_RETURN_LOG(node, NULL, ERROR_NULL_PTR, 0);

	MEMSET(node, 0, sizeof(DatagramTransferNode));
	
	
	state = RingQueueInit(&(node->reciveRing), capacity);
	if (!state)
	{
		return ERROR_MALLOC;
	}
	state = RingQueueInit(&(node->sendRing), capacity);
	if (!state)
	{
		return ERROR_MALLOC;
	}
	node->id = id;
	node->Write = RouterDatagram_WritePacket;
	node->Send = RouterDatagram_SendPacketNode;
	return ERROR_OK_NULL;
}

/**
* @brief : 数据复制
* @param : const DatagramFrame*  frame
* @param : DatagramFrame**  pNewframe 新帧
* @return: ErrorCode
* @update: [2018-07-19][张宇飞][]
*/
static ErrorCode RouterDatagram_CopyFrame(const DatagramFrame*  frame, DatagramFrame**  pNewframe)
{
	*pNewframe = (DatagramFrame*)CALLOC(1, sizeof(DatagramFrame));
	CHECK_POINT_RETURN_LOG(*pNewframe, NULL, ERROR_MALLOC, 0);

	(*pNewframe)->destAddress = frame->destAddress;
	(*pNewframe)->sourceAddress = frame->sourceAddress;
	uint16_t size = frame->size;
	(*pNewframe)->size = size;
	(*pNewframe)->pData = (uint8_t*)MALLOC(size * sizeof(uint8_t));
	CHECK_POINT_RETURN_LOG((*pNewframe)->pData, NULL, ERROR_MALLOC, 0);

	MEMCPY((*pNewframe)->pData, frame->pData, size);

	return ERROR_OK_NULL;
}


extern DatagramTransferNode g_VirtualNode;

/**
* @brief : 路由器传输中心
* @param : Router* router
* @return: ErrorCode
* @update: [2018-07-18][张宇飞][]
*/
ErrorCode RouterDatagram_TransmissionCenter(ListDouble* stationPointList)
{
	CHECK_POINT_RETURN_LOG(stationPointList, NULL, ERROR_NULL_PTR, 0);
	DatagramTransferNode* transferNodeSend;
	DatagramTransferNode* transferNodeRecive;
	
	RingQueue* ringSend;
	RingQueue* ringRecive;
	DatagramFrame* frame;
	DatagramFrame* newFrame;
	ErrorCode error;
	bool state;
	FOR_EARCH_LIST_START(stationPointList);
	{
		transferNodeSend = &(((StationPoint*)list_data(m_foreach))->transferNode);
	
		do
		{
			ringSend = &(transferNodeSend->sendRing);
			state = ringSend->Read(ringSend, (void**)&frame);
			if (!state)
			{
				break;
			}

			FOR_EARCH_LIST_START(stationPointList);
			//地址不相等 && （地址符合 || 目的地址为广播地址）
			transferNodeRecive = &(((StationPoint*)list_data(m_foreach))->transferNode);
			if ((transferNodeSend->id != transferNodeRecive->id) && 
				((GET_UINT16(transferNodeRecive->id) == frame->destAddress) || (frame->destAddress == BROADCAST_ADDRESS)))
			{
				//复制一份新帧
				error = RouterDatagram_CopyFrame(frame, &newFrame);
				if (error)
				{
					perror("RouterDatagram_CopyFrame Error");
					return error;
				}
				ringRecive = &(transferNodeRecive->reciveRing);
				state = ringRecive->Write(ringRecive, newFrame);
				if (!state)
				{
					break;
				}
			}

			FOR_EARCH_LIST_END();

			VirtualMonitorRecive(&g_VirtualNode, frame);
			
			//此处释放数据 报数据与原始数据
			error = Datagram_Destory(frame);
			if (error)
			{
				perror("Datagram_Destory Error");
				return error;
			}
		} while (true);
	}
	FOR_EARCH_LIST_END();


	VirtualMonitorSend(&g_VirtualNode, stationPointList);
    return ERROR_OK_NULL;
}
/**
* @brief : 虚拟处理节点接收
* @param : Router* router
* @return: ErrorCode
* @update: [2018-07-19][张宇飞][]
*/
static void VirtualMonitorRecive(DatagramTransferNode* pTransferNode, DatagramFrame* frame)
{
	
	DatagramFrame* newFrame;
	//复制一份新帧
	ErrorCode error = RouterDatagram_CopyFrame(frame, &newFrame);
	if (error)
	{
		perror("RouterDatagram_CopyFrame Error");
		return;
	}
	RingQueue* ringRecive = &(pTransferNode->reciveRing);
	bool state = ringRecive->Write(ringRecive, newFrame);
	
	if (!state)
	{
		perror("ringRecive->Write Error");
	}
}
/**
* @brief : 虚拟处理节点发送
* @param : Router* router
* @return: ErrorCode
* @update: [2018-07-19][张宇飞][]
*/
static void VirtualMonitorSend(DatagramTransferNode* pTransferNode, ListDouble* stationPointList)
{

	DatagramTransferNode* transferNodeRecive;
	ErrorCode error;
	RingQueue* ringSend;
	RingQueue* ringRecive;
	DatagramFrame* frame;
	DatagramFrame* newFrame;
	bool state;

	do
	{
		ringSend = &(pTransferNode->sendRing);
		state = ringSend->Read(ringSend, (void**)&frame);
		if (!state)
		{
			break;
		}

		FOR_EARCH_LIST_START(stationPointList);
		//地址不相等 && （地址符合 || 目的地址为广播地址）
		transferNodeRecive = &(((StationPoint*)list_data(m_foreach))->transferNode);
		if ((pTransferNode->id != transferNodeRecive->id) &&
			((GET_UINT16(transferNodeRecive->id) == frame->destAddress) || (frame->destAddress == BROADCAST_ADDRESS)))
		{
			//复制一份新帧
			error = RouterDatagram_CopyFrame(frame, &newFrame);
			if (error)
			{
				perror("RouterDatagram_CopyFrame Error");
				return ;
			}
			ringRecive = &(transferNodeRecive->reciveRing);
			state = ringRecive->Write(ringRecive, newFrame);
			if (!state)
			{
				break;
			}
		}

		FOR_EARCH_LIST_END();
		error = Datagram_Destory(frame);
		if (error)
		{
			perror("Datagram_Destory Error");
			return ;
		}
	}
	while (true);
}
