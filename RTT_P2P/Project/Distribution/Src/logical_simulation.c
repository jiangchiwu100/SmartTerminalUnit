/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_simulation.c
  * @brief:     仿真处理
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-08
  * @update:    添加开关状态模拟
  */
  
 

#include "distribution.h"
#include "extern_interface.h"
#include "distribution_config.h"
#include "miscellaneous.h"
#include "quick_break_protect.h"
#include "drv_timer.h"
#include "output.h"
#include "input.h"



#define DELAY_MS(ms)  rt_thread_delay((ms));




static ErrorCode  AddSimulationStation(ListDouble* stationList, uint32_t id, SwitchProperty** switchProperty);
static uint8_t  RemoveSimulationStation(SimulationStation** node);
static void DestorySimulationStation(void* node);



/**
  * @brief :获取当前时间
  * @param  RemovalHandle* handle 控制句柄
  * @return: 0-正常返回
  * @update: [2018-06-05][张宇飞][BRIEF]
  */
static inline uint8_t SimulationGetNowTime(SimulationStation* handle)
{
	handle->startTime =  rt_tick_get();
    return NULL;
}

/**
  * @brief :判断时间是否超时
  * @param  RemovalHandle* handle 控制句柄
  * @return: 0-正常返回
  * @update: [2018-06-05][张宇飞][BRIEF]
  */
static inline uint8_t SimulationIsOverTime(SimulationStation* handle)
{
    return SystemIsOverTime(handle->startTime, handle->limitTime);
}




/**
  * @brief : 合闸操作, 当前状态是分位储能方能有效
  * @param  SimulationStation* station 模拟的站点
  * @return: 0--正常
  * @update: [2018-06-09][张宇飞][创建]
  *[2018-07-11][张宇飞][添加拒动判断]
  */
static inline void SimulationOpenOperate(SimulationStation* station)
{
    if ((RUN_STATE_CLOSE_POSITION_STORED == station->runState)
		&& (!station->isRejectAction))
    {
        (*(station->pBindSwitch))->isOpening = true;
		OpeningclosingOperate(TMR_50MS_OPEN);

        rt_kprintf("-------------open----------\r\n");
        station->isStartOpen = 0xff;
    }
    
}
/**
  * @brief : 分闸操作, 当前状态是合位储能方能有效
  * @param  SimulationStation* station 模拟的站点
  * @return: 0--正常
  * @update: [2018-06-09][张宇飞][创建]
  *[2018-07-11][张宇飞][添加拒动判断]
  */
static inline void SimulationCloseOperate(SimulationStation* station)
{
    if (RUN_STATE_OPEN_POSITION_STORED == station->runState && (!station->isRejectAction))
    {
        (*(station->pBindSwitch))->isClosing = true;
		OpeningclosingOperate(TMR_50MS_CLOSE);

        rt_kprintf("-------------close----------\r\n");
        station->isStartClose = 0xff;
    }    
}


/**
  * @brief :开关状态模拟，只考虑，合闸储能，分闸动作，分闸储能，合闸动作 4个状态，逐步完善
  * @param  SimulationStation* station 模拟的站点
  * @return: 0--正常
  * @update: [2018-06-09][张宇飞][创建]
  *@update: [2018-06-20][张宇飞][修改runState nextState错误逻辑]
  */
uint8_t SwitchRunStateSimulation(SimulationStation* station)
{
    do
    {
        switch(station->nextRunState)
        {
            case RUN_STATE_CLOSE_POSITION_STORED: //合位储能
            {
                station->runState = RUN_STATE_CLOSE_POSITION_STORED;
                station->switchState = SWITCH_CLOSE;
                
                if (station->isStartOpen)
                {
                    station->nextRunState = RUN_STATE_OPEN_ACTION;
                    station->limitTime = station->openTime;
                    station->GetNowTime(station);                   
                }
                else
                {
                    station->nextRunState = RUN_STATE_CLOSE_POSITION_STORED;
                }
                break;
            }
            case RUN_STATE_OPEN_ACTION: //分闸进行状态
            {
                station->runState = RUN_STATE_OPEN_ACTION;
                station->isStartOpen = 0;
                if (station->IsOverTime(station))
                {
                    station->nextRunState = RUN_STATE_OPEN_POSITION_STORED;
                }
                else
                {
                    station->nextRunState = RUN_STATE_OPEN_ACTION;
                }                
                break;
            }
            case RUN_STATE_OPEN_POSITION_STORED: //分闸储能
            {
                station->runState = RUN_STATE_OPEN_POSITION_STORED;
                station->switchState = SWITCH_OPEN;
                station->faultState = FAULT_NO;   //置为无故障状态
                
                
                if (station->isStartClose)
                {
                    station->nextRunState = RUN_STATE_CLOSE_ACTION;
                    station->limitTime = station->closeTime;
                    station->GetNowTime(station);     
                }
                else
                {
                    station->nextRunState = RUN_STATE_OPEN_POSITION_STORED;
                }
                break;
            }
            case RUN_STATE_CLOSE_ACTION:  //合闸进行状态
            {
                station->runState = RUN_STATE_CLOSE_ACTION;
                station->isStartClose = 0;
                if (station->IsOverTime(station))
                {
                    station->nextRunState = RUN_STATE_CLOSE_POSITION_STORED;
                }
                else
                {
                    station->nextRunState = RUN_STATE_CLOSE_ACTION;
                }                
                break;
            }
            default:
            {
                return ERROR_OVER_LIMIT;
            }
        }
        //状态连续更新
        if (station->runState != station->nextRunState)
        {
            continue;
        }
        else            
        {
            break;
        }
    }while(true);
 
    return NULL;
}

/**
  * @brief :仿真站初始化
  * @param  ListDouble* stationList 模拟的站点
  * @return: 0--正常
  * @update: [2018-06-09][张宇飞][创建]
  */
uint8_t SimulationStationInit(ListDouble* stationList)
{
    ListInit(stationList, DestorySimulationStation);
    return 0;
    
}
/**
  * @brief :所有节点状态更新
  * @param  ListDouble* stationList 模拟的站点
  * @return: 0--正常
  * @update: [2018-06-09][张宇飞][创建]
  */
uint8_t SwitchRunStateSimulationUpdateAll(ListDouble* stationList)
{
    if (stationList == NULL)
    {
        return ERROR_NULL_PTR;
    }
    uint8_t size = list_size(stationList);
    ListElment* element = list_head(stationList);
    for (uint8_t i = 0; i < size; i++)
    {
        SimulationStation* station = (SimulationStation*)(element->data);
        SwitchRunStateSimulation(station);
        element = element->next;
    }
    return 0;       
}
/**
 * @brief : 通过ID查找FIFO node信息中的指定元素
 * @param  ：void
 * @return: 0-未找到 ，非0 指针
 * @update:  [2018-06-08[张宇飞][]
 */
SimulationStation* FindSimulationStationById(const  ListDouble* list, uint32_t id)
{
    if(list == NULL)
    {
        return NULL;
    }
    
    uint8_t size = list_size(list);
     ListElment* element = list_head(list); 
    for (uint8_t i = 0; i < size; i++)
    {
        if (((SimulationStation*)(element->data))->id == id)
           {
               return (SimulationStation*)(element->data);
           }
        element = element->next;
    }
    return NULL;
}
/**
  * @brief :添加模拟节点
  * @param ： ListDouble* stationList 模拟的站点
  * @param ： id
  * @param ： SwitchProperty* switchProperty 开关属性
  * @return: ErrorCode
  * @update: [2018-06-09][张宇飞][创建]
  * [2018-06-11][张宇飞][修改初始化遗漏错误]
  * [2018-06-16][张宇飞][添加形参，SwitchProperty* switchProperty，返回参数 ErrorCode]
  * [2018-06-20][张宇飞][修改遗漏赋值id]
  */
static ErrorCode  AddSimulationStation(ListDouble* stationList, uint32_t id, SwitchProperty** switchProperty)
{
    CHECK_POINT_RETURN(stationList, NULL, ERROR_NULL_PTR);  
    CHECK_POINT_RETURN(switchProperty, NULL, ERROR_NULL_PTR);
    SimulationStation* station = FindSimulationStationById(stationList, id);
   // CHECK_POINT_RETURN(station, NULL, ERROR_NULL_PTR);
    
    station = (SimulationStation*)MALLOC(sizeof(SimulationStation));   
    if (station == NULL)
    {
        return ERROR_MALLOC;
    }
    MEMSET(station, 0, sizeof(SimulationStation));
    //TODO: c初始化参数与方法
    station->id = id;
    station->faultState = FAULT_NO;
    station->switchState = (*switchProperty)->state; //初始状态
        
    station->closeTime = 50;
    station->openTime = 30;
    
    if (station->switchState == SWITCH_CLOSE)
    {
        station->runState = RUN_STATE_CLOSE_POSITION_STORED;
        station->nextRunState = RUN_STATE_CLOSE_POSITION_STORED;
    }
    else
    {
        station->runState = RUN_STATE_OPEN_POSITION_STORED;
        station->nextRunState = RUN_STATE_OPEN_POSITION_STORED;
    }
    
          
    station->isStartClose = 0;
    station->isStartOpen = 0;        
    station->startTime = 0;
    station->limitTime = 0;        
    station->pBindSwitch = switchProperty;
	station->isRejectAction = false;
    station->CloseOperate = SimulationCloseOperate;
    station->OpenOperate = SimulationOpenOperate;
    station->GetNowTime = SimulationGetNowTime;
    station->IsOverTime = SimulationIsOverTime;
        
    ListInsertNext(stationList, NULL, station);
    
    return ERROR_OK_NULL;
}
/**
 * @brief  : 删除节点释放内存,供列表删除使用
 * @param  : 
 * @param  ：
 * @return: 0-正常，非空--未找到或异常
 * @update: [2018-06-08][张宇飞][]
 */
static void DestorySimulationStation(void* node)
{
    RemoveSimulationStation((SimulationStation**)(&node));
}
/**
 * @brief  : 删除节点释放内存
 * @param  : 
 * @param  ：
 * @return: 0-正常，非空--未找到或异常
 * @update: [2018-06-08][张宇飞][]
 */
static uint8_t  RemoveSimulationStation(SimulationStation** node)
{
    if (*node == 0)
    {
        return ERROR_NULL_PTR;
    }
    
    FREE(*node);
    *node = 0;
    
    return 0;
}
/**
 * @brief : 仿真站初始化
 * @param  ：void
 * @return: 0-正常 非0错误
 * @update: [2018-06-11[张宇飞][]
 */
ErrorCode SimulationStationServerInit(SimulationStationServer* server)
{
    SimulationStationInit(&(server->SimulationStationList));
    return ERROR_OK_NULL;
}

/**
 * @brief : 仿真站 增加成员
 * @param  ：void
 * @return: 0-正常 非0错误
 * @update: [2018-06-11[张宇飞][]
 */
ErrorCode SimulationStationServerAddMember(SimulationStationServer* server, uint32_t id, SwitchProperty** pswitch)
{
    ErrorCode result;
    result = AddSimulationStation(&(server->SimulationStationList), id, pswitch);
    return result;
}

/**
* @brief : 更新绑定开关状态
* @param  ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
* [2018-06-20[张宇飞][删去开关操作]
* [2018-06-31[张宇飞][设置检测时标]
* [2018-09-20[田晓亮][添加开关状态]
*/
ErrorCode UpdateBindSwitchState(SimulationStation* station)
{    
    CHECK_POINT_RETURN(station, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(station->pBindSwitch, NULL, ERROR_NULL_PTR);
    SwitchProperty* pswitch = *(station->pBindSwitch);
    CHECK_POINT_RETURN(pswitch, NULL, ERROR_NULL_PTR);
    
    //输出状态变化
    if (pswitch->fault.state != station->faultState)
    {
        //rt_kprintf("ID: %X, FAULT: %X， TICK:%d.\n", station->id, station->faultState, rt_tick_get());
        pswitch->isChanged = true;
    }
    if (pswitch->state != station->switchState)
    {
        //rt_kprintf("ID: %X, SwitchState: %X. TICK:%d\n ", station->id, station->switchState, rt_tick_get());
        pswitch->isChanged = true;
    }
	
    openingclosing();
    if(((float)0.3 < g_TelemetryDB[g_TelemetryAddr.Ia]))
    {
		station->faultState = FAULT_YES;
        pswitch->fault.state = FAULT_YES;
    }
    else
    {
		station->faultState = FAULT_NO;
        pswitch->fault.state = FAULT_NO;
    }
	pswitch->state = curStation.state;

//    pswitch->fault.state = station->faultState;
//    pswitch->state = station->switchState;
    //从switch 进行的分闸操作 TODO：这种意容易混乱，应该统一所有动作接口
//    if (pswitch->isOpening)
//    {
//        station->OpenOperate(station);
//        pswitch->isOpening = false;
//    }
//    if (pswitch->isClosing)
//    {
//        station->CloseOperate(station);
//        pswitch->isClosing = false;
//    }

	ValidCheckStamp(pswitch->timeStamp);
	
    return ERROR_OK_NULL;
}


/**
* @brief : 开关控制操作
* @param  ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*[2018-06-20[张宇飞][修改操作错误]
*[2018-07-31[张宇飞][修改错误提示]
*/
ErrorCode SimulationSwitchControlOperate(SimulationStation* station, SwitchControlOperate operate)
{
    CHECK_POINT_RETURN(station, NULL, ERROR_NULL_PTR);    
    switch (operate)
    {
    case CONTROL_OPEN:
    {
        station->OpenOperate(station);
        PrintIDTipsTick(station->id, "open");
		
        break;            
    }
    case CONTROL_CLOSE:
    {
        station->CloseOperate(station);
        PrintIDTipsTick(station->id, "close");
		
        break;
    }
    case CONTROL_SET_OVERCURRENT:
    {
        station->faultState = FAULT_YES;
        PrintIDTipsTick(station->id, " set fault");
		
        break;
    }
    case  CONTROL_REMOVAL_RESET:
    {
        StationPoint* point = g_StationManger.stationServer.FindMemberById(&g_StationManger.stationServer.stationPointList, station->id);
        if (point != NULL)
        {
            DatagramTransferNode* pTransferNode = &(point->transferNode);
            //仅在此处修改更新标志
            point->isAllowUpdate = false;
            point->removalHandle.Reset(&(point->removalHandle));
            TransmitMessageExtern(point->topology.localSwitch, pTransferNode, STATUS_MESSAGE, BROADCAST_ADDRESS);           
            point->isAllowUpdate = true;
			//发送复归后的信息
			
			
			//TransmitMessageExtern(point->topology.localSwitch, pTransferNode, REMOVAL_MESSAGE, BROADCAST_ADDRESS);
			//TransmitMessageExtern(point->topology.localSwitch, pTransferNode, INSULATE_MESSAGE, BROADCAST_ADDRESS);
            PrintIDTipsTick(station->id, "CONTROL_REMOVAL_RESET");
            
            
            
        }
        break;
    }
	case CONTROL_SET_REJECT_ACTION:
	{
		station->isRejectAction = true;
        PrintIDTipsTick(station->id, "set reject action.");
		
		break;
	}
	case CONTROL_CANCER_REJECT_ACTION:
	{
		station->isRejectAction = false;
        PrintIDTipsTick(station->id, "cancer reject action.");
		
		break;
	}
	case CONTROL_CANCER_OVERCURRENT:
	{
		station->faultState = FAULT_NO;
        PrintIDTipsTick(station->id, "cancer fault.");		
		break;
	}
	case  CONTROL_POWER_INCOME_LOSS:
	{
		station->faultState = FAULT_INCOME_LOSS;
        PrintIDTipsTick(station->id, "set power income loss fault.");	
		
		break;
	}
	case  CONTROL_RESET:
	{
        PrintIDTipsTick(station->id, "reset。。。。.\n");
		
		rt_thread_delay(600);
		SystemReset();
		break;
	}
	case CONTROL_DSITRIBUTION_INTO:
	{

		StationPoint* point = g_StationManger.stationServer.FindMemberById(&g_StationManger.stationServer.stationPointList, station->id);
		if (point)
		{
			point->topology.isRunDistribution = true;
			PrintIDTipsTick(station->id, "distribution into.\n");
		}
		else
		{
			PrintIDTipsTick(station->id, "point is null.\n");
		}
		break;
	}
	case CONTROL_DSITRIBUTION_EXIT:
	{

		StationPoint* point = g_StationManger.stationServer.FindMemberById(&g_StationManger.stationServer.stationPointList, station->id);
		if (point)
		{
			point->topology.isRunDistribution = false;
			PrintIDTipsTick(station->id, "distribution exit.\n");
		}
		else
		{
			PrintIDTipsTick(station->id, "point is null.\n");
		}
		break;
	}
    default:
        break;
    }

    return ERROR_OK_NULL;
}


