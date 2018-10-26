/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      topology.c
  * @brief:     文件中多次使用动态内存分配与链表，对于重新配置情况，需要注意内存释放问题，防止内存泄露。
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-25
  * @update:    2018-06-14 size 与MDK不一样，需要注意，更新
  FifoHandle queue;
  uint8_t queueBuffer[size];//TODO:注意MDK此处为size
  PointUint8 queuePack;
  */
  
#include "topology.h" 
#include "helper.h"
#include <rtthread.h>

#include "buffer.h"
#include "distribution_interface.h" 

#include "distribution_test_case.h" 
#include "extern_interface.h"



static ErrorCode SearchIndexByID(uint32_t* data, uint8_t len, uint32_t id, uint8_t* index);
static ErrorCode CalTopologyMessageLength(const TopologyMessage* topology, uint16_t* len);

/**
  * @brief :数据反序列化，即将数组数据转换成格式化数据，数据格式定义见数据手册
  * @param  sourceArray    元字节数组
  * @param  startIndex     开始索引
  * @param  topology       拓扑属性
  * @return: 0-正常返回
  * @update: [2018-05-25][张宇飞][BRIEF]
  *  [2018-05-31][张宇飞][增加状态量转换]
  *  [2018-06-19][张宇飞][修改判定]
  */
ErrorCode  ReserializeCollectTopology(const uint8_t* sourceArray, uint16_t startIndex, TopologyMessage** topology)
{
    uint8_t id1 = 0, id2 = 0, id3 = 0, id4 = 0;
    //计算累加和
    uint16_t len = COMBINE_UINT16( sourceArray[startIndex + 1],  sourceArray[startIndex +0]);
    if (len > 100)
    {
        return ERROR_LEN;
    }
    uint16_t sumCal = SumCheck(sourceArray, startIndex + 0, len-2 );
    uint16_t sumdata = COMBINE_UINT16( sourceArray[startIndex + len - 1],  sourceArray[startIndex + len - 2]);
    if (sumCal != sumdata)
    {
        return ERROR_PARAMTER;
    }
    *topology  = (TopologyMessage* )MALLOC(sizeof(TopologyMessage));

    if( *topology ==  NULL) //简单的指针检测
    {
        return ERROR_MALLOC;
    }
    MEMSET(*topology, 0, sizeof(TopologyMessage));
    
    
    id1 = sourceArray[startIndex + 2];
    id2 = sourceArray[startIndex + 3];
    id3 = sourceArray[startIndex + 4];
    id4 = sourceArray[startIndex + 5];
    (*topology)->id = COMBINE_UINT32(id4, id3, id2, id1);
    (*topology)->type = (TopologyType) sourceArray[startIndex + 6];
    (*topology)->switchNum = sourceArray[startIndex + 7];
    
    uint8_t switchNum =  (*topology)->switchNum;//开关数量
    
    
    SwitchProperty* switchCollect  = (SwitchProperty*)MALLOC(sizeof(SwitchProperty) * switchNum);
    
    if( switchCollect ==  NULL) //简单的指针检测
    {
        FREE(*topology ); //释放内存
        *topology = 0;
        return ERROR_MALLOC;
    }
     MEMSET(switchCollect, 0, sizeof(SwitchProperty) * switchNum);
    
    //循环获取开关属性
    uint16_t offset = 8 + startIndex;
    
    for(uint8_t i = 0; i < switchNum; i++)
    {
        id1 = sourceArray[offset++];
        id2 = sourceArray[offset++];
        id3 = sourceArray[offset++];
        id4 = sourceArray[offset++];
        switchCollect[i].id = COMBINE_UINT32(id4, id3, id2, id1);
        switchCollect[i].type = (SwitchType)(sourceArray[offset++]);
        switchCollect[i].state = (SwitchState)(sourceArray[offset++]);
        switchCollect[i].neighbourNum = sourceArray[offset++];
        switchCollect[i].neighbourCollect = (uint32_t*)MALLOC(sizeof(uint32_t) * switchCollect[i].neighbourNum);
        if(switchCollect[i].neighbourCollect ==  NULL) //简单的指针检测
        {
            //释放内存
            FREE((*topology)->switchCollect ); 
            (*topology)->switchCollect = 0;
            FREE(*topology ); 
            *topology = 0;
            return ERROR_NULL_PTR;
        }
         MEMSET(switchCollect[i].neighbourCollect, 0, sizeof(uint32_t) * switchCollect[i].neighbourNum);
        for (uint8_t k = 0; k < switchCollect[i].neighbourNum; k++)
        {
            id1 = sourceArray[offset++];
            id2 = sourceArray[offset++];
            id3 = sourceArray[offset++];
            id4 = sourceArray[offset++];
            switchCollect[i].neighbourCollect[k] = COMBINE_UINT32(id4, id3, id2, id1);
        }    
    }
    (*topology)->switchCollect = switchCollect;
   
    
    return ERROR_OK_NULL;
}

/**
  * @brief :计算拓扑结构信息序列化字节长度
  * @param  TopologyMessage* topology 拓扑信息
  * @return: 0-正常返回
    字节序	描述	        长度
    0	总长度不含校验	          2
    2	id	                  4
    6	TopologyType	      1
    7	开关数量	              1
    8	ID	                  4
        SwitchType            1        
        SwitchState	          1
        neighbourNum	      1
        neighbourCollect	 neighbourNum*4
        。。。。
        累加和	              2
  * @update: [2018-05-25][张宇飞][BRIEF]
  */
static ErrorCode CalTopologyMessageLength(const TopologyMessage* topology, uint16_t* len)
{
    *len =  2 + 4 + 1 + 1 ; //见上述注释
    *len  +=  topology->switchNum * 7; //开关公共数量
    for (uint8_t i = 0; i < topology->switchNum; i++)
    {
        *len  += topology->switchCollect[i].neighbourNum * 4;
    }
    *len  += 2; //2bytes 校验和
    return ERROR_OK_NULL;
}

/**
  * @brief :数据序列化，即将数组数据转换成格式化数据，数据格式定义见数据手册,有动态内存分配
  * @param  const TopologyMessage* topology 拓扑
  * @param  PointUint8* packet 打包数据
  * @param  uint16_t addLen  附加长度
  * @param  uint16_t offset  偏移
  * @return: ErrorCode
  * @update: [2018-05-25][张宇飞][BRIEF]
  *          [2018-05-25][张宇飞][序列化增加状态量]
  *          [2018-06-21][张宇飞][序列化增加状态量
  */
ErrorCode  SerializeCollectTopology(const TopologyMessage* topology, PointUint8* packet, uint16_t addLen, uint16_t offset)
{
    CHECK_POINT_RETURN(topology, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);



    uint16_t index = 0;
    uint32_t data = 0;
    uint16_t calLen = 0; //计算长度
    CalTopologyMessageLength(topology, &calLen);

    packet->len = addLen + calLen;
    packet->pData = (uint8_t*)MALLOC(sizeof(uint8_t) * packet->len);
    MEMSET(packet->pData, 0, sizeof(uint8_t) * packet->len);
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_MALLOC);
    
    uint8_t* sourceArray = packet->pData;
    index = offset;
   
    //长度
    sourceArray[index++] =   GET_N_BYTE(calLen, 0);
    sourceArray[index++] =   GET_N_BYTE(calLen, 1);
    //ID
    data = topology->id;
    sourceArray[index++] =   GET_N_BYTE(data, 0);
    sourceArray[index++] =   GET_N_BYTE(data, 1);
    sourceArray[index++] =   GET_N_BYTE(data, 2);
    sourceArray[index++] =   GET_N_BYTE(data, 3);
    
    sourceArray[index++] = topology->type;
    sourceArray[index++] = topology->switchNum; 

    for (uint8_t i = 0; i < topology->switchNum; i++)
    {
        data = topology->switchCollect[i].id;
        sourceArray[index++] =   GET_N_BYTE(data, 0);
        sourceArray[index++] =   GET_N_BYTE(data, 1);
        sourceArray[index++] =   GET_N_BYTE(data, 2);
        sourceArray[index++] =   GET_N_BYTE(data, 3);
        sourceArray[index++] = topology->switchCollect[i].type;
        sourceArray[index++] = topology->switchCollect[i].state;
        sourceArray[index++] = topology->switchCollect[i].neighbourNum;
        for (uint8_t k = 0; k < topology->switchCollect[i].neighbourNum; k++)
        {            
            data = topology->switchCollect[i].neighbourCollect[k];
            sourceArray[index++] =   GET_N_BYTE(data, 0);
            sourceArray[index++] =   GET_N_BYTE(data, 1);
            sourceArray[index++] =   GET_N_BYTE(data, 2);
            sourceArray[index++] =   GET_N_BYTE(data, 3);
        }               
    }
    //长度检测
    if ((index - offset) != calLen -2)
    {
        FREE(packet->pData);
        return ERROR_LEN;
    }
    //计算累加和
    data = SumCheck(sourceArray,0,  index);
    sourceArray[index++] =   GET_N_BYTE(data, 0);
    sourceArray[index++] =   GET_N_BYTE(data, 1);
        
    return ERROR_OK_NULL;
}
/**
  * @brief : 拓扑信息配置
  * @param  uint32_t         id; //由 4bytes IP组成的地, IP高位在前
  * @param  uint8_t          idArray[6]; //拓扑片ID号数组表示形式
  * @param  TopologyType     type;  //拓扑片类型
  * @param  SwitchProperty*  switchCollect;//开关集合
  * @return: 0-正常 非0错误
  * @update: [2018-05-25][张宇飞][todo：完善内存释放机制]
  */
uint8_t  TopologyMessageInit(
    TopologyMessage** topology,    
    uint32_t         id,
    TopologyType     type,    
    uint8_t*  topologyArray)
{
    //格式化输入数组，转化为结构数据
    uint8_t result = ReserializeCollectTopology(topologyArray, 0,  topology);
    if (result!= 0 )
    {
        return result;
    }
  
    return 0;
}

/**
 * @brief : 通过ID查找拓扑信息中的指定元素的上一个节点
 *          返回前一项。若没有或错误返回空。
 * @param  ：void
 * @return: 0-未找到 ，非0 指针
 * @update: [2018-05-26[张宇飞][]
 *  [2018-05-30[张宇飞][修正如果删除项为head，则无法获取想要删除的头一项]
 */
ListElment* FindNodeBeforeById(const  ListDouble* topology, uint32_t id)
{
    if(topology == NULL)
    {
        return NULL;
    }
    ListElment* element = list_head(topology); 
    if (element == NULL)
    {
        return NULL;
    }
    

    while(element->next != NULL)
    {
       
           if (((TopologyMessage*)(element->next->data))->id == id)
           {
               return element;
           }
        element = element->next;
        
    }

    return NULL;
}

/**
 * @brief : 由拓扑列表获取获取开关列表
 * @param ：const  ListDouble* listTopology  拓扑列表
 * @param ：ListDouble* listSwitch  开关列表
 * @return: 0-正常 非0错误
 * @update: [2018-06-06][张宇飞][]
 */
ErrorCode GetSwitchList(const  ListDouble* listTopology, ListDouble* listSwitch)
{
   
    
    if (listTopology == NULL)
    {
        perror("element is NULL!\n");
        return ERROR_NULL_PTR;
    }
    if (listSwitch == NULL)
    {
        perror("listSwitch is NULL!\n");
        return ERROR_NULL_PTR;
    }       

    uint8_t size  = list_size(listTopology);
    ListElment* element = list_head(listTopology); 
    for (uint8_t i = 0; i < size; i++)
    {        
        uint8_t num = GET_TOPOLOGY_ELEMENT(element)->switchNum;
        for (uint8_t k = 0; k < num; k++)
        {
            ListInsertNext(listSwitch, NULL, (GET_TOPOLOGY_ELEMENT(element)->switchCollect + k));
             
        }
        element = element->next;
    }

    return ERROR_OK_NULL;
}

/**
 * @brief : 通过ID，从拓扑链表中删除指定元素，并释放内存占用
 * @param  : ListDouble* topologyList  拓扑信息链表
 * @param  : uint32_t id         id
 * @update: [2018-6-01][张宇飞][创建]
 */
void DeleteTopologyListNodeById(ListDouble* topologyList, uint32_t id)
{
    
   ListElment *element;
   
   if (topologyList == NULL)
   {
       return;
   }
    
   TopologyMessage* tempTopology;
   if (((TopologyMessage*)(list_head(topologyList)->data))->id == id)
    {
        ListRemoveNext(topologyList, NULL, (void**)&tempTopology);//移除
        FreeTopologyMemory(&tempTopology);//释放
    }
    else 
    {    
    
        element = FindNodeBeforeById(topologyList, id);
        if (element)//若找到，先先删除
        {
             ListRemoveNext(topologyList, element, (void**)&tempTopology);//移除
             FreeTopologyMemory(&tempTopology);//释放
        }               
    } 
}
/**
 * @brief  : 添加邻居列表判断ID号，若不存在则添加。
 * @param  : uint8_t data[] 数据
 * @param  : uitn8_t len  数据长度
 * @param  : TopologyMessage* topolog
 * @return : ErrorCode
 * @update : [2018-05-29][张宇飞][创建]
 *           [2018-06-07][张宇飞][修改接口，增加为]
 *           [2018-06-21][张宇飞][改返回值为ErrorCode,添加形参检测]
 *           [2018-09-11][张宇飞][若已经存在，则不再添加]
 */

ErrorCode  AddTopologyMember(const uint8_t data[], uint8_t len,  ListDouble* topologyList)
{
    CHECK_POINT_RETURN(topologyList, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(data, NULL, ERROR_NULL_PTR);
    TopologyMessage* topologyMessage;
    uint8_t size = 0;
    ErrorCode error = ReserializeCollectTopology(data, 0, &topologyMessage);
    if (error == ERROR_OK_NULL)
    {       
        
        size = list_size(topologyList);
        //直接插入
        if (size == 0)
        {
            ListInsertNext(topologyList, NULL, topologyMessage);            
        }
        else
        {
        	TopologyMessage* find;
        	error = FindTopologyNodeByID(topologyList, topologyMessage->id, &find);
        	//不存在则插入
        	if (error == ERROR_UNFIND)
        	{
        		ListInsertNext(topologyList, NULL, topologyMessage);
                error = ERROR_OK_NULL;
        	}
        	else
        	{
        		if (error)
        		{
        			perror("FindTopologyNodeByID. error:0x%x.\n", error);
        		}

        		FreeTopologyMemory(&topologyMessage);
        	}
        }
     
    }
    else
    {
        perror("reserialize error, error code:{ %d }\n", error);
        
    }
    return error;
}

/**
 * @brief  : 添加邻居列表判断ID号，若不存在则添加。
 * @param  : ListDouble* topologyListSrc
 * @param  : ListDouble* topologyListDest
 * @return : ErrorCode
 * @update : [2018-10-25][李  磊][创建]
 */
ErrorCode  AddTopologyMemberByList(ListDouble* topologyListSrc, ListDouble* topologyListDest)
{
	uint8_t size = 0;
	ErrorCode error;
	TopologyMessage* find = NULL;
	
	size = list_size(topologyListDest);
	
	FOR_EARCH_LIST_START(topologyListSrc);
	 //直接插入
	if (size == 0)
	{
		ListInsertNext(topologyListDest, NULL, GET_TOPOLOGY_ELEMENT(m_foreach));
	}
	else
	{
		
		error = FindTopologyNodeByID(topologyListDest, GET_TOPOLOGY_ELEMENT(m_foreach)->id, &find);
		//不存在则插入
		if (error == ERROR_UNFIND)
		{
			ListInsertNext(topologyListDest, NULL, GET_TOPOLOGY_ELEMENT(m_foreach));
			error = ERROR_OK_NULL;
		}
		else
		{
			if (error)
			{
				perror("FindTopologyNodeByID. error:0x%x.\n", error);
			}
		}
		find = NULL;
	}
	FOR_EARCH_LIST_END();

	return error;
	
}

/**
* @brief : 将拓扑节点元素增加到列表
* @param  : TopologyMessage* topolog
* @param  : ListDouble* topologyList
* @return: ErrorCode
* @update: [2018-06-19][张宇飞][创建]
*         
*/
ErrorCode  AddMemberByTopology(TopologyMessage*  topologyMessage, ListDouble* topologyList)
{
    CHECK_POINT_RETURN(topologyMessage, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(topologyList, NULL, ERROR_NULL_PTR);

    uint8_t size = 0;    
    
    size = list_size(topologyList);
    //直接插入
    if (size == 0)
    {
        ListInsertNext(topologyList, NULL, topologyMessage);
    }
    else
    {

        DeleteTopologyListNodeById(topologyList, topologyMessage->id);
        ListInsertNext(topologyList, NULL, topologyMessage);//最后统一插入
    }
    return ERROR_OK_NULL;
}
/**
* @brief : 释放拓扑指针指向的内存区域，层层递推由内向外释放。
   使用前提条件:
  1)不能重复释放,
  2)内部指向switchCollect，neighbourCollect，均为同一动态分配原始指向，未提前释放。
 * @param  : TopologyMessage** topolog
 * @return: 0-正常 非0错误
 * @update: [2018-05-29][张宇飞][]
 */
void FreeTopologyMemory(TopologyMessage** topology)
{
   
    //rt_uint32_t total;
    //rt_uint32_t used;
    //rt_uint32_t maxused;
    //rt_memory_info(&total, &used, &maxused);
    //rt_kprintf("FreeTopologyMemory before memory total: %d, used:%d, maxused: %d\n", total, used, maxused);
    if (*topology == NULL)
    {
        rt_kprintf("topology is null, cann't relese!\n");
        return;
    }
    if ( (*topology)->switchCollect != NULL)
    {
        if((*topology)->switchCollect->neighbourCollect != NULL)
        {
            FREE((*topology)->switchCollect->neighbourCollect);
            (*topology)->switchCollect->neighbourCollect = 0;
            FREE((*topology)->switchCollect);           
            (*topology)->switchCollect = 0;           
            FREE(*topology);
            *topology = 0;          
        }
        else
        {
            FREE((*topology)->switchCollect);
            (*topology)->switchCollect = 0;
            FREE(*topology);
            *topology = 0;
        }
    }
    else
    {
        FREE(*topology);
        *topology = 0;
    }
    //rt_memory_info(&total, &used, &maxused);    
    //rt_kprintf("FreeTopologyMemory after memory total: %d, used:%d, maxused: %d\n", total, used, maxused);
}
/**
 * @brief : 释放Helper 结构数据
 * @param ：
 * @return: 0-正常 非0错误
 * @update: [2018-06-06][张宇飞][]
 */
void FreeBFSHelper(BFSHelper** helper)
{
    if (*helper != NULL)
    {
        if ((*helper)->edgeTo != NULL)
        {
            FREE((*helper)->edgeTo );
            (*helper)->edgeTo = NULL;
        }
        if ((*helper)->marked != NULL)
        {
            FREE((*helper)->marked );
            (*helper)->marked = NULL;
        }
        if ((*helper)->idArray != NULL)
        {
            FREE((*helper)->idArray );
            (*helper)->idArray  = NULL;
        }
        if ((*helper)->path != NULL)
        {            
            uint8_t len = list_size((*helper)->path);
            uint8_t *data;
            for (uint8_t i = 0; i < len; i++)
            {
                ListRemoveNext((*helper)->path, NULL, (void**)&data);//移除
                FREE(data);          
            } 
            
            FREE((*helper)->path);
        }
		
        FREE(*helper);		
        *helper = NULL; //TODO:释放链表
    }
}
/**
* @brief : 用于链表释放
* @param ：
* @return: 0-正常 非0错误
* @update: [2018-07-4][张宇飞][]
*[2018-07-24][张宇飞][BFSHelper* helper 改为 void*]
*/
void DestoryBFSHelper(void* pHelper)
{
    BFSHelper* helper = (BFSHelper*)pHelper;
    FreeBFSHelper(&helper);
}

/**
 * @brief : 搜索索引，通过ID
 * @param : 
 * @return: 
 * @update: [2018-06-06][张宇飞][]
 */
static ErrorCode SearchIndexByID(uint32_t* data, uint8_t len, uint32_t id, uint8_t* index)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (data[i] == id)
        {
            *index = i;
            return ERROR_OK_NULL;
        }
    }
    
    
    return ERROR_UNFIND;
}


/**
 * @brief : 广度优先搜索
 * @param  : 
 * @return: 
 * @update: [2018-06-06][张宇飞][]
 */
ErrorCode BreadthFirstPath(const ListDouble* switchList, const SwitchProperty* start, BFSHelper** helper)
{
    ErrorCode result = ERROR_OK_NULL;
    if (switchList == NULL)
    {
        return ERROR_NULL_PTR;
    }
    if (start == NULL)
    {
        return ERROR_NULL_PTR;
    }
  
    //生成ID数组
    uint8_t size = list_size(switchList);
    if (size == 0)
    {
        return ERROR_PARAMTER;
    } 
    *helper = (BFSHelper*)MALLOC(sizeof(BFSHelper));
    if (*helper == NULL)
    {
        return ERROR_MALLOC;
    }
    MEMSET(*helper, 0, sizeof(BFSHelper));
    uint32_t* idArray = (uint32_t*)MALLOC(size * sizeof(uint32_t)); //标记数组
    if (idArray == NULL)
    {
        FREE(*helper);
         return ERROR_MALLOC;
    }
    uint8_t* edgeTo = (uint8_t*)MALLOC(size * sizeof(uint8_t)); //标记数组
    if (edgeTo == NULL)
    {
        FREE(*helper);
        FREE(idArray);
         return ERROR_MALLOC;
    } 
    uint8_t* marked = (uint8_t*)MALLOC(size * sizeof(uint8_t)); //标记数组
    if (marked == NULL)
    {
        FREE(*helper);
        FREE(idArray);
        FREE(edgeTo);
         return ERROR_MALLOC;
    }
    (*helper)->path = (ListDouble*)MALLOC(sizeof(ListDouble));
    if ((*helper)->path == NULL)
    {
        FREE(*helper);
        FREE(idArray);
        FREE(edgeTo);
        FREE((*helper)->path);
         return ERROR_MALLOC;
    }
    ListInit((*helper)->path, NULL);
    
    (*helper)->size = size;
    (*helper)->idArray = idArray;
    (*helper)->edgeTo = edgeTo;
    (*helper)->marked = marked;    

    ListElment* element = list_head(switchList); 
        
    for (uint8_t i = 0; i < size; i++)
    {
        idArray[i] = GET_SWITCH_ELEMENT(element)->id;  
        marked[i] = 0; 
        edgeTo[i] = 0;  
        element = element->next;
    }
    FifoHandle queue;

#ifdef  MSVC  //  MSVC
	uint8_t queueBuffer[32];//TODO:注意MDK此处为size
#else
	uint8_t queueBuffer[32];//TODO:注意MDK此处为size
#endif

    
    PointUint8 queuePack;
    
    queuePack.len = size;
    queuePack.pData = queueBuffer;
    FifoInit(&queue, &queuePack);
    
    uint8_t s;// 起始索引
    uint8_t v;
    uint8_t w;
    result = SearchIndexByID(idArray, size, start->id, &s);
    if (result)
    {
        return result;
    }
    queue.Enqueue(&queue, s); //添加首节点
    marked[s] = 0xff;
    while(queue.fifo.count)
    {
        queue.Dequeue(&queue, &v); 
        SwitchProperty* node;
        result = FindSwitchNodeByID(switchList, idArray[v], &node);
        if (!result)
        {
            for (uint8_t k = 0; k < node->neighbourNum; k++)
            {
                result = SearchIndexByID(idArray, size, node->neighbourCollect[k], &w);
                if (result)
                {
                    continue;
                }
                if (! marked[w])
                {
                     edgeTo[w] = v;
                     marked[w] = 0xff;
                     queue.Enqueue(&queue, w); //添加首节点
                }               
                
            }
        }
    }
      
    return ERROR_OK_NULL;    
        
}
/**
 * @brief : 查找指定两点之间的路径,
 * @param  : 
 * @return: 
 * @update: [2018-06-06][张宇飞][]
 */
ErrorCode PathTo(const ListDouble* switchList, const BFSHelper* helper, const SwitchProperty* start,  const SwitchProperty* end)  
{
    ErrorCode result;
    uint8_t s , v;
    result = SearchIndexByID(helper->idArray, helper->size, start->id, &s);
    if (result)
    {
        return result;
    }
    
    result = SearchIndexByID(helper->idArray, helper->size, end->id, &v);
    if (result)
    {
        return result;
    }
    
    if (! helper->marked[v])
    {
        return ERROR_UNFIND;
    }        
    
    
    
    //TODO：内存分配失败释放
    for (uint8_t x = v; x != s; x = helper->edgeTo[x])
    {
        uint8_t *data =(uint8_t*)MALLOC(sizeof(uint8_t));
        if (data == NULL)
        {
             return ERROR_MALLOC;
        }
        *data = x;
        ListInsertNext(helper->path, NULL,  data);//添加当前开关
    }
    uint8_t *data =(uint8_t*)MALLOC(sizeof(uint8_t));
    if (data == NULL)
    {
         return ERROR_MALLOC;
    }
    *data = s;
    ListInsertNext(helper->path, NULL,  data);//添加当前开关
       
        
    return ERROR_OK_NULL;
    
  
    
}




/**
 * @brief : 查找指定两点之间的路径,
 * @param : 
 * @return: 
 * @update: [2018-06-06][张宇飞][]
 */
ErrorCode FindPath(const ListDouble* switchList, const SwitchProperty* start, const SwitchProperty* end, BFSHelper** helper)
{
   ErrorCode result;
   result = BreadthFirstPath(switchList, start,   helper);
    if ( result  )
    {
        //rt_kprintf("BreadthFirstPath ERROR: %d\n", result);
        return result;
    } 
    result = PathTo(switchList, *helper, start, end);
    if ( result  )
    {
        //rt_kprintf("PathTo ERROR: %d\n", result);
        return result;
    }

    return ERROR_OK_NULL;
}

#ifdef RTT_P2P
//消除错误
void _ttywrch(int ch)
{
}
#endif






