/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      distribution_connect_check.c
* @brief:     判断自身开关信息
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-09-06
* @update:    
*/

#include "distribution_interface.h"   
#include "distribution.h"
#include "common_def.h"
#include "topology.h" 
#include "list.h"


static ErrorCode ExtractThroughPath(ListDouble* globalList, ConnectSwitch* connect,SwitchProperty* sourceSwitch,   ListDouble* bfsList);
static ErrorCode CheckSetConnectPath(uint32_t id, ConnectSwitch* pNode, StationTopology* station);
static bool JudgeConnectSwitch(SwitchProperty* sourceSwitch, ListDouble* listGloabal, ConnectSwitch* connect);
static bool StationJudgeConnectSwitch(StationTopology* station);


/**
* @brief : 检测开关链表是否完整，若不完整进行重新计算再判断
* @param : StationTopology* station
* @return: bool  是否通过
* @update: [2018-09-06][张宇飞][创建]
*/
bool CheckIsCompletedAndCalGloabalSwitch(StationTopology* station)
{
    
	if(!station)
	{
		perror("station is NULL\n");
		return false;
	}
    ErrorCode result = ERROR_OK_NULL;    
    ListDouble* globalList = &(station->globalSwitchList);
    ListDouble* globalTopologyList; 
    if (station->areaID.count == 0)
    {
    	perror("station->areaID.count == 0\n");
        return false;
    }
    if (globalList->size != station->areaID.count)
    {     
        globalTopologyList = &(station->globalTopologyList);
        if (list_size(globalTopologyList) < station->areaID.count)
        {
            return false;
        }
        printf("Re generate globalList,size:%d, Allcount:%d.\n", globalList->size, station->areaID.count);
        //销毁链表
        Listdestroy(globalList);
		//重新初始化
		ListInit(globalList, NULL);

		result = GetSwitchList(globalTopologyList, globalList);
		if (result)
		{
			perror("GetSwitchList Error: %d\n", result);
			LogAddException(result, station->id);
			return false;
		}
		if (globalList->size != station->areaID.count)
		{
			perror("globalList->size != station->connect.count. %d\n", globalList->size);
			LogAddException(ERROR_OVER_LIMIT, station->id);
			return false;
		}
        else
        {
            return true;
        }
        
    }
    return true;
}



/**
* @brief : 根据拓扑信息判断当前开关的否为联络开关，若不是则判别路径是否位于联络路径上及到联络开关的跳数
*          static函数，不进行参数检测，由调用者负责
* @param : SwitchProperty* sourceSwitch
* @param : ListDouble* listGloabal
* @param : ConnectSwitch* connect
* @return: bool 判别完成
* @update: [2018-09-06][张宇飞][由StationCalConnectPathAndJudge修改形参简化而来]
*/
static bool StationJudgeConnectSwitch(StationTopology* station)
{
	bool result;
	SwitchProperty* sourceSwitch = station->localSwitch;
	ListDouble* listGloabal = &(station->globalSwitchList);
	ConnectSwitch* connect = &(station->connect);

	//首先判断当前开关
	Listdestroy(&(station->connectPath));
	ListInit(&(station->connectPath), FREE);

	result = JudgeConnectSwitch(sourceSwitch, listGloabal, connect);
	if (result)
	{
		//若为联络开关则直接跳出，不在进入后续判断
		if(connect->isConnect)
		{
			return true;
		}
	}



    
	ConnectSwitch connectNode;
	ConnectSwitch* pNode = &connectNode;
	MEMSET(pNode, 0, sizeof(ConnectSwitch));

	FOR_EARCH_LIST_START(listGloabal)
	sourceSwitch = GET_SWITCH_ELEMENT(m_foreach);

	if (sourceSwitch == station->localSwitch)
	{
		continue;
	}
	result = JudgeConnectSwitch(sourceSwitch, listGloabal, pNode);
	if (result)
	{
		//若为联络开关则直接跳出，则判断本开关是否在路径上
		if(pNode->isConnect)
		{
			CheckSetConnectPath(sourceSwitch->id,  pNode, station);
		}
	}
	FOR_EARCH_LIST_END();

	Listdestroy(pNode->path);
	Listdestroy(pNode->path + 1);

	//更新isFaultEdgeConnected
	if (list_size(&station->connectPath) > 0)
	{
		station->localSwitch->fault.isFaultEdgeConnected = true;
	}
	else
	{
		station->localSwitch->fault.isFaultEdgeConnected = false;
	}
    return true;
}
/**
* @brief : 判断指定开关是否为联络开关
* @param : ConnectSwitch* pNode
* @param : StationTopology* station
* @return: bool 判别完成
* @update: [2018-09-06][张宇飞][设置联络路径信息]
*/
static ErrorCode CheckSetConnectPath(uint32_t id, ConnectSwitch* pNode, StationTopology* station)
{
	ListDouble* list;
	SwitchProperty* node;
	for (uint8_t i = 0; i < pNode->count; i++)
	{
		list = pNode->path + i;
		FOR_EARCH_LIST_START(list);
		node = GET_SWITCH_ELEMENT(m_foreach);
		if (node ==  station->localSwitch)
		{
			ConnectPath* pid = (ConnectPath*)CALLOC(1, sizeof(ConnectPath));
			CHECK_POINT_RETURN(pid, NULL, ERROR_MALLOC);
			pid->hopsNumber = list_size(list) - iii;
			pid->id = id;
			pid->timeStamp.updateTime = GetTime();
			pid->timeStamp.isValid = true;
			ListInsertNext(&(station->connectPath), NULL, pid);

			break;
		}

		FOR_EARCH_LIST_END();

	}
    return ERROR_OK_NULL;
}

/**
* @brief : 判断指定开关是否为联络开关
* @param : SwitchProperty* sourceSwitch
* @param : ListDouble* listGloabal
* @param : ConnectSwitch* connect
* @return: bool 判别完成
* @update: [2018-09-06][张宇飞][由StationCalConnectPathAndJudge修改形参简化而来]
*/
static bool JudgeConnectSwitch(SwitchProperty* sourceSwitch, ListDouble* listGloabal, ConnectSwitch* connect)
{
    ErrorCode result = ERROR_OK_NULL;
    //PrintMemoryUsed();
    

    //使用头作为计算节点
    if (sourceSwitch == NULL)
    {
        perror("sourceSwitch is null: \n");
        return false;
    }
    //查找电源开关数量
    ListDouble listPower;
    ListDouble* powerList = &listPower;
    ListInit(powerList, NULL);

    ListDouble findList;  //去除分位开关
    ListDouble* find = &findList;
    ListInit(find, NULL);

	result = SerachPowerConditionList(sourceSwitch, listGloabal, powerList, find);
	if (result)
	{
		perror("SerachPowerConditionList Error: 0x%X: \n", result);
        return false;
	}

    ListDouble bfsListInstance;
    ListDouble* bfsList = &bfsListInstance;
    ListInit(bfsList, DestoryBFSHelper);
    
	result = SerachPowerPath(sourceSwitch, powerList, find, bfsList);
	if (result)
	{
		perror("SerachPowerPath Error: 0x%X: \n", result);
        return false;
	}
	


    //至少有两条路径到开关，对于非合环路径只有两个，TODO:(对于合环或者分布式电源可能有大于3个，且可能重复)
    //存在两条路径即认为是联络开关
    //uint8_t size = list_size(bfsList);
	//ConnectSwitch connect;
	Listdestroy(connect->path);
	ListInit(connect->path, NULL);
	Listdestroy(connect->path + 1);
	ListInit(connect->path + 1, NULL);
	connect->count = 0;
   
	result = ExtractThroughPath(listGloabal, connect, sourceSwitch,   bfsList);
	if (result)
	{
        perror("ExtractSwitchPath Error: 0x%X: \n", result);
		return false;
	}
	//数量为2，且各个链表数量均不为0
	if ((connect->count == 2) &&
		(list_size(connect->path) != 0)
		&& (list_size(connect->path + 1) != 0)
		&& (sourceSwitch->state == SWITCH_OPEN))
	{
		connect->isConnect = true;
	}
	else
	{
		connect->isConnect = false;
	}
   

	Listdestroy(&listPower); //没有泄露
	Listdestroy(&findList);  //没有泄露
    Listdestroy(&bfsListInstance);
    return true;
    //PrintMemoryUsed();
}

/**
* @brief : 提取经过的开关路径
* @param : ListDouble* globalList
* @param : ConnectSwitch* connect
* @param : SwitchProperty* sourceSwitch
* @param : ListDouble* bfsList
* @return: ErrorCode
* @update: [2018-07-10][张宇飞][未进行参数检测，由调用者负责, 适应多个开关查找情况]
*  [2018-09-6][张宇飞][由ExtractSwitchPath修改形参而来，提高通用型]
*/
static ErrorCode ExtractThroughPath(ListDouble* globalList, ConnectSwitch* connect,
		SwitchProperty* sourceSwitch,   ListDouble* bfsList)
{	
	ListDouble* list;
	BFSHelper* bfs;
	uint8_t size = list_size(bfsList);
	uint8_t count = 0, index = 0;
	uint32_t id = 0;
	SwitchProperty* findSwitch;
	ErrorCode result;
	ListElment* element = list_head(bfsList);
	connect->count = 0;
	for (uint8_t i = 0; i < size; i++, element = element->next)
	{
		list = connect->path + i;
		if (list_size(list) != 0)
		{
			Listdestroy(list);
		}
		ListInit(list, NULL);

		bfs = (BFSHelper*)(element->data);

		count = list_size(bfs->path);
		ListElment* node = list_head(bfs->path);
		if (count == 0)
		{
			continue;
		}
		//跳过自身开关
		for (uint8_t k = 0; k < count; k++)
		{
			index = (*(uint8_t*)(node->data));
			id = bfs->idArray[index];
			if (id != sourceSwitch->id)
			{
				ErrorCode error = FindSwitchNodeByID(globalList, id, &findSwitch);
				if (error != ERROR_OK_NULL)
				{
					perror("FindSwitchNodeByID ERROR: 0x%X\n", error);
					LogAddException(result, sourceSwitch->id);
					return error;								
				}				
				ListInsertNext(list, NULL, findSwitch);				
			}
			node = node->next;
		}
		connect->count++;
		
	}
	return ERROR_OK_NULL;
}


                         

/**
* @brief : 联络开关自判断
* @param :GetNeighboorHandle* handle
* @return:
* @update: [2018-09-06][张宇飞][联络信息自检和判断]
*[2018-09-06][张宇飞][完整性判断移到接收处理]
*/
ErrorCode ConnectedSwitch_SelfCheck_APP(StationPoint* point)
{
    uint8_t try_count = 0;
    StationTopology* topology = &(point->topology);
    bool *result = &(topology->areaID.isGainComplted);
    ErrorCode error;
    CHECK_POINT_RETURN_LOG(point, NULL, ERROR_NULL_PTR, 0);
	bool before = topology->connect.isConnect;
    bool isPassed;


//	 if (!topology->connect.isConnect)
//	 {
//		   if (topology ->localSwitch->state != SWITCH_OPEN)
//		   {
//			   return ERROR_OK_NULL;
//		   }
//
//	 }
    if(point->topology.connect.isLock)
    {
    	return ERROR_OK_NULL;
    }
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
			return ERROR_OVER_LIMIT;
		}

        state = CheckIsMeetConnectCondition(topology);
        if (state)
        {
        	return ERROR_OK_NULL;
        }
    }	

    isPassed = CheckIsCompletedAndCalGloabalSwitch(&(point->topology));
    if (!isPassed)
    {
    	perror("CheckIsCompletedAndCalGloabalSwitch isn't passed\n");
    	return ERROR_PARAMTER;
    }

    StationJudgeConnectSwitch(&(point->topology));

		
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


