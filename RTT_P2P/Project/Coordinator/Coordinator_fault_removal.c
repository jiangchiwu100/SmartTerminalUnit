/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      Coordinator.c
* @brief:     协调器文件，本文件主要用于补充分布式的集中控制性
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-08-22
* @update:    
*/

#include <stdbool.h>
#include "Coordinator.h"

 /**
  * @brief :协调性故障移除状态——启动Start
  * @param
  * @return: 0-正常
  * @update: [2018-08-23][张宇飞][BRIEF]
  */
 StateResult CoordinatorRemovalState_Start(FaultDealHandle* handle)
{
	SwitchProperty* switchProperty = handle->switchProperty;
	DeviceIndicate*  di = (DeviceIndicate*)(handle->switchProperty->pDeviceIndicate);
    StateResult result = RESULT_NULL;
    if (handle->nextState != REMOVAL_START)
    {
        return RESULT_ERROR_MATCH;
    }

	handle->state = REMOVAL_START;

	if (handle->IsFault(handle))
	{
        handle->isRun = true;
		//发送故障信息
		handle->TransmitMessage(handle, STATUS_MESSAGE);
        PrintIDTipsTick(switchProperty->id, "Fault TransmitMessage");
		handle->limitTime = handle->t1;
		handle->GetNowTime(handle);
		handle->nextState = REMOVAL_GATHER;
	}
	else if (handle->IsTrigger(handle))
	{
		//发送非故障信息
        handle->isRun = true;
		switchProperty->distributionArea->SignExitFaultMessage(switchProperty);
		handle->TransmitMessage(handle, STATUS_MESSAGE);
        PrintIDTipsTick(switchProperty->id, "Trigger TransmitMessage");
		handle->limitTime = handle->t2;
		handle->GetNowTime(handle);
        handle->step = 0;

		handle->nextState =  REMOVAL_DELAY_GATHER;
	}
	//是否为隔离期间拒绝接收故障
	else if (handle->IsRejectInsulate(handle))
	{
		handle->isRun = true;
		handle->step = INSULATE_BACKUP;
		PrintIDTipsTick(switchProperty->id, "Meet Reject Insulate");
		handle->nextState = INSULATE_DEAL;
	}
	//是否允许转供电
	else if (handle->IsPermitInTransferPowerMode(handle))
	{
		handle->isRun = true;
		handle->step = 0;
		PrintIDTipsTick(switchProperty->id, "Meet In TransferPower Mode");
		handle->nextState = TRANSFER_CONNECT;
	}
	else if (handle->IsPowerIncomeLossFault(handle))
	{
		handle->isRun = true;
		handle->step = INSULATE_BACKUP;
		PrintIDTipsTick(switchProperty->id, "Power income Loss Fault");
		handle->nextState = INSULATE_DEAL;
	}


	else
	{
		handle->nextState =  REMOVAL_START;
	}

	handle->lastState = handle->state;
	return result;
}


