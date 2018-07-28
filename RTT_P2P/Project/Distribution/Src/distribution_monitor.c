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

/**
* @brief :检测信息的有效性
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-16][张宇飞][创建]
*/
ErrorCode CheckMessageValid(StationPoint* station)
{
	CHECK_POINT_RETURN_LOG(station, NULL, ERROR_NULL_PTR, 0);
	ListDouble* pSwitchList = &(station->topology.globalSwitchList);
	CHECK_POINT_RETURN_LOG(pSwitchList, NULL, ERROR_NULL_PTR, 0);
	SwitchProperty* nodeSwitch;
	FOR_EARCH_LIST_START(pSwitchList);
	{
		nodeSwitch = GET_SWITCH_ELEMENT(m_foreach);
		//检测有效状态下时间是否超时
		if (nodeSwitch->isValid)
		{
			if (SystemIsOverTime(nodeSwitch->updateTime, MONITOR_LIFT_TIME))
			{
				nodeSwitch->isValid = false;
			}
		}

	}
	FOR_EARCH_LIST_END();
	

}

