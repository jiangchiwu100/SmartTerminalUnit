/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      power_area.c
  * @brief:     配电区域相关处理与计算
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-25
  * @update:    
  */
  
  
#include "distribution.h"




static ErrorCode DistributionAreaDeal_BreakerBranch(SwitchProperty* current, ListDouble* listArea, uint8_t areaNum);
static ErrorCode DistributionAreaDeal_SpecialDeal(SwitchProperty* current, ListDouble* listArea, uint8_t areaNum);
static ErrorCode GetIntersection(ListDouble* listNeighboorSwitch, SwitchProperty* left, SwitchProperty* right, uint8_t flag, ListDouble* listResult);
static ErrorCode JudgeUpdatePowerAreaCompleted(SwitchProperty* switchProperty, bool* updated);
static ErrorCode ReStartUpdatePowerArea(SwitchProperty* switchProperty);
static ErrorCode SignExitFaultMessage(SwitchProperty* switchProperty);
static ErrorCode SignRemovalMessage(SwitchProperty* switchProperty, ResultType reult);
static ErrorCode SignInsulateMessage(SwitchProperty* switchProperty);
static bool IsAlreayExitedFault(DistributionStation* distributionArea);
static void GatherCompletedAndJudgeFaultArea(DistributionStation* distribution, SwitchProperty* switchProperty);
static bool IsExitFaultMessage(SwitchProperty* switchProperty);
/**
 * @brief : 根据ID，查找是否存在该节点
 * @param  : SwitchProperty* left 
 * @param  ：SwitchProperty* right
 * @param  ：uint8_t flag
 * @return: 0-正常，非空--未找到或异常
 * @update: [2018-05-26][张宇飞][]
 *[2018-07-0][张宇飞][重构]
 */
ErrorCode FindSwitchNodeByID(const ListDouble* listNeighboorSwitch, uint32_t id, SwitchProperty** find)
{
	CHECK_POINT_RETURN_LOG(listNeighboorSwitch, NULL, ERROR_NULL_PTR, 0);
    
	FOR_EARCH_LIST_START(listNeighboorSwitch);

	if (GET_SWITCH_ELEMENT(m_foreach)->id == id)
	{
		*find = GET_SWITCH_ELEMENT(m_foreach);
		return ERROR_OK_NULL;
	}

	FOR_EARCH_LIST_END();

    return ERROR_UNFIND;
}
/**
 * @brief : 根据ID，查找是否存在该节点
 * @param  : ListDouble* listNeighboorSwitch 拓扑列表
 * @param  ：uint32_t id 要查找的ID
 * @param  ：TopologyMessage** find 拓扑信息节点
 * @return: 0-正常，非空--未找到或异常
 * @update: [2018-05-26][张宇飞][]
 *[2018-07-10][张宇飞][出现几次异常，怀疑括号优先级问题]
 *[2018-07-17][张宇飞][修改错误GET_TOPOLOGY_ELEMENT 错为GET_SWITCH_ELEMENT]
 */
ErrorCode FindTopologyNodeByID(ListDouble* listNeighboorSwitch, uint32_t id, TopologyMessage** find)
{
    if (listNeighboorSwitch == 0)
    {
        return ERROR_NULL_PTR;
    }
    ListElment* element = list_head(listNeighboorSwitch);        
    do
    {
        if(GET_SWITCH_ELEMENT(element)->id == id)
        {
            *find = GET_TOPOLOGY_ELEMENT(element);
            return ERROR_OK_NULL;
        }
    }
    while((element = element->next) != NULL);
    return ERROR_UNFIND;
}


/**
 * @brief : 获取配电区域交集，跳过标志属性项目
 * @param  : SwitchProperty* left 
 * @param  ：SwitchProperty* right
 * @param  ：uint8_t flag
 * @return: 0-正常 非0错误
 * @update: [2018-05-26][张宇飞][]
 */
static ErrorCode GetIntersection(ListDouble* listNeighboorSwitch, SwitchProperty* left, SwitchProperty* right, uint8_t flag, ListDouble* listResult)
{
    uint8_t len = left->neighbourNum;
    uint8_t rightLen = right->neighbourNum;
    uint32_t* leftNeighbourCollect = left->neighbourCollect;
    uint32_t* rightNeighbourCollect = right->neighbourCollect;
    SwitchProperty* find;
    uint8_t result = 0;
    
    
    for (uint8_t i = 0; i < len; i++)
    {
        for (uint8_t k = 0; k < rightLen; k++)
        {
            if (leftNeighbourCollect[i] == rightNeighbourCollect[k])//ID 匹配
            {
                //由ID号查找开关属性指针
                result = FindSwitchNodeByID(listNeighboorSwitch, leftNeighbourCollect[i], &find);
                if (result)
                {
                    continue; //
                }
                //标志是否满足，一般查看是否未处理
                if((find)->tempflag == flag)
                {
                    (find)->tempflag  = 0xff;
                    ListInsertNext(listResult, NULL, find);
                }
            }
        }     
    }
    return ERROR_OK_NULL;
}

/**
 * @brief : 特殊化处理配电区域,断路器分支开关,增加自己的单独分支,删去配电区域内的分位开关。
 * @param :  SwitchProperty* current 当前计算开关
 * @param : 求得的配电区域
 * @param ：ListDouble* listArea 配电区域数组
 * @return: ErrorCode
 * @update: [2018-05-31][张宇飞][]
 *[2018-07-06][张宇飞][修正删除错误,在均为分位的情况下]
 */
static ErrorCode  DistributionAreaDeal_BreakerBranch(SwitchProperty* current, ListDouble* listArea, uint8_t areaNum)
{
    
    //此处应该为1
    if (areaNum == 1)
    {      
        ListInsertNext( ++listArea , NULL,  current);//插入当前节点作为单独的配电区域
        
    }
    else
    {        
        return ERROR_PARAMTER;
    }           
    return ERROR_OK_NULL;
}

/**
 * @brief : 特殊化处理配电区域,
 * @param :  SwitchProperty* current 当前计算开关
 * @param : 求得的配电区域
 * @param ：ListDouble* listArea 配电区域数组
 * @return: 0-正常 非0错误
 * @update: [2018-05-31][张宇飞][]
 */
static ErrorCode DistributionAreaDeal_SpecialDeal(SwitchProperty* current, ListDouble* listArea, uint8_t areaNum)
{    
    switch(current->type)
    {
        case SWITCH_TYPE_BREAKER_BRANCH:        
        {
            DistributionAreaDeal_BreakerBranch(current, listArea, areaNum);
            break;
        }
    }
    return ERROR_OK_NULL;
}
/**
 * @brief : 获取配电区域,判定配电区域以开关为基础
 * @param  : TopologyMessage* local  本节点拓扑信息
 * @param  ：ListDouble* listNeighboor  邻居节点拓扑信息，元素类型 SwitchProperty
 * @param  ：ListDouble* listArea 配电区域数组
 * @return: 0-正常 非0错误
 * @update: [2018-05-31][张宇飞][改变双层链表为数组链表]
 */
ErrorCode GetPowerDistributionArea(TopologyMessage* local, ListDouble* listNeighboorSwitch, ListDouble* listArea)
{
    uint8_t result = 0;
    uint8_t areaNum = 0;
    ListDouble* origion = listArea;
    if ((local == NULL) || (listNeighboorSwitch == NULL))
    {
        return ERROR_NULL_PTR ;
    }
    uint8_t len = list_size(listNeighboorSwitch);
    if (len == 0 )
    {
        return ERROR_PARAMTER;
    }
    ListElment* element = list_head(listNeighboorSwitch);        
    do
    {
        GET_SWITCH_ELEMENT(element)->tempflag = 0;//清空临时标志位，为处理做准备
    }
    while((element = element->next) != NULL);
    
    areaNum = 0;
    element = list_head(listNeighboorSwitch);
    do
    {
       SwitchProperty* switchCollect = GET_SWITCH_ELEMENT(element);
       if (switchCollect->tempflag == 0)
       {
           switchCollect->tempflag = 0xff;           
           
           ListInit(listArea, NULL);
           ListInsertNext(listArea, NULL, local->switchCollect);//添加当前开关
           ListInsertNext(listArea, NULL, switchCollect);//添加选择开关
           result = GetIntersection(listNeighboorSwitch,  local->switchCollect, switchCollect, 0, listArea);
           if ( result)
           {
               return (ErrorCode)result;
           }
           //配电区域数量限制
           if (areaNum++ < POWER_AREA_NUM)
           {
                listArea += areaNum;
           }
           else
           {
                return ERROR_OVER_LIMIT;
           }
       }
    }while((element = element->next) != NULL);
    DistributionAreaDeal_SpecialDeal(local->switchCollect, origion, areaNum);
    return ERROR_OK_NULL;
}

/**
 * @brief :  销毁配电区域链表，释放占用内存
 * @param : ListDouble* areaList 数组指针
           注意没有单独分配动态分配空间。
 * @param :  len 数组长度
 * @return: void
 * @update: [2018-05-31][张宇飞][修改为释放数组指针]
 */
void FreeAreaList(ListDouble* areaList, uint8_t len)
{
    for (uint8_t i = 0; i< len ;i++)
    if (areaList+i != NULL)
    {
       Listdestroy(areaList+i);     
    }
}


/**
* @brief : 获取邻居开关, 不进行元素检查
 * @param  : destID     目的ID
 * @param  ：ListDouble* listNeighboorSwitch 邻居节点，元素类型 SwitchProperty*
 * @return: 0-正常 非0错误
 * @update: [2018-05-26][张宇飞][todo：完善内存释放机制]
 */
ErrorCode GetNeighboorSwitch(ListDouble* neighbourSwitchList,  TopologyMessage* local, const  ListDouble* listNeighboor, ListDouble* listNeighboorSwitch)
{
    ListElment* element = list_head(listNeighboor); 
    
    if (element == NULL)
    {
        rt_kprintf("element is NULL!\n");
        return ERROR_NULL_PTR;
    }
    
   
    uint8_t num = local->switchCollect->neighbourNum; 
    for (uint8_t i = 0; i < num; i++)
    {
        element = list_head(listNeighboor); 
        
        while(element != NULL)
        {       
            if (GET_TOPOLOGY_ELEMENT(element)->id ==  local->switchCollect->neighbourCollect[i])   
            {
                ListInsertNext(neighbourSwitchList, NULL, GET_TOPOLOGY_ELEMENT(element)->switchCollect);
            }
            element = element->next;            
        }
        
    }
    
    return ERROR_OK_NULL;
}


/**
* @brief : 更新配电区域故障状态
* @param  :SwitchProperty* switchProperty 开关
* @return: ErrorCode
* @update: [2018-06-22][张宇飞][适应一个到两个配电区域]
*/
ErrorCode UpdatePowerAreaFaultState(SwitchProperty* switchProperty)
{
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);    
    DistributionStation* distributionArea = switchProperty->distributionArea;
    CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
    DistributionPowerArea* area = distributionArea->powerArea;
    //清空状态位


    for (uint8_t i = 0; i < distributionArea->areaCount; i++)
    {
        //是否存在
        if (switchProperty->isExitArea[i])
        {
            SET_N_BIT(area->upadetedFlag, switchProperty->indexArea[i]);
        }        
        area++;
    }
    return ERROR_OK_NULL;
}


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
*[2018-08-30][张宇飞][对于配电区域不存在信息则取消标记]
*/
static  ErrorCode SignInsulateMessage(SwitchProperty* switchProperty)
{
	CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
	DistributionStation* distributionArea = switchProperty->distributionArea;
	CHECK_POINT_RETURN(distributionArea, NULL, ERROR_NULL_PTR);
	DistributionPowerArea* area = distributionArea->powerArea;
	
    bool isExitedFailure = false;
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
    //清除隔离消失的信息
    SwitchProperty* sw;
    SwitchProperty* switchRef = distributionArea->switchRef;
    distributionArea = switchRef->distributionArea;
    area = distributionArea->powerArea;
    for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
	{
        
		if (switchRef->isExitArea[i])
		{
			isExitedFailure = false;
            for(uint8_t k = 0; k < area->switchNum; k++)
            {
               // as/as/此处有bug
            	sw = area->areaSwitch[k];
                if (sw && (sw->insulateType == RESULT_FAILURE))
            	{
            		isExitedFailure = true;
            	}
            }
            area->insulateType = isExitedFailure;

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
* @param : StationTopology* topology 拓扑节点
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
