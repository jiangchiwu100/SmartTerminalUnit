/**
*             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
* @file:      buffer.c
* @brief:     缓冲队列
* @version:   V0.0.0
* @author:    Zhang Yufei
* @date:      2018-07-18
* @update:
*/
#include "buffer.h"
#include <string.h>
#include "extern_interface.h"


/*2018/6/12 增加peek函数*/

static void BufferInit( FifoInformation *const fifo, PointUint8 *pdata);

static uint8_t BufferEnqueue( FifoHandle *const fifohanlde, uint8_t const indata);
static uint8_t BufferDequeue(FifoHandle *const fifohanlde, uint8_t  *dedata);
static uint8_t BufferPeek(FifoHandle *const fifohanlde, uint8_t  *peekdata, uint8_t len);

/**
 * 使用句柄对FIFO初始化
 * <p>
 *
 * @param  handle     FIFO句柄
 * @param  pdata   指向的缓冲区
 * @return  void
 */
void FifoInit(FifoHandle *handle, PointUint8 *pdata)
{
    BufferInit( &handle->fifo, pdata);
    handle->Enqueue = BufferEnqueue ;
    handle->Dequeue = BufferDequeue;
    handle->Peek = BufferPeek;
}

/**
 *初始化缓冲区
 */

static void BufferInit( FifoInformation *const fifo, PointUint8 *pdata)
{
    fifo->capacity = pdata->len;
    fifo->count = 0;
    fifo->end = 0;
    fifo->head = 0;
    fifo->pData = pdata->pData;
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
static uint8_t BufferEnqueue( FifoHandle *const fifohanlde, uint8_t const indata)
{
    FifoInformation *pInf = &fifohanlde->fifo;
    //队列未满

    if (pInf->count < pInf->capacity)
    {

        pInf->pData[pInf->end] =  indata;

        pInf->end = (pInf->end + 1) % pInf->capacity;
        pInf->count++;
        return true;
    }
	perror("BufferEnqueue OverFlow");
    //溢出报错
    //TODO:溢出报错机制
    return false;
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
static uint8_t BufferDequeue(FifoHandle *const fifohanlde, uint8_t  *dedata)
{
    FifoInformation *pInf =  &fifohanlde->fifo;

    if (pInf->count > 0)
    {
        *dedata = pInf->pData[pInf->head];
        pInf->head = (pInf->head + 1) % pInf->capacity;
        pInf->count--;
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
static uint8_t BufferPeek(FifoHandle *const fifohanlde, uint8_t  *peekdata, uint8_t len)
{
    FifoInformation *pInf =  &fifohanlde->fifo;

    if (pInf->count >= len)
    {
        uint16_t head  = pInf->head;
        
        for (uint16_t k = 0; k < len; k++)
        {
            peekdata[k] = pInf->pData[head++];
            head= head % pInf->capacity;
        }                             
        return true;
    }
    return false;
}
