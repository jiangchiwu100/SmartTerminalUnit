/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      communicaton.c
  * @brief:     
  * @version:   V0.2.0 
  * @author:    Zhang Yufei
  * @date:      2018-05-27
  * @update:    2018/7/20 修改数据流为数据包   
  */

#include "distribution.h"
#include "distribution_test_case.h" 
#include "serialport.h"
#include "parse_implement.h" 
#include "Datagram.h"

#include "extern_interface.h"

static ProtocolAnylast LocalAnylast;
static NodeFifo* LocalFifo;




uint8_t CommunicationServeice(void)
{
    uint8_t result = 0, data = 0;
    //初始化串口4
    SerialPort4Init();
    ListDataInit();
    rt_kprintf("init usart4.\r\n");


    result  = BuildNodeFifo(0, 1024, &LocalFifo);
    if (result)
    {
        rt_kprintf(" BuildNodeFifo failure ERROR:%X.\r\n", result);
    }            
    ProtocolAnylastInit(&LocalAnylast, 0);
    LocalAnylast.fifohanlde = &(LocalFifo->reciveHandle);
    LocalAnylast.sendFifohanlde = &(LocalFifo->sendHandle);
    
    PointUint8 packet;
    RingQueue* ring;
    ErrorCode error;
    FifoHandle* handle = LocalAnylast.fifohanlde;
    uint16_t resideLen = 0;
    uint16_t lastResideLen = 0;
    do
    {       
            //获取数据
        do
        {
           result =  Uart4FifoHandle.Dequeue(&Uart4FifoHandle, &data);      
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
                    ring = &(g_VirtualNode.sendRing);
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
void Monitor(void)
{
	uint8_t data;
	RingQueue* ring = &(g_VirtualNode.reciveRing);
	DatagramFrame* frame;
	do
	{

		bool state = ring->Read(ring, (void**)&frame);
		if (state)
		{

			UartSend(UART4, frame->pData, frame->size);			
			Datagram_Destory(frame);
		}
		else
		{
			break;
		}
	} while (true);
    
}




