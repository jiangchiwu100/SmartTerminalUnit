/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      logical_simulation.c
  * @brief:     仿真处理
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-08
  * @update:    添加开关状态模拟
  */
#include "station_manager.h"  
#include "distribution_app.h"
#include "distribution.h"
#include "distribution_config.h"

#include "database.h"

/**
 *站点管理器
 */
StationManger g_StationManger;
DatagramTransferNode g_VirtualNode;

static ProtocolAnylastDatagram LocalDatagramAnylast;
static DatagramTransferNode LocalTransferNode;


static void MaintaceInit(void);
   
/**
 * @brief : 维护初始化
 * @param  ：void
 * @return: void
 * @update: [2018-07-24[张宇飞][创建]
 */
static void MaintaceInit(void)
{
    ErrorCode error = RouterDatagram_NewTransferNode(LOCAL_ADDRESS, 100, &LocalTransferNode);
	if (error)
	{
		perror("RouterDatagram_NewTransferNode:0x%x", error);
	}
	error = ProtocolAnylastDatagramInit(&LocalDatagramAnylast, &LocalTransferNode, LOCAL_ADDRESS);
	if (error)
	{
		perror("ProtocolAnylastDatagramInit:0x%x", error);
	}
}

/**
* @brief :维护帧处理
* @param  uint8_t* pData
* @param  uint8_t len
* @return: void
* @update: [2018-07-24][张宇飞][创建]
*/
void MantaiceFrameDeal(uint8_t* pData, uint8_t len)
{

    PointUint8 packet;
    ErrorCode error;
    if(pData == NULL)
    {
        perror("pData == NULL\n");
        return;
    }
    error = Datagram_CopyToPacket(pData, len , &packet);
    if (error)
    {
        perror("Datagram_CopyToPacket Error: 0x%x\n", error);
        return;
        
    }
                               
    //写缓冲
    LocalTransferNode.Write(&(LocalTransferNode.reciveRing) , &packet);   
     //直接处理
    error = LocalDatagramAnylast.ProtocolAnylastDeal(&LocalDatagramAnylast);
    if (!error)
    {
        if (LocalDatagramAnylast.recvRtu.completeFlag)
        {
            if (LocalDatagramAnylast.recvRtu.destAddress == LOCAL_ADDRESS)
            {
                //PrintMemoryUsed();
                ExecuteFunctioncode(&LocalDatagramAnylast.recvRtu, &g_StationManger.simulationServer);
                if ((!g_StationManger.firstRun) && ( g_StationManger.pWorkPoint) )
                {
                    g_StationManger.firstRun = true;
                    StartSinglePointNormalThread();                    
                    StationMessageSave(g_StationManger.pWorkPoint);
                    //TopologyMessageSave(g_StationManger.pWorkPoint->topology.localTopology);
                    
                }
            }
            else
            {
                perror("ExecuteFunctioncode destAddress isn't fit.\n");
            }
            error = Datagram_Destory(LocalDatagramAnylast.pReiveDatagram);
            if (error)
            {
                perror("Datagram_Destory Error:0x%X", error);
                return ;
            }
            LocalDatagramAnylast.pReiveDatagram = NULL;
        }
        LocalDatagramAnylast.recvRtu.completeFlag = false;
        
        
				
    }  
}



/**
* @brief :站点帧处理
* @param  uint8_t* pData
* @param  uint8_t len
* @return: void
* @update: [2018-07-24][张宇飞][创建]
*/
void StationPointFrameDeal(uint8_t* pData, uint8_t len)
{
     PointUint8 packet;
    if ( g_StationManger.pWorkPoint == NULL)
    {
        perror("g_StationManger.pWorkPoint == NULL\n");
        return;
    }
    
    RingQueue* pReciveRing = &(g_StationManger.pWorkPoint->transferNode.reciveRing);
    DatagramTransferNode* pTransferNode = &(g_StationManger.pWorkPoint->transferNode);
    
    ErrorCode error = Datagram_CopyToPacket(pData, len , &packet);
    if (error)
    {
        perror("Datagram_CopyToPacket Error: 0x%x\n", error);
        return;
        
    }
                               
    //写缓冲
    pTransferNode->Write(pReciveRing, &packet);   
     //直接处理
    StationCommunicationServer(g_StationManger.pWorkPoint);
}

/**
 * @brief : 站管理器初始化
 * @param  ：void
 * @return: 0-正常 非0错误
 * @update: [2018-06-11[张宇飞][]
 *[2018-07-19[张宇飞][删去router成员]
 *[2018-07-24[张宇飞][添加维护帧初始化]
 */
uint8_t  StationMangerInit(StationManger* manger)
{
    uint16_t result = 0;   
    //初始化路由管理器
    MEMSET(manger, 0, sizeof(StationManger));
	
    result = SimulationStationServerInit(&(manger->simulationServer));
    if (result)
    {
        perror("SimulationStationServerInit ERROR : %X\n", result);
    }
    result = StationServerInit(&(manger->stationServer));    
    if (result)
    {
        perror("StationServerInit ERROR : %X\n", result);
    }
    MaintaceInit();
    return 0;    
}

/**
 * @brief : 站管理器--添加成员
 * @param  StationManger* manger 管理器
 * @param  id 
 * @return: 0-正常 非0错误
 * @update: [2018-06-11[张宇飞][]
 *[2018-07-19[张宇飞][删去router成员]
 */
uint8_t StationMangerAddMember(StationManger* manger, uint32_t id)
{	
	StationPoint* station;
	uint8_t result;

	result = StationServerAddPoint(&(manger->stationServer), NULL, &station);
	if (result)
	{
		perror("StationServerAddPoint ERROR : %X\n", result);
		return result;
	}


	result = SimulationStationServerAddMember(&(manger->simulationServer), id, &(station->topology.localSwitch));
	if (result)
	{
		rt_kprintf("SimulationStationServerAddMember ERROR : %X\n", result);
		return result;
	}

    return 0;
}
/**
* @brief : 通过交互，获取邻居拓扑信息 APP 作为任务单独使用
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-06-21][张宇飞][]
*/
ErrorCode GetNeighboorRunState(StationPoint* point)
{

    CHECK_POINT_RETURN(point, NULL, ERROR_NULL_PTR);    
    CHECK_POINT_RETURN(point->topology.localSwitch, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(point->topology.localTopology, NULL, ERROR_NULL_PTR);
   
    bool result;
    ErrorCode error;


    error = CheckNeighboorTopologyCompleted(point->topology.localSwitch, &(point->topology.globalTopologyList), &result);
    if (error != ERROR_OK_NULL)
    {
        return error;
    }

    point->topology.isNeighbourComplted = result; //更新已完成标识
    if (!result)
    {
        error = GetNeighboorTopologyByMutual(point);
    }

    return error;

}

/**
* @brief : 通过交互，获取所有拓扑信息
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-07-05][张宇飞][]
*/
ErrorCode GetAllTopologyRunState(StationPoint* point)
{

    CHECK_POINT_RETURN(point, NULL, ERROR_NULL_PTR);   
    CHECK_POINT_RETURN(point->topology.localSwitch, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(point->topology.localTopology, NULL, ERROR_NULL_PTR);    
    bool result;
    ErrorCode error;


    error = CheckAllTopologyCompleted(point, &result);
    if (error != ERROR_OK_NULL)
    {
        return error;
    }

    point->topology.areaID.isGainComplted = result; //更新已完成标识
    if (!result)
    {
        error = GetAllTopologyByMutual(point);
    }

    return error;

}



/**
* @brief : 站管理器--添加成员
* @param  StationManger* manger 管理器
* @param  id
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*/
void  GainMangerNeighbourMembers(StationManger* manager)
{
    ListElment* element;
    ListDouble* list;
    uint8_t size;
    StationPoint* station;    
    if (manager == NULL)
    {
        rt_kprintf("GainMangerNeighbourMembers ERROR :manager = NULL.\n");
    }
    StationServer* server = &(manager->stationServer);
    if (server == NULL)
    {
        rt_kprintf("GainMangerNeighbourMembers ERROR :server = NULL.\n");
    }
    list = &(server->stationPointList);


    element = list_head(list);
    size = list_size(list);
    for (uint8_t i = 0; i < size; i++)
    {
        station = (StationPoint*)(element->data);
        if (station != NULL)
        {
            GetNeighboorRunState(station);
        }
        else
        {
            rt_kprintf("StationPoint* station = NULL.\n");
            break;
        }
        element = element->next;
    }
}


/**
* @brief : 站点服务器初始化
* @param  ：void
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
*/
uint8_t StationServerInit(StationServer* server)
{
    ListInit(&(server->stationPointList), NULL);
    server->FindMemberById = FindStationPointById;
    return 0;
}
/**
* @brief : 通过ID查找StationPoint信息中的指定元素
* @param  ：void
* @return: 0-未找到 ，非0 指针
* @update:  [2018-06-08[张宇飞][]
*/
StationPoint* FindStationPointById(const  ListDouble* list, uint32_t id)
{
    if (list == NULL)
    {
        return NULL;
    }
    uint8_t size = list_size(list);
    ListElment* element = list_head(list);
    for (uint8_t i = 0; i < size; i++)
    {
        if (((StationPoint*)(element->data))->id == id)
        {
            return (StationPoint*)(element->data);
        }
        element = element->next;
    }
    return NULL;
}
/**
* @brief : 站点服务器增加服务成员
* @param  ：StationServer* server 服务器
* @param  ：uint32_t id  id号
* @return: 0-正常 非0错误
* @update: [2018-06-11[张宇飞][]
* [2018-06-16[张宇飞][增加形参 StationPoint** pstation]
* [2018-06-19[张宇飞][增加形参 const TopologyMessage*  topologyMessage]
*[2018-07-19[张宇飞][修改nodefifo]
*/
ErrorCode StationServerAddPoint(StationServer* server,   TopologyMessage*  topologyMessage, StationPoint** pstation)
{
    CHECK_POINT_RETURN(server, NULL, ERROR_NULL_PTR);
    
    CHECK_POINT_RETURN(topologyMessage, NULL, ERROR_NULL_PTR);
    
    uint32_t id = topologyMessage->id;
    ErrorCode error;
    ListDouble* list = &(server->stationPointList);
    StationPoint* station = FindStationPointById(list, id);
    CHECK_UNEQUAL_RETURN(station, NULL, ERROR_EXIST);
  
    station = (StationPoint*)CALLOC(1, sizeof(StationPoint));
    CHECK_POINT_RETURN(station, NULL, ERROR_MALLOC);    
   
    station->id = id;	
	RouterDatagram_NewTransferNode(id, 100, &(station->transferNode));
	ProtocolAnylastDatagramInit(&(station->anylast), &(station->transferNode), id);

    StationTopology* stationTopology = &(station->topology);
    stationTopology->id = id;
    stationTopology->isNeighbourComplted = false;
	stationTopology->powerArea.isComplted = false;
    StationInit(stationTopology, id);
    //增加首个成员
    error = AddMemberByTopology(topologyMessage, &(stationTopology->globalTopologyList));
    CHECK_UNEQUAL_RETURN(error, ERROR_OK_NULL, error);
    //设置首个成员为本地拓扑和开关成员
    stationTopology->localTopology = topologyMessage;    
    stationTopology->localSwitch = topologyMessage->switchCollect;
	stationTopology->localSwitch->parent = stationTopology;
    ListInit(&(stationTopology->connectPath), FREE);


    FaultDealHandle* handle = &(station->removalHandle);
    RemovalHandleInit(handle, stationTopology->localSwitch, &(station->transferNode));

    *pstation = station;
    ListInsertNext(list, NULL, station);
    return ERROR_OK_NULL;
}




/**
* @brief : 站点服务器增加服务成员 通过拓扑信息
* @param  ：TopologyMessage*  topologyMessage
* @param  ： StationManger* manger 
* @return: ErrorCode
* @update: [2018-07-25[张宇飞][]
*/
ErrorCode StationManagerAddMemberByTopology(TopologyMessage*  topologyMessage, StationManger* manger)
{

	CHECK_POINT_RETURN_LOG(topologyMessage, NULL, ERROR_NULL_PTR, 0);

	ListDouble* list = &(manger->stationServer.stationPointList);
	uint32_t id = topologyMessage->id;
	StationPoint*	 station = FindStationPointById(list, id);
	if (station != NULL)
	{
		FreeTopologyMemory(&topologyMessage);
		perror("Node Has Exited.\n");
		return ERROR_EXIST;
	}

	PrintTopologyMessage(topologyMessage);
	//增加站点
	ErrorCode error = StationServerAddPoint(&(manger->stationServer), topologyMessage, &station);
	if (error != ERROR_OK_NULL)
	{
		perror("StationServerAddPoint ERROR : 0x%X\n", error);
		//TODO: 释放内存
		return error;
	}

	error = SimulationStationServerAddMember(&(manger->simulationServer), id, &(station->topology.localSwitch));
	if (error)
	{
		perror("SimulationStationServerAddMember ERROR : 0x%X\n", error);
	}
#if SINGLE_POINT
	//首次添加的站点作为工作站点
	if (manger->pWorkPoint == NULL)
	{
		rt_kprintf("Single Work Station\n");
		manger->pWorkPoint = station;
		ListElment* element = list_head(&(manger->simulationServer.SimulationStationList));
		manger->pWorkSimulation = (SimulationStation*)list_data(element);
	}
#endif
	return error;
}
