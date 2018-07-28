/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_simulation_app.c
  * @brief:     开关逻辑仿真任务
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-11
  * @update:    
  */
#include "distribution_config.h"
#include "logical_simulation_app.h"
#include "distribution.h"
#include "communication.h"

#include "log.h"
#include "extern_interface.h"
#include "distribution_config.h"
#include "distribution_app.h"

void  MonitorApp(StationManger* manager);


static struct rt_thread switch_thread;//线程控制块



ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_switch_thread_stack[THREAD_SIMSWITCH_STACK_SIZE];//线程栈




static void SimulationSwitchStationLogicalApp(StationManger* manager);
static void SingleSimulationSwitchStationLogicalApp(StationManger* manager);

static void switch_thread_entry(void* parameter)
{    
	rt_kprintf("thread simulation start.\r\n");
	SimulationSwitchStationLogicalApp(&g_StationManger);

}
  


static void monitor_thread_entry(void* parameter)
{
    rt_kprintf("thread monitor_thread_entry start.\r\n");
    MonitorApp(&g_StationManger);
}



/**
  * @brief :开关站点模拟
  * @param  SimulationStationServer*  simulationServer
  * @return: 0--正常
  * @update: [2018-06-11][张宇飞][创建]
  */
static void SimulationSwitchStationLogicalApp(StationManger* manager)
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
* @brief :单个 开关站点模拟
* @param  SimulationStationServer*  simulationServer
* @return: 0--正常
* @update: [2018-07-24][张宇飞][创建]
*/
static void SingleSimulationSwitchStationLogicalApp(StationManger* manager)
{
	if (manager == NULL)
	{
		perror("SingleSimulationSwitchStationLogicalApp ERROR :manager = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
		return;
	}
	SimulationStation* station = manager->pWorkSimulation;
	if (station == NULL)
	{
		perror("manager->pWorkSimulation = NULL.\n");
		LogAddException(ERROR_NULL_PTR, 0);
		return;
	}


	//循环更新模拟开关状态
	do
	{			
		SwitchRunStateSimulation(station);
		UpdateBindSwitchState(station);					
		rt_thread_delay(5);
	} while (true);


}






/**
  * @brief :仿真初始化，总入口
  * @param  void
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */
void LogicalSimulationAppInit(void)
{

    rt_thread_init(&switch_thread,                 //线程控制块
		THREAD_SIMSWITCH_NAME,                       //线程名字，在shell里面可以看到
                   switch_thread_entry,            //线程入口函数
                   RT_NULL,                      //线程入口函数参数
                   &rt_switch_thread_stack,     //线程栈起始地址
		sizeof(rt_switch_thread_stack), //线程栈大小
		THREAD_SIMSWITCH_PRIORITY,                            //线程的优先级
		THREAD_SIMSWITCH_TIMESLICE);                          //线程时间片
                               
    rt_thread_startup(&switch_thread);  
    


    return;
    //rt_thread_init(&monitor_thread,                 //线程控制块
    //    "gain",                       //线程名字，在shell里面可以看到
    //    monitor_thread_entry,            //线程入口函数
    //    RT_NULL,                      //线程入口函数参数
    //    &rt_monitor_thread_stack,     //线程栈起始地址
    //    sizeof(rt_monitor_thread_stack), //线程栈大小
    //    14,                            //线程的优先级
    //    20);                          //线程时间片

    //rt_thread_startup(&monitor_thread);
}

