/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      RingQueue.c
* @brief:     环形队列,由队列改型而来 TODO：高速无锁环形队列
* @version:   V1.0.0
* @author:    Zhang Yufei
* @date:      2018-06-27
* @update
*/

#include "RingQueue.h"
#include <string.h>
#include "extern_interface.h"




static  bool RingQueueWrite(RingQueue* ring,  void*  indata);
static  bool  RingQueueRead(RingQueue* ring, void** indata);
static bool RingQueuePeek(RingQueue *const ring, void  **peekdata, uint16_t len);



/**
 * 使用句柄对FIFO初始化
 * <p>
 *
 * @param  handle     FIFO句柄
 * @param  pdata   指向的缓冲区
 * @return  void
 */
bool RingQueueInit(RingQueue* ring,  uint16_t capacity)
{
    if (!ring)
    {
        return false;
    }
   
    MEMSET(ring, 0, sizeof(RingQueue));
    ring->capacity = capacity;
    ring->arrayPtr = CALLOC(capacity ,  sizeof(void*));
    if (!ring->arrayPtr)
    {
        return false;
    }
    
    ring->count = 0;
    ring->head = 0;
    ring->tail = 0;

    ring->Read = RingQueueRead;
    ring->Write = RingQueueWrite;
    ring->Peek = RingQueuePeek;    
    return true;
}

/**
* 使用句柄对FIFO初始化
* <p>
*
* @param  handle     FIFO句柄
* @param  pdata   指向的缓冲区
* @return  void
*/
bool RingQueueDestory(RingQueue* ring)
{
    if (!ring || !ring->arrayPtr)
    {
        return false;
    }

    FREE(ring->arrayPtr);
    ring->arrayPtr = 0;
    return true;
}


/**
 * 缓冲数据入队  多个调用注意互斥问题
 * <p>
 *
 * @param  pInf     FIFO信息
 * @param  pMsg    入队信息
 * @return          <code>true</code>   成功入队
 *                  <code>FASLE</code>  失败
 */
static inline bool RingQueueWrite(RingQueue* ring,  void*  indata)
{
    if (!ring)
    {
        return false;
    }      

    ring->arrayPtr[ring->tail] = indata;
    ring->tail = (ring->tail + 1) % ring->capacity;
    ring->count++;
    
    if (ring->count >= ring->capacity)//超出时候，更新head，保证最新更新
    {
        ring->head = ring->tail; 
        rt_kprintf("Over Range\n");
    }
    return true;
}
/**
 * 缓冲数据出队  多个调用注意互斥问题
 * <p>
 *
 * @param  pInf     FIFO信息
 * @param  pMsg    出队信息
 * @return          <code>true</code>   成功出队
 *                  <code>FASLE</code>  失败
 */
static inline bool  RingQueueRead(RingQueue* ring,  void** indata)
{
    if (!ring)
    {
        return false;
    }

    if (ring->count > 0)
    {
        *indata = ring->arrayPtr[ring->head];
        ring->head = (ring->head + 1) % ring->capacity;
        ring->count--;
        return true;
    }
    return false;
}
/**
 * 缓冲数据检视，但不出队  多个调用注意互斥问题
 * <p>
 *
 * @param  pInf     FIFO信息
 * @param  pMsg    出队信息
 * @return          <code>true</code>   成功检视
 *                  <code>FASLE</code>  失败
 */
static bool RingQueuePeek(RingQueue *const ring, void  **peekdata, uint16_t len)
{
    if (!ring )
    {
        return false;
    }

    if (ring->count >= len)
    {
        uint16_t head  = ring->head;
        
        for (uint16_t k = 0; k < len; k++)
        {
            peekdata[k] = ring->arrayPtr[head++];
            head= head % ring->capacity;
        }                             
        return true;
    }
    return false;
}
