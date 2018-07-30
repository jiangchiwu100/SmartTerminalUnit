/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_interface.c
  * @brief:     分布式接口函数
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-07
  * @update:   
  */


#include "distribution_interface.h"   
#include "distribution.h"

#include "distribution_config.h"

static __inline bool SystemNowTime(FaultDealHandle* handle) ;
static __inline uint32_t DiffTime(uint32_t lastTime);
static __inline bool IsOverTime(FaultDealHandle* handle);
static __inline bool IsOpenPosition(FaultDealHandle* handle);
static __inline bool IsFault(FaultDealHandle* handle);
static __inline bool IsTrigger(FaultDealHandle* handle);
static __inline bool IsOpenPosition(FaultDealHandle* handle);
static __inline bool IsClosePosition(FaultDealHandle* handle);
static __inline bool IsGatherCompleted(FaultDealHandle* handle);
static __inline bool IsFaultEdgeConnected(FaultDealHandle* handle);
static __inline bool IsFaultArea(FaultDealHandle* handle);
static __inline bool IsRejectArea(FaultDealHandle* handle);
static __inline bool IsRejectInsulate(FaultDealHandle* handle);
static __inline bool IsPermitOpen(FaultDealHandle* handle);
static bool OpenOperate(FaultDealHandle* handle);
static bool CloseOperate(FaultDealHandle* handle);
static uint8_t TransmitData(FaultDealHandle* handle, PointUint8* packet);
static bool TransmitMessage(FaultDealHandle* handle, FuncionCode code);
static bool FaultRemovalReset(FaultDealHandle* handle);
static __inline bool IsRemovalSuccessInsulate(FaultDealHandle* handle);
static __inline bool IsRemovalSuccessInsulateBackup(FaultDealHandle* handle);
static __inline bool IsRemovalSuccessBackup(FaultDealHandle* handle);
static __inline bool IsPowerIncomeLossFault(FaultDealHandle* handle);

static bool IsExitRemoveSuccessPath(StationTopology* topology);
/**
  * @brief :判断时间是否超时
  * @param  
  * @return: 0-正常返回
  * @update: [2018-06-09][张宇飞][BRIEF]
  */
bool SystemIsOverTime(uint32_t startTime, uint32_t limitTime)
{
    uint32_t currentTime = rt_tick_get();
	if (UINT32_MAX - limitTime < startTime) //判断是否溢出,若溢出则进行则先判断是否超出一个周期
    {
         if( currentTime < startTime)//先判断是否小于startTime,已经为下一个周期
         {
			if (currentTime >= (limitTime + startTime))
             {
                 return true;
             }
         }
    }
    else
    {
        if (currentTime >= startTime + limitTime)
        {
            return true;
        }
    }
    return false;
}

/**
* @brief ：传输信息, 此处动态内存，内部分配，内部释放
* @param ：
* @param ： len    元字节数组长度
* @param ： ListDouble* switchList    待更新列表
* @return： 0-正常返回
* @update： [2018-06-26][张宇飞][BRIEF]
* [2018-07-28][张宇飞][添加循环状态LOOP_STATUS]
*/
ErrorCode TransmitMessageExtern(const SwitchProperty* const switchProperty, DatagramTransferNode* pTransferNode, FuncionCode code, uint16_t destAddress)
{
    uint8_t result = 0;
    PointUint8 packet;   
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(pTransferNode, NULL, ERROR_NULL_PTR);

    switch (code)
	{
	case LOOP_STATUS:
    {
        result = MakeSingleLoopStatusMessage(switchProperty->id, switchProperty->fault.state,
            switchProperty->state,
            switchProperty->operateType, switchProperty->overTimeType,
            &packet);
        break;
    }
    case STATUS_MESSAGE:
    {
        result = MakeSingleStatusMessage(switchProperty->id, switchProperty->fault.state,
            switchProperty->state,
            switchProperty->operateType, switchProperty->overTimeType,
            &packet);
        break;
    }
    case REMOVAL_MESSAGE:
    {
        result = MakeRemovalMessage(switchProperty, switchProperty->removalType, &packet);
        break;
    }
	case INSULATE_MESSAGE:
	{
		result = MakeInsulateMessage(switchProperty->id, switchProperty->insulateType, &packet);
		break;
	}
	case  TRANSFER_MESSAGE:
	{

		break;
	}
    default:
        return ERROR_UNKONOW;       
    }

    if (result)
    {
        return (ErrorCode)result;
    }
    
    MakePacketMessage(&packet, destAddress, GET_UINT16(pTransferNode->id));
    result = pTransferNode->Send(pTransferNode, &packet);
   
    return (ErrorCode)result;
}
/**
* @brief ：传输信息, 此处动态内存，内部分配，内部释放
* @param ：
* @param ： len    元字节数组长度
* @param ： ListDouble* switchList    待更新列表
* @return： 0-正常返回
* @update： [2018-06-26][张宇飞][BRIEF]
*[2018-07-19][张宇飞][修改形参]
*/
ErrorCode TransmitTransferMessageExtern(DatagramTransferNode* pTransferNode, TransferCode code, uint32_t objectID, uint16_t destAddress)
{
	uint8_t result = 0;
	PointUint8 packet;
	CHECK_POINT_RETURN(pTransferNode, NULL, ERROR_NULL_PTR);

	switch (code)
	{
	case  TRANSFER_REMAINDER_CAP:
	{
		//TODO:容量暂假设为ID
		MakeCapacityMessage(code, objectID, objectID, &packet);
		break;
	}
	case  TRANSFER_SET:
	case  TRANSFER_CANCER:
	{
		MakeTransferSetCancerMessage(code, objectID, &packet);
		break;
	}

		default:
		return ERROR_UNKONOW;
	}

	MakePacketMessage(&packet, destAddress, GET_UINT16(pTransferNode->id));
	result = pTransferNode->Send(pTransferNode, &packet);

	

	return (ErrorCode)result;
}

/**
  * @brief :获取当前时间
  * @param  RemovalHandle* handle 控制句柄
  * @return: 0-正常返回
  * @update: [2018-06-05][张宇飞][BRIEF]
  */
static __inline bool SystemNowTime(FaultDealHandle* handle)
{
	handle->startTime =  rt_tick_get();
    return NULL;
}
/**
  * @brief :获取当前的时间差值，最多一个周期事件
  * @param  RemovalHandle* handle 控制句柄
  * @return: 时间差值
  * @update: [2018-06-05][张宇飞][BRIEF]
  */
static __inline uint32_t DiffTime(uint32_t lastTime)
{
    uint32_t currentTime = rt_tick_get();
    if (currentTime >= lastTime)
    {
        return currentTime - lastTime;
    }
    else
    {
        return  UINT32_MAX  - lastTime + currentTime;
    }    
	
}

/**
  * @brief :判断时间是否超时
  * @param  RemovalHandle* handle 控制句柄
  * @return: 0-正常返回
  * @update: [2018-06-05][张宇飞][BRIEF]
  */
static __inline bool IsOverTime(FaultDealHandle* handle)
{
    return SystemIsOverTime(handle->startTime, handle->limitTime);
}



/**
* @brief :是否为发送故障
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*/
static __inline bool IsFault(FaultDealHandle* handle)
{
    if (handle->switchProperty->fault.state == FAULT_YES)
    {
        return true;
    }
	return false;
}
/**
* @brief :是否为分位
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*[2018-07-09][张宇飞][错误分析]
*/
static __inline bool IsOpenPosition(FaultDealHandle* handle)
{
    if (handle->switchProperty->state == SWITCH_OPEN)
    {
        return true;
    }
    return false;
}
/**
* @brief :是否为合位
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*[2018-07-09][张宇飞][错误分析]
*/
static __inline bool IsClosePosition(FaultDealHandle* handle)
{
	if (handle->switchProperty->state == SWITCH_CLOSE)
	{
		return true;
	}
	return false;
}
/**
* @brief :是否为触发，外部故障信息触发,并进行标记
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*/
static __inline bool IsTrigger(FaultDealHandle* handle)
{
    DistributionStation* distributionArea = handle->switchProperty->distributionArea;
    DistributionPowerArea* area = distributionArea->powerArea;
    
    for (uint8_t i = 0; i < distributionArea->areaCount; i++)
    {
        if (area->isExitFaultMessage)
        {            
            return true;
        }
        area++;
    }
    return false;
}
/**
* @brief :信息从触发至今是否收集完成
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-15][张宇飞][BRIEF]
*[2018-07-09][张宇飞][修正地址复制错误,收集且计算完成]
*/
static __inline bool IsGatherCompleted(FaultDealHandle* handle)
{
	DistributionStation* distributionArea = handle->switchProperty->distributionArea;
   
	return distributionArea->isGatherCalculateCompleted;
	//if (!distributionArea->isGatherCompleted)
	//{
	//	distributionArea->JudgeUpdateCompleted(handle->switchProperty, &(distributionArea->isGatherCompleted));
	//}
	//return distributionArea->isGatherCompleted;
//	return handle->switchProperty->distributionArea->isGatherCompleted;
}
/**
* @brief :是否为故障区域边缘，且为联络开关路径上
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-14][张宇飞][BRIEF]
[2018-07-16][张宇飞][增加跳数条件不全为1]
*/
static __inline bool IsFaultEdgeConnected(FaultDealHandle* handle)//故障区域边缘，且为联络开关路径上
{
    bool isFaultEdgeConnected = handle->switchProperty->fault.isFaultEdgeConnected;
	ListDouble* list = &(handle->switchProperty->parent->connectPath);
	uint8_t size = list_size(list);
	uint8_t num = 0;
	if (size == 0)
	{
		return false;
	}

	FOR_EARCH_LIST_START(list);
	if (((ConnectPath*)list_data(m_foreach))->hopsNumber <= 1)
	{
		num++;
	}
	FOR_EARCH_LIST_END();

	if ((num != size) && (isFaultEdgeConnected))
	{
		return true;
	}
	else
	{
		return false;
	}


}
/**
* @brief :是否为故障区域
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-13][张宇飞][BRIEF]
*          [2018-06-23][张宇飞][适应双配电区域]
*/
static __inline bool IsFaultArea(FaultDealHandle* handle)
{
    DistributionStation* distributionArea = handle->switchProperty->distributionArea;
   	
	//如果已经是故障区域，则跳过直到复归
	//if (!distributionArea->isAlreayExitedFault)
	//{
	//	distributionArea->JudgeIsExitFaultArea(distributionArea, true); //判断是否存在故障区域		
	//	distributionArea->isAlreayExitedFault = distributionArea->IsAlreayExitedFault(distributionArea);
	//}
	    
    return distributionArea->isAlreayExitedFault;
	
}

/**
* @brief :判断是否启动后备跳闸，条件: 1.后备开关,2.拒动信息来自同一故障区域
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-07-07][张宇飞][BRIEF]
*[2018-07-09][张宇飞][修改判别错误]
*[2018-07-11][张宇飞][修改累加，取消配电区域判断，改为直接判断拒分]         
*/
static __inline bool IsRejectArea(FaultDealHandle* handle)
{
	DistributionStation* distributionArea = handle->switchProperty->distributionArea;
	DistributionPowerArea* area = distributionArea->powerArea;
	
	for (uint8_t i = 0; i < distributionArea->areaCount; i++, area++)
	{		
		for (uint8_t k = 0; k < area->switchNum; k++)
		{
			if (OPERATE_REJECT_OPEN == area->areaSwitch[k]->operateType)
			{
				return true;
			}
		}
		
	}
	return false;
}

/**
* @brief :是否为切除成功--针对隔离使用
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-07-07][张宇飞][BRIEF]
*
*/
static __inline bool IsRemovalSuccessInsulate(FaultDealHandle* handle)
{
	DistributionStation* distributionArea = handle->switchProperty->distributionArea;
	DistributionPowerArea* area = distributionArea->powerArea;

	for (uint8_t i = 0; i < distributionArea->areaCount; i++)
	{
		//是故障区域，且切除成功
		if (area->isFaultArea && area->removalType == RESULT_SUCCESS)
		{
			return true;
		}
		area++;
	}
	return false;
}
/**
* @brief :检测到电源路径上是否有切除成功信息
* @param :StationTopology* topology
* @return:
* @update: [2018-07-16][张宇飞][]
*/
static bool IsExitRemoveSuccessPath(StationTopology* topology)
{
	bool state = false;
	SwitchProperty* node;
	ListDouble* list;

	for (uint8_t i = 0; i < topology->connect.count; i++)
	{
		list = topology->connect.path + i;

		FOR_EARCH_LIST_START(list);
		node = GET_SWITCH_ELEMENT(m_foreach);
		if (RESULT_SUCCESS == node->removalType)
		{
			state = true;
			break;
		}	
		FOR_EARCH_LIST_END();
	}
	return state;
}
/**
* @brief :是否为切除成功--针对隔离备份使用
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-07-16][张宇飞][BRIEF]
*
*/
static __inline bool IsRemovalSuccessInsulateBackup(FaultDealHandle* handle)
{
	//SwitchProperty* switchProperty = handle->switchProperty;
	
	bool state = IsExitRemoveSuccessPath(handle->switchProperty->parent);
	
	return state;
}
/**
* @brief :是否为切除成功--针对后备保护使用
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-07-08][张宇飞][BRIEF]
*
*/
static __inline bool IsRemovalSuccessBackup(FaultDealHandle* handle)
{
	DistributionStation* distributionArea = handle->switchProperty->distributionArea;
	DistributionPowerArea* area = distributionArea->powerArea;

	for (uint8_t i = 0; i < distributionArea->areaCount; i++)
	{
		//切除成功
		if (area->removalType == RESULT_SUCCESS)
		{
			return true;
		}
		area++;
	}
	return false;
}
/**
* @brief :是否为故允许分闸
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*/
static __inline bool IsPermitOpen(FaultDealHandle* handle)
{
	//return handle->switchProperty->isPermitOpen;
    return true;
}
/**
* @brief :隔离失败，且是在配电区域内部
* @param  FaultDealHandle* handle
* @return: true bool
* @update: [2018-07-11][张宇飞][BRIEF]
*/
static __inline bool IsRejectInsulate(FaultDealHandle* handle)
{
	SwitchProperty* switchProperty = handle->switchProperty;
	//位于联络开关路径上，存在切除失败信息  list_size(&(switchProperty->parent->connectPath)) > 0
	return ((handle->IsFaultEdgeConnected(handle))
		&& (switchProperty->distributionArea->isExitedInsulateFailure));
}
/**
* @brief :分闸操作
* @param  RemovalHandle* handle 控制句柄
* @return: true bool
* @update: [2018-06-16][张宇飞][BRIEF]
*/
static bool OpenOperate(FaultDealHandle* handle)
{   
    handle->switchProperty->isOpening = true;
	return true;
}
/**
* @brief : 合闸操作
* @param  FaultDealHandle* handle
* @return: true bool
* @update: [2018-07-12][张宇飞][BRIEF]
*/
static bool CloseOperate(FaultDealHandle* handle)
{	
	handle->switchProperty->isClosing = true;
	return true;
}
/**
* @brief :发送数据
* @param
* @return: 0-正常
* @update: [2018-06-14][张宇飞][BRIEF]
[2018-07-28][张宇飞][修改广播地址]
*/
static uint8_t TransmitData(FaultDealHandle* handle, PointUint8* packet)
{
    CHECK_POINT_RETURN(handle, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);
	
	DatagramTransferNode* pTransferNode = handle->pTransferNode;
    MakePacketMessage(packet, BROADCAST_ADDRESS, GET_UINT16( (handle->pTransferNode->id)));// FFFF广播地址
    return pTransferNode->Send(pTransferNode, packet);
}
/**
* @brief ：传输信息, 此处动态内存，内部分配，内部释放
* @param ：RemovalHandle* handle 句柄
* @param ： len    元字节数组长度
* @param ： ListDouble* switchList    待更新列表
* @return： 0-正常返回
* @update： [2018-06-07][张宇飞][BRIEF]
*/
bool TransmitMessage(FaultDealHandle* handle, FuncionCode code)
{
	ErrorCode result;	 
	
    result = TransmitMessageExtern(handle->switchProperty, handle->pTransferNode, code, BROADCAST_ADDRESS);
	if (ERROR_OK_NULL == result)
	{
        return true;
	}
    else
    {
        return false;
    }
}
/**
* @brief :是否允许转供电
* @param  FaultDealHandle* handle
* @return: true bool
* @update: [2018-07-12][张宇飞][BRIEF]
*/
static bool IsPermitInTransferPowerMode(FaultDealHandle* handle)
{
//	bool state;
	ConnectSwitch* connect = &( handle->switchProperty->parent->connect);
	if (connect->isConnect)
	{
		return CheckIsPermitTransferpower(connect);
	}
	return false;
}
/**
* @brief :是否为电源进线失压故障
* @param  FaultDealHandle* handle
* @return: true bool
* @update: [2018-07-12][张宇飞][BRIEF]
*/
static __inline bool IsPowerIncomeLossFault(FaultDealHandle* handle)
{
	return (handle->switchProperty->type == SWITCH_TYPE_BREAKER_POWER) 
		&& (handle->switchProperty->fault.state == FAULT_INCOME_LOSS);
}

/**
  * @brief :故障切除句柄初始化
  * @param  
  * @return: 0-正常
  * @update: [2018-06-11][张宇飞][BRIEF]
  *[2018-07-19][张宇飞][修改形参]
  */
bool RemovalHandleInit(FaultDealHandle* handle, SwitchProperty* switchProperty, DatagramTransferNode* pTransferNode)
{
	if (handle == NULL || switchProperty == NULL || pTransferNode==NULL)
    {
        return ERROR_NULL_PTR;
    }
    handle->state = REMOVAL_START;
    handle->lastState = REMOVAL_START;
    handle->nextState = REMOVAL_START;
    handle->step = 0;
    handle->startTime = 0;
    handle->limitTime = 0;
    
    handle->t1 = 300;
    handle->t2 = 150;
    handle->t3 = 200;
    handle->t4 = 400;
    handle->t5 = 400;
    
    handle->checkOpenTime = 100;        
    handle->checkBackupTime = 400;
    handle->rejectTime = 100;
	handle->reciveRemovalSuccessTime = 500;
	handle->reciveConnectTime = 500;

    handle->switchProperty = switchProperty;
    
    handle->pTransferNode = pTransferNode;
    handle->isCheckPass = false;
	handle->isRun = false;
    handle->IsFault = IsFault;    
    handle->IsOpenPosition = IsOpenPosition;
	handle->IsClosePosition = IsClosePosition;
    handle->IsTrigger = IsTrigger;
    handle->IsGatherCompleted = IsGatherCompleted;
    handle->IsFaultEdgeConnected = IsFaultEdgeConnected;
    handle->IsFaultArea = IsFaultArea;
	handle->IsRejectArea = IsRejectArea;
	handle->IsRejectInsulate = IsRejectInsulate;
    handle->IsPermitOpen = IsPermitOpen;
    handle->OpenOperate = OpenOperate;
	handle->CloseOperate = CloseOperate;
	handle->IsRemovalSuccessInsulate = IsRemovalSuccessInsulate;
	handle->IsRemovalSuccessBackup = IsRemovalSuccessBackup;
	handle->IsRemovalSuccessInsulateBackup = IsRemovalSuccessInsulateBackup;
    handle->DiffTime = DiffTime;    
    handle->GetNowTime = SystemNowTime;
    handle->IsOverTime = IsOverTime;
	
    handle->TransmitMessage = TransmitMessage;
	handle->IsPermitInTransferPowerMode = IsPermitInTransferPowerMode;
	handle->TransmitData = TransmitData;
	handle->IsPowerIncomeLossFault = IsPowerIncomeLossFault;
    handle->Reset = FaultRemovalReset;
   
    return true;
}

/**
* @brief :故障切除状态复归状态，复归配电区域，开关状态，故障切除
* @param
* @return: 0-正常
* @update: [2018-06-23][张宇飞][创建,注意复归顺序]
*/
static bool FaultRemovalReset(FaultDealHandle* handle)
{
    if (handle == NULL)
    {
        return ERROR_NULL_PTR;
    }
    SwitchProperty* switchProperty = handle->switchProperty;
    DistributionStation* distributionArea = switchProperty->distributionArea;
    //重启更新
    distributionArea->StartUpdate(switchProperty);


    
    handle->step = 0;
    handle->startTime = 0;
    handle->limitTime = 0;
    handle->state = REMOVAL_START;
    handle->lastState = REMOVAL_START;
    handle->nextState = REMOVAL_START;
	handle->isRun = false;
	handle->isCheckPass = false;

	//开关状态复位
	switchProperty->insulateType = RESULT_TYPE_NULL;
	switchProperty->removalType = RESULT_TYPE_NULL;
	switchProperty->operateType = OPERATE_NULL;
	switchProperty->overTimeType = OVER_TIME_NULL;
    return true;
}
/**
* @brief :故障切除句柄更新switchProperty
* @param
* @return: 0-正常
* @update: [2018-06-11][张宇飞][BRIEF]
*/
uint8_t RemovalHandleUpdateSwitchProperty(FaultDealHandle* handle, SwitchProperty* switchProperty)
{
    handle->switchProperty = switchProperty;
    return 0;
}
/**
* @brief :RemovalHandle局部自检 --待完善
* @param
* @return: bool
* @update: [2018-06-16][张宇飞][TODO:待完善]
* [2018-06-23][张宇飞][添加复归操作]
*/
ErrorCode RemovalHandleCheckself(FaultDealHandle* handle)
{
    CHECK_POINT_RETURN(handle, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(handle->switchProperty, NULL, ERROR_NULL_PTR);    
    CHECK_POINT_RETURN(handle->switchProperty->distributionArea, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(handle->pTransferNode, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(handle->IsOpenPosition, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(handle->IsTrigger, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(handle->IsGatherCompleted, NULL, ERROR_NULL_PTR);
    //首次检测通过后进行复归操作
    if (!handle->isCheckPass)
    {
        handle->Reset(handle);
    }
    handle->isCheckPass = true;
    
    return ERROR_OK_NULL;
}
