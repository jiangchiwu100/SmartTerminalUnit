/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_monitor.c
* @brief:     用于分布式监控相关，维护常用状态
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-28
* @update:
*/


#include "distribution.h"
#include "distribution_monitor.h"
#include "distribution_config.h"


static ErrorCode CheckGlobalSwitchMessageValid(StationTopology* pTtopology);
static ErrorCode CheckConnectMessageValid(StationTopology* pTtopology);

/**
* @brief :检测信息的有效性
* @param  StationTopology* pTtopology
* @return: 0--正常
* @update: [2018-07-27][张宇飞][创建]
*[2018-07-30][张宇飞][修改形参为StationTopology* pTtopology]
*/
static ErrorCode CheckGlobalSwitchMessageValid(StationTopology* pTtopology)
{
	CHECK_POINT_RETURN_LOG(pTtopology, NULL, ERROR_NULL_PTR, 0);
	ListDouble* pSwitchList = &(pTtopology->globalSwitchList);
	CHECK_POINT_RETURN_LOG(pSwitchList, NULL, ERROR_NULL_PTR, 0);
	SwitchProperty* nodeSwitch;
	uint8_t cnValid = 0;
	uint8_t size = list_size(pSwitchList);
	if (size == 0)
	{
		pTtopology->isValidAll = false;
		return ERROR_OK_NULL;
	}
	FOR_EARCH_LIST_START(pSwitchList);
	{
		nodeSwitch = GET_SWITCH_ELEMENT(m_foreach);
		//检测有效状态下时间是否超时
		if (nodeSwitch->timeStamp.isValid)
		{
			if (SystemIsOverTime(nodeSwitch->timeStamp.updateTime, MONITOR_LIFT_TIME))
			{
				nodeSwitch->timeStamp.isValid = false;
			}
			else
			{
				cnValid++;
			}
		}

	}
	FOR_EARCH_LIST_END();
	if (cnValid == size)
	{
		pTtopology->isValidAll = true;
	}
	else
	{
		pTtopology->isValidAll = false;
	}
	return ERROR_OK_NULL;
}

/**
* @brief :检测联络路径上的有效性
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-30][张宇飞][创建]
*/
static ErrorCode CheckConnectMessageValid(StationTopology* pTtopology)
{
	CHECK_POINT_RETURN_LOG(pTtopology, NULL, ERROR_NULL_PTR, 0);
	ListDouble* pConnectPath =&( pTtopology->connectPath);
	ConnectPath* pPath;
	uint8_t cnValid = 0;
	uint8_t size = list_size(pConnectPath);
	if (size == 0)
	{
		pTtopology->isValidConnectPath = false;
		return ERROR_OK_NULL;
	}

	FOR_EARCH_LIST_START(pConnectPath);
	{
		pPath = (ConnectPath*)list_data(m_foreach);
		if (SystemIsOverTime(pPath->timeStamp.updateTime, MONITOR_LIFT_TIME))
		{
			pPath->timeStamp.isValid = false;
		}
		else
		{
			cnValid++;
		}
	}
	FOR_EARCH_LIST_END();
	if (cnValid == size)
	{
		pTtopology->isValidConnectPath = true;
	}
	else
	{
		pTtopology->isValidConnectPath = false;
	}

	return ERROR_OK_NULL;

}


/**
* @brief  检测信息的有效性,开关检测，联络信息 ， TODO:超时信息的处理
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-30][张宇飞][创建]
*[2018-07-31][张宇飞][添加CheckIsOnlineStatus]
*/
ErrorCode CheckMessageValid(StationPoint* station)
{
	CHECK_POINT_RETURN_LOG(station, NULL, ERROR_NULL_PTR, 0);
	StationTopology* pTtopology = &(station->topology);
	ErrorCode error =  CheckGlobalSwitchMessageValid(pTtopology);
	if (error)
	{
		perror("CheckGlobalSwitchMessageValid ERORR:0x%x\n", error);
		return error;
	}
	error = CheckConnectMessageValid(pTtopology);
	if (error)
	{
		perror("CheckConnectMessageValid ERORR:0x%x\n", error);
		return error;
	}
    
	bool beforeValidAll = pTtopology->isValidAll;
	bool beforeSystemOnlineValid = pTtopology->systemOnlineStamp.isValid;
	bool beforeOnlineValid = pTtopology->localSwitch->onlineStamp.isValid;
	


    error = CheckIsOnlineStatus(station);
    
    if (error)
	{
		perror("CheckIsOnlineStatus ERORR:0x%x\n", error);
		return error;
	}
	if (beforeValidAll != pTtopology->isValidAll)
	{
		rt_kprintf("Switch Valid All : 0x%x\n", pTtopology->isValidAll);
	}
	if (beforeSystemOnlineValid != pTtopology->systemOnlineStamp.isValid)
	{
		rt_kprintf("SystemOnlineValid : 0x%x\n", pTtopology->systemOnlineStamp.isValid);
	}
	if (beforeOnlineValid != pTtopology->localSwitch->onlineStamp.isValid)
	{
		rt_kprintf("OnlineStamp Valid  : 0x%x\n", pTtopology->localSwitch->onlineStamp.isValid);
	}

    
	return ERROR_OK_NULL;
}
