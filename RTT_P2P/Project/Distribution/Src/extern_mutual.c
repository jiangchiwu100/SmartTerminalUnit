/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      extern_mutal.c
* @brief:	  用于通信外部交互
* @version:   V0.0.1
* @author:    Zhang Yufei
* @date:      2018-06-14
* @update:
*/



#include "distribution.h"
#include "distribution_test_case.h"
#include "extern_interface.h"

#include "miscellaneous.h"
#include "database.h"
/**************************************************
*函数名： ExecuteFunctioncode()
*功能：  执行功能代码
*形参：  接收帧指针 FrameRtu* pRtu
*返回值：void
****************************************************/
void ExecuteFunctioncode(FrameRtu* pRtu, SimulationStationServer* server)
{
	//该数据帧未处理过
	if (pRtu->completeFlag == true)
	{		
        rt_kprintf("Virtual Node:0x%X, ExecuteFunctioncode, Functioncode: 0x%X.\n", pRtu->address, pRtu->funcode);
        PrintMemoryUsed();
		switch (pRtu->funcode)
		{
		case UPDATE_NEIGHBOUR:
		{
			TopologyTest(pRtu->pValidData, pRtu->datalen);
			break;
		}
		case CAL_POWER_AREA:
		{
			CalTest();
			break;
		}
		case CAL_PATH:
		{
			TestPath();
			break;
		}
		case CAL_PATH_ALL:
		{
			TestPathAll();
			break;
		}
		case LOCAL_MESSAGE:
		{
			SetLocalMessage(pRtu->pValidData, pRtu->datalen);
			break;
		}
		case AIM_MESSAGE:
		{
			SetAimMessage(pRtu->pValidData, pRtu->datalen);
			break;
		}
		case DELETE_ONE:
		{
			DeleteTopologyNode(pRtu->pValidData, pRtu->datalen);
			break;
		}
		case ADD_STATION:
		{
			ManagerAddStation(pRtu->pValidData, pRtu->datalen, &g_StationManger);
			break;
		}
        case SWITCH_OPERATE:
        {
            StationOperateSwitch(pRtu->pValidData, pRtu->datalen, server);
            break;
        }
        case GET_MESSAGE:
        {
            GainMangerNeighbourMembers(&g_StationManger);
            break;
        }
        case GET_LOG:
        {
            SendLogMessage(LOCAL_ADDRESS, 0);

            break;
        }
        case  NANOPB_TYPE:
        {
            //更新站点信息并重启
            if ((NanopbType)(pRtu->pValidData[0]) == STATION_MESSAGE)
            {
                StationManger manger;
                StationMangerInit(&manger);
                ManagerAddStationByStationMessage(pRtu->pValidData, pRtu->datalen, &manger);
                if(manger.pWorkPoint)
                {
                    bool state = StationMessageSave(manger.pWorkPoint);
                    if (state)
                    {
                        rt_kprintf("update sucess! and restart.......\n");
                        SystemReset();

                    }
                }
            }	
            break;
        }
        default:
        {
             PrintIDTipsTick(LOCAL_ADDRESS, "Unknown Funcode.");
            break;
        }

		}

		pRtu->completeFlag = false;
	}
}

/**
* @brief : 协议处理器
* @param  ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*[2018-07-20[张宇飞][适应数据包]
*/
uint8_t StationCommunicationServer(StationPoint* point)
{
	ErrorCode error = ERROR_OK_NULL;
	if (point == NULL)
	{
		perror(" ERROR: StationPoint* point = null.\n");
		return ERROR_NULL_PTR;
	}

	ProtocolAnylastDatagram* anylast = &(point->anylast);
	FrameRtu* pRtu = &(point->anylast.recvRtu);
	RingQueue* pReciveRing = &(point->transferNode.reciveRing);
	if (anylast == NULL || pRtu == NULL)
	{
		return ERROR_NULL_PTR;
	}
	while (pReciveRing->count > 0)
	
	{
		error = anylast->ProtocolAnylastDeal(anylast);


		if ((!error) &&( pRtu->completeFlag == true))
		{
			StationExecuteFunctioncode(point);
		}
		error = Datagram_Destory(anylast->pReiveDatagram);
		if (error)
		{
			perror("Datagram_Destory Error:0x%X", error);
			return error;
		}
		anylast->pReiveDatagram = NULL;
		
	} 


	return 0;
}
/**
* @brief : 协议处理器
* @param  ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*/
//uint8_t StationCommunicationServer(StationPoint* point)
//{
//    uint16_t residelen = 0, lastResideLen = 0;
//	if (point == NULL)
//	{
//		rt_kprintf(" ERROR: StationPoint* point = null.\n");
//		return ERROR_NULL_PTR;
//	}
//
//	ProtocolAnylast* anylast = &(point->anylast);
//	FrameRtu* pRtu = &(point->anylast.recvRtu);
//	if (anylast == NULL || pRtu == NULL)
//	{
//		return ERROR_NULL_PTR;
//	}
//    do 
//    {
//        residelen = anylast->ProtocolAnylastDeal(anylast);
//
//
//        if (pRtu->completeFlag == true)
//        {
//            StationExecuteFunctioncode(point);
//        }
//        if (lastResideLen == residelen)
//        {
//            break;
//        }
//        lastResideLen = residelen;
//    } while (residelen > 0);
//	
//
//	return 0;
//}

/**
* @brief : 执行功能码
* @param ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*/
void StationExecuteFunctioncode(StationPoint* point)
{
	if (point == NULL)
	{
		rt_kprintf(" ERROR: StationPoint* point = null.\n");
		return;
	}
	FrameRtu* pRtu = &(point->anylast.recvRtu);
	StationTopology* station = &(point->topology);
	ErrorCode error = ERROR_OK_NULL;

	if (pRtu == NULL || station == NULL)
	{
		rt_kprintf("ID: %X, ERROR: rtu == NULL || station==NULL.\n", point->id);
		return;
	}


	//该数据帧未处理过
	if (pRtu->completeFlag == true)
	{
		//rt_kprintf("STATION:%X, Code: %X. Tick: %6d\n", station->id, pRtu->funcode, rt_tick_get());
        switch (pRtu->funcode)
        {
        case UPDATE_NEIGHBOUR:
        {
            StationAddTopologyByArrayData(pRtu->pValidData,
                pRtu->datalen, station);
            break;
        }
        case CAL_POWER_AREA:
        {
            StationCalDistributionPowerArea(station);
            break;
        }
        case CAL_PATH:
        {
            StationCalConnectPathAndJudge(station);
            break;
        }
        case CAL_PATH_ALL:
        {
            StationTestPathAll(station);
            break;
        }
        case LOCAL_MESSAGE:
        {
            StationSetLocalMessage(pRtu->pValidData, pRtu->datalen, point);
            break;
        }
        case AIM_MESSAGE:
        {
            StationSetAimMessage(pRtu->pValidData, pRtu->datalen, station);
            break;
        }
        case DELETE_ONE:
        {
            StationDeleteTopologyNode(pRtu->pValidData, pRtu->datalen, station);
            break;
        }
        case STATUS_MESSAGE:
        {
            StationUpdateStatusMessage(pRtu->pValidData, pRtu->datalen, point);
            break;
        }
		case LOOP_STATUS:
		{
			StationUpdateLoopStatusMessage(pRtu->pValidData, pRtu->datalen, point);
		    break;
		}
        case REPLY_MESSAGE:
        {
            StationReciveReplyMessage(pRtu->pValidData, pRtu->datalen, &(point->topology));
            break;
        }
        case GET_MESSAGE:
        {
            StationReciveGetMessage( pRtu, point);
            break;
        }
        case REMOVAL_MESSAGE:
        {
			StationReciveRemovalMessage(pRtu->pValidData, pRtu->datalen, &(point->topology));
            break;
        }
		case INSULATE_MESSAGE:
		{
			StationReciveInsulateMessage(pRtu->pValidData, pRtu->datalen, &(point->topology));
			break;
		}		
        case  GET_ALL:
        {
			error = GetAllTopologyRunState(point);
            break;
        }
        case  NANOPB_TYPE:
        {		
            
			rt_kprintf("STATION:%X, Code: %X. Tick: %6d\n", station->id, pRtu->funcode, rt_tick_get());
            PrintMemoryUsed();
			error = ParseNanopb(point, pRtu->address, pRtu->pValidData, pRtu->datalen);
            break;
        }
        case CONNECTED_MESSAGE:
        {
			//故障正在运行时禁止进行路径判别
			if (!point->removalHandle.isRun)
			{
				StationSetConnectPath(pRtu->pValidData, pRtu->datalen, station);
			}
            
            break;
        }
		case TRANSFER_MESSAGE:
		{
			error = Transfer_ParseDeal(pRtu->pValidData, pRtu->datalen, station);
			break;
		}
       
        default:
        {            
            PrintIDTipsTick(point->id, "Unknown Funcode.");
            break;
        }
        }
       
		pRtu->completeFlag = false;

		if (error)
		{
			perror("StationExecuteFunctioncode: error code: 0x%X", error);
		}

	}


}

