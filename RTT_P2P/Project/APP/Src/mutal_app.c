/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      mutal_app.c
  * @brief:     
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-21 交互APP
  * @update:    
  */

#include "distribution.h"
#include "communication.h"
#include "Datagram.h"

#include "extern_interface.h"

#include "distribution_app.h"
#include "distribution_config.h"

static struct rt_thread router_thread;
//static struct rt_thread comself_thread;
static struct rt_thread communication_thread;


ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_router_thread_stack[THREAD_MUTAL_STACK_SIZE];//线程栈
//static rt_uint8_t rt_comself_thread_stack[1024];//线程栈
static rt_uint8_t rt_communication_thread_stack[1024];//线程栈


static void router_thread_entry(void* parameter);
static void communication_thread_entry(void* parameter);
static void MutalCommunicationApp(StationManger* manager);
static void MutalRouterApp(StationManger* manager);
static void MutalCommunicationServer(StationManger* manager);


static void router_thread_entry(void* parameter)
{
    rt_kprintf("thread router start.\r\n");     
    MutalRouterApp(&g_StationManger);    
}
static void communication_thread_entry(void* parameter)
{
    rt_kprintf("thread communication start.\r\n");     
    CommunicationServeice();        
} 

//static void comself_thread_entry(void* parameter)
//{
//    rt_kprintf("thread comself start.\r\n");     
//    MutalCommunicationApp(&g_StationManger);         
//} 



/**
  * @brief :开关站点模拟 通信路由情况
  * @param  StationManger* manager
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  * [2018-06-12][张宇飞][删去多余的循环]
  *[2018-06-23][张宇飞][合并通信服务]
  *[2018-07-19][张宇飞][删除router，增加RouterDatagram_TransmissionCenter]
  *[2018-07-21][张宇飞][添加通讯服务任务]
*/

static void MutalRouterApp(StationManger* manager)
{
	if (manager == NULL)
	{
		rt_kprintf("SimulationSwitchStationLogicalApp ERROR :manager = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
	}
	CommunicationServerInitSingle();
	do
	{
		RouterDatagram_TransmissionCenter(&(manager->stationServer.stationPointList));
		Monitor();
		MutalCommunicationServer(manager);
		CommunicationServerSingle();
		rt_thread_delay(3);


	} while (1);
}

/**
* @brief :开关站点模拟 通信解析
* @param  StationManger* manager
* @return: 0--正常
* @update: [2018-06-23][张宇飞][创建]
*/
static void MutalCommunicationServer(StationManger* manager)
{
    if (manager == NULL)
    {
        perror("MutalCommunicationServer ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer*   stationServer = &(manager->stationServer);
    if (stationServer == NULL)
    {
		perror("MutalCommunicationServer ERROR :router = NULL.\n");
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
			perror("stationServer* station = NULL.\n");
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
static void MutalCommunicationApp(StationManger* manager)
{
     if (manager == NULL)
    {
		 perror("MutalCommunicationApp ERROR :manager = NULL.\n");
        LogAddException(ERROR_NULL_PTR, 0);
    }
    StationServer*   stationServer = &(manager->stationServer);
    if (stationServer == NULL)
    {
		perror("MutalCommunicationApp ERROR :router = NULL.\n");
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
* @brief :分布式交互通讯应用
* @param  void
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void DistributionMutalAppInit(void)
{
	rt_thread_init(&router_thread,                 //线程控制块
		THREAD_MUTAL_NAME,                       //线程名字，在shell里面可以看到
		router_thread_entry,            //线程入口函数
		RT_NULL,                      //线程入口函数参数
		&rt_router_thread_stack,     //线程栈起始地址
		sizeof(rt_router_thread_stack), //线程栈大小
		THREAD_MUTAL_PRIORITY,                            //线程的优先级
		THREAD_MUTAL_TIMESLICE);                          //线程时间片

	rt_thread_startup(&router_thread);

	//rt_thread_init(&communication_thread,                 //线程控制块
	//	"communication",                       //线程名字，在shell里面可以看到
	//	communication_thread_entry,            //线程入口函数
	//	RT_NULL,                      //线程入口函数参数
	//	&rt_communication_thread_stack,     //线程栈起始地址
	//	sizeof(rt_communication_thread_stack), //线程栈大小
	//	3,                            //线程的优先级
	//	20);                          //线程时间片

	//rt_thread_startup(&communication_thread);
}

