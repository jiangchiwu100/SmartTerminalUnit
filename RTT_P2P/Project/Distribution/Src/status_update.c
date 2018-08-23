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
#include "coordinator.h"
#include "status_update.h"

/**
* @brief : 同goose发布开关属性
* @param : SwitchProperty* sw
* @param : DeviceIndicate* di
* @return: void
* @update:[2018-08-23][张宇飞][创建]
*/
void SwitchPropertyGoosePublish(void)
{
	perror("Unimplenment\n");
}


/**
* @brief : 由数据集信息更新本属性信息
* @param : SwitchProperty* sw
* @param : DeviceIndicate* di
* @return: void
* @update:[2018-08-23][张宇飞][创建]
*/
void DataArributeToLocalProperty(SwitchProperty* sw, DeviceIndicate* di)
{
	if (!sw || !di)
	{
		perror("!sw || !di\n");
		return;
	}
	//更新故障信息 TODO 故障信息不完整
	if (DeviceIndicate_getBooleanStatus(di, DEVICE_IED_FAULT))
	{
		sw->fault.state = FAULT_YES;
	}
	else
	{
		sw->fault.state = FAULT_NO;
	}
	//更新开关状态 TODO: 双点信息
	if(DeviceIndicate_getBooleanStatus(di, DEVICE_IED_SWITCH_POS))
	{
		sw->state = SWITCH_CLOSE;
	}
	else
	{
		sw->state = SWITCH_OPEN;
	}

	//切除信息
	if(DeviceIndicate_getBooleanStatus(di, DEVICE_IED_REMOVE_SUCESS))
	{
		sw->removalType = RESULT_SUCCESS;

	}
	else
	{
		sw->removalType = RESULT_FAILURE;
	}
	//隔离信息
	if(DeviceIndicate_getBooleanStatus(di, DEVICE_IED_INSULATE_SUCESS))
	{
		sw->insulateType = RESULT_SUCCESS;

	}
	else
	{
		sw->insulateType = RESULT_FAILURE;
	}
}

/**
* @brief : 由数据集信息更新本属性信息
* @param : SwitchProperty* sw
* @param : DeviceIndicate* di
* @return: void
* @update:[2018-08-23][张宇飞][创建]
*/
void LocalPropertyToDataArribute(const SwitchProperty*  const sw, DeviceIndicate* di)
{
	if (!sw || !di)
	{
		perror("!sw || !di\n");
		return;
	}
	//更新故障信息 TODO 故障信息不完整

	if (sw->fault.state == FAULT_YES)
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_FAULT, true);
	}
	else
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_FAULT, false);
	}

	//更新开关状态 TODO: 双点信息
	if (sw->state == SWITCH_CLOSE)
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_SWITCH_POS, true);
	}
	else
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_SWITCH_POS, false);
	}


	//切除信息
	if (sw->removalType == RESULT_SUCCESS)
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_REMOVE_SUCESS, true);
	}
	else
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_REMOVE_SUCESS, false);
	}
	//隔离信息
	if (sw->insulateType == RESULT_SUCCESS)
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_INSULATE_SUCESS, true);
	}
	else
	{
		DeviceIndicate_setBooleanStatus(di, DEVICE_IED_INSULATE_SUCESS, false);
	}

}

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

	if (result != ERROR_OK_NULL)
	{
		perror("Unfind id\n");
		return;
	}

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

