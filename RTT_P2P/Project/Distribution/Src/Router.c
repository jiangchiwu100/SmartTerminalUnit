/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      Router.c
  * @brief:     路由器
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-06-09
  * @update:    
  */
  
#include "fault_treatment.h"   
#include "topology.h" 
#include "helper.h"
#include "router.h"
#include "extern_interface.h"


static uint8_t  RemoveNodeFifo(NodeFifo** node);
static void  DestroyNodeFifoList(void* node);
static uint8_t RouterSendData(FifoHandle* handle, PointUint8* packet);
/**
 * @brief : 通过ID查找FIFO node信息中的指定元素
 * @param  ：void
 * @return: 0-未找到 ，非0 指针
 * @update:  [2018-06-08[张宇飞][]
 */
NodeFifo* FindRouteNodeById(const  ListDouble* list, uint32_t id)
{
    if(list == NULL)
    {
        return NULL;
    }
    
    uint8_t size = list_size(list);
     ListElment* element = list_head(list); 
    for (uint8_t i = 0; i < size; i++)
    {
        if (((NodeFifo*)(element->data))->id == id)
           {
               return (NodeFifo*)(element->data);
           }
        element = element->next;
    }
    return NULL;
}

/**
 * @brief  : 初始化节点
 * @param  : 
 * @param  ：
 * @return: 0-正常，非空--未找到或异常 
 * @update: [2018-06-08][张宇飞][]
 */
ErrorCode RouteInit(Router* router)
{
    ListInit(&(router->nodeStation), DestroyNodeFifoList );
    router->FindMemberById = FindRouteNodeById;
    return ERROR_OK_NULL;
}


/**
 * @brief  : 新建节点
 * @param  : 
 * @param  ：
 * @return: 0-正常，非空--未找到或异常 
 * @update: [2018-06-08][张宇飞][]
 * [2018-06-11][张宇飞][修改遗漏初始化ID错误]
 * [2018-06-12][张宇飞][修改错误的释放,赋值错误]
 */
uint8_t  BuildNodeFifo(uint32_t id, uint16_t capacity, NodeFifo** node)
{
  
    *node= (NodeFifo*)MALLOC(sizeof(NodeFifo));

    if (*node == NULL)
    {
        return ERROR_MALLOC;
    }
    MEMSET(*node, 0, sizeof(NodeFifo));    
    
    PointUint8 nodeRecive;
    nodeRecive.pData = (uint8_t*)MALLOC(sizeof(uint8_t) * capacity);
    if (nodeRecive.pData == NULL)
    {
        FREE(*node );
        return ERROR_MALLOC;
    }
    MEMSET(nodeRecive.pData, 0, sizeof(uint8_t) * capacity);    
    
    nodeRecive.len = capacity;
    FifoInit(&((*node)->reciveHandle), &nodeRecive);
    
    PointUint8 nodeSend;
    nodeSend.pData = (uint8_t*)MALLOC(sizeof(uint8_t) * capacity);
    if (nodeSend.pData == NULL)
    {
        FREE(nodeRecive.pData);
        FREE(*node );
        return ERROR_MALLOC;
    }
    MEMSET(nodeSend.pData, 0, sizeof(uint8_t) * capacity); 
    
    nodeSend.len = capacity;
    FifoInit(&((*node)->sendHandle), &nodeSend);
    (*node)->id = id;  
    return 0;
}

/**
 * @brief : 删除节点释放内存
 * @param : void* node  （NodeFifo*）
 * @return: 空
 * @update: [2018-06-08][张宇飞][]
 */
static void  DestroyNodeFifoList(void* node)
{
    RemoveNodeFifo((NodeFifo**) &node);
}

/**
 * @brief  : 删除节点释放内存
 * @param  : 
 * @param  ：
 * @return: 0-正常，非空--未找到或异常 
 * @update: [2018-06-08][张宇飞][]
 */
static uint8_t  RemoveNodeFifo(NodeFifo** node)
{
    if (node == 0  || *node == 0)
    {
        return ERROR_NULL_PTR;
    }
    if ((*node)->reciveHandle.fifo.pData != 0)
    {
        FREE((*node)->reciveHandle.fifo.pData);
        (*node)->reciveHandle.fifo.pData = 0;
    }
    if ((*node)->sendHandle.fifo.pData != 0)
    {
        FREE((*node)->sendHandle.fifo.pData);
        (*node)->sendHandle.fifo.pData = 0;
    }
    FREE(*node);
    *node = 0;
    
    return 0;
}

/**
* @brief  : 路由器发送数据
* @param  : 队列句柄
* @param  ：PointUint8* packet 报数据
* @return: 0-正常，非空--异常
* @update: [2018-06-14][张宇飞][]
*/
static uint8_t RouterSendData(FifoHandle* handle, PointUint8* packet)
{
    CHECK_POINT_RETURN(handle, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet, NULL, ERROR_NULL_PTR);
    CHECK_POINT_RETURN(packet->pData, NULL, ERROR_NULL_PTR);

    if (handle->fifo.capacity - handle->fifo.count < packet->len)
    {
        return ERROR_VOLUM;
    }

    for (uint8_t i = 0; i < packet->len; i++)
    {
        handle->Enqueue(handle, packet->pData[i]);
    }
    return ERROR_OK_NULL;
}

/**
 * @brief  : 增加节点
 * @param  : Router* router 路由器
 * @param  ：id 添加的ID
 * @param  ：capacity 容量
 * @return: 0-正常，非空--未找到或异常 
 * @update: [2018-06-08][张宇飞][]
 */
ErrorCode  AddNodeFifo(Router* router, uint32_t id, uint16_t capacity, NodeFifo** addNde)
{
    NodeFifo* node;
    if (router == NULL)
    {
        return ERROR_NULL_PTR;
    }
    if (capacity == 0)
    {
        return ERROR_PARAMTER;
    }   
        
    node = FindRouteNodeById(&(router->nodeStation), id);
    //没有找到，即新建
    if (node == NULL)
    {        
        uint8_t result = BuildNodeFifo(id, capacity, &node);
        if (result)
        {
            return (ErrorCode)result;
        }
        result = ListInsertNext(&(router->nodeStation), NULL, node);
        if (result)
        {
            return ERROR_INSERT_LIST;
        }
    }
    else
    {
        //找到，若容量相等就修改，否则就修改容量
       
        if ( node->reciveHandle.fifo.capacity != capacity)   //容量不相等则重新分配
        {
           
            node->reciveHandle.fifo.capacity = capacity;
            node->reciveHandle.fifo.count = 0;
            node->reciveHandle.fifo.end = 0;
            node->reciveHandle.fifo.head = 0;
            //释放后重新分配
            FREE(node->reciveHandle.fifo.pData);
            node->reciveHandle.fifo.pData = (uint8_t*)MALLOC(sizeof(uint8_t) * capacity);
            if (node->reciveHandle.fifo.pData == NULL)
            {
                return ERROR_MALLOC;
            }
        }  
        if ( node->sendHandle.fifo.capacity != capacity)   //容量不相等则重新分配
        {
           
            node->sendHandle.fifo.capacity = capacity;
            node->sendHandle.fifo.count = 0;
            node->sendHandle.fifo.end = 0;
            node->sendHandle.fifo.head = 0;
            //释放后重新分配
            FREE(node->sendHandle.fifo.pData);
            node->sendHandle.fifo.pData = (uint8_t*)MALLOC(sizeof(uint8_t) * capacity);
            if (node->sendHandle.fifo.pData == NULL)
            {
                return ERROR_MALLOC;
            }
        }         
    }
    node->Send = RouterSendData;
    *addNde = node; //赋值输出
    return ERROR_OK_NULL;
}
/**
 * @brief : 路由器发送数据,将数据放到缓冲区
 * @param : Router* Router 路由器
 * @param : uint32_t fromID 来源ID
 * @param : uint32_t toID   目的ID
 * @return: 空
 * @update: [2018-06-08][张宇飞][]
 */
uint8_t RouterNodeSendData(const Router* router, uint32_t fromID, uint32_t toID, uint8_t* data, uint16_t len)
{
    if (router == 0 || data == 0)
    {
        return ERROR_NULL_PTR;
    }
    
    NodeFifo*  node = FindRouteNodeById(&(router->nodeStation), fromID);
    FifoHandle* handle = &(node->sendHandle);
    if (node == NULL)
    {
        return ERROR_UNFIND;
    }
    if (handle->fifo.capacity - handle->fifo.count < len )
    {
        return ERROR_VOLUM;
    }
    
    for(uint8_t i = 0; i < len; i++)
    {
        handle->Enqueue(handle, data[i]);
    }   
    return NULL;    
}



/**
 * @brief : 路由器发送数据, 从缓冲区取走数据
 * @param : Router* Router 路由器
 * @param : uint32_t fromID 来源ID
 * @param : uint32_t toID   目的ID
 * @param : uint8_t* data 数据
 * @param :  uint16_t *reciveLen 输入参数为期望得到的长度，*reciveLen 调用后为实际长度
 * @return: 0-成功 非0 失败
 * @update: [2018-06-09][张宇飞][]
 */
uint8_t RouterNodeReciveData(Router* router, uint32_t fromID, uint32_t* toID, uint8_t* data, uint16_t *reciveLen)
{
    if (router == 0 || data == 0)
    {
        return ERROR_NULL_PTR;
    }
    
    NodeFifo*  node = FindRouteNodeById(&(router->nodeStation), fromID);
    FifoHandle* handle = &(node->sendHandle);
    if (node == NULL)
    {
        return ERROR_UNFIND;
    }
    //实际大小
    if (handle->fifo.count <  *reciveLen)
    {
        *reciveLen = handle->fifo.count;
    }
    
    for(uint8_t i = 0; i < *reciveLen; i++)
    {
        handle->Dequeue(handle, data + i);
    }   
    return NULL;    
}

/**
 * @brief : 路由器广播数据，从接收队列全部转发到其它结点
 * @param : Router* router
 * @return: 0-成功 非0 失败
 * @update: [2018-06-09][张宇飞][]
 *[2018-06-11][张宇飞][修正指向错误,接收满的检测，修改赋值逻辑错误]
 */
uint8_t RouterBroadcast(Router* router)
{
    
    if (router == 0)
    {
        return ERROR_NULL_PTR;
    }
    ListDouble* list = &(router->nodeStation);
    uint8_t size = list_size(list);
    ListElment* element = list_head(list);
    uint8_t data;
    for (uint8_t i = 0; i < size; i++)
    {
        NodeFifo* node = GET_NODEFIFO_ELEMENT(element);
        
         //有要发送的数据
        FifoHandle* send = &(node->sendHandle);
        while (send->fifo.count > 0)
        {
             send->Dequeue(send, &data); 
             ListElment* elementSub = list_head(list);            
             for (uint8_t i = 0; i < size; i++)
             {
                 //跳过ID相等的部分, 送入方向 element -->>elementSub
                 if (GET_NODEFIFO_ELEMENT(elementSub)->id != GET_NODEFIFO_ELEMENT(element)->id)
                 {                     
                      FifoHandle* recive = &(GET_NODEFIFO_ELEMENT(elementSub)->reciveHandle);                                                           
                         //检测剩余个数是否已满
                      //if(recive->fifo.capacity - recive->fifo.count  > 0)
                     // {                                                     
                       recive->Enqueue(recive,  data);                                                                                                            
                 }             
                 elementSub = elementSub->next;
             }
         }
         element = element->next;
    }
    return NULL;
}
