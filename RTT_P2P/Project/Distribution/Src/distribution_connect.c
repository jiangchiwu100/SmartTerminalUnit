/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_connect.c
* @brief:     
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-06-08
* @update:    
*/

#include "distribution_interface.h"   
#include "distribution.h"
#include "common_def.h"
#include "topology.h" 
#include "list.h"

static ErrorCode SendConnectPathMessage(uint32_t id, PathConnected iseset, uint8_t hops,  DatagramTransferNode* pTransferNode);
static bool CheckIsMeetConnectCondition(StationTopology* topology);
static ErrorCode SendConnectPathCmd(List* path, PathConnected iseset, DatagramTransferNode* pTransferNode);
static ErrorCode SerachPowerConditionList(SwitchProperty* sourceSwitch, List* list, List* powerList, List* findList);
static ErrorCode SerachPowerPath(SwitchProperty* sourceSwitch, List* powerList, List* findList, List* bfsList);
static ErrorCode ExtractSwitchPath(StationTopology* station, List* bfsList);
static bool CheckIsLockConnectJudge(StationTopology* topology);
/**
* @brief :  判别是否为联络开关，并计算其路径 
* @param  ：StationTopology* station 站点
* @return: void
* @update: [2018-07-05][张宇飞][]
*[2018-07-08][张宇飞][修改计算前提条件,修改重复分配内存错误,堵住内存分配漏洞]
*[2018-07-10][张宇飞][提取重构模块]
*[2018-07-17][张宇飞][修改返回值错误]
*/
void StationCalConnectPathAndJudge(StationTopology* station)
{
    ErrorCode result = ERROR_OK_NULL;
    uint32_t id;
	uint8_t size = 0;
    SwitchProperty* sourceSwitch = station->localSwitch;
    List* globalList = &(station->globalSwitchList);
    //PrintMemoryUsed();
    if (station->globalTopologyList.size != station->areaID.count)
    {
        LogAddException(ERROR_OVER_LIMIT, station->id);
        return;
    }

    if (globalList->size != station->areaID.count)
    {
        //销毁链表
        list_destroy(&(station->globalSwitchList));
		//重新初始化
		list_init(&(station->globalSwitchList), NULL);

		result = GetSwitchList(&(station->globalTopologyList), globalList);
		if (result)
		{
			rt_kprintf("GetSwitchList Error: %d\n", result);
			LogAddException(result, station->id);
			return;
		}
		if (globalList->size != station->areaID.count)
		{
			rt_kprintf("globalList->size != station->connect.count. %d\n", globalList->size);
			LogAddException(ERROR_OVER_LIMIT, station->id);
			return;
		}
       
    }
    
    //使用头作为计算节点
    if (sourceSwitch == NULL)
    {
        rt_kprintf("localTopology is null: \n");
        LogAddException(ERROR_NULL_PTR, station->id);
        return;
    }
    //查找电源开关数量
    List* list = &(station->globalSwitchList);

    List listPower;
    List* powerList = &listPower;
    list_init(powerList, NULL);

    List findList;  //去除分位开关
    List* find = &findList;
    list_init(find, NULL);

	result = SerachPowerConditionList(sourceSwitch, list, powerList, find);
	if (result)
	{
		perror("SerachPowerConditionList Error: 0x%X: \n", result);
        return;
	}

    List bfsListInstance;
    List* bfsList = &bfsListInstance;
    list_init(bfsList, DestoryBFSHelper);
    
	result = SerachPowerPath(sourceSwitch, powerList, find, bfsList);
	if (result)
	{
		perror("SerachPowerPath Error: 0x%X: \n", result);
        return;
	}
	


    //至少有两条路径到开关，对于非合环路径只有两个，TODO:(对于合环或者分布式电源可能有大于3个，且可能重复)
    //存在两条路径即认为是联络开关
    size = list_size(bfsList);

	list_destroy(station->connect.path);
	list_init(station->connect.path, NULL);
	list_destroy(station->connect.path + 1);
	list_init(station->connect.path + 1, NULL);
	station->connect.count = 0;

   
	result = ExtractSwitchPath(station, bfsList); 
		
	if (result)
	{
        perror("ExtractSwitchPath Error: 0x%X: \n", result);
		return ;
	}
	//数量为2，且各个链表数量均不为0
	if ((station->connect.count == 2) && 
		(list_size(station->connect.path) != 0)
		&& (list_size(station->connect.path + 1) != 0))
	{
		station->connect.isConnect = true;
	}
	else
	{
		station->connect.isConnect = false;
	}
   

	list_destroy(&listPower); //没有泄露
	list_destroy(&findList);  //没有泄露
    list_destroy(&bfsListInstance);
    //PrintMemoryUsed();
}
/**
* @brief : 搜索电源开关，条件路径
* @param  ：
* @return: void
* @update: [2018-07-10][张宇飞][未进行参数检测，由调用者负责]
*/
static ErrorCode SerachPowerConditionList(SwitchProperty* sourceSwitch, List* list, List* powerList, List* findList)
{
	uint8_t size = list_size(list);
	ListElmt* element = list_head(list);
	for (uint8_t i = 0; i < size; i++)
	{
		SwitchProperty* sw = GET_SWITCH_ELEMENT(element);
		//TODO: 需要添加电源开关且有压。才认为为有效的电源开关
		if (sw->type == SWITCH_TYPE_BREAKER_POWER)
		{
			list_ins_next(powerList, NULL, sw);
		}
		//排除除了自己，为分位的开关
		if (sw->state != SWITCH_OPEN || sw == sourceSwitch)
		{
			list_ins_next(findList, NULL, sw);
		}
		element = element->next;
	}
	return ERROR_OK_NULL;
}
/**
* @brief : 根据指导条件搜索路径
* @param  ：
* @return: void
* @update: [2018-07-10][张宇飞][未进行参数检测，由调用者负责]
*/
static ErrorCode SerachPowerPath(SwitchProperty* sourceSwitch, List* powerList, List* findList, List* bfsList)
{
	BFSHelper* bfs;
	uint8_t size = list_size(powerList);
	ListElmt* element = list_head(powerList);
	ErrorCode result;
	for (uint8_t i = 0; i < size; i++)
	{
		result = FindPath(findList, sourceSwitch, GET_SWITCH_ELEMENT(element), &bfs);
		if (result)
		{
			if (result != ERROR_UNFIND)
			{
				perror("FindPath ERROR: 0x%X\n", result);
				LogAddException(result, 0);
				return result;
			}
			FreeBFSHelper(&bfs);
		}
		else
		{
			list_ins_next(bfsList, NULL, bfs);
			//PrintBFSHelperSimple(bfs);
		}
		element = element->next;
	}
	return ERROR_OK_NULL;
}
/**
* @brief : 提取联络开关路径
* @param  ：
* @return: void
* @update: [2018-07-10][张宇飞][未进行参数检测，由调用者负责, 适应多个开关查找情况]
*/
static ErrorCode ExtractSwitchPath(StationTopology* station,   List* bfsList)
{	
	List* list;
	BFSHelper* bfs;
	uint8_t size = list_size(bfsList);
	uint8_t count = 0, index = 0;
	uint32_t id = 0;
	SwitchProperty* findSwitch;
	ErrorCode result;
	ListElmt* element = list_head(bfsList);
	List* globalList = &(station->globalSwitchList);
	ConnectSwitch* connect = &(station->connect);
	connect->count = 0;
	for (uint8_t i = 0; i < size; i++, element = element->next)
	{
		list = connect->path + i;
		if (list_size(list) != 0)
		{
			list_destroy(list);
		}
		list_init(list, NULL);

		bfs = (BFSHelper*)(element->data);

		count = list_size(bfs->path);
		ListElmt* node = list_head(bfs->path);
		if (count == 0)
		{
			continue;
		}
		//跳过自身开关
		for (uint8_t k = 0; k < count; k++)
		{
			index = (*(uint8_t*)(node->data));
			id = bfs->idArray[index];
			if (id != station->id)
			{
				ErrorCode error = FindSwitchNodeByID(globalList, id, &findSwitch);
				if (error != ERROR_OK_NULL)
				{
					perror("FindSwitchNodeByID ERROR: 0x%X\n", error);
					LogAddException(result, station->id);
					return error;								
				}				
				list_ins_next(list, NULL, findSwitch);				
			}
			node = node->next;
		}
		connect->count++;
		
	}
	return ERROR_OK_NULL;
}
/**
* @brief :  根据表格获取拓扑信息
* @param  ：StationTopology* station 站点
* @return: void
* @update: [2018-07-05][张宇飞][创建]
*/
ErrorCode GetAllTopologyByMutual(StationPoint* point)
{
    CHECK_POINT_RETURN_LOG(point, NULL, ERROR_NULL_PTR, 0);
    ErrorCode error;
    TopologyMessage* topology;
    
    List* topologylist = &(point->topology.globalTopologyList);
    AreaID* area = &(point->topology.areaID);
    for (uint8_t i = 0; i < area->count; i++)
    {
        //检查是否存在，存在则跳过
        error = FindTopologyNodeByID(topologylist, area->idCollect[i], &topology);
        if (error == ERROR_OK_NULL)
        {
            continue;
        }	
        error = SendGetTopologyMessage(area->idCollect[i], &(point->transferNode));
        CHECK_UNEQUAL_RETURN_LOG(error, ERROR_OK_NULL, error, point->id);
        rt_thread_delay(10);//延迟10ms进行获取，避免阻塞 
    }
    return ERROR_OK_NULL;
}



/**
* @brief : 检测所有拓扑新是否齐全。 简单判断数量.TODO:后期应完善
* @param :
* @return:
* @update: [2018-07-05][张宇飞][创建]
*/
ErrorCode CheckAllTopologyCompleted(StationPoint* point, bool* result)
{
    CHECK_POINT_RETURN_LOG(point, NULL, ERROR_NULL_PTR, 0);
    AreaID* area = &(point->topology.areaID);
    uint8_t size = list_size(&(point->topology.globalTopologyList));
    if (area->count == size)
    {
        *result = true;
    }
    else
    {
        *result = false;
    }
    return ERROR_OK_NULL;

}


/**
* @brief : 联络开关识别APP
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-07-05][张宇飞][]
*/
ErrorCode ConnectedSwitchJuadgeAPP(StationPoint* point)
{
    uint8_t try_count = 0;
    StationTopology* topology = &(point->topology);
    bool *result = &(topology->areaID.isGainComplted);
    ErrorCode error;
    CHECK_POINT_RETURN_LOG(point, NULL, ERROR_NULL_PTR, 0);
	bool before = topology->connect.isConnect;


	 //if (!topology->connect.isConnect)
	 //{
		//   if (topology ->localSwitch->state != SWITCH_OPEN)
		//   {
		//	   return ERROR_OK_NULL;
		//   }

	 //} 
    
    if (topology->areaID.count == 0)
    {
        return ERROR_OVER_LIMIT;
    }

    while (!(*result))
    {
        if (try_count++ > 3)
        {
            LogAddException(ERROR_OVER_LIMIT, point->id);
            return ERROR_OVER_LIMIT;
        }
        GetAllTopologyRunState(point);
        for (uint8_t i = 0; i < 10; i++)
        {
            rt_thread_delay(200);// 等待5000收集完成
            CheckAllTopologyCompleted(point, result);
            if (*result)
            {
                break;
            }
        }
        if (*result)
        {
            break;
        }
    }
    //没有收集完成，不再进行之后的操作
    if (! *result)
    {
        return ERROR_OK_NULL;
    }
    //若已经是联络开关,检测是否仍然满足条件
    if (topology->connect.isConnect)
    {
		bool state = CheckIsLockConnectJudge(topology);
		if (state)
		{
			return ERROR_OK_NULL;
		}

        state = CheckIsMeetConnectCondition(topology);
        if (state)
        {
            return ERROR_OK_NULL;
        }   
        else //说明是由联络开关转换到非联络开关
        {
            error = SendConnectPathCmd(topology->connect.path, CANCER_PATH_CONNECT, &(point->transferNode));
            return error;
        }
    }	

    StationCalConnectPathAndJudge(topology);

	if (topology->connect.isConnect)
	{
		//通知路径上的开关
		SendConnectPathCmd(topology->connect.path, SET_PATH_CONNECT, &(point->transferNode));
	}	

	//变化时显示提示
	if (before != topology->connect.isConnect)
	{
		if (topology->connect.isConnect)
		{
			//通知路径上的开关			
			rt_kprintf("ID:%X, is connect swtich.\n", topology->id);
			PrintSwitchList(topology->connect.path);
			PrintSwitchList(topology->connect.path + 1);
		}
		else
		{
			rt_kprintf("ID:%X, isn't connect swtich.\n", topology->id);
		}
	}
    

    return ERROR_OK_NULL;

}
/**
* @brief : 搜索到电源开关路径APP
* @param : StationPoint* point 站点
* @return:  
* @update: [2018-07-10][张宇飞][]
*/
ErrorCode SearchToPowerPathAPP(StationPoint* point)
{
	StationTopology* topology = &(point->topology);
	bool *result = &(topology->areaID.isGainComplted);
	ErrorCode error;
	CHECK_POINT_RETURN_LOG(point, NULL, ERROR_NULL_PTR, 0);
	bool before = false;

	//满足条件即更新
	if (*result )
	{
		StationCalConnectPathAndJudge(topology);
	}
	
	
	
	//变化时显示提示
	if (before != topology->connect.isConnect)
	{
		if (topology->connect.isConnect)
		{
			//通知路径上的开关			
			rt_kprintf("ID:%X, is connect swtich.\n", topology->id);
			PrintSwitchList(topology->connect.path);
			PrintSwitchList(topology->connect.path + 1);
		}
		else
		{
			rt_kprintf("ID:%X, isn't connect swtich.\n", topology->id);
		}
	}


	return ERROR_OK_NULL;

}

/**
* @brief : 设置是否位于联络开关路径上
* @param  :uint32_t id
* @param  :PathConnected isSet
* @param  :StationTopology* toplogy
* @return:
* @update: [2018-07-06][张宇飞][]
*[2018-07-12][张宇飞][更新联络路径信息，改uint32为ConnectPath结构体]
*[2018-07-16][张宇飞][添加跳数信息hops]
*/
ErrorCode SetConnectPath(uint32_t id,  PathConnected isSet, uint8_t hops, StationTopology* toplogy)
{
    CHECK_POINT_RETURN_LOG(toplogy, NULL, ERROR_NULL_PTR, 0);
    List* list = &(toplogy->connectPath);
    
    FOR_EARCH_LIST_START(list);
    uint32_t getID = ((ConnectPath*)list_data(m_foreach))->id;
    if (getID == id)
    {
        if (isSet == CANCER_PATH_CONNECT)
        {
			ConnectPath* pid;
            list_rem_next(list, m_foreach->prev, (void**)&pid);
            SafeFree(pid);
            return ERROR_OK_NULL;
        }       
    }
    FOR_EARCH_LIST_END();
    
    if (isSet == SET_PATH_CONNECT)
    {
		ConnectPath* pid = (ConnectPath*)CALLOC(1, sizeof(ConnectPath));
        CHECK_POINT_RETURN_LOG(pid, NULL, ERROR_MALLOC, toplogy->id);
		pid->hopsNumber = hops;
        pid->id = id;
        list_ins_next(list, NULL, pid);
    }

    return ERROR_OK_NULL;
}


                            

/**
* @brief : 检测是否满足联络开关条件---当前开关为分位，两条路径，开关为合位
* @param  :GetNeighboorHandle* handle
* @return:
* @update: [2018-07-06][张宇飞][]
*/
static bool CheckIsMeetConnectCondition(StationTopology* topology)
{
    bool state = true;
    SwitchProperty* node;
    if ((topology->localSwitch->state != SWITCH_OPEN) || (topology->connect.count != 2))
    {
        topology->connect.isConnect = false;
        return false;
    }

	
    for (uint8_t i = 0; i < topology->connect.count; i++)
    {
        List* list = topology->connect.path + i;
        
        if (list_size(list) == 0)
        {
            state = false;
            break;
        }
       
        FOR_EARCH_LIST_START(list);
        node = GET_SWITCH_ELEMENT(m_foreach);

		//合位, 电源开关没有故障（进线失压）
        if ((node->state != SWITCH_CLOSE) || 
				(( node->type == SWITCH_TYPE_BREAKER_POWER) && ( node->fault.state == FAULT_INCOME_LOSS))
			)
        {
            state = false;
            break;
        }
        FOR_EARCH_LIST_END();

    }
    topology->connect.isConnect = state;
    return state;
}
/**
* @brief :对于已经是联络开关的开关并保存路径，检测是否闭锁联络开关判断
* @param :StationTopology* topology
* @return:
* @update: [2018-07-12][张宇飞][]
*/
static bool CheckIsLockConnectJudge(StationTopology* topology)
{
	bool state = false;
	SwitchProperty* node;
	List* list;
		
	for (uint8_t i = 0; i < topology->connect.count; i++)
	{
		list = topology->connect.path + i;
		
		FOR_EARCH_LIST_START(list);
		node = GET_SWITCH_ELEMENT(m_foreach);
		if (node->fault.state == FAULT_YES)
		{
			state = true;
			break;
		}
		if (node->insulateType == RESULT_FAILURE)
		{
			state = true;
			break;
		}
		//TODO：没有开关处于故障处理过程中
		FOR_EARCH_LIST_END();

	}
	return state;
}

/**
* @brief :根据条件查找满足条件的开关，TODO:简单处理，找出最大的。需要完善的转供策略
* @param :List* list
* @return:
* @update: [2018-07-13][张宇飞][]
*/
ErrorCode ConnectPath_MeetCondition(List* list, ConnectPath** pcp)
{
	CHECK_POINT_RETURN_LOG(list, NULL, ERROR_NULL_PTR, 0);
	CHECK_POINT_RETURN_LOG(list_size(list), 0, ERROR_OVER_LIMIT, 0);
	ConnectPath* maxcp = (ConnectPath*)(list_head(list));
	FOR_EARCH_LIST_START(list);

	ConnectPath* cp = ((ConnectPath*)list_data(m_foreach));
	if (maxcp->remainderCapacity < cp->remainderCapacity)
	{
		maxcp = cp;
	}	
	FOR_EARCH_LIST_END();
	*pcp = maxcp;
	return ERROR_OK_NULL;
}
/**
* @brief : 复位connectpath更新标志
* @param :StationTopology* topology
* @return:
* @update: [2018-07-13][张宇飞][]
*/
ErrorCode ConnectPath_ResetUpdateFlag(List* list)
{
	CHECK_POINT_RETURN_LOG(list, NULL, ERROR_NULL_PTR, 0);
	CHECK_EQUAL_RETURN_LOG(list_size(list), 0, ERROR_OVER_LIMIT, 0);
	ConnectPath* cp;
	FOR_EARCH_LIST_START(list);

	cp = ((ConnectPath*)list_data(m_foreach));

	cp->isUpdated = false;

	FOR_EARCH_LIST_END();
	return ERROR_OK_NULL;
}
/**
* @brief : 检测是否全部更新
* @param : List* list
* @return:
* @update: [2018-07-13][张宇飞][]
*/
ErrorCode ConnectPath_CheckIsUpdateComplete(List* list, bool* result)
{
	CHECK_POINT_RETURN_LOG(list, NULL, ERROR_NULL_PTR , 0);
	CHECK_EQUAL_RETURN_LOG(list_size(list), 0, ERROR_OVER_LIMIT, 0);
	*result = true;	
	FOR_EARCH_LIST_START(list);

	ConnectPath* cp = ((ConnectPath*)list_data(m_foreach));

	if (! cp->isUpdated)
	{
		*result = false;
		return ERROR_OK_NULL;
	}

	FOR_EARCH_LIST_END();
	return ERROR_OK_NULL;
}
/**
* @brief : 对于联络开关，检测是否允许转供电 TODO:待完善条件
* @param :StationTopology* topology
* @return:
* @update: [2018-07-12][张宇飞][]
*/
bool CheckIsPermitTransferpower(ConnectSwitch* connect)
{
	bool state = false;
	SwitchProperty* node;
	List* list;
	
	if (connect == NULL)
	{
		return false;
	}	

	if (!connect->isConnect)
	{
		return false;
	}
	for (uint8_t i = 0; i < connect->count; i++)
	{
		list = connect->path + i;

		FOR_EARCH_LIST_START(list);
		node = GET_SWITCH_ELEMENT(m_foreach);
		
		if (node->insulateType == RESULT_SUCCESS)
		{
			state = true;
			break;
		}
		//TODO：没有开关处于故障处理过程中
		FOR_EARCH_LIST_END();

	}
	return state;
}
/**
* @brief : 发送获取拓扑结构信息
* @param  :uint32_t dest 目的ID
* @param  :NodeFifo* nodeFifo 节点队列
* @return:
* @update: [2018-07-4][张宇飞][创建]
* @update: [2018-07-16][张宇飞][增加跳数信息]
*@update: [2018-07-19][张宇飞][修改形参与发送函数,由静态分配改为动态分配]
*/
static ErrorCode SendConnectPathMessage(uint32_t id, PathConnected iseset, uint8_t hops, DatagramTransferNode* pTransferNode)
{
    ErrorCode error = ERROR_OK_NULL;
    PointUint8 packet;
    packet.len = 6 + FRAME_MIN_LEN;

    

    uint8_t index = FRAME_FUNCODE_INDEX;
	packet.pData = (uint8_t*)MALLOC(6 + FRAME_MIN_LEN);
	CHECK_POINT_RETURN(packet.pData, NULL, ERROR_MALLOC);
    
    packet.pData[index++] = CONNECTED_MESSAGE;
	uint32_t sourceID = pTransferNode->id;
    packet.pData[index++] = GET_N_BYTE(sourceID, 0);
    packet.pData[index++] = GET_N_BYTE(sourceID, 1);
    packet.pData[index++] = GET_N_BYTE(sourceID, 2);
    packet.pData[index++] = GET_N_BYTE(sourceID, 3);
    packet.pData[index++] = iseset;
	packet.pData[index++] = hops;
    error = MakePacketMessage(&packet, GET_UINT16(id), GET_UINT16(sourceID));
	if (error)
	{
		return error;
	}
	error = pTransferNode->Send(pTransferNode, &packet);
    
    return error;    
}


/**
* @brief : 发送路径连接命令
* @param : 
* @param :
* @return:
* @update: [2018-07-06][张宇飞][创建]
* [2018-07-16][张宇飞][增加跳数，按照当前生成的路径是倒序]
*[2018-07-19][张宇飞][修改参数]
*/
static ErrorCode SendConnectPathCmd(List* path, PathConnected iseset, DatagramTransferNode* pTransferNode)
{
	uint8_t hops = 0, size = 0; 
	List* list;
    for (uint8_t i = 0; i < 2; i++)
    {
        list = path + i;
		size = list_size(list);
        FOR_EARCH_LIST_START(list);
        SendConnectPathMessage(*(uint32_t*)list_data(m_foreach), iseset, size - iii, pTransferNode);
        FOR_EARCH_LIST_END();
    }
    return ERROR_OK_NULL;
}


