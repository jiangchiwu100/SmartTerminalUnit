/**
  *             Copyright (C) TOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_removal.c
  * @brief:     故障移除，此处主要指故障移除
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-05
  * @update:    
  */
  
#include "fault_removal.h"   
#include "topology.h" 
#include "helper.h"
#include "extern_interface.h"
#include "distribution_helper.h"
#include "fault_insulate.h"



/**
  * @brief :故障移除状态——启动Start
  * @param  
  * @return: 0-正常
  * @update: [2018-06-04][张宇飞][BRIEF]
  *  [2018-07-11][张宇飞][添加切除拒动]
  *[2018-09-03][张宇飞][取消delaygatger判别，修改t1为最大时间暂设为50ms]
  *[2018-09-05][张宇飞][区分触发与直接故障检测]
  *[2018-09-07][张宇飞][故障状态时增加快照]
  */
 StateResult RemovalState_Start(FaultDealHandle* handle)
{
    SwitchProperty* switchProperty = handle->switchProperty;
    StateResult result = RESULT_NULL;
    if (handle->nextState != REMOVAL_START)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_START;
        
	if (handle->IsFault(handle) )
	{	
        handle->isRun = true;
		//此处可能和后面的冲突
		switchProperty->distributionArea->StartUpdate(switchProperty);// 重新复位
        switchProperty->distributionArea->SignExitFaultMessage(switchProperty);
		//发送故障信息
		handle->TransmitMessage(handle, STATUS_MESSAGE);              
        PrintIDTipsTick(switchProperty->id, "Fault TransmitMessage");
		handle->limitTime = handle->t1; 
		handle->step = 0;
		handle->GetNowTime(handle);
		handle->nextState = REMOVAL_GATHER;
		Station_Snapshoot();
	}
	else if (handle->IsTrigger(handle))
	{
		//发送非故障信息
        handle->isRun = true;
		switchProperty->distributionArea->SignExitFaultMessage(switchProperty);
		handle->TransmitMessage(handle, STATUS_MESSAGE);
        PrintIDTipsTick(switchProperty->id, "Trigger TransmitMessage");
		handle->limitTime = handle->t1;
		handle->GetNowTime(handle);

		handle->isFirstReadySend = true;
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

/**
  * @brief :故障移除状态——故障信息收集状态Gather
  * @param  
  * @return: 0-正常
  * @update: [2018-06-05][张宇飞][BRIEF]
  *  [2018-09-03][张宇飞][超时作为最大期限处理]
  *  [2018-09-05][张宇飞][对于不存在故障情况，跳转到后备处理]
  */
StateResult RemovalState_Gather(FaultDealHandle* handle)
{
    //StateResult result = RESULT_NULL;
    
    if (handle->nextState != REMOVAL_GATHER)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_GATHER;

    //是否超时
	if (handle->IsOverTime(handle))
	{       
        //handle->switchProperty->overTimeType = OVER_TIME_GATHER;
		//handle->nextState =  REMOVAL_OVERTIME;
		handle->nextState = REMOVAL_TREATMENT;

	}
//	else
//	{
//		if (handle->IsGatherCompleted(handle))
//		{
//			handle->nextState = REMOVAL_TREATMENT;
//		}
//		else
//		{
//			handle->nextState = REMOVAL_GATHER;
//		}
//	}
	
		
	handle->lastState = handle->state;
	return RESULT_NULL;
}

/**
  * @brief :故障移除状态——延时检测故障收集状态
  * @param  
  * @return: 0-正常
  * @update: [2018-05-25][张宇飞][BRIEF]
  *  [2018-06-04][张宇飞][首次]
  *[2018-07-07][张宇飞][添加进入隔离状态的判别条件]
  *[2018-09-05][张宇飞][修改时间判断]
  *[2018-10-11][张宇飞][添加首次触发后发送故障信息，TODO:应该提取故障信息]
  */
StateResult RemovalState_DelayGather(FaultDealHandle* handle)
{
    SwitchProperty* switchProperty = handle->switchProperty;
    if (handle->nextState != REMOVAL_DELAY_GATHER)
    {
        return RESULT_ERROR_MATCH;
    }
	handle->state = REMOVAL_DELAY_GATHER;

    switch(handle->step)
    {
        case 0:
        {
            //是否超时
        if (handle->IsOverTime(handle))
        {
        	if(handle->IsFault(handle))
			{
        		handle->nextState = REMOVAL_TREATMENT;

			}
        	else
        	{
        		handle->step =  1;
        	}

            //handle->limitTime =  handle->t1 -  handle->t2;
            //handle->GetNowTime(handle);

        }
        else if ( handle->IsFault(handle) && handle->isFirstReadySend)
		{
        	handle->isFirstReadySend = false;
        	switchProperty->distributionArea->SignExitFaultMessage(switchProperty);
			//发送故障信息
			handle->TransmitMessage(handle, STATUS_MESSAGE);
			PrintIDTipsTick(switchProperty->id, "Fault TransmitMessage");
		}

        break;
        }
        case 1:
        {

                //if (handle->IsGatherCompleted(handle))//覆盖问题
                {
					//是故障区域 且是联络开关路径上的开关
					bool isFaultArea = handle->IsFaultArea(handle);
					bool isFaultEdgeConnected = handle->IsFaultEdgeConnected(handle);

					rt_kprintf("isFaultArea:%d, isFaultEdgeConnected:%d.\n", isFaultArea, isFaultEdgeConnected);

                    if (isFaultArea && isFaultEdgeConnected)
                    {
						handle->step = INSULATE_INIT;
                        handle->nextState = INSULATE_DEAL;
                    }
					else if (isFaultEdgeConnected)//仅仅是联络路径上的开关
					{
						handle->step = INSULATE_AREA_BACKUP;
						handle->nextState = INSULATE_DEAL;
					}					
                    else 
                    {
                        handle->nextState = REMOVAL_EXIT;
                    }                  
                }	               	
           
            
           break;
        }
    }

	handle->lastState = handle->state;
	return RESULT_NULL;
}

/**
  * @brief :故障移除——故障处理状态 REMOVAL_TREATMENT
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  */
 StateResult RemovalState_Treatment(FaultDealHandle* handle)
{
    if (handle->nextState != REMOVAL_TREATMENT)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_TREATMENT;


    if (handle->IsFaultArea(handle))
    {
        if (handle->IsPermitOpen(handle))
        {
			PrintIDTipsTick(handle->switchProperty->id, "treatment open");
            handle->OpenOperate(handle);//分闸操作
            handle->limitTime = handle->checkOpenTime;
            handle->GetNowTime(handle);        
            handle->step = 0;
            handle->nextState =  REMOVAL_CENTER_DECTECT;
        }
    }
    else
    {        
            handle->limitTime = handle->checkBackupTime;
            handle->GetNowTime(handle);    
            handle->nextState =  REMOVAL_BACKUP;               
    }

	handle->lastState = handle->state;
	return RESULT_NULL;
}



/**
  * @brief :故障移除——控制中心的保护监测状态 REMOVAL_CENTER_DECTECT
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  *[2018-07-09][张宇飞][修正缺少break问题]
  */
 StateResult RemovalState_CenterDectect(FaultDealHandle* handle)
{
    if (handle->nextState != REMOVAL_CENTER_DECTECT)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_CENTER_DECTECT;

    switch(handle->step)
    {
     
    case 0:
    {
        if (handle->IsOverTime(handle))
        {
            //发送拒分信息
			PrintIDTipsTick(handle->switchProperty->id, "reject open");
            handle->switchProperty->operateType = OPERATE_REJECT_OPEN;
            handle->TransmitMessage(handle, STATUS_MESSAGE);
            handle->step = 1;
            handle->limitTime = handle->rejectTime;
            handle->GetNowTime(handle);    
        }
        else
        {
            //TODO：故障消失但是，没有分位信号的情况？
            if (! handle->IsFault(handle))
            {
                if (handle->IsOpenPosition(handle))
                {
                     //发送分闸信息。故障隔离成功信息
                    handle->switchProperty->removalType = RESULT_SUCCESS;
                    handle->TransmitMessage(handle, REMOVAL_MESSAGE);
					PrintIDTipsTick(handle->switchProperty->id, "Removal Sucess");
                    handle->nextState =  DISTRIBUTION_LOCK;
                }
            }
        }
		break;
    }
    case 1:
    {
        if (handle->IsOverTime(handle))
        {
            //发送切除失败
            handle->switchProperty->removalType = RESULT_FAILURE;
            handle->TransmitMessage(handle, REMOVAL_MESSAGE);
			PrintIDTipsTick(handle->switchProperty->id, "Removal  FAILURE : Backup ");
            handle->nextState =     PROTECT_LOCK;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ;
        }
        else
        {
			//故障消失或收到切除成功信息
            if ((! handle->IsFault(handle)) || (handle->IsRemovalSuccessBackup(handle)) )
            {
                     //发送保护切除成功
                    handle->switchProperty->removalType = RESULT_SUCCESS;
					handle->TransmitMessage(handle, REMOVAL_MESSAGE);
					PrintIDTipsTick(handle->switchProperty->id, "Removal Sucess: Backup ");
                    handle->nextState =  PROTECT_LOCK;
                
            }
			
        }
		break;
    }
    }
	
	handle->lastState = handle->state;
	return RESULT_NULL;
}
/**
  * @brief :故障移除——故障处理状态 REMOVAL_BACKUP
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  *[2018-07-08][张宇飞][增加关于移除成功的判别]
  */
 StateResult RemovalState_Backup(FaultDealHandle* handle)
{
    if (handle->nextState != REMOVAL_BACKUP)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_BACKUP;


    if (handle->IsOverTime(handle))
    {
        if (handle->IsPermitOpen(handle))
        {
			PrintIDTipsTick(handle->switchProperty->id, "backup overtime open");
            handle->OpenOperate(handle);//分闸操作
            handle->limitTime = handle->checkOpenTime;
            handle->GetNowTime(handle);                   
            handle->nextState =  REMOVAL_BACKUP_DECTECT;
        }
    }
    else
    {
        //故障切除
        if (! handle->IsFault(handle))
        {
            handle->nextState =  REMOVAL_EXIT;
        }  
		//相邻区域收到拒分信息，执行跳闸命令
		if (handle->IsRejectArea(handle))
		{
			if (handle->IsPermitOpen(handle))
			{
				PrintIDTipsTick(handle->switchProperty->id, "backup  open because of Reject");
				handle->OpenOperate(handle);//分闸操作
				handle->limitTime = handle->checkOpenTime;
				handle->GetNowTime(handle);
				handle->nextState = REMOVAL_BACKUP_DECTECT;
			}
		}
		if (handle->IsRemovalSuccessBackup(handle))
		{
			handle->nextState = DISTRIBUTION_LOCK;
		}
		

    }
    
	
	handle->lastState = handle->state;
	return RESULT_NULL;
}
/**
  * @brief :故障移除——控制中心的后备保护监控 REMOVAL_BACKUP_DECTECT
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  */
 StateResult RemovalState_BackupDectect(FaultDealHandle* handle)
{
    if (handle->nextState != REMOVAL_BACKUP_DECTECT)
    {
        return RESULT_ERROR_MATCH;
    }
    
	handle->state = REMOVAL_BACKUP_DECTECT;

    
    if (handle->IsOverTime(handle))
    {
        //发送拒分
        handle->switchProperty->operateType = OPERATE_REJECT_OPEN;
        handle->TransmitMessage(handle, STATUS_MESSAGE);
		PrintIDTipsTick(handle->switchProperty->id, "Backup Reject Open");
        handle->nextState =     PROTECT_LOCK;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            ;
    }
    else
    {
        if (! handle->IsFault(handle))
        {
                 //发送保护切除成功
            handle->switchProperty->removalType = RESULT_SUCCESS;
            handle->TransmitMessage(handle, REMOVAL_MESSAGE);
			PrintIDTipsTick(handle->switchProperty->id, "Backup: Removal Sucess");
            handle->nextState =  DISTRIBUTION_LOCK;  //迁移到 分布式闭锁状态
        }
    }

	
	handle->lastState = handle->state;
	return RESULT_NULL;
}
/**
  * @brief :故障移除——超时处理
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  */
StateResult RemovalState_OverTime(FaultDealHandle* handle)
{
    if (handle->nextState != REMOVAL_OVERTIME)
    {
        return RESULT_ERROR_MATCH;
    }  
   
    handle->state = REMOVAL_OVERTIME;
    handle->TransmitMessage(handle, STATUS_MESSAGE); //发送超时信息
	PrintIDTipsTick(handle->switchProperty->id, "OverTime TransmitMessage");
    handle->nextState = DISTRIBUTION_LOCK; //迁移到 分布式闭锁状态
    
    handle->lastState = handle->state;
    return RESULT_NULL;
}


