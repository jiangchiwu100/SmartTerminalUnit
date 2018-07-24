/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      communicaton.c
  * @brief:     
  * @version:   V0.2.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-27
  * @update:    2018/7/20 修改数据流为数据包   
  */

#include "communication.h"
#include "distribution.h"
#include "distribution_test_case.h" 
#include "serialport.h"
#include "parse_implement.h" 
#include "Datagram.h"

#include "extern_interface.h"

static ProtocolAnylast LocalAnylast;
static NodeFifo* LocalFifo;



/**
*  @brief :通讯服务，用于串口数据流处理
  * @param void
  * @return: 0--正常
  * @update: [2018-07-24][张宇飞][修改]
*/
uint8_t CommunicationServeice(void)
{
    uint8_t result = 0, data = 0;
    //初始化串口5
    SerialPort5Init();
    ListDataInit();
    rt_kprintf("init usart5.\r\n");


    result  = BuildNodeFifo(0, 1024, &LocalFifo);
    if (result)
    {
        rt_kprintf(" BuildNodeFifo failure ERROR:%X.\r\n", result);
    }            
    ProtocolAnylastInit(&LocalAnylast, 0);
    LocalAnylast.fifohanlde = &(LocalFifo->reciveHandle);
    LocalAnylast.sendFifohanlde = &(LocalFifo->sendHandle);
    
    PointUint8 packet;   
    ErrorCode error;
    FifoHandle* handle = LocalAnylast.fifohanlde;
    uint16_t resideLen = 0;
    uint16_t lastResideLen = 0;
    do
    {       
            //获取数据
        do
        {
           result =  Uart5FifoHandle.Dequeue(&Uart5FifoHandle, &data);      
           if(result)
           {
               handle->Enqueue(handle, data);
           }
       }while(result);

        do
        {
            resideLen =  LocalAnylast.ProtocolAnylastDeal(&LocalAnylast);
            if (LocalAnylast.recvRtu.completeFlag == true)
            {
                if (LocalAnylast.recvRtu.destAddress == LOCAL_ADDRESS)
                {
                    //PrintMemoryUsed();
                    ExecuteFunctioncode(&LocalAnylast.recvRtu, &g_StationManger.simulationServer);
                }
                else
                {
                    
                    //写接收用于测试
                    RingQueue* ring = &(g_VirtualNode.sendRing);
                    error = Datagram_CopyToPacket(LocalAnylast.recvRtu.pData, LocalAnylast.recvRtu.datalen + FRAME_MIN_LEN, &packet);
                    if (error)
                    {
                        perror("Datagram_CopyToPacket Error: 0x%x\n", error);
                        return 0;
                    }
                    
                    error = g_VirtualNode.Send(&g_VirtualNode, &packet) ;
                    if (error)
                    {
                        perror("g_VirtualNode.Send Error: 0x%x\n", error);
                    }

                }
            }
            LocalAnylast.recvRtu.completeFlag = false;
            
            if (lastResideLen == resideLen)
            {
                break;
            }
            lastResideLen = resideLen;
        }while(resideLen > 0);
        rt_thread_delay(10);
          
    }while(1);
       
}

static uint16_t resideLen = 0;
static uint16_t lastResideLen = 0;


/**
* @brief :初始化，为单次调用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-21][张宇飞][创建]
*/
uint8_t CommunicationServerInitSingle(void)
{
     uint8_t result = 0;
    //初始化串口5
    SerialPort5Init();
    ListDataInit();
    rt_kprintf("init usart5.\r\n");


    result  = BuildNodeFifo(0, 1024, &LocalFifo);
    if (result)
    {
        rt_kprintf(" BuildNodeFifo failure ERROR:%X.\r\n", result);
    }            
    ProtocolAnylastInit(&LocalAnylast, 0);
    LocalAnylast.fifohanlde = &(LocalFifo->reciveHandle);
    LocalAnylast.sendFifohanlde = &(LocalFifo->sendHandle);
    
    
    resideLen = 0;
    lastResideLen = 0;
    return 0;
}
/**
  * @brief :单次调用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-21][张宇飞][创建]
*/
uint8_t CommunicationServerSingle(void)
{
    PointUint8 packet;   
    ErrorCode error;
    FifoHandle* handle = LocalAnylast.fifohanlde;
    uint8_t result = 0, data = 0;       
            //获取数据
    do
    {
       result =  Uart5FifoHandle.Dequeue(&Uart5FifoHandle, &data);      
       if(result)
       {
           handle->Enqueue(handle, data);
       }
   }while(result);

    do
    {
        resideLen =  LocalAnylast.ProtocolAnylastDeal(&LocalAnylast);
        if (LocalAnylast.recvRtu.completeFlag == true)
        {
            if (LocalAnylast.recvRtu.destAddress == LOCAL_ADDRESS)
            {
                //PrintMemoryUsed();
                ExecuteFunctioncode(&LocalAnylast.recvRtu, &g_StationManger.simulationServer);
            }
            else
            {
                
                //写接收用于测试
                RingQueue* ring = &(g_VirtualNode.sendRing);
                error = Datagram_CopyToPacket(LocalAnylast.recvRtu.pData, LocalAnylast.recvRtu.datalen + FRAME_MIN_LEN, &packet);
                if (error)
                {
                    perror("Datagram_CopyToPacket Error: 0x%x\n", error);
                    return 0;
                }
                
                error = g_VirtualNode.Send(&g_VirtualNode, &packet) ;
                if (error)
                {
                    perror("g_VirtualNode.Send Error: 0x%x\n", error);
                }

            }
        }
        LocalAnylast.recvRtu.completeFlag = false;
        
        if (lastResideLen == resideLen)
        {
            break;
        }
        lastResideLen = resideLen;
    }while(resideLen > 0);
        
        return 0;
}
/**
  * @brief :监控使用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-1][张宇飞][创建]
*/
//void Monitor(void)
//{
//	uint8_t data;
//	RingQueue* ring = &(g_VirtualNode.reciveRing);
//	DatagramFrame* frame;
//	do
//	{

//		bool state = ring->Read(ring, (void**)&frame);
//		if (state)
//		{

//			UartSend(UART5, frame->pData, frame->size);			
//			Datagram_Destory(frame);
//		}
//		else
//		{
//			break;
//		}
//	} while (true);
//    
//}

/**
  * @brief :监控使用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-1][张宇飞][创建]
*/
ErrorCode TypeConvertAndVirtualNodeSend(uint8_t* pData, uint8_t len)
{
    if(pData == NULL)
    {
        perror("pData == NULL\n");
        return ERROR_NULL_PTR;
    }
    PointUint8 packet;
    ErrorCode error = Datagram_CopyToPacket(pData, len , &packet);
    if (error)
    {
        perror("Datagram_CopyToPacket Error: 0x%x\n", error);
        return error;
        
    }
    
    error = g_VirtualNode.Send(&g_VirtualNode, &packet) ;
    if (error)
    {
        perror("g_VirtualNode.Send Error: 0x%x\n", error);
        return error;
    }
    return error;
}



