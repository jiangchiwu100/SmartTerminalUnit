/**
  *             Copyright (C) TOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      fault_deal.c
  * @brief:     故障处理，独立故障移除中的函数
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-07-12
  * @update:    
  */
#include "fault_deal.h"   
#include "fault_removal.h"   
#include "topology.h" 
#include "helper.h"
#include "extern_interface.h"
#include "distribution_helper.h"
#include "fault_insulate.h"
#include "distribution_transfer.h"
/**
  * @brief :故障切除状态中心
  * @param  
  * @return: 0-正常
  * @update: [2018-06-06][张宇飞][BRIEF]
  * [2018-06-22][张宇飞][BRIEF]
  */
uint8_t FaultDealStateCenter(FaultDealHandle* handle)
{    
    StateResult result = RESULT_NULL;
    do
    {
        switch(handle->nextState)
        {
            case REMOVAL_START:
            {
                result = RemovalState_Start(handle);                
                break;
            }
            case REMOVAL_GATHER:
            {
                result = RemovalState_Gather(handle);
                break;
            }
            case REMOVAL_DELAY_GATHER:
            {
                result = RemovalState_DelayGather(handle);
                break;
            }
            case REMOVAL_TREATMENT:
            {
                result = RemovalState_Treatment(handle);
                break;
            }           
            case REMOVAL_CENTER_DECTECT:
            {
                result = RemovalState_CenterDectect(handle);
                break;
            }
			case REMOVAL_BACKUP:
			{
				result = RemovalState_Backup(handle);
				break;
			}
            case REMOVAL_BACKUP_DECTECT:
            {
                result = RemovalState_BackupDectect(handle);
                break;
            }
            case REMOVAL_OVERTIME:
            {
                result = RemovalState_OverTime(handle);
                break;
            }
            case INSULATE_DEAL:
            {               
				result = FaultInsulate_Deal(handle);
                break;
            }
			case TRANSFER_MASTER:
			{
				result = TransferPowerSupply_Master(handle);
				break;
			}
			case TRANSFER_CONNECT:
			{
				result = TransferPowerSupply_Connect(handle);
				break;
			}
            case REMOVAL_EXIT:
            {
              
                handle->state = REMOVAL_EXIT;
                handle->nextState = REMOVAL_STOP;
                break;
            }
            case DISTRIBUTION_LOCK:
            {
              
                handle->state = DISTRIBUTION_LOCK;
                handle->nextState = REMOVAL_STOP;
                 break;
            }
            case PROTECT_LOCK:
            {
               
                handle->state = PROTECT_LOCK;
                handle->nextState = REMOVAL_STOP;
                break;
            }
            case REMOVAL_STOP:
            {
                if (handle->isRun)
                {
                    handle->TransmitMessage(handle, STATUS_MESSAGE);
                }
                handle->isRun = false;
                handle->state = REMOVAL_STOP;
                break;
            }

            default :
            {
                result = RESULT_ERROR_EXIST;
                perror("0x%X :RESULT_ERROR_EXIST.\n", handle->switchProperty->id);                
                break;
            }
        }
        if (result)
        {
            perror("0x%X :FaultRemovalStateCenter ERROR : 0x%X.\n", handle->switchProperty->id, result);
            handle->state = PROTECT_LOCK;
            handle->nextState = PROTECT_LOCK;
            return result;
        }
        //状态更改时更新状态
        if (handle->state != handle->nextState)
        {
            rt_kprintf("0x%X :FROM: 0x%X TO: 0x%X. TICK: %6d.\n", 
                handle->switchProperty->id, handle->state, handle->nextState, rt_tick_get());
        }
    }
    while(handle->state != handle->nextState);//若状态不一致则重复进行调用，若一致退出本次调用
    return NULL;
}



