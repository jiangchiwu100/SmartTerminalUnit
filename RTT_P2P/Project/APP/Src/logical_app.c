/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_app.c
  * @brief:     分布式保护逻辑
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-21
  * @update:     
  */

#include "distribution.h"
#include "distribution_app.h"
#include "extern_interface.h"
#include "distribution_config.h"

static struct rt_thread distribution_thread;
static struct rt_thread connected_thread;
static struct rt_thread monitor_thread;


static void DistributionLogicalApp(StationManger* manager);
static void distribution_thread_entry(void* parameter);
static void SationMonitorGainCheck(StationPoint* station);

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_distribution_thread_stack[THREAD_DISTRIBUTION_STACK_SIZE];//线程栈
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_connected_thread_stack[THREAD_CONNECT_STACK_SIZE];//线程栈
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_monitor_thread_stack[1024];//线程栈





static void distribution_thread_entry(void* parameter)
{
    rt_kprintf("thread distribution_thread_entry start.\r\n");
    DistributionLogicalApp(&g_StationManger);
}

/**
* @brief :分布式应用
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-06-16][张宇飞][创建]
*[2018-07-31][张宇飞][添加置isCheckPass为false]
*[2018-09-10][张宇飞][添加获取监控状态]
*/
static void DistributionLogicalApp(StationManger* manager)
{
    ListElment* element;
    ListDouble* list;
    uint8_t size;
    StationPoint* station;
    FaultDealHandle* handle;
    ErrorCode code;
    static uint32_t currentTime = 0;


    if (manager == NULL)
    {
        perror("DistributionLogicalApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer* server = &(manager->stationServer);
    if (server == NULL)
    {
		perror("DistributionLogicalApp ERROR :server = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    //FaultRemovalInit()
    list = &(server->stationPointList);

    
    do
    {
        element = list_head(list);
        size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            station = (StationPoint*)(element->data);
            if (station != NULL)
            {
//            	if (SystemIsOverTime(currentTime, MONITOR_CHECK_TIME))
//				{
//					SationMonitorGainCheck(station);
//					currentTime = rt_tick_get();
//				}


                handle = &(((StationPoint*)(element->data))->removalHandle);
                code = RemovalHandleCheckself(handle);
                if (code == ERROR_OK_NULL)
                {
                    FaultDealStateCenter(handle); 

					if ( (!handle->isRun) && handle->switchProperty->isChanged)
					{
						handle->switchProperty->isChanged = false;
						handle->TransmitMessage(handle, STATUS_MESSAGE);
						//handle->TransmitMessage(handle, REMOVAL_MESSAGE);
						//handle->TransmitMessage(handle, INSULATE_MESSAGE);
					}
                }  
				else
				{		
					handle->isCheckPass = false;				
				}

            }
            else
            {
                perror("StationPoint* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }

            element = element->next;
        }
        
    
        rt_thread_delay(1);
    } while (1);

}

/**
* @brief : 监控获取检测
* @param : StationServer*   stationServer
* @return: 0--正常
* @update: [2018-09-10][张宇飞][创建]
*[2018-09-11][张宇飞][添加更新配电区域]
*/
static void SationMonitorGainCheck(StationPoint* station)
{
    extern void MaintaceServer(void);

	//ConnectedSwitchJuadgeAPP(station);//获取所有开关
	ConnectedSwitch_SelfCheck_APP(station);
	if (station->topology.areaID.isGainComplted)
	{
        UpdateDistributionPowerArea(&(station->topology));
		//GetNeighboorRunState(station); //获取邻居
	}
	//周期性发送状态信息
	//StationSendStatusMessage(station);
	//CheckMessageValid(station);

}
/**
* @brief :联络功能判别
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-07-16][张宇飞][创建]
*/
static void connected_thread_entry(void* parameter)
{
    extern void MaintaceServer(void);
	rt_kprintf("thread connected_thread_entry start.\r\n");

	StationManger* manager = (StationManger*)parameter;
	if (manager == NULL)
	{
		rt_kprintf("connect ERROR :manager = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
	}
	StationServer*   stationServer = &(manager->stationServer);
	if (stationServer == NULL)
	{
		rt_kprintf("MutalCommunicationServer ERROR :router = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
	}

	ListDouble* list = &(stationServer->stationPointList);


	do
	{
		FOR_EARCH_LIST_START(list);
		{
			StationPoint* station = (StationPoint*)list_data(m_foreach);
			if (station != NULL)
			{
				SationMonitorGainCheck(station);
			}
			else
			{
				rt_kprintf("stationServer* station = NULL.\n");
				LogAddException(ERROR_NULL_PTR, 0);
				break;
			}

		}
		FOR_EARCH_LIST_END();
      //  MaintaceServer();
		rt_thread_delay(MONITOR_CHECK_TIME);
	} while (1);

}




/**
* @brief : 状态变化更新任务
* @param  :StationManger* manager
* @return:
* @update: [2018-07-13][张宇飞][]
*/
void  MonitorApp(StationManger* manager)
{
	ListElment* element;
	ListDouble* list;
	uint8_t size;
	StationPoint* station;

	if (manager == NULL)
	{
		rt_kprintf("MonitorApp ERROR :manager = NULL.\n");
	}
	StationServer* server = &(manager->stationServer);
	if (server == NULL)
	{
		rt_kprintf("MonitorApp ERROR :server = NULL.\n");
	}
	list = &(server->stationPointList);

	uint8_t cn = 0;
	const uint8_t total = 30;
	do
	{
		element = list_head(list);
		size = list_size(list);
		for (uint8_t i = 0; i < size; i++)
		{
			station = (StationPoint*)(element->data);
			if (station != NULL)
			{
				SwitchProperty* switchNode = station->topology.localSwitch;
				if ((!station->removalHandle.isRun) && (switchNode->isChanged || (cn == total)))
				{
					switchNode->isChanged = false;
					DatagramTransferNode* pTransferNode = &(station->transferNode);
					TransmitMessageExtern(switchNode, pTransferNode, STATUS_MESSAGE, BROADCAST_ADDRESS);
					TransmitMessageExtern(switchNode, pTransferNode, REMOVAL_MESSAGE, BROADCAST_ADDRESS);
					TransmitMessageExtern(switchNode, pTransferNode, INSULATE_MESSAGE, BROADCAST_ADDRESS);
				}
			}
			else
			{
				rt_kprintf("StationPoint* station = NULL.\n");
				break;
			}
			element = element->next;
		}

		if (cn++ > total)
		{
			cn = 0;
		}
		rt_thread_delay(10);
	} while (1);

}
/**
* @brief :挂起 判断线程，并置闭锁位
* @param
* @return: 0--正常
* @update: [2018-09-06][张宇飞][创建]
*/
void SuspendConnectedThread(StationTopology* topology)
{
	rt_thread_suspend(&connected_thread);
	topology->connect.isLock = true;
}
/**
* @brief :恢复 判断线程，并解除闭锁位
* @param
* @return: 0--正常
* @update: [2018-09-06][张宇飞][创建]
*/
void ResumeConnectedThread(StationTopology* topology)
{

	rt_thread_startup(&connected_thread);
	topology->connect.isLock = false;
}
/**
* @brief :分布式逻辑功能引用初始化：分布式保护，联络判别
* @param  
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void DistributionLogicalAppInit(void)
{
	rt_thread_init(&distribution_thread,                 //线程控制块
		THREAD_DISTRIBUTION_NAME,                       //线程名字，在shell里面可以看到
		distribution_thread_entry,            //线程入口函数
		RT_NULL,                      //线程入口函数参数
		&rt_distribution_thread_stack,     //线程栈起始地址
		sizeof(rt_distribution_thread_stack), //线程栈大小
		THREAD_DISTRIBUTION_PRIORITY,                            //线程的优先级
		THREAD_DISTRIBUTION_TIMESLICE);                          //线程时间片

	rt_thread_startup(&distribution_thread);

	rt_thread_init(&connected_thread,                 //线程控制块
		THREAD_CONNECT_NAME,                       //线程名字，在shell里面可以看到
		connected_thread_entry,            //线程入口函数
		&g_StationManger,                      //线程入口函数参数
		&rt_connected_thread_stack,     //线程栈起始地址
		sizeof(rt_connected_thread_stack), //线程栈大小
		THREAD_CONNECT_PRIORITY,                            //线程的优先级
		THREAD_CONNECT_TIMESLICE);                          //线程时间片

	rt_thread_startup(&connected_thread);
}


