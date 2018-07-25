/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      w5500_udp.c
  * @brief:     
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-21 w5500 udp server专用
  * @update:    
  */
  
#include "w5500_server.h"
#include "drv_w5500_socket.h"
#include "drv_w5500.h"

#include "distribution.h"
#include "distribution_config.h"
#include "distribution_app.h"

uint8_t SocketNum = 0; //使用的W5500 Socket号
uint8_t SocketMaintanceNum = 1; //使用的维护 Socket号



static struct rt_thread udpserver_thread;

static void MaintaceServer(void);

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_udpserver_thread_stack[THREAD_UDPSERVER_STACK_SIZE];//线程栈
static uint8_t UdpReciveBuffer[1024];
static uint8_t UdpSendBuffer[1024];
static uint8_t DefautIp[4];
static uint16_t LocalPort; 
static uint16_t LocalMaintancePort;
static uint16_t RemotePort;






/**
  * @brief :W5500用于UDP通信
  * @param  void *param
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */  
static inline void W5500Init(void)
{    
    MEMSET(UdpReciveBuffer, 0, sizeof(UdpReciveBuffer));
    MEMSET(UdpSendBuffer, 0, sizeof(UdpSendBuffer));
    DefautIp[0] = 192;
    DefautIp[1] = 168;
    DefautIp[2] = 10;
    DefautIp[3] = 111;
    LocalPort = 5555;
    RemotePort = 5555;
    LocalMaintancePort = 5500;
}



/**
  * @brief :W5500用于UDP通信
  * @param  void *param
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */
static void udpserver_thread_entry(void *param)
{  
    int32_t ret;
    uint16_t length = 0;    
    uint8_t srcip[4];
    uint8_t cn = 0;
    uint16_t destport;	
   

    W5500Init();
	
    //setSIMR(0x01);//
    //setSn_IMR(SocketNum, Sn_IR_RECV); //
	uint8_t get_result =0;
	
    rt_kprintf("udpserver start!\n");
    g_StationManger.isMaintanceRun = false;
    for (;;)
    {   	 
       
       
		get_result = getSn_SR(SocketNum);
		switch (get_result)
		{
			case SOCK_UDP:			
                do
                {
                    if(getSn_IR(SocketNum) & Sn_IR_RECV)
					{
						setSn_IR(SocketNum, Sn_IR_RECV);// Sn_IR的RECV位置1
					}
                    
                    length = getSn_RX_RSR(SocketNum);
                    
                                        
                    if (length <= 0)
                    {
                        break;
                    } 
                    else
                    {
                        ret = w5500_recvfrom(SocketNum, UdpReciveBuffer, length, srcip, &destport);
                        if(ret > 0)
                        {                            
                           StationPointFrameDeal(UdpReciveBuffer, ret);
                        //    w5500_sendto(SocketNum, UdpReciveBuffer, length, DefautIp, LocalPort);
                        }
                    }    
                }while(true);
								
                break;
			case SOCK_CLOSED:	
				if ((ret = w5500_socket(SocketNum, Sn_MR_UDP, LocalPort, 0)) != SocketNum)
				{
					rt_kprintf("udpserver close!\n");
				} 
				//rt_thread_delay(1);
				break;

			default:
				break;
		}
		
       
        rt_thread_delay(4);
        if (cn ++ > 10)
        {
            cn = 0;
            MaintaceServer();
        }
		
    }
}


/**
* @brief :维护服务
* @param  void
* @return: 0--正常
* @update: [2018-07-23][张宇飞][创建]
*/
static void MaintaceServer(void)
{
    extern ProtocolAnylast LocalAnylast;
    uint8_t srcip[4];
    uint16_t destport;	
    int32_t ret;
    uint16_t length = 0;    
    rt_err_t  get_result = getSn_SR(SocketMaintanceNum);
    g_StationManger.isMaintanceRun = true;
    switch (get_result)
    {
        case SOCK_UDP:			
            do
            {
                g_StationManger.isMaintanceRun = true;
                if(getSn_IR(SocketMaintanceNum) & Sn_IR_RECV)
                {
                    setSn_IR(SocketMaintanceNum, Sn_IR_RECV);// Sn_IR的RECV位置1
                }
                length = getSn_RX_RSR(SocketMaintanceNum);		
                if (length <= 0)
                {
                    break;
                } 
                else
                {
                    ret = w5500_recvfrom(SocketMaintanceNum, UdpReciveBuffer, length, srcip, &destport);
                    
                    if(ret > 0 )
                    {
                        MantaiceFrameDeal(UdpReciveBuffer, ret);                           
                    //    w5500_sendto(SocketNum, UdpReciveBuffer, length, DefautIp, LocalPort);
                    }
                        
                }    
            }while(true);
                            
            break;
        case SOCK_CLOSED:	
            if ((ret = w5500_socket(SocketMaintanceNum, Sn_MR_UDP, LocalMaintancePort, 0)) != SocketNum)
            {
                g_StationManger.isMaintanceRun = false;
                rt_kprintf("udpserver close!\n");
            } 
            //rt_thread_delay(1);
            break;

        default:
            break;
    }
}

#include "datagram.h"


/**
  * @brief :外部发送,不进行参数检查
  * @param void
  * @return: 0--正常
  * @update: [2018-07-23][张宇飞][创建]
*/
ErrorCode ExternSend(PointUint8* pPacket)
{
    uint8_t destId[4] = {0};
    destId[0] = 192;
    destId[1] = 168;
    destId[2] = pPacket->pData[FRAME_DEST_INDEX + 1];
    destId[3] = pPacket->pData[FRAME_DEST_INDEX];
    int ret = w5500_sendto(SocketNum, pPacket->pData, pPacket->len, destId, RemotePort);
    if(ret == pPacket->len)
    {
        return ERROR_OK_NULL;
    }
    else
    {
        return ERROR_UNKONOW;
    }
}
/**
  * @brief :监控使用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-23][张宇飞][创建]
*/
void Monitor(void)
{    
    extern DatagramTransferNode g_VirtualNode;
	RingQueue* ring = &(g_VirtualNode.reciveRing);
	DatagramFrame* frame;
    DefautIp[0] = 192;
    DefautIp[1] = 168;
    DefautIp[2] = 10;
    DefautIp[3] = 111;
	do
	{

		bool state = ring->Read(ring, (void**)&frame);
		if (state)
		{
			w5500_sendto(SocketMaintanceNum, frame->pData, frame->size, DefautIp, RemotePort);	
			Datagram_Destory(frame);
		}
		else
		{
			break;
		}
	} while (true);
    
}

/**
* @brief :UDP任务初始化
* @param  void
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void udp_debug_printf(const char *fmt, ...)
{
    if (!g_StationManger.isMaintanceRun)
    {
        return;
    }
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);

    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;

    w5500_sendto(SocketMaintanceNum, (uint8_t*)rt_log_buf, rt_strlen(rt_log_buf) + 1, DefautIp, 5533);	
    
    
    va_end(args);
}




/**
* @brief  : 外部发送
* @param  : 队列句柄
* @param  ：DatagramTransferNode* node 
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-23][张宇飞][]
*/
ErrorCode Udp_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket)
{       
	CHECK_POINT_RETURN(node, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(pPacket, NULL, ERROR_NULL_PTR);

	ErrorCode error = ExternSend(pPacket);    
    //释放内存
	FREE(pPacket->pData);
	return error;
}

/**
* @brief :UDP任务初始化
* @param  void
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void UdpServerAppInit(void)
{
    
	rt_thread_init(&udpserver_thread,                 //线程控制块
		THREAD_UDPSERVER_NAME,                       //线程名字，在shell里面可以看到
		udpserver_thread_entry,            //线程入口函数
		RT_NULL,                      //线程入口函数参数
		rt_udpserver_thread_stack,     //线程栈起始地址
		sizeof(rt_udpserver_thread_stack), //线程栈大小
		THREAD_UDPSERVER_PRIORITY,                            //线程的优先级
		THREAD_UDPSERVER_TIMESLICE);                          //线程时间片

	rt_thread_startup(&udpserver_thread);

	
}



