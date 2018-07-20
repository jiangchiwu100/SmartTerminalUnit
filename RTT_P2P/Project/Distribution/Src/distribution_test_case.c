/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_test_case.c
  * @brief:     相关测试用例
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-07
  * @update:    
  */
 
#include "distribution_test_case.h" 
#include "distribution.h"

/**
*查找结果保存
*/
BFSHelper* BFSResult;

/**
*本节点拓扑信息定义
*/
TopologyMessage*  LocalTopologyMessage;

/**
*邻居节点拓扑信息定义
*/
TopologyMessage*  NeighbourTopologyMessage;

/**
*全局节点拓扑信息链表形式保存
*/
List GlobalTopologyList;
/**
*邻居节点开关节点保存
*/
List NeighbourSwitchList;

/**
*全局开关节点保存
*/
List GlobalSwitchList;


/**
*配电区域定义, 实际上一个开关最多有两个配电区域。
*/
List PowerArea[POWER_AREA_NUM];


/**
*目的开关节点
*/
TopologyMessage*  AimTopologyMessage;



/**
 * @brief : 拓扑数据 初始化
 * @param  ：void
 * @return: 0-正常 非0错误
 * @update: [2018-05-26[张宇飞][]
 */
uint8_t ListDataInit(void)
{
    
    list_init(&GlobalTopologyList, NULL); //初始化列表合计
    
    list_init(&NeighbourSwitchList, NULL);// 初始化开关集合
    
    return 0;
}
void TopologyTest(uint8_t data[], uint8_t len)
{    
    AddTopologyMember(data, len, &GlobalTopologyList);
    PrintTopologyMessage(GET_TOPOLOGY_ELEMENT(list_head(&GlobalTopologyList)));
}

/**
 * @brief :设置本地信息
 * @param : uint8_t data[] 序列化数据
 * @len   : uint8_t len 数据长度
 * @return: void
 * @update: [2018-05-6-1][张宇飞][]
 */
void SetLocalMessage(uint8_t data[], uint8_t len)
{
    if  (4 == len)
    {
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (GlobalTopologyList.size !=0)
        {
            uint8_t result = FindTopologyNodeByID(&GlobalTopologyList, id, &LocalTopologyMessage);
            if (!result)
            {
                rt_kprintf("Local Topology:\n");
                PrintTopologyMessage(LocalTopologyMessage);
            }
            else
            {
                rt_kprintf("Local Topology EOOR: 0x%X:\n", result);
            }
        }
        else
        {
            rt_kprintf("GlobalTopologyList Topology is 0.\n");
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
 * @return: void
 * @update: [2018-05-6-1][张宇飞][]
 */
void SetAimMessage(uint8_t data[], uint8_t len)
{
    if  (4 == len)
    {
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (GlobalTopologyList.size !=0)
        {
            FindTopologyNodeByID(&GlobalTopologyList, id, &AimTopologyMessage);
            rt_kprintf("Aim Topology:\n");
            PrintTopologyMessage(AimTopologyMessage);
        }
        else
        {
            rt_kprintf("GlobalTopologyList Topology is 0.\n");
        }
    }
    else
    {
        rt_kprintf("Data length is  wrong!\n");
    }
}

/**
 * @brief :  配电区域计算测试
 * @param  : TopologyMessage** topolog
 * @return: 0-正常 非0错误
 * @update: [2018-05-30][张宇飞][]
 */
void CalTest(void)
{
    uint8_t result = 0;
    if (GlobalTopologyList.size < 2)
    {
        rt_kprintf("GlobalTopologyList.size is too small!\n");
        return ;
    }
    
    if (NeighbourSwitchList.size !=0)
    {
        //销毁链表
        list_destroy(&NeighbourSwitchList);
        //重新初始化
        list_init(&NeighbourSwitchList, NULL);
    }
    
    //使用头作为计算节点
    if (LocalTopologyMessage == NULL)
    {        
        LocalTopologyMessage = GET_TOPOLOGY_ELEMENT(list_head(&GlobalTopologyList));
    }
    rt_kprintf("\n Local Node Message:\n");
    PrintTopologyMessage(LocalTopologyMessage); 
    
    rt_kprintf(" Neighbour Node Message:\n");
    
    GetNeighboorSwitch(&NeighbourSwitchList, LocalTopologyMessage, &GlobalTopologyList, &NeighbourSwitchList);
    
    ListElmt* element = list_head(&NeighbourSwitchList);
    
    for(uint8_t i = 0; i < list_size(&NeighbourSwitchList); i++)
    {
        PrintSwitchMessage(GET_SWITCH_ELEMENT(element));
        element = element->next;
    }
    
    
    
   //释放内存 并重新初始化
   FreeAreaList(PowerArea, 2);       
   
    
   result = GetPowerDistributionArea(LocalTopologyMessage, &NeighbourSwitchList, PowerArea);
   if (result) 
   {
        rt_kprintf("GetPowerDistributionArea error, error code:{ %d }\n", result);
   }
   else
   {
        rt_kprintf("GetPowerDistributionArea success.\n");
        PrintPowerAreaMessage(PowerArea, 2);
   }
}


/**
 * @brief :  路径搜索计算测试
 * @param  : TopologyMessage** topolog
 * @return: 0-正常 非0错误
 * @update: [2018-05-30][张宇飞][]
 */

void TestPath(void)
{
    uint8_t result = 0;
    if (GlobalTopologyList.size < 2)
    {
        rt_kprintf("GlobalTopologyList.size is too small!\n");
        return ;
    }
    
    if (GlobalSwitchList.size !=0)
    {
        //销毁链表
        list_destroy(&GlobalSwitchList);
        //重新初始化
        list_init(&GlobalSwitchList, NULL);
    }
    
    
    result = GetSwitchList(&GlobalTopologyList, &GlobalSwitchList);
    if (result)
    {
        rt_kprintf("GetSwitchList Error: %d\n", result);
        return ;
    }
    
    if (GlobalSwitchList.size < 2)
    {
        rt_kprintf("GetSwitchList size is too small: %d\n", GlobalSwitchList.size);
        return ;
    }
    
    //使用头作为计算节点
    if (LocalTopologyMessage == NULL)
    {        
        LocalTopologyMessage = GET_TOPOLOGY_ELEMENT(list_head(&GlobalSwitchList));
    }
    if (AimTopologyMessage == NULL)
    {        
        AimTopologyMessage = GET_TOPOLOGY_ELEMENT(list_tail(&GlobalSwitchList));
    }
    
    
    if (BFSResult->size != 0)
    {
        FreeBFSHelper(&BFSResult);
    }
    
    result = FindPath(&GlobalSwitchList, LocalTopologyMessage->switchCollect, AimTopologyMessage->switchCollect, &BFSResult);
    if ( result  )
    {
        rt_kprintf("FindPath ERROR: %d\n", result);
        return ;
    }
   // PrintBFSHelper(BFSResult);
    PrintBFSHelperSimple(BFSResult);
}

void TestPathAll(void)
{
    
    ListElmt* element = list_head(&GlobalTopologyList);
    uint8_t size = list_size(&GlobalTopologyList);
    for (uint8_t i = 0; i < size; i++)
    {
        LocalTopologyMessage = GET_TOPOLOGY_ELEMENT(element);
        
        ListElmt* node = list_head(&GlobalTopologyList);
       
        for (uint8_t k = 0; k < size; k++)
        {
            AimTopologyMessage = GET_TOPOLOGY_ELEMENT(node);    
            rt_kprintf("\n");   
            rt_kprintf("index, %d, %d, Local, %X, Aim, %X,\n", i, k,  LocalTopologyMessage->switchCollect->id, AimTopologyMessage->switchCollect->id);            
            node = node->next;
            
            TestPath();
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
void DeleteTopologyNode(uint8_t data[], uint8_t len)
{
    if  (4 == len)
    {
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        if (LocalTopologyMessage->id == id)
        {
            LocalTopologyMessage = 0;
             rt_kprintf("node delete.");
        }
        DeleteTopologyListNodeById(&NeighbourSwitchList, id);
        
    }
    else
    {
        rt_kprintf("Data length is  wrong!");
    }
}



/**
 * @brief : 增加StationPoint
 * @param : 
 * @return: 0-正常 非0错误
 * @update: [2018-06-11][张宇飞][]
 */
void AddStationPoint(uint8_t data[], uint8_t len)
{    
    if  (4 == len)
    {
        uint32_t id = COMBINE_UINT32(data[3], data[2], data[1], data[0]);
        StationMangerAddMember(&g_StationManger, id);
        
    }
    else
    {
        rt_kprintf("Data length is  wrong!");
    }
   
    
}

#ifdef _DEBUG_TEST_RING
#include "RingQueue.h"
uint16_t test_data[2000];
void RingQueueTest()
{
    RingQueue ring;
    uint16_t cn = 0;
    bool state = RingQueueInit(&ring, 1000);
    if (!state)
    {
        rt_kprintf("RingQueueInit Init.\n");
    }
    uint16_t* pdata;
    for (uint16_t i = 0; i< 2000; i++)
    {
        test_data[i] = i;
        state = ring.Write(&ring, test_data + i);
        if (!state)
        {
            rt_kprintf("ring.Write read.\n");
        }
        
        
    }
   
    for (uint16_t i = 0; i < 500; i++)
    {
        ring.Read(&ring, &pdata);
        if (*pdata != i + 1000)
        {
            rt_kprintf("ring.Read read.\n");
            break;
        }
    }
    for (uint16_t i = 0; i < 2000; i++)
    {
        test_data[i] = i;
        state = ring.Write(&ring, test_data + i);
        if (!state)
        {
            rt_kprintf("ring.Write read.\n");
        }


    }
}
#endif


#ifdef TEST_LIST_CASE
void TestListPrevCase(void)
{
    PrintMemoryUsed();
    List listTest;
    List* list = &listTest;

    list_init(list, FREE);

    for (uint8_t i = 0; i < 100; i++)
    {
        uint32_t* pid = CALLOC(1, sizeof(uint32_t));
        *pid = i;
        list_ins_next(list, NULL, pid);
    }

    FOR_EARCH_LIST_START(list);
    rt_kprintf("%d,", *(uint32_t*)(m_foreach->data));
    FOR_EARCH_LIST_END();
    rt_kprintf("\n");
    FOR_EARCH_LIST_TAIL_START(list);
    rt_kprintf("%d,", *(uint32_t*)(m_foreach->data));
    FOR_EARCH_LIST_TAIL_END();
    rt_kprintf("\n");

    FOR_EARCH_LIST_TAIL_START(list);
    if (i % 2)
    {
        uint32_t* data;
        list_rem_next(list, m_foreach->prev, &data);
        FREE(data);
    }
    FOR_EARCH_LIST_TAIL_END();

    FOR_EARCH_LIST_TAIL_START(list);
    rt_kprintf("%d,", *(uint32_t*)(m_foreach->data));
    FOR_EARCH_LIST_TAIL_END();
    rt_kprintf("\n");

    FOR_EARCH_LIST_TAIL_START(list);
    if ((i + 1) % 2)
    {
        uint32_t* data;
        list_rem_next(list, m_foreach->prev, &data);
        FREE(data);
    }
    FOR_EARCH_LIST_TAIL_END();


    FOR_EARCH_LIST_TAIL_START(list);
    rt_kprintf("%d,", *(uint32_t*)(m_foreach->data));
    FOR_EARCH_LIST_TAIL_END();
    rt_kprintf("\n");

    list_destroy(list);
    PrintMemoryUsed();

}
#endif