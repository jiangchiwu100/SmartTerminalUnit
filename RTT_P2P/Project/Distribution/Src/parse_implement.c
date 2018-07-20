/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      parse_action.c
  * @brief:     用于解析与反解析
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-07
  * @update:    2018-06-14 增加反解析（序列化）内容，序列化与反序列化需要成对放置，以便修改。
  *   2018-06-21 增加获取,生成信息
  */
 
#include "distribution_def.h"
#include "parse_implement.h" 
#include "distribution.h"
#include "log.h"

/**
 * @brief : 拓扑数据 初始化
 * @param  ：void
 * @return: 0-正常 非0错误
 * @update: [2018-05-26[张宇飞][]
 */
uint8_t StationInit(StationTopology* station,  uint32_t id)
{
    station->id = id;
    ListInit( &(station->globalTopologyList), NULL); //初始化列表列表    
    ListInit(&(station->neighbourSwitchList), NULL);// 初始化开关列表
    
    return 0;
}
/**
 * @brief : 增加拓扑成员，通过数组数据
 * @param  ： data  数据数组
 * @param  ： len 数据长度
 * @param  ：StationTopology* station 站点
 * @return: 0-正常 非0错误
 * @update: [2018-05-26[张宇飞][]
 */
void StationAddTopologyByArrayData(uint8_t data[], uint8_t len, StationTopology* station )
{   
    AddTopologyMember(data, len, &(station->globalTopologyList));   
    //PrintTopologyMessage(GET_TOPOLOGY_ELEMENT(list_head(&(station->globalTopologyList))));
}

/**
 * @brief :设置本地信息
 * @param : uint8_t data[] 序列化数据
 * @len   : uint8_t len 数据长度
 * @param  ：StationTopology* station 站点
 * @return: void
 * @update: [2018-6-1][张宇飞][]
 *  [2018-6-16][张宇飞][增加本地开关节点设置]
 */
void StationSetLocalMessage(uint8_t data[], uint8_t len, StationPoint* point)
{   
    if  (4 == len)
    {
        StationTopology* station = &(point->topology);
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (station->globalTopologyList.size !=0)
        {
            ErrorCode result = FindTopologyNodeByID(&(station->globalTopologyList), id, &(station->localTopology));
            if ((!result) && (station->localTopology->switchCollect == NULL))
            {
                station->localSwitch = station->localTopology->switchCollect;                
                RemovalHandleUpdateSwitchProperty(&(point->removalHandle), station->localSwitch);

                //rt_kprintf("Local Topology:\n");
                //PrintTopologyMessage(station->localTopology);
            }
            else
            {
                rt_kprintf("Local Topology EOOR: %X:\n", result);
            }
        }
        else
        {
            rt_kprintf("station->globalTopologyList Topology is 0.\n");
        }
    }
    else
    {
        rt_kprintf("Data length is  wrong!");
    }
}
/**
 * @brief :设置目的
 * @param : uint8_t data[] 序列化数据
 * @len   : uint8_t len 数据长度
 * @param  ：StationTopology* station 站点
 * @return: void
 * @update: [2018-05-6-1][张宇飞][]
 */
void StationSetAimMessage(uint8_t data[], uint8_t len, StationTopology* station )
{    
    if  (4 == len)
    {
       
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (station->globalTopologyList.size !=0)
        {
            uint8_t result = FindTopologyNodeByID(&(station->globalTopologyList), id, &(station->aimTopology));
            if (!result)
            {
               // rt_kprintf("Aim Topology:\n");
               // PrintTopologyMessage(station->aimTopology);
            }
            else
            {
                rt_kprintf("Aim Topology EOOR: %X:\n", result);
            }           
        }
        else
        {
            rt_kprintf("station->globalTopologyList Topology is 0.\n");
        }
    }
    else
    {
        rt_kprintf("Data length is  wrong!\n");
    }
}



/**
 * @brief :  配电区域计算测试
 * @param  ：StationTopology* station 站点
 * @return: 0-正常 非0错误
 * @update: [2018-05-30][张宇飞][]
 * @update: [2018-06-23][张宇飞][引入局部变量，若不存在本地拓扑则不进行计算]
 */
void StationCalDistributionPowerArea(StationTopology* station)
{
    uint8_t result = 0;
    if (station->globalTopologyList.size < 2)
    {
        rt_kprintf("station->globalTopologyList.size is too small!\n");
        return ;
    }
    
    if (station->neighbourSwitchList.size !=0)
    {
        //销毁链表
        Listdestroy(&(station->neighbourSwitchList));
        //重新初始化
        ListInit(&(station->neighbourSwitchList), NULL);
    }
   
    //使用头作为计算节点
    if (station->localTopology == NULL)
    {        
        rt_kprintf("StationCalDistributionPowerArea: station->localTopology == NULL\n");
        return;
    }
    TopologyMessage* topology = station->localTopology;
    //rt_kprintf("\n Local Node Message:\n");
    //PrintTopologyMessage(topology); 
    
    //rt_kprintf(" Neighbour Node Message:\n");
    
    GetNeighboorSwitch(&(station->neighbourSwitchList), topology, &(station->globalTopologyList), &(station->neighbourSwitchList));
    
    ListElment* element = list_head(&(station->neighbourSwitchList));
    
   /* for(uint8_t i = 0; i < list_size(&(station->neighbourSwitchList)); i++)
    {
        PrintSwitchMessage(GET_SWITCH_ELEMENT(element));
        element = element->next;
    }*/
    
    
   
   //释放内存 并重新初始化
   FreeAreaList(station->powerArea.powerAreaList, 2);       
  
   
   result = GetPowerDistributionArea(topology, &(station->neighbourSwitchList), station->powerArea.powerAreaList);
  
   if (result) 
   {
        rt_kprintf("GetPowerDistributionArea error, error code:{ %d }\n", result);
   }
   else
   {
        rt_kprintf("GetPowerDistributionArea success.\n");
        PrintPowerAreaMessage(station->powerArea.powerAreaList, 2);
        //标记并提取配电区域
        ErrorCode error = ExtractPowerAreaFromList(topology->switchCollect, &(station->powerArea), 2);
        if (error != ERROR_OK_NULL)
        {
            rt_kprintf("ExtractPowerAreaFromList is ERROR: %X.\n", error);
        }
   }
}


/**
 * @brief :  路径搜索计算测试
 * @param  ：StationTopology* station 站点
 * @return: 0-正常 非0错误
 * @update: [2018-05-30][张宇飞][]
 */

void StationTestPath(StationTopology* station)
{
    uint8_t result = 0;
    if (station->globalTopologyList.size < 2)
    {
        rt_kprintf("station->globalTopologyList.size is too small!\n");
        return ;
    }
    
    if (station->globalSwitchList.size !=0)
    {
        //销毁链表
        Listdestroy(&(station->globalSwitchList));
        //重新初始化
        ListInit(&(station->globalSwitchList), NULL);
    }
    
    
    result = GetSwitchList(&(station->globalTopologyList), &(station->globalSwitchList));
    if (result)
    {
        rt_kprintf("GetSwitchList Error: %d\n", result);
        return ;
    }
    
    if (station->globalSwitchList.size < 2)
    {
        rt_kprintf("GetSwitchList size is too small: %d\n", station->globalSwitchList.size);
        return ;
    }
    
    //使用头作为计算节点
    if (station->localTopology == NULL)
    {        
        station->localTopology = GET_TOPOLOGY_ELEMENT(list_head(&(station->globalSwitchList)));
    }
    if (station->aimTopology == NULL)
    {        
        station->aimTopology = GET_TOPOLOGY_ELEMENT(list_tail(&(station->globalSwitchList)));
    }
    
    
    if (station->BFSResult->size != 0)
    {
        FreeBFSHelper(&(station->BFSResult));
    }
    
    result = FindPath(&(station->globalSwitchList), station->localTopology->switchCollect, station->aimTopology->switchCollect, &(station->BFSResult));
    if ( result  )
    {
        rt_kprintf("FindPath ERROR: %d\n", result);
        return ;
    }
   // PrintBFSHelper(BFSResult);
    PrintBFSHelperSimple(station->BFSResult);
}

void StationTestPathAll(StationTopology* station)
{
    
    ListElment* element = list_head(&(station->globalTopologyList));
    uint8_t size = list_size(&(station->globalTopologyList));
    for (uint8_t i = 0; i < size; i++)
    {
        station->localTopology = GET_TOPOLOGY_ELEMENT(element);
        
        ListElment* node = list_head(&(station->globalTopologyList));
       
        for (uint8_t k = 0; k < size; k++)
        {
            station->aimTopology = GET_TOPOLOGY_ELEMENT(node);    
            rt_kprintf("\n");   
            rt_kprintf("index, %d, %d, Local, %X, Aim, %X,\n", i, k,  station->localTopology->switchCollect->id, station->aimTopology->switchCollect->id);            
            node = node->next;
            
            StationTestPath(station);
            rt_kprintf("\n");   
        }
        
        element = element->next;
        
    }

}
/**
 * @brief : 删除指定拓扑节点
 * @param : 
 * @return: 0-正常 非0错误
 * @update: [2018-05-29][张宇飞][]
 */
void StationDeleteTopologyNode(uint8_t data[], uint8_t len, StationTopology* station)
{
    if  (4 == len)
    {
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (station->localTopology->id == id)
        {
            station->localTopology = 0;
             rt_kprintf("node delete.");
        }
        DeleteTopologyListNodeById(&(station->neighbourSwitchList), id);
        
    }
    else
    {
        rt_kprintf("Data length is  wrong!");
    }
}

/**
 * @brief : 管理器增加站点
 * @param : 
 * @return: void
 * @update: [2018-06-12][张宇飞][]
 * [2018-06-19][张宇飞][增加站点时候，与此同时配置本站信息，将更新邻居信息与此合并。]
 *[2018-07-19][张宇飞][修补重复添加内存泄漏问题,删去router]
 */
void  ManagerAddStation(uint8_t data[], uint8_t len, StationManger* manger)
{
    uint32_t id;
    StationPoint* station;
    TopologyMessage*  topologyMessage;

    //反序列化生成拓扑信息
    ErrorCode error = ReserializeCollectTopology(data, 0, &topologyMessage);
    if (error != ERROR_OK_NULL)
    {
        rt_kprintf("ReserializeCollectTopology ERROR : %X\n", error);
        return;
    }
	id = topologyMessage->id;

	//是否已经存在此ID
	ListDouble* list = &(manger->stationServer.stationPointList);
	station = FindStationPointById(list, id);
	if (station != NULL)
	{
		FreeTopologyMemory(&topologyMessage);
		rt_kprintf("Node Has Exited.\n");
		return;
	}

    PrintTopologyMessage(topologyMessage);
    //增加站点
    error = StationServerAddPoint(&(manger->stationServer),  topologyMessage, &station);
    if (error != ERROR_OK_NULL)
    {
        rt_kprintf("StationServerAddPoint ERROR : %X\n", error);
        //TODO: 释放内存
        return;
    }

    error = SimulationStationServerAddMember(&(manger->simulationServer), id, &(station->topology.localSwitch));
    if (error)
    {
        rt_kprintf("SimulationStationServerAddMember ERROR : %X\n", error);
    }
}

/**
* @brief : 更新状态信息，更新配电区域相关信息,
* @param :
* @return: void
* @update: [2018-06-12][张宇飞][]
*          [2018-06-23][张宇飞][适应配电区域属性修改]
*          [2018-06-26][张宇飞][设拓扑结构只含有一个开关，开关数与列表数不相等，则更新开关列表]
*		   [2018-07-09][张宇飞][收集完成与否判别，将判别放到标志内还是外部，如何分工]
*/
void  StationUpdateStatusMessage(uint8_t data[], uint8_t len, StationPoint* point)
{
    uint32_t id;
    uint16_t index;
    SwitchProperty* switchProperty;
    uint8_t result;
    DistributionStation* distribution;
    ListDouble* list = &(point->topology.globalSwitchList);
    if (data == NULL  || point == NULL)
    {
        perror("data == NULL  || point == NULL!.\n");
        return;
    }
    ListDouble* listaq = &(point->topology.globalTopologyList);
    //若为0，则更新开关列表
    if (list->size != listaq->size)
    {
        Listdestroy(list);//清空列表
        ListInit(list,  NULL);//重新初始化
        result = GetSwitchList(listaq, list);
        //开关列表更新失败
        if (result)
        {
            perror("globalTopologyList Update Failure.\n");
            return;
        }
    }

    //校验长度
    if (data[0]* 8 + 1 == len)
    {   
        for (uint16_t i = 0; i < data[0]; i++)
        {
            id = COMBINE_UINT32(data[4 + i*8], data[3 + i * 8], data[2 + i * 8], data[1 + i * 8]);
            index = 5 + i * 8;                     
            result = FindSwitchNodeByID(list, id, &switchProperty);
            if (result != ERROR_OK_NULL )
            {
                return ;
            }
            switchProperty->fault.state = (FaultState)data[index++];
            switchProperty->state = (SwitchState)data[index++];
            switchProperty->operateType = (OperateType)data[index++];
            switchProperty->overTimeType = (OverTimeType)data[index++];
            distribution = switchProperty->distributionArea;
           
            if (distribution != NULL)
            {                
                   //distribution->UpdatePowerArea(switchProperty);                                
                   distribution->SignExitFaultMessage(switchProperty);
				   //运行时, 存在故障信息时进行判别
				  if (point->removalHandle.isRun || distribution->JudgeIsExitFault(distribution->switchRef))
				   {	
					   distribution->UpdatePowerArea(switchProperty);
					   if (! distribution->isGatherCalculateCompleted)
					   {
						   distribution->GatherCompletedAndJudgeFaultArea(distribution, switchProperty);
						   distribution->isAlreayExitedFault = distribution->IsAlreayExitedFault(distribution);
					   }					   
				   }				   
            }                       
        }
    }
    else
    {
        perror("Data length is  wrong!.\n");
    }

}

/**
* @brief : 接收应答信息
* @param :
* @return: void
* @update: [2018-06-21][张宇飞][]
*/
void  StationReciveReplyMessage(uint8_t data[], uint8_t len, StationTopology* station)
{
    uint16_t offset = 1;
    uint16_t count = data[0]; 
    ErrorCode error;
    for (uint8_t i = 0; i < count; i++)
    {
        switch ((GetMessageCmd)data[offset++])
        {
        case  GET_TOPOLOGY:
        {
            error = AddTopologyMember(data + offset, len, &(station->globalTopologyList));
            if (error == ERROR_OK_NULL)
            {
                UpdateDistributionPowerArea(station);
              //  PrintTopologyMessage(GET_TOPOLOGY_ELEMENT(list_head(&(station->globalTopologyList))));
               //计算下次偏移
                offset = COMBINE_UINT16(data[offset+2], data[offset + 1]) + offset;
            }
            else
            {
                rt_kprintf("StationReciveReplyMessage ERROR.\n");
                return;
            }
           
            break;
        }
        default:
            rt_kprintf("StationReciveReplyMessage UNKOWN ID.\n");
            break;
        }
    }   
}

/**
* @brief : 接收获取信息
* @param :
* @return: void
* @update: [2018-06-21][张宇飞][]
*/
void  StationReciveGetMessage(FrameRtu* pRtu, StationPoint* point)
{
    uint8_t* data = pRtu->pValidData;
    uint16_t len = pRtu->datalen;
    uint8_t count = data[0];
    uint32_t id1, id2, id3, id4, id;
    uint16_t index = 0;
    TopologyMessage* topology;
    ErrorCode error;
    PointUint8 packet;
	
    if (data == NULL || point == NULL)
    {
        rt_kprintf("StationReciveGetMessage EOOR:data == NULL || point == NULL .\n");
        return;
    }
    

    if ((count * 5+1) != len)
    {
        rt_kprintf("Data Length is wrong.\n");
        return;
    }
    index = 1;
    for (uint8_t i = 0; i < count; i++)
    {
        id1 = data[index++];
        id2 = data[index++];
        id3 = data[index++];
        id4 = data[index++];
        id = COMBINE_UINT32(id4, id3, id2, id1);
        switch ((GetMessageCmd)data[index++])
        {
        case GET_TOPOLOGY:
        {
            error = FindTopologyNodeByID(&(point->topology.globalTopologyList), id, &topology);
            if (error == ERROR_OK_NULL)
            {
                error = MakeSingleReplyTopologyMessage(topology, &packet);
                if (error != ERROR_OK_NULL)
                {
                    rt_kprintf("StationReciveGetMessage, GET_TOPOLOGY. ERROR:%X.\n", error);
                    return;
                }
                MakePacketMessage(&packet, pRtu->address, GET_UINT16(point->id));
               
				DatagramTransferNode* pTransferNode = &(point->transferNode);
				pTransferNode->Send(pTransferNode, &packet);
                
            }
           
            break;
        }
        default:
            rt_kprintf("StationReciveGetMessage Unkonw ID.\n");
            break;
        }
    }
}

/**
* @brief : 接收故障切除成功信息，只考虑单个故障切除信息
* @param :
* @return: void
* @update: [2018-07-07][张宇飞][创建]
*[2018-07-10][张宇飞][适应多个节点]
*[2018-07-16][张宇飞][改邻居列表为全局列表，修改故障标记]
*/
void  StationReciveRemovalMessage(uint8_t data[], uint8_t len, StationTopology* station)
{
	uint8_t count = data[0];
	uint16_t index = 0;
	uint32_t id, id1, id2,id3, id4;
	ResultType type;
	ErrorCode error;
	SwitchProperty* find;
	if (data == NULL || len != (count *5+1) || station == NULL)
	{
		LogAddException(ERROR_OVER_LIMIT, 0);
		perror("data == NULL || len != (count *5+1) || server == NULL");
	}
	index = 1;

	//后面实际上没有使用	
	for (uint8_t i = 0 ; i < count; i++)
	{
		id1 = data[index++];
		id2 = data[index++];
		id3 = data[index++];
		id4 = data[index++];

		id = COMBINE_UINT32(id4, id3, id2, id1);
		type = (ResultType)data[index++];
		error = FindSwitchNodeByID(&(station->globalSwitchList), id, &find);
		if (error == ERROR_OK_NULL)
		{
			if (i == 0)//只针对首个进行标注。其它只标记配电区域
			{
				find->removalType = type;
			}		
			//配电区域为链路
            if(find->distributionArea != NULL)
            {
                find->distributionArea->SignRemovalMessage(find, type);
            }
                
			//(find->distributionArea != NULL) ? (find->distributionArea->SignRemovalMessage(find, type)) : (find );
		}
	}

	
}
/**
* @brief : 接收隔离信息，只考虑单个隔离信息信息
* @param :
* @return: void
* @update: [2018-07-07][张宇飞][创建]
*2018-07-12][张宇飞][搜索列表从邻居列表改为全局列表]
*/
void  StationReciveInsulateMessage(uint8_t data[], uint8_t len, StationTopology* station)
{
	if (data == NULL || len != 6 || station == NULL)
	{
		LogAddException(ERROR_OVER_LIMIT, 0);
		perror("data == NULL || len != 6 || server == NULL");
	}

	SwitchProperty* find;
	uint32_t id = COMBINE_UINT32(data[4], data[3], data[2], data[1]);
	ResultType type = (ResultType)data[5];
	ErrorCode error = FindSwitchNodeByID(&(station->globalSwitchList), id, &find);
	if (error == ERROR_OK_NULL)
	{
		find->insulateType = type;
        if(find->distributionArea != NULL)
        {
            find->distributionArea->SignInsulateMessage(find);
        }
		//(find->distributionArea != NULL) ? (find->distributionArea->SignInsulateMessage(find)) : (find);
	}
}
/**
* @brief : 生成单个状态信息,头空出9字节，尾巴空出3字节，以备发送。 内部有动态内存分配
* @param  ：uint32_t id
* @return: 0-正常 非0错误
* @update: [2018-06-13[张宇飞][]
*/
uint8_t MakeSingleStatusMessage(uint32_t id, FaultState state, SwitchState position,
    OperateType operateType, OverTimeType overTime,
    PointUint8* packet)
{
    packet->len = FRAME_MIN_LEN + 9;
    packet->pData = (uint8_t* )MALLOC(sizeof(uint8_t) * packet->len);
    if (NULL == packet->pData)
    {
        return ERROR_MALLOC;
    }
    uint16_t index = FRAME_FUNCODE_INDEX;
    packet->pData[index++] = STATUS_MESSAGE;
    packet->pData[index++] = 1;
    packet->pData[index++] = GET_N_BYTE(id, 0);
    packet->pData[index++] = GET_N_BYTE(id, 1);
    packet->pData[index++] = GET_N_BYTE(id, 2);
    packet->pData[index++] = GET_N_BYTE(id, 3);
    packet->pData[index++] = (uint8_t)state;
    packet->pData[index++] = (uint8_t)position;
    packet->pData[index++] = (uint8_t)operateType;
    packet->pData[index++] = (uint8_t)overTime;
    return 0;
}
/**
* @brief : 生成单个简单状态信息,头尾空出自己以备发送，以备发送, 内部有动态内存分配
* @param  ：uint32_t id
* @return: 0-正常 非0错误
* @update: [2018-07-07[张宇飞][]
*/
ErrorCode MakeSimpleMessage(FuncionCode code, uint32_t id, uint8_t data, PointUint8* packet)
{
	packet->len = FRAME_MIN_LEN + 6;
	packet->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * packet->len);
	if (NULL == packet->pData)
	{
		return ERROR_MALLOC;
	}
	uint16_t index = FRAME_FUNCODE_INDEX;
	packet->pData[index++] = code;
	packet->pData[index++] = 1;
	packet->pData[index++] = GET_N_BYTE(id, 0);
	packet->pData[index++] = GET_N_BYTE(id, 1);
	packet->pData[index++] = GET_N_BYTE(id, 2);
	packet->pData[index++] = GET_N_BYTE(id, 3);
	packet->pData[index++] = data;
	return ERROR_OK_NULL;
}
/**
* @brief : 生成转供容量信息
* @param  ：uint32_t id
* @return: 0-正常 非0错误
* @update: [2018-07-12[张宇飞][]
*/
ErrorCode MakeCapacityMessage(TransferCode code, uint32_t id,  uint32_t data, PointUint8* packet)
{
	packet->len = FRAME_MIN_LEN + 9;
	packet->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * packet->len);
	if (NULL == packet->pData)
	{
		return ERROR_MALLOC;
	}
	uint16_t index = FRAME_FUNCODE_INDEX;
	packet->pData[index++] = TRANSFER_MESSAGE;
	packet->pData[index++] = code;	
	packet->pData[index++] = GET_N_BYTE(id, 0);
	packet->pData[index++] = GET_N_BYTE(id, 1);
	packet->pData[index++] = GET_N_BYTE(id, 2);
	packet->pData[index++] = GET_N_BYTE(id, 3);
	packet->pData[index++] = GET_N_BYTE(data, 0);
	packet->pData[index++] = GET_N_BYTE(data, 1);
	packet->pData[index++] = GET_N_BYTE(data, 2);
	packet->pData[index++] = GET_N_BYTE(data, 3);
	return ERROR_OK_NULL;
}

/**
* @brief : 设定或取消转供
* @param  ：uint32_t id
* @return: 0-正常 非0错误
* @update: [2018-07-12[张宇飞][]
*/
ErrorCode MakeTransferSetCancerMessage(TransferCode code, uint32_t id, PointUint8* packet)
{
	packet->len = FRAME_MIN_LEN + 5;
	packet->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * packet->len);
	if (NULL == packet->pData)
	{
		return ERROR_MALLOC;
	}
	uint16_t index = FRAME_FUNCODE_INDEX;
	packet->pData[index++] = TRANSFER_MESSAGE;
	packet->pData[index++] = code;
	packet->pData[index++] = GET_N_BYTE(id, 0);
	packet->pData[index++] = GET_N_BYTE(id, 1);
	packet->pData[index++] = GET_N_BYTE(id, 2);
	packet->pData[index++] = GET_N_BYTE(id, 3);
	return ERROR_OK_NULL;
}
/**
* @brief : 生成单个状态信息,头空出9字节，尾巴空出3字节，以备发送, 内部有动态内存分配
* @param  ：uint32_t id
* @return: 0-正常 非0错误
* @update: [2018-06-13[张宇飞][]
*/
ErrorCode MakeRemovalMessage(const SwitchProperty* const switchProperty, ResultType type, PointUint8* packet)
{	
	CHECK_POINT_RETURN_LOG(switchProperty, NULL, ERROR_NULL_PTR, 0);
	CHECK_POINT_RETURN_LOG(switchProperty->parent, NULL, ERROR_NULL_PTR, 0);
	ConnectSwitch* connect = &(switchProperty->parent->connect);
	uint8_t count = 0;
	uint32_t id = 0;
	ListDouble* list ;
	//计算总数
	for (uint8_t i = 0; i < connect->count; i++)
	{
		count += list_size(connect->path + i);
	}
	count += 1;//加本身

	


	packet->len = FRAME_MIN_LEN + 1 + 5 * count;
	packet->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * packet->len);
	if (NULL == packet->pData)
	{
		return ERROR_MALLOC;
	}
	uint16_t index = FRAME_FUNCODE_INDEX;
	packet->pData[index++] = REMOVAL_MESSAGE;
	packet->pData[index++] = count;

	id = switchProperty->id;
	packet->pData[index++] = GET_N_BYTE(id, 0);
	packet->pData[index++] = GET_N_BYTE(id, 1);
	packet->pData[index++] = GET_N_BYTE(id, 2);
	packet->pData[index++] = GET_N_BYTE(id, 3);
	packet->pData[index++] = (uint8_t)type;

	for (uint8_t i = 0; i < connect->count; i++)
	{
		list = connect->path + i;
		FOR_EARCH_LIST_START(list)

		id = GET_SWITCH_ELEMENT(m_foreach)->id;
		packet->pData[index++] = GET_N_BYTE(id, 0);
		packet->pData[index++] = GET_N_BYTE(id, 1);
		packet->pData[index++] = GET_N_BYTE(id, 2);
		packet->pData[index++] = GET_N_BYTE(id, 3);
		packet->pData[index++] = (uint8_t)type;

		FOR_EARCH_LIST_END();

		
	}
	
	return ERROR_OK_NULL;

	
}
/**
* @brief : 生成单个获取信息命令,有内存动态分配
* @param  ：uint32_t id  获取ID
* @param  ：GetMessageCmd cmd 获取命令
* @param  ：PointUint8* packet 打包数据
* @return: ErrorCode
* @update: [2018-06-21[张宇飞][]
*/
ErrorCode MakeSingleGetMessage(uint32_t id, GetMessageCmd cmd, PointUint8* packet)
{
	return MakeSimpleMessage(GET_MESSAGE, id, (uint8_t)cmd, packet);
    
}
/**
* @brief : 生成单个故障隔离信息
* @param  ：uint32_t id  获取ID
* @param  ：GetMessageCmd cmd 获取命令
* @param  ：PointUint8* packet 打包数据
* @return: ErrorCode
* @update: [2018-07-07[张宇飞][]
*/
ErrorCode MakeInsulateMessage(uint32_t id, ResultType result, PointUint8* packet)
{
	return MakeSimpleMessage(INSULATE_MESSAGE,  id, (uint8_t)result, packet);
}
/**
* @brief : 产生应答信息
* @param  ：uint32_t id  获取ID
* @param  ：GetMessageCmd cmd 获取命令
* @param  ：PointUint8* packet 打包数据
* @return: ErrorCode
* @update: [2018-06-21[张宇飞][]
*/
ErrorCode MakeSingleReplyTopologyMessage(TopologyMessage* topology, PointUint8* packet)
{
    ErrorCode error = SerializeCollectTopology(topology, packet, FRAME_MIN_LEN + 2, 3 + FRAME_FUNCODE_INDEX);//帧结构确定附加长度
    CHECK_UNEQUAL_RETURN(error, ERROR_OK_NULL, error);
    packet->pData[FRAME_FUNCODE_INDEX] = (uint8_t)REPLY_MESSAGE;
    packet->pData[FRAME_FUNCODE_INDEX +1] = 1;
    packet->pData[FRAME_FUNCODE_INDEX +2] = (uint8_t)GET_TOPOLOGY;
    
    return ERROR_OK_NULL;
}


/**
* @brief : 生成日志消息
* @param  ：
* @return: void
* @update: [2018-06-30[张宇飞][]
*/
ErrorCode SendLogMessage(uint16_t source, uint16_t dest)
{
    PointUint8 packet;
    ErrorCode error =  PacketEncodeLogMessage(&(g_Loghandle.log), &packet, FRAME_MIN_LEN, FRAME_FUNCODE_INDEX);
    CHECK_UNEQUAL_RETURN(error, ERROR_OK_NULL, error);
    MakePacketMessage(&packet, dest, source);


    g_VirtualNode.Write(&(g_VirtualNode.reciveRing), &packet);
 

    return ERROR_OK_NULL;
}


/**
* @brief : 开关操作控制
* @param  ：uint32_t id
* @return: void
* @update: [2018-06-16[张宇飞][]
*[2018-07-09[张宇飞][修改数量计算错误]
*/
void  StationOperateSwitch(uint8_t data[], uint8_t len, SimulationStationServer* server)
{
    SimulationStation* station;
    uint32_t id,id1,id2,id3,id4;
    uint16_t index = 0;
    SwitchControlOperate operate;
	
    if (data == NULL || server == NULL)
    {
        rt_kprintf("data == NULL || manager == NULL .\n");
        return;
    }
	uint8_t count = data[0];
    if (count * 5 + 1 != len)
    {
        rt_kprintf("Data length is  wrong!.\n");
        return;
    }


    ListDouble* list = &(server->SimulationStationList);    
	
    index = 1;
    for (uint8_t i = 0; i < count; i++)
    {
        id1 = data[index++];
        id2 = data[index++];
        id3 = data[index++];
        id4 = data[index++];
        id = COMBINE_UINT32(id4, id3, id2, id1);
        operate = (SwitchControlOperate)data[index++];
        station = FindSimulationStationById(list, id);
        SimulationSwitchControlOperate(station,  operate);
    }
}




/**
* @brief : 开关操作控制
* @param  ：uint32_t id
* @return: void
* @update: [2018-06-16[张宇飞][]
*/
void StationSetConnectPath(uint8_t data[], uint8_t len, StationTopology* topology)
{
    if (data == NULL || len != 6 || topology == NULL)
    {
        LogAddException(ERROR_OVER_LIMIT, 0);
        perror("data == NULL || len != 6 || server == NULL");
		return;
    }
    uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);

	uint8_t before = list_size(&(topology->connectPath));
	
    ErrorCode error = SetConnectPath(id, (PathConnected)data[4], data[5], topology);
    if (error != ERROR_OK_NULL)
    {
        perror("SetConnectPath error: %X", error);
		return;
    }
	uint8_t after = list_size(&(topology->connectPath));
	if (after != before)//变化时提示
	{
		if (after)
		{
			topology->localSwitch->fault.isFaultEdgeConnected = true;
			PrintIDTipsTick(topology->id, "It's path connected.");
		}
		else
		{
			topology->localSwitch->fault.isFaultEdgeConnected = false;
			PrintIDTipsTick(topology->id, "It's not path connected.");
		}
	}
}
