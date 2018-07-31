/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_online.c
* @brief:     用于分布式在线与离线处理
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-31
* @update:
*/


#include "distribution.h"
#include "distribution_online.h"
#include "distribution_config.h"





/**
* @brief : 检测是否处于在线状态，已经是否能够投入分布式
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-31][张宇飞][创建]
*/
ErrorCode CheckIsOnlineStatus(StationPoint* pPoint)
{
	CHECK_POINT_RETURN_LOG(pPoint, NULL, ERROR_NULL_PTR, 0);
	StationTopology* pTtopology = &(pPoint->topology);
	CHECK_POINT_RETURN_LOG(pTtopology, NULL, ERROR_NULL_PTR, 0);
	ListDouble* pSwitchList = &(pTtopology->globalSwitchList);
	CHECK_POINT_RETURN_LOG(pSwitchList, NULL, ERROR_NULL_PTR, 0);
	SwitchProperty* nodeSwitch;
	SwitchProperty* pLocalSwitch = pTtopology->localSwitch;
	uint8_t cnValid = 0;

	//检测所有状态是否有效:条件是收集完成且相等
	uint8_t size = list_size(pSwitchList);
	if ((size != pTtopology->areaID.count) ||
		(pTtopology->areaID.count == 0))
	{
		InValidCheckStamp(pLocalSwitch->onlineStamp);
		return ERROR_OK_NULL;
	}


	//是否全是上线有效状态，需要先判定此状态
	if (!pTtopology->isValidAll)
	{
		InValidCheckStamp(pLocalSwitch->onlineStamp);
		return ERROR_OK_NULL;
	}
	else
	{
		ValidCheckStamp(pLocalSwitch->onlineStamp);
	}
	//广播发送本地在线信息
	TransmitMessageExtern(pLocalSwitch, &(pPoint->transferNode), ONLINE_STATUS, BROADCAST_ADDRESS);
	
	//检测是否全部在线
	FOR_EARCH_LIST_START(pSwitchList);
	{
		nodeSwitch = GET_SWITCH_ELEMENT(m_foreach);
		//检测有效状态下时间是否超时
		if (nodeSwitch->onlineStamp.isValid)
		{
			if (SystemIsOverTime(nodeSwitch->onlineStamp.updateTime, ONLINE_LIFT_TIME))
			{
				InValidCheckStamp(nodeSwitch->onlineStamp);
			}
			else
			{
				cnValid++;
			}
		}

	}
	FOR_EARCH_LIST_END();
	//系统上线
	//说明所有节点均处于在线状态
	if (cnValid == size)
	{
		ValidCheckStamp(pTtopology->systemOnlineStamp);
		rt_kprintf("Online Status!\n");
	}
	else
	{
		InValidCheckStamp(pTtopology->systemOnlineStamp);
		rt_kprintf("Offline Status!\n");
	}
	return ERROR_OK_NULL;
}

/**
* @brief : 更新在线信息
* @param :uint8_t data[]
* @param : uint8_t len
* @param : StationPoint* point
* @return: void
* @update: [2018-07-31][张宇飞][创建]
*/
void  StationUpdateOnlineStatusMessage(uint8_t data[], uint8_t len, StationPoint* point)
{
	uint32_t id;
	uint16_t index;
	SwitchProperty* switchProperty;
	uint8_t result;

	ListDouble* list = &(point->topology.globalSwitchList);
	if (data == NULL || point == NULL)
	{
		perror("data == NULL  || point == NULL!.\n");
		return;
	}


	//校验长度
	if (data[0] * 5 + 1 == len)
	{
		for (uint16_t i = 0; i < data[0]; i++)
		{
			id = COMBINE_UINT32(data[4 + i * 5], data[3 + i * 5], data[2 + i * 5], data[1 + i * 5]);
			index = 5 + i * 5;
			result = FindSwitchNodeByID(list, id, &switchProperty);
			if (result != ERROR_OK_NULL)
			{
				return;
			}
			OnlineStatus status = data[5 + i * 5];

			if (status == ONLINE_ON)
			{
				ValidCheckStamp(switchProperty->onlineStamp);
			}
			else if (status == ONLINE_OFF)
			{
				InValidCheckStamp(switchProperty->onlineStamp);
				InValidCheckStamp(point->topology.systemOnlineStamp);
			}
			
		}
	}
	else
	{
		perror("Data length is  wrong!.\n");
	}

}