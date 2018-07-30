/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      distribution_helper.c
  * @brief:     相关测试用例
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    将打印输出统一于此文件下
  */

#include "distribution_helper.h"
#include "distribution_test_case.h" 
#include "distribution_interface.h"   
#include "topology.h" 
#include "helper.h"
#include "fault_removal.h"  
#include "extern_interface.h"
//#include "core_cm4.h"  




void PrintPowerAreaMessage(ListDouble* areaList, uint8_t len)
{
    if (areaList == NULL)
    {
        rt_kprintf("DistributionArea areaList is NULL.\n");
        return;
    }
    rt_kprintf("\n");
    rt_kprintf("Area out Start.\n");
    for (uint8_t i = 0; i< len ;i++)
    {
        if (areaList+i != NULL)
        {
            rt_kprintf("Area: %d\n", i );
             uint8_t lenArea = list_size(areaList+i);
             ListElment* areaElement = list_head(areaList+i);
             for (uint8_t k = 0; k < lenArea; k++)
             {
                 if (areaElement != NULL)
                 {
                     rt_kprintf("0x%X\n", GET_SWITCH_ELEMENT(areaElement)->id);
                     areaElement = areaElement->next;
                 }
             }
        }
    }
    
    rt_kprintf("Area out finished.\n\n");
}
/**
* @brief : 输出拓扑信息
 * @param  : TopologyMessage* topolog
 * @update: [2018-05-30][张宇飞][DEBUG_TOPOLOGY 控制是否编译]
 */

void PrintTopologyMessage(const TopologyMessage* topologyMessage)
{
#ifndef SIMPLE_PRINT
    uint32_t id = topologyMessage->id;
    rt_kprintf("toplogy id: 0x%x, ip: %d:%d:%d:%d\n", id, GET_N_BYTE(id, 3), GET_N_BYTE(id, 2), GET_N_BYTE(id, 1), GET_N_BYTE(id, 0));
    rt_kprintf("toplogy type: 0x%x\n", topologyMessage->type);
    rt_kprintf("toplogy switch num: 0x%x\n", topologyMessage->switchNum);
    for(uint8_t i = 0; i <  topologyMessage->switchNum; i++)
    {
        id = (topologyMessage->switchCollect[i]).id;
        rt_kprintf("switch %d id:0x%x ip: %d:%d:%d:%d\n",i, id, GET_N_BYTE(id, 3), GET_N_BYTE(id, 2), GET_N_BYTE(id,1), GET_N_BYTE(id, 0));
        rt_kprintf("neighrbour num %d \n", (topologyMessage->switchCollect[i]).neighbourNum);
        for (uint8_t k = 0; k < (topologyMessage->switchCollect[i]).neighbourNum; k++)
        {
            id = (topologyMessage->switchCollect[i]).neighbourCollect[k];
            rt_kprintf("neighrbour %d id:0x%x, ip: %d:%d:%d:%d\n",k, id, GET_N_BYTE(id, 3), GET_N_BYTE(id, 2), GET_N_BYTE(id, 1), GET_N_BYTE(id, 0));
        }
    }  
#else
	uint32_t id = topologyMessage->id;
	rt_kprintf("toplogy id: 0x%x, type: 0x%x, switch count: %d\n", id,  topologyMessage->type,  topologyMessage->switchNum);
		
	for (uint8_t i = 0; i < topologyMessage->switchNum; i++)
	{
		id = (topologyMessage->switchCollect[i]).id;
		rt_kprintf("%d, switch id:0x%x, ", i, id);
		rt_kprintf("neighbour count: %d, ", (topologyMessage->switchCollect[i]).neighbourNum);
		for (uint8_t k = 0; k < (topologyMessage->switchCollect[i]).neighbourNum; k++)
		{
			id = (topologyMessage->switchCollect[i]).neighbourCollect[k];
			rt_kprintf("%d-0x%x ", k, id);
		}
		rt_kprintf("\n\n");
	}


#endif
}
/**
 * @brief : 输出开关信息
 * @param  :SwitchProperty*  switchNode
 * @update: [2018-06-07][张宇飞][输出开关信息]
 */
void PrintSwitchMessage(const SwitchProperty*  switchNode)
{  
#ifndef SIMPLE_PRINT
    uint32_t id = switchNode->id;

    id = switchNode->id;
    rt_kprintf("switch id:0x%x ip: %d:%d:%d:%d\n", id, GET_N_BYTE(id, 3), GET_N_BYTE(id, 2), GET_N_BYTE(id,1), GET_N_BYTE(id, 0));
    rt_kprintf("neighrbour count: %d \n", switchNode->neighbourNum);
    for (uint8_t k = 0; k < switchNode->neighbourNum; k++)
    {
        id = switchNode->neighbourCollect[k];
        rt_kprintf("neighrbour %d id:0x%x, ip: %d:%d:%d:%d\n",k, id, GET_N_BYTE(id, 3), GET_N_BYTE(id, 2), GET_N_BYTE(id, 1), GET_N_BYTE(id, 0));
    }
#else
	uint32_t id = switchNode->id;

	id = switchNode->id;
	rt_kprintf("switch id:0x%x", id);
	rt_kprintf("neighrbour count: %d ,", switchNode->neighbourNum);
	for (uint8_t k = 0; k < switchNode->neighbourNum; k++)
	{
		id = switchNode->neighbourCollect[k];
		rt_kprintf("%d-0x%x,", k, id);
	}
	rt_kprintf("\n");


#endif

}
/**
 * @brief : 输出辅助类信息
 * @param  : TopologyMessage* topolog
 * @update: [2018-05-30][张宇飞][DEBUG_TOPOLOGY 控制是否编译]
 */
void PrintBFSHelper(const BFSHelper* helper)
{
    if (helper != NULL)
    {
        rt_kprintf("helper output start.\n");
        rt_kprintf("helper size : %d.\n", helper->size);
        
        rt_kprintf("helper idArray.\n");
        for (uint8_t i = 0; i < helper->size; i++)
        {
            rt_kprintf("%d, %X\n",i,  helper->idArray[i]);
        }
        
        rt_kprintf("helper edgeTo.\n");
        for (uint8_t i = 0; i < helper->size; i++)
        {
            rt_kprintf("%d, %X\n",i,  helper->edgeTo[i]);
        }
         rt_kprintf("helper marked.\n");
        for (uint8_t i = 0; i < helper->size; i++)
        {
            rt_kprintf("%d, %X\n",i,  helper->marked[i]);
        }
        
        rt_kprintf("helper path.\n");
        ListElment* element = list_head(helper->path);
        for (uint8_t i = 0; i < list_size(helper->path); i++)
        {
            
                         
            rt_kprintf("%d, %X\n",i,   *(uint8_t* )(element->data));
            element = element->next;
        }       
        
        rt_kprintf("helper output end.\n");
    }
}

/**
 * @brief : 输出辅助类信息
 * @param  : TopologyMessage* topolog
 * @update: [2018-05-30][张宇飞][DEBUG_TOPOLOGY 控制是否编译]
 */
void PrintBFSHelperSimple(const BFSHelper* helper)
{   
    if (helper != NULL)
    {
        rt_kprintf("helper size : %d.\n", helper->size);
        for (uint8_t i = 0; i < helper->size; i++)
        {
            rt_kprintf("%X,", helper->idArray[i]);
        }
        rt_kprintf("\n");
        
        rt_kprintf("helper edgeTo:\n");
        for (uint8_t i = 0; i < helper->size; i++)
        {
            rt_kprintf("%d,", helper->edgeTo[i]);
        }       
        rt_kprintf("\n");
        
        rt_kprintf("helper path:\n");
        ListElment* element = list_head(helper->path);
        for (uint8_t i = 0; i < list_size(helper->path); i++)
        {       
            rt_kprintf("%d,",  *(uint8_t* )(element->data));
            element = element->next;
        }         
        rt_kprintf("\n");
    }
    PrintMemoryUsed();
}

void PrintMemoryUsed(void)
{
    rt_uint32_t total;
    rt_uint32_t used;
    rt_uint32_t maxused;
    rt_memory_info(&total, &used, &maxused);
    rt_kprintf("memory total: %d, used:%d, maxused: %d\n", total, used, maxused);
}

/**
* @brief : 输出ID，TICK，MESSAGE
* @param  : uint32_t id
* @param  : uint8_t* tips 
* @update: [2018-06-23][张宇飞][创建]
*/
void PrintIDTipsTick(uint32_t id, uint8_t* tips)
{
    rt_kprintf("%X,", id);
    rt_kprintf((const char*)tips);
    rt_kprintf(",TICK:%6d.\n", rt_tick_get());

}


/**
* @brief : 打印开关链表
* @param  : ListDouble* list
* @update: [2018-07-05][张宇飞][创建]
*/
void PrintSwitchList(ListDouble* list)
{
	rt_kprintf("power->");
	FOR_EARCH_LIST_START(list);

	rt_kprintf("%X->", GET_SWITCH_ELEMENT(m_foreach)->id);

	FOR_EARCH_LIST_END();
	rt_kprintf("connected switch.");

    rt_kprintf("\n");
}
