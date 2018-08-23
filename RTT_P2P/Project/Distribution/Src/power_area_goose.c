/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      power_area_goose.c
  * @brief:     配电区域相关处理与计算, 针对数据属性信息进行修改
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-23
  * @update:    
  */
  
  
#include "distribution.h"


static ErrorCode JudgeUpdatePowerAreaCompleted(SwitchProperty* switchProperty, bool* updated);
static ErrorCode ReStartUpdatePowerArea(SwitchProperty* switchProperty);
static ErrorCode SignExitFaultMessage(SwitchProperty* switchProperty);
static ErrorCode SignRemovalMessage(SwitchProperty* switchProperty, ResultType reult);
static ErrorCode SignInsulateMessage(SwitchProperty* switchProperty);
static bool IsAlreayExitedFault(DistributionStation* distributionArea);
static void GatherCompletedAndJudgeFaultArea(DistributionStation* distribution, SwitchProperty* switchProperty);
static bool IsExitFaultMessage(SwitchProperty* switchProperty);



/**
* @brief : 判断配电区域是否收集完成，两个配电区域
* @param  :SwitchProperty* switchProperty 开关
* @param  : uint8_t* updated
* @return: ErrorCode
* @update: [2018-06-14][张宇飞][]
*          [2018-06-23][张宇飞][将完成标志与收集判断统一判定]
*          [2018-06-26][张宇飞][修改累加错误]
*/
static ErrorCode JudgeUpdatePowerAreaCompleted(SwitchProperty* switchProperty, bool* updated)
{
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
    DistributionStation* distributionArea = switchProperty->distributionArea;
    CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
    DistributionPowerArea* area = distributionArea->powerArea;
    
    *updated = false;
    for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
    {
        //判断是否需要收集
       if (!switchProperty->isGather[i])
       {          
           continue;
       }      
        if ((area->upadetedFull != 0) && (area->upadetedFull == area->upadetedFlag))
        {
            area->isUpadeted = true;
            *updated = true;
        }
        else
        {
            area->isUpadeted = false;
            *updated = false;
            return ERROR_OK_NULL;
        }
        
    }

    return ERROR_OK_NULL;
}

/**
* @brief : 标识配电区域内是否存在故障信息
* @param  :SwitchProperty* switchProperty 开关
* @return: 0-正常 非0错误
* @update: [2018-06-14][张宇飞][]
*          [2018-06-23][张宇飞][适应两个配电区]
*		   [2018-07-10][张宇飞][将更新标志与更新配电区域合并]
*/
static ErrorCode SignExitFaultMessage(SwitchProperty* switchProperty)
{
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
    DistributionStation* distributionArea = switchProperty->distributionArea;
    CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
    DistributionPowerArea* area = distributionArea->powerArea;
    //清空状态位
    for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
    {
        if (switchProperty->isExitArea[i] && 
            (switchProperty->fault.state == FAULT_YES))
        {            
              area->isExitFaultMessage = true;
              switchProperty->isGather[i] = true; //标记收集与否   
              distributionArea->switchRef->isGather[i] = true;
			 
        } 
		//SET_N_BIT(area->upadetedFlag, switchProperty->indexArea[i]);
                     
    } 
    return ERROR_OK_NULL;
}
/**
* @brief : 标记移除成功信息
* @param  :SwitchProperty* switchProperty 开关属性
* @return:
* @update: [2018-07-07][张宇飞][创建]
*[2018-07-16][张宇飞][添加形参ResultType reult，改属性赋值为开关赋值]
*/
ErrorCode SignRemovalMessage(SwitchProperty* switchProperty, ResultType reult)
{

	CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
	DistributionStation* distributionArea = switchProperty->distributionArea;
	CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
	DistributionPowerArea* area = distributionArea->powerArea;
	//清空状态位
	for (uint8_t i = 0; i < distributionArea->areaCount; i++)
	{
		if (switchProperty->isExitArea[i] )
		{			
			area->removalType = reult;
		}
		area++;
	}
	return ERROR_OK_NULL;
}
/**
* @brief : 标记隔离信息
* @param  :SwitchProperty* switchProperty 开关属性
* @return:
* @update: [2018-07-07][张宇飞][创建]
*/
static  ErrorCode SignInsulateMessage(SwitchProperty* switchProperty)
{
	CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
	DistributionStation* distributionArea = switchProperty->distributionArea;
	CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
	DistributionPowerArea* area = distributionArea->powerArea;
	//清空状态位
	for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
	{
		if (switchProperty->isExitArea[i])
		{
			area->insulateType = switchProperty->insulateType;
			//标记失败信息
			if (area->insulateType == RESULT_FAILURE)
			{
				distributionArea->isExitedInsulateFailure = true;
			}
		}
		
	}
	return ERROR_OK_NULL;
}

/**
* @brief : 检测邻居信息是否完整
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-06-21][张宇飞][]
*/
ErrorCode CheckNeighboorTopologyCompleted(SwitchProperty* curent, ListDouble* list, bool* result)
{
    uint8_t num;   
    uint32_t* neighboor;
    TopologyMessage* topology ;
    ErrorCode error;
    *result = false;
    CHECK_POINT_RETURN(curent, NULL, ERROR_NULL_PTR);   
    neighboor = curent->neighbourCollect;
    CHECK_POINT_RETURN(neighboor, NULL, ERROR_NULL_PTR);
    num = curent->neighbourNum;
    CHECK_POINT_RETURN(num, 0, ERROR_OVER_LIMIT);   
    for (uint8_t i = 0; i < num; i++)
    {
        error = FindTopologyNodeByID(list, neighboor[i], &topology);
        if (error != ERROR_OK_NULL)
        {
            *result = false;
            return ERROR_OK_NULL;
        }
       
    }
    *result = true;
    return ERROR_OK_NULL;
}

/**
* @brief : 通过交互，获取邻居拓扑信息 APP 作为任务单独使用
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-06-21][张宇飞][]
*          [2018-06-23][张宇飞][添加内存分配释放函数]
*		   [2018-07-19][张宇飞][改变形参为StationPoint* point]
*/
ErrorCode GetNeighboorTopologyByMutual(StationPoint* point)
{
    TopologyMessage* topology;
    ErrorCode error = ERROR_OK_NULL;
    uint8_t num;    
    uint32_t* neighboor;
  
   
    CHECK_POINT_RETURN(point, NULL, ERROR_NULL_PTR);
    
	SwitchProperty *switchProperty = point->topology.localSwitch;
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
    neighboor = switchProperty->neighbourCollect;
    CHECK_POINT_RETURN(neighboor, NULL, ERROR_NULL_PTR);
    num = switchProperty->neighbourNum;
    CHECK_EQUAL_RETURN(num, 0, ERROR_OVER_LIMIT);
	ListDouble* topologylist = &(point->topology.globalTopologyList);
    CHECK_EQUAL_RETURN(list_size(topologylist), 0, ERROR_OVER_LIMIT);
   
	
    
    for (uint8_t i =0 ; i < num; i++)
    {
        //跳过已有的
        error = FindTopologyNodeByID(topologylist, neighboor[i], &topology);
        if (error == ERROR_OK_NULL)
        {          
            continue;
        }
        SendGetTopologyMessage(neighboor[i], &(point->transferNode));
    }
  
    return error;
}
/**
* @brief : 发送获取拓扑结构信息
* @param  :uint32_t dest 目的ID
* @param  :NodeFifo* nodeFifo 节点队列
* @return:
* @update: [2018-07-4][张宇飞][创建]
*[2018-07-19][张宇飞][修改形参，修改发送方式]
*/
ErrorCode SendGetTopologyMessage(uint32_t dest, DatagramTransferNode* pTransferNode)
{
    ErrorCode error = ERROR_OK_NULL;
    PointUint8 packet;
    error = MakeSingleGetMessage(dest, GET_TOPOLOGY, &packet);
    CHECK_UNEQUAL_RETURN(error, ERROR_OK_NULL, error);
    MakePacketMessage(&packet, GET_UINT16(dest), GET_UINT16(pTransferNode->id));

	error = pTransferNode->Send(pTransferNode, &packet);
   
    return error;
}


/**
* @brief : 判断邻居是否收集完整，若完整则更新配电区域
* @param  :StationTopology* topology 拓扑节点
* @return:
* @update: [2018-06-22][张宇飞][创建]
*/
ErrorCode UpdateDistributionPowerArea(StationTopology* topology)
{
    ErrorCode error;
    bool result;
    CHECK_POINT_RETURN(topology, NULL, ERROR_NULL_PTR);

    error = CheckNeighboorTopologyCompleted(topology->localSwitch,
        &(topology->globalTopologyList), &result);
    
    if (error == ERROR_OK_NULL)
    {
        if (result == true)
        {
            if (!topology->powerArea.isComplted)
            {
                StationCalDistributionPowerArea(topology);//邻居齐全且未计算配电区域，进行配电区域计算
            }           
        }
        topology->isNeighbourComplted = result; //更新已完成标识
    }
    return error;       
}

/**
* @brief :  判断是否已经存在故障区域
* @param  ：DistributionStation* distriStation 配电区域点
* @return: 0-正常 非0错误
* @update: [2018-07-07][张宇飞][]
*/
static bool IsAlreayExitedFault(DistributionStation* distributionArea)
{
	DistributionPowerArea* area = distributionArea->powerArea;
	for (uint8_t i = 0; i < distributionArea->areaCount; i++)
	{
		if (area->isFaultArea)
		{
			return true;
		}
		area++;
	}
	return false;
}
/**
* @brief :  判断是否收集完成，如果完成则进行故障判，一旦出现故障不再取消故障（只能置故障）。
* @param  ：DistributionStation* distriStation 配电区域点
* @return: 0-正常 非0错误
* @update: [2018-07-07][张宇飞][]
*/
static void GatherCompletedAndJudgeFaultArea(DistributionStation* distribution, SwitchProperty* switchProperty)
{
	if (!distribution->isGatherCompleted)
	{
		distribution->JudgeUpdateCompleted(distribution->switchRef, &(distribution->isGatherCompleted));
	}
	if (distribution->isGatherCompleted)
	{
		distribution->JudgeIsExitFaultArea(distribution, true);
		distribution->isGatherCalculateCompleted = true;
	}
}
/**
* @brief : 判断是否存在故障信息
* @param  ：DistributionStation* distriStation 配电区域点
* @return: bool
* @update: [2018-07-10][张宇飞][创建]
*/
static bool IsExitFaultMessage( SwitchProperty* switchProperty)
{
	DistributionStation* distributionArea = switchProperty->distributionArea;
	DistributionPowerArea* area = switchProperty->distributionArea->powerArea;
	for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
	{
		if (switchProperty->isGather[i] && area->isExitFaultMessage)
		{
			return true;
		}
	}
	return false;
}
/**
* @brief : 启动更新配电区域
* @param  :SwitchProperty* switchProperty 开关
* @return: 0-正常 非0错误
* @update: [2018-06-22][张宇飞][适应一个到两个配电区域]
*/
static ErrorCode ReStartUpdatePowerArea(SwitchProperty* switchProperty)
{
	CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
	DistributionStation* distributionArea = switchProperty->distributionArea;
	CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
	DistributionPowerArea* area = distributionArea->powerArea;
	//清空状态位
	for (uint8_t i = 0; i < distributionArea->areaCount; i++)
	{
		if (!switchProperty->isExitArea[i])
		{
			continue;
		}
		for (uint8_t k = 0; k < area->switchNum; k++)
		{
			area->areaSwitch[k]->overTimeType = OVER_TIME_NULL;
			area->areaSwitch[k]->isGather[0] = false;
			area->areaSwitch[k]->isGather[1] = false;
		}

		area->upadetedFlag = 0;
		SET_N_BIT(area->upadetedFlag, switchProperty->indexArea[i]);//把自己占用bit置1
		area->isUpadeted = false;
		area->isFaultArea = false;
		area->isExitFaultMessage = false;
		area->removalType = RESULT_TYPE_NULL;
		area->insulateType = RESULT_TYPE_NULL;
		
		area->upadetedFull = CalMaskAllOne(area->switchNum);
		area++;
	}
	distributionArea->isGatherCompleted = false;
	distributionArea->isAlreayExitedFault = false;
	distributionArea->isGatherCalculateCompleted = false;
	distributionArea->isExitedInsulateFailure = false;
	return ERROR_OK_NULL;
}
/**
* @brief :  从生成的链表配电区域提取为数组形式
* @param  ：DistributionStation* distriStation 配电区域点
* @return: 0-正常 非0错误
* @update: [2018-06-15][张宇飞][]
* [2018-06-20][张宇飞][修改查询赋值遗留]
* [2018-06-23][张宇飞][增加形参参考节点ref]
*/
ErrorCode ExtractPowerAreaFromList(SwitchProperty* switchRef, DistributionStation* distriStation, uint8_t len)
{
	CHECK_POINT_RETURN(distriStation, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(switchRef, NULL, ERROR_NULL_PTR);
	uint8_t size = 0;
	ListElment* element;
	ListDouble* list;
	DistributionPowerArea* area;
	area = distriStation->powerArea;

	//先释放内存
	if (area->areaSwitch != NULL)
	{
		FREE(area->areaSwitch);
	}
	MEMSET(area, 0, sizeof(distriStation->powerArea)); //清空区域

	for (uint8_t i = 0; i < len; i++, area++)
	{
		list = distriStation->powerAreaList + i;
		if (list != NULL)
		{
			size = list_size(list);
			if (size == 0)
			{
				continue;
			}

			element = list_head(list);
			area->areaSwitch = (SwitchProperty**)MALLOC(sizeof(SwitchProperty*) * size); //此处分配的是指针空间，不是实际数据空间
			CHECK_POINT_RETURN(area->areaSwitch, NULL, ERROR_MALLOC);
			area->switchNum = size;
			for (uint8_t k = 0; k < size; k++)
			{
				area->areaSwitch[k] = GET_SWITCH_ELEMENT(element);
				area->areaSwitch[k]->distributionArea = distriStation; //标识所在的配电区域
				area->areaSwitch[k]->indexArea[i] = k;    //设置索引
				area->areaSwitch[k]->isExitArea[i] = true;
				element = element->next;
			}
			area->upadetedFlag = 0;
			SET_N_BIT(area->upadetedFlag, switchRef->indexArea[i]);//把自己占用bit置1
			area->upadetedFull = CalMaskAllOne(area->switchNum); //计算掩码

			area->isFaultArea = false;
			area->isExitFaultMessage = false;
			area->removalType = RESULT_TYPE_NULL;
			area->insulateType = RESULT_TYPE_NULL;

			distriStation->areaCount++;

		}
	}
	distriStation->switchRef = switchRef;
	distriStation->JudgeUpdateCompleted = JudgeUpdatePowerAreaCompleted;
	distriStation->StartUpdate = ReStartUpdatePowerArea;
	distriStation->UpdatePowerArea = UpdatePowerAreaFaultState;
	distriStation->SignExitFaultMessage = SignExitFaultMessage;
	distriStation->SignRemovalMessage = SignRemovalMessage;
	distriStation->SignInsulateMessage = SignInsulateMessage;
	distriStation->JudgeIsExitFaultArea = JudgeFaultArea;
	distriStation->IsAlreayExitedFault = IsAlreayExitedFault;
	distriStation->GatherCompletedAndJudgeFaultArea = GatherCompletedAndJudgeFaultArea;
	distriStation->JudgeIsExitFault = IsExitFaultMessage;
	distriStation->isComplted = true;
	distriStation->isGatherCompleted = false;
	distriStation->isGatherCalculateCompleted = false;
	distriStation->isAlreayExitedFault = false;
	distriStation->isExitedInsulateFailure = false;
	return ERROR_OK_NULL;

}
