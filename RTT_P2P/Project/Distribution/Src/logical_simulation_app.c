/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_simulation_app.c
  * @brief:     开关逻辑仿真任务
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-11
  * @update:    
  */
#include "logical_simulation_app.h"
#include "distribution.h"
#include <rtthread.h>
#include "communicaton.h"

#include "log.h"
#include "extern_interface.h"

void  MonitorApp(StationManger* manager);

static struct rt_thread switch_thread;//线程控制块
static struct rt_thread router_thread;
static struct rt_thread comself_thread;
static struct rt_thread communication_thread;
static struct rt_thread distribution_thread;
static struct rt_thread monitor_thread;
static struct rt_thread connected_thread;


//ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_switch_thread_stack[1024];//线程栈
static rt_uint8_t rt_router_thread_stack[2048];//线程栈
static rt_uint8_t rt_comself_thread_stack[1024];//线程栈
static rt_uint8_t rt_communication_thread_stack[1024];//线程栈
static rt_uint8_t rt_distribution_thread_stack[1024];//线程栈
static rt_uint8_t rt_monitor_thread_stack[1024];//线程栈
static rt_uint8_t rt_connected_thread_stack[1024];//线程栈

static void switch_thread_entry(void* parameter)
{    
	rt_kprintf("thread simulation start.\r\n");         
    SimulationSwitchStationLogicalApp(&g_StationManger);
}
  
static void router_thread_entry(void* parameter)
{
    rt_kprintf("thread router start.\r\n");     
    SimulationRouterApp(&g_StationManger);    
}
static void communication_thread_entry(void* parameter)
{
    rt_kprintf("thread communication start.\r\n");     
    CommunicationServeice();        
} 

static void comself_thread_entry(void* parameter)
{
    rt_kprintf("thread comself start.\r\n");     
    SimulationCommunicationApp(&g_StationManger);         
} 
static void distribution_thread_entry(void* parameter)
{
    rt_kprintf("thread distribution_thread_entry start.\r\n");
    SimulationDistributionApp(&g_StationManger);
}
static void monitor_thread_entry(void* parameter)
{
    rt_kprintf("thread monitor_thread_entry start.\r\n");
    MonitorApp(&g_StationManger);
}
static void connected_thread_entry(void* parameter)
{
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
        rt_kprintf("SimulationCommunicationServer ERROR :router = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }

    ListDouble* list = &(stationServer->stationPointList);
    ListElment* element;
    uint8_t size;
    do 
    {    		
        element = list_head(list);
        size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            StationPoint* station = (StationPoint*)(element->data);
            if (station != NULL)
            {
                ConnectedSwitchJuadgeAPP(station);//获取所有开关
				if (station->topology.areaID.isGainComplted)
				{
					GetNeighboorRunState(station); //获取邻居
				}
				
            }
            else
            {
                rt_kprintf("stationServer* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }
            element = element->next;

            
        }
        rt_thread_delay(1000);
    } while (1);
    
}

static void SimulationCommunicationServer(StationManger* manager);
/**
  * @brief :开关站点模拟
  * @param  SimulationStationServer*  simulationServer
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  */
void SimulationSwitchStationLogicalApp(StationManger* manager)
{    
    if (manager == NULL)
    {
        rt_kprintf("SimulationSwitchStationLogicalApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    SimulationStationServer*  simulationServer =&( manager->simulationServer);
    if (simulationServer == NULL)
    {
        rt_kprintf("SimulationSwitchStationLogicalApp ERROR :simulationServer = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    ListDouble* list = &(simulationServer->SimulationStationList);
    
    
  
        //循环更新模拟开关状态
    do
    {
        ListElment* element = list_head(list);        
        uint8_t size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            SimulationStation* station = (SimulationStation*)(element->data);
            if (station != NULL)
            {
                SwitchRunStateSimulation(station);
            }
            else
            {
                rt_kprintf("imulationSation* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }
            UpdateBindSwitchState(station); 
            element = element->next;
        }
        rt_thread_delay(5);
    }while(1);
        
    
}
/**
  * @brief :开关站点模拟 通信路由情况
  * @param  SimulationStationServer*  simulationServer
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  * [2018-06-12][张宇飞][删去多余的循环]
  *[2018-06-23][张宇飞][合并通信服务]
  *[2018-07-19][张宇飞][删除router，增加RouterDatagram_TransmissionCenter]
*/
void SimulationRouterApp(StationManger* manager)
{
    if (manager == NULL)
    {
        rt_kprintf("SimulationSwitchStationLogicalApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
   
    do
    {        
		RouterDatagram_TransmissionCenter(&(manager->stationServer.stationPointList));
        Monitor();
        SimulationCommunicationServer(manager);
        
        rt_thread_delay(3);
    }while(1);
}

/**
* @brief :开关站点模拟 通信解析
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-06-23][张宇飞][创建]
*/
static void SimulationCommunicationServer(StationManger* manager)
{
    if (manager == NULL)
    {
        rt_kprintf("SimulationCommunicationServer ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer*   stationServer = &(manager->stationServer);
    if (stationServer == NULL)
    {
        rt_kprintf("SimulationCommunicationServer ERROR :router = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }

    ListDouble* list = &(stationServer->stationPointList);
    

    ListElment* element = list_head(list);
    uint8_t size = list_size(list);
    for (uint8_t i = 0; i < size; i++)
    {
        StationPoint* station = (StationPoint*)(element->data);
        if (station != NULL)
        {
            StationCommunicationServer(station);
        }
        else
        {
            rt_kprintf("stationServer* station = NULL.\n");
            LogAddException(ERROR_NULL_PTR, 0);
            break;
        }
        element = element->next;
    }     
}


/**
  * @brief :开关站点模拟 通信解析
  * @param  StationManger* manager
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  */
void SimulationCommunicationApp(StationManger* manager)
{
     if (manager == NULL)
    {
        rt_kprintf("SimulationCommunicationApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer*   stationServer = &(manager->stationServer);
    if (stationServer == NULL)
    {
        rt_kprintf("SimulationCommunicationApp ERROR :router = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    
    ListDouble* list = &(stationServer->stationPointList);
    do
    {
        
        ListElment* element = list_head(list);
        uint8_t size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            StationPoint* station = (StationPoint*)(element->data);
            if (station != NULL)
            {
                StationCommunicationServer(station);
            }
            else
            {
                rt_kprintf("stationServer* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }
            element = element->next;
        }
        rt_thread_delay(3);
    }while(1);
}
/**
* @brief :分布式应用
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-06-16][张宇飞][创建]
*/
void SimulationDistributionApp(StationManger* manager)
{
    ListElment* element;
    ListDouble* list;
    uint8_t size;
    StationPoint* station;
    FaultDealHandle* handle;
    ErrorCode code;
    if (manager == NULL)
    {
        rt_kprintf("SimulationDistributionApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer* server = &(manager->stationServer);
    if (server == NULL)
    {
        rt_kprintf("SimulationDistributionApp ERROR :server = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    //FaultRemovalInit()
    list = &(server->stationPointList);

    //循环更新模拟开关状态
    do
    {
        element = list_head(list);
        size = list_size(list);
        for (uint8_t i = 0; i < size; i++)
        {
            station = (StationPoint*)(element->data);
            if (station != NULL)
            {
                handle = &(((StationPoint*)(element->data))->removalHandle);
                code = RemovalHandleCheckself(handle);
                if (code == ERROR_OK_NULL)
                {
                    FaultDealStateCenter(handle); 

					if ( (!handle->isRun) && handle->switchProperty->isChanged)
					{
						handle->switchProperty->isChanged = false;
						handle->TransmitMessage(handle, STATUS_MESSAGE);
						handle->TransmitMessage(handle, REMOVAL_MESSAGE);
						handle->TransmitMessage(handle, INSULATE_MESSAGE);
					}
                }  
				else
				{
					code = code;
				}

            }
            else
            {
                rt_kprintf("StationPoint* station = NULL.\n");
                LogAddException(ERROR_NULL_PTR, 0);
                break;
            }           
            element = element->next;
        }
        
    
        rt_thread_delay(3);
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
                if ((!station->removalHandle.isRun)  && (switchNode->isChanged || (cn == total)))
                {
                    switchNode->isChanged = false;
					DatagramTransferNode* pTransferNode =&( station->transferNode);
                    TransmitMessageExtern(switchNode, pTransferNode, STATUS_MESSAGE, 0xFFFF);
					TransmitMessageExtern(switchNode, pTransferNode, REMOVAL_MESSAGE, 0xFFFF);
					TransmitMessageExtern(switchNode, pTransferNode, INSULATE_MESSAGE, 0xFFFF);
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
  * @brief :仿真初始化，总入口
  * @param  void
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  */
void SimulationInit(void)
{
    
    LogInit(&g_Loghandle);

    NodeFifo* nodefifo;
    StationMangerInit(&g_StationManger);
    rt_kprintf("DATA:%s,%s, %s, %d\n",__DATE__, __TIME__, __FUNCTION__, __LINE__);
   // TestListPrevCase();
    uint32_t id = 0x0; //虚拟管理成员，对应上位机

   
    
	ErrorCode error = RouterDatagram_NewTransferNode(LOCAL_ADDRESS, 100, &g_VirtualNode);
	if (error)
	{
		perror("RouterDatagram_NewTransferNode ： 0x%x", error);
	}
   
   // RingQueueTest();
    LogAddException(ERROR_OK_NULL, 0);
    

    rt_thread_init(&distribution_thread,                 //线程控制块
        "distribution",                       //线程名字，在shell里面可以看到
        distribution_thread_entry,            //线程入口函数
        RT_NULL,                      //线程入口函数参数
        &rt_distribution_thread_stack,     //线程栈起始地址
        sizeof(rt_distribution_thread_stack), //线程栈大小
        5,                            //线程的优先级
        20);                          //线程时间片

    rt_thread_startup(&distribution_thread);
    
    rt_thread_init(&switch_thread,                 //线程控制块
                   "switch",                       //线程名字，在shell里面可以看到
                   switch_thread_entry,            //线程入口函数
                   RT_NULL,                      //线程入口函数参数
                   &rt_switch_thread_stack,     //线程栈起始地址
                   sizeof(rt_switch_thread_stack), //线程栈大小
                   4,                            //线程的优先级
                   20);                          //线程时间片
                               
    rt_thread_startup(&switch_thread);  
    
    rt_thread_init(&router_thread,                 //线程控制块
                    "router_self",                       //线程名字，在shell里面可以看到
                    router_thread_entry,            //线程入口函数
                    RT_NULL,                      //线程入口函数参数
                    &rt_router_thread_stack,     //线程栈起始地址
                    sizeof(rt_router_thread_stack), //线程栈大小
                    7,                            //线程的优先级
                    20);                          //线程时间片
                               
    rt_thread_startup(&router_thread);  
               
    //rt_thread_init(&comself_thread,                 //线程控制块
    //                "comself",                       //线程名字，在shell里面可以看到
    //                comself_thread_entry,            //线程入口函数
    //                RT_NULL,                      //线程入口函数参数
    //                &rt_comself_thread_stack,     //线程栈起始地址
    //                sizeof(rt_comself_thread_stack), //线程栈大小
    //                6,                            //线程的优先级
    //                20);                          //线程时间片
    //                           
    //rt_thread_startup(&comself_thread); 
        

    rt_thread_init(&communication_thread,                 //线程控制块
        "communication",                       //线程名字，在shell里面可以看到
        communication_thread_entry,            //线程入口函数
        RT_NULL,                      //线程入口函数参数
        &rt_communication_thread_stack,     //线程栈起始地址
        sizeof(rt_communication_thread_stack), //线程栈大小
        3,                            //线程的优先级
        20);                          //线程时间片

    rt_thread_startup(&communication_thread);


    rt_thread_init(&connected_thread,                 //线程控制块
        "connected",                       //线程名字，在shell里面可以看到
        connected_thread_entry,            //线程入口函数
        &g_StationManger,                      //线程入口函数参数
        &rt_connected_thread_stack,     //线程栈起始地址
        sizeof(rt_connected_thread_stack), //线程栈大小
        20,                            //线程的优先级
        20);                          //线程时间片

    rt_thread_startup(&connected_thread);


    return;
    rt_thread_init(&monitor_thread,                 //线程控制块
        "gain",                       //线程名字，在shell里面可以看到
        monitor_thread_entry,            //线程入口函数
        RT_NULL,                      //线程入口函数参数
        &rt_monitor_thread_stack,     //线程栈起始地址
        sizeof(rt_monitor_thread_stack), //线程栈大小
        14,                            //线程的优先级
        20);                          //线程时间片

    rt_thread_startup(&monitor_thread);
}

