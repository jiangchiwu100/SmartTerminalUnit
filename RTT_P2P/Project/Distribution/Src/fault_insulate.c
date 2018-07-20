/**
  *             Copyright (C) TOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_insulate.c
  * @brief:     用于故障隔离
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-07
  * @update:    
  */
  
#include "fault_insulate.h"   
#include "topology.h" 
#include "helper.h"

#include "distribution_helper.h"
#include "extern_interface.h"


/**
* @brief :故障移除——超时处理
* @param
* @return: 0-正常
* @update: [2018-06-06][张宇飞][BRIEF]
*[2018-07-11][张宇飞][隔离处理状态]
*/
StateResult FaultInsulate_Deal(FaultDealHandle* handle)
{
	SwitchProperty* switchProperty = handle->switchProperty;
	if (handle->nextState != INSULATE_DEAL)
	{
		return RESULT_ERROR_MATCH;
	}

	handle->state = INSULATE_DEAL;
	
	switch (handle->step)
	{
	case INSULATE_INIT: //初始化状态
	{			
		handle->limitTime = handle->reciveRemovalSuccessTime;
		handle->GetNowTime(handle);
		handle->nextState = INSULATE_DEAL;
		handle->step = INSULATE_IS_REMOVE;
		break;
	}
	case INSULATE_IS_REMOVE: //延时收集切除成功信息
	{
		if (handle->IsRemovalSuccessInsulate(handle))
		{
			if (handle->IsPermitOpen(handle))
			{
				handle->OpenOperate(handle);//分闸操作
				handle->limitTime = handle->checkOpenTime;
				handle->GetNowTime(handle);
				handle->step = INSULATE_DECT;
				handle->nextState = INSULATE_DEAL;
			}
			break;
		}

		if (handle->IsOverTime(handle))
		{			
			handle->nextState = DISTRIBUTION_LOCK;
		}
		break;
	}	
	case INSULATE_DECT://延时判别跳闸成功
	{
		if (handle->IsOpenPosition(handle))
		{
			switchProperty->insulateType = RESULT_SUCCESS;
			handle->TransmitMessage(handle, INSULATE_MESSAGE);// 成功隔离消息
			PrintIDTipsTick(handle->switchProperty->id, "Insulate SUCCESS");
			handle->step = 0;
			handle->nextState = TRANSFER_MASTER; //此处转移到转供电逻辑
			break;
		}

		if (handle->IsOverTime(handle))
		{
			switchProperty->operateType = OPERATE_REJECT_OPEN;
			handle->TransmitMessage(handle, STATUS_MESSAGE);
			switchProperty->insulateType = RESULT_FAILURE;
			handle->TransmitMessage(handle, INSULATE_MESSAGE);// 失败隔离消息
			PrintIDTipsTick(handle->switchProperty->id, "Insulate FAILURE");
			handle->nextState = PROTECT_LOCK;
		}
		break;
	}
	case INSULATE_BACKUP: //上一级失败进入
	{
		if (handle->IsPermitOpen(handle))
		{
			handle->OpenOperate(handle);//分闸操作
			handle->limitTime = handle->checkOpenTime;
			handle->GetNowTime(handle);
			handle->step = INSULATE_DECT;
			handle->nextState = INSULATE_DEAL;
		}
		break;		
	}
	case INSULATE_AREA_BACKUP: //隔离区域备份状态
	{
		handle->limitTime = handle->reciveRemovalSuccessTime;
		handle->GetNowTime(handle);
		handle->nextState = INSULATE_DEAL;
		handle->step = INSULATE_BACKUP_IS_MEET;
		break;
	}
	case INSULATE_BACKUP_IS_MEET://隔离区域备份状态是否满足条件
	{
		if (handle->IsRemovalSuccessInsulateBackup(handle))
		{
			if (handle->IsPermitOpen(handle))
			{
				PrintIDTipsTick(handle->switchProperty->id, "Insulate area backup");
				handle->OpenOperate(handle);//分闸操作
				handle->limitTime = handle->checkOpenTime;
				handle->GetNowTime(handle);
				handle->step = INSULATE_DECT;
				handle->nextState = INSULATE_DEAL;
			}
			break;
		}

		if (handle->IsOverTime(handle))
		{
			handle->nextState = DISTRIBUTION_LOCK;
		}
		break;
	}
	default:
	{
		perror("error step:");
		return RESULT_ERROR_EXIST;		
	}		
	}
	handle->lastState = handle->state;
	return RESULT_NULL;
}






