/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      statu_update.c
  * @brief:     提取通过数据更新的部分
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  */
 
#include "distribution_def.h"
#include "parse_implement.h" 
#include "distribution.h"
#include "log.h"
#include "distribution_config.h"




/**
* @brief : 更新状态信息，更新配电区域相关信息,
* @param :
* @return: void
* @update: [2018-06-12][张宇飞][]
*          [2018-06-23][张宇飞][适应配电区域属性修改]
*          [2018-06-26][张宇飞][设拓扑结构只含有一个开关，开关数与列表数不相等，则更新开关列表]
*		   [2018-07-09][张宇飞][收集完成与否判别，将判别放到标志内还是外部，如何分工]
*		    [2018-07-28][张宇飞][添加更新标识]
*[2018-07-30][张宇飞][添加联络开关判别闭锁]
*[2018-08-23][张宇飞][修改适应直接数据更新，取消开关列表检测， 根据数据属性信息进行计算]
*/
void  Station_updateFaultStatus(uint32_t id, StationPoint* point)
{
    SwitchProperty* switchProperty;
    ErrorCode result;
    DistributionStation* distribution;
    ListDouble* list = &(point->topology.globalSwitchList);
    if ( point == NULL)
    {
        perror("data == NULL  || point == NULL!.\n");
        return;
    }

	result = FindSwitchNodeByID(list, id, &switchProperty);
	distribution = switchProperty->distributionArea;
	if (distribution != NULL)
	{
	   distribution->SignExitFaultMessage(switchProperty);
	   //运行时, 存在故障信息时进行判别
	   if (point->removalHandle.isRun || distribution->JudgeIsExitFault(distribution->switchRef))
	   {
		   distribution->UpdatePowerArea(switchProperty);
		   if (! distribution->isGatherCalculateCompleted)
		   {
			   distribution->GatherCompletedAndJudgeFaultArea(distribution, switchProperty);
			   distribution->isAlreayExitedFault = distribution->IsAlreayExitedFault(distribution);
		   }
	   }
	}
}

/**
* @brief : 接收故障切除成功信息，只考虑单个故障切除信息
* @param :
* @return: void
* @update: [2018-07-07][张宇飞][创建]
*[2018-07-10][张宇飞][适应多个节点]
*[2018-07-16][张宇飞][改邻居列表为全局列表，修改故障标记]
*[2018-08-23][张宇飞][适应goose，修改为更新移除信息状态]
*/
void  Station_updateRemovalStatus(uint32_t id, StationTopology* station)
{

	uint16_t index = 0;
	uint32_t id, id1, id2,id3, id4;
	ResultType type;
	ErrorCode error;
	SwitchProperty* find;
	if ( station == NULL)
	{
		LogAddException(ERROR_OVER_LIMIT, 0);
		perror("station == NULL");
	}

	error = FindSwitchNodeByID(&(station->globalSwitchList), id, &find);
	if (error == ERROR_OK_NULL)
	{
		if(find->distributionArea != NULL)
		{
			find->distributionArea->SignRemovalMessage(find, type);
		}

		//(find->distributionArea != NULL) ? (find->distributionArea->SignRemovalMessage(find, type)) : (find );
	}
}
/**
* @brief : 接收隔离信息，只考虑单个隔离信息信息
* @param :
* @return: void
* @update: [2018-07-07][张宇飞][创建]
*[2018-07-12][张宇飞][搜索列表从邻居列表改为全局列表]
*[2018-08-23][张宇飞][修改为更新移除信息状态]
*/
void  Station_updateInsulateStatus(uint32_t id, StationTopology* station)
{
	if (station == NULL)
	{
		LogAddException(ERROR_OVER_LIMIT, 0);
		perror("data == NULL || len != 6 || server == NULL");
	}

	SwitchProperty* find;
	ErrorCode error = FindSwitchNodeByID(&(station->globalSwitchList), id, &find);
	if (error == ERROR_OK_NULL)
	{
        if(find->distributionArea != NULL)
        {
            find->distributionArea->SignInsulateMessage(find);
        }
		//(find->distributionArea != NULL) ? (find->distributionArea->SignInsulateMessage(find)) : (find);
	}
}

