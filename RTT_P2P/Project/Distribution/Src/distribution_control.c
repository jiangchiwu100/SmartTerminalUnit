/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_control.c
  * @brief:     提取通过数据更新的部分
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-10-12
  */
 
#include "distribution_def.h"
#include "parse_implement.h" 
#include "distribution.h"
#include "log.h"
#include "distribution_config.h"
#include "coordinator.h"
#include "station_manager.h"  
#include "ied_data_ref.h"
#include "server_model.h"
#include "status_update.h"
#include "extern_interface.h"
#include "snapshoot.h"
#include "distribution_control.h"
/**
* @brief : 更新然后发送
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static void UpdateAndSend( StationPoint* point)
{
	DatagramTransferNode* pTransferNode = &(point->transferNode);
	TransmitMessageExtern(point->topology.localSwitch, pTransferNode, STATUS_MESSAGE, BROADCAST_ADDRESS);
}

typedef enum TagDistributionRunState
{
	DISTRIBUTION_EXIT = 0,
	DISTRIBUTION_RESET = 2,
	DISTRIBUTION_READY = 3,
	DISTRIBUTION_RUN = 1,
}DistributionRunState;

/**
* @brief : 获取状态代码
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static uint8_t GetStateCode( bool isRest, bool isRun)
{
	uint8_t h = 0, l = 0;
	(isRest) ? (h = 0x2) : (h = 0);
	(isRun) ? (l =  0x1) : (l = 0);
	return h + l;
}
/**
* @brief : 检测是否进入能进入就绪状态
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static void CheckIsReady(SwitchProperty* local, StationPoint* point)
{
	bool isEnterAll = false;
	FOR_EARCH_LIST_START(&point->topology.globalSwitchList)
	{
		SwitchProperty* find = GET_SWITCH_ELEMENT(m_foreach);
		if(!find->isResetDistribution)
		{
			isEnterAll = false;
			break;
		}
		else
		{
			isEnterAll = true;
		}

	}
	FOR_EARCH_LIST_END();

	if (isEnterAll)
	{
		local->isResetDistribution = true;
		local->isRunDistribution = true;
		UpdateAndSend(point);
	}
}
/**
* @brief : 检测是否进入能进入运行状态
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static void CheckIsEnterRun(SwitchProperty* local, StationPoint* point)
{
	bool isEnterAll = false;
	FOR_EARCH_LIST_START(&point->topology.globalSwitchList)
	{
		SwitchProperty* find = GET_SWITCH_ELEMENT(m_foreach);
		if(! find->isRunDistribution)
		{
			isEnterAll = false;
			break;
		}
		else
		{
			isEnterAll = true;
		}

	}
	FOR_EARCH_LIST_END();

	if (isEnterAll)
	{
		local->isResetDistribution = false;
		local->isRunDistribution = true;
		UpdateAndSend(point);
	}
}
/**
* @brief : 检测是否进入能进入退出
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static void CheckIsExit(SwitchProperty* local, StationPoint* point)
{
	bool isExit = false;
	FOR_EARCH_LIST_START(&point->topology.globalSwitchList)
	{
		SwitchProperty* find = GET_SWITCH_ELEMENT(m_foreach);
		if(!find->isRunDistribution)
		{
			isExit = true;
			break;
		}
	}
	FOR_EARCH_LIST_END();

	if (isExit)
	{
		local->isResetDistribution = false;
		local->isRunDistribution = false;
		UpdateAndSend(point);
	}
}

/**
* @brief : 检测是否从复归到退出状态——不存在复归状态
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
static void CheckIsResetToExit(SwitchProperty* local, StationPoint* point)
{
	bool isExit = true;
	FOR_EARCH_LIST_START(&point->topology.globalSwitchList)
	{
		SwitchProperty* find = GET_SWITCH_ELEMENT(m_foreach);
		if(find->isResetDistribution)
		{
			isExit = false;
			break;
		}
	}
	FOR_EARCH_LIST_END();
   
	if (isExit)
	{
		local->isResetDistribution = false;
		local->isRunDistribution = false;
		UpdateAndSend(point);
	}
}
/**
* @brief : 退出站点
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
void EnterExitAndUpdate(SwitchProperty* local, StationPoint* point)
{
	local->isResetDistribution = false;
	local->isRunDistribution = false;

	UpdateAndSend(point);
}
/**
* @brief : 复归站点
* @param :
* @return: void
* @update: [2018-10-12][张宇飞][]
*/
void EnterResetAndUpdate(SwitchProperty* local, StationPoint* point)
{
	local->isResetDistribution = true;
	local->isRunDistribution = false;
    point->removalHandle.Reset(&(point->removalHandle));
	UpdateAndSend(point);
}

/**
* @brief : 站点分布式状态控制
* @param :
* @return: void
* @update: [2018-06-12][张宇飞][]
*/
void  Station_ControlDistribution(SwitchProperty* switchProperty, StationPoint* point)
{
	SwitchProperty* local = point->topology.localSwitch;
	DistributionRunState localRunState =
			GetStateCode(local->isResetDistribution, local->isRunDistribution);

	DistributionRunState outRunState =
				GetStateCode(switchProperty->isResetDistribution, switchProperty->isRunDistribution);
	switch (localRunState)
	{
	case DISTRIBUTION_EXIT://当前状态位退出，才允许进入复归状态
	{
		//触发进入复归状态
		if(DISTRIBUTION_RESET == outRunState)
		{
			EnterResetAndUpdate(local, point);
		}
		break;
	}
	case DISTRIBUTION_RESET:
	{
		if((DISTRIBUTION_RESET == outRunState) ||
            (DISTRIBUTION_READY == outRunState))
		{
			CheckIsReady(local, point);
		}
		else if (DISTRIBUTION_EXIT == outRunState)
		{
            CheckIsResetToExit(local, point);			
		}
		break;
	}
	case DISTRIBUTION_READY:
	{
		if((DISTRIBUTION_READY == outRunState) ||
            (DISTRIBUTION_RUN == outRunState))
		{
			CheckIsEnterRun(local, point);
		}
		else if (DISTRIBUTION_EXIT == outRunState  )
		{
			EnterExitAndUpdate(local, point);
		}
		break;
	}
	case DISTRIBUTION_RUN:
	{
		//触发进入复归状态
		if ((DISTRIBUTION_EXIT == outRunState  ) 
            || (DISTRIBUTION_RESET == outRunState  ))
		{
			EnterExitAndUpdate(local, point);
		}
		CheckIsExit(local, point);
		break;
	}
	default:
	{
		perror("Unknow code\r\n");
		break;
	}
	}






}

