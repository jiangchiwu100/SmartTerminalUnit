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
#include "extern_interface.h"
#include <stdbool.h>
#include "distribution_config.h"

extern uint8_t socketNO;



static struct rt_thread udpserver_thread;



ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_udpserver_thread_stack[THREAD_UDPSERVER_STACK_SIZE];//线程栈
static uint8_t UdpReciveBuffer[1024];
static uint8_t UdpSendBuffer[1024];
static uint8_t DefautIp[4];



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
    DefautIp[2] = 60;
    DefautIp[3] = 111;
}
/**
  * @brief :W5500用于UDP通信
  * @param  void *param
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */
static void udpserver_thread_entry(void *param)
{
    rt_err_t result;
    int32_t ret;
    uint16_t length = 0;
    uint8_t buf[256];
    uint8_t srcip[4];
    uint16_t destport;	
    W5500Init();
	
    //setSIMR(0x01);//
    //setSn_IMR(socketNO, Sn_IR_RECV); //
	uint8_t get_result =0;
	
    rt_kprintf("udpserver start!\n");
    for (;;)
    {   	 
       
        rt_thread_delay(10);
		get_result = getSn_SR(socketNO);
		
		switch (get_result)
		{
			case SOCK_UDP:
				
                do
                {
                    length = getSn_RX_RSR(socketNO);		
                    if (length <= 0)
                    {
                        break;
                    } 
                    else
                    {
                        ret = w5500_recvfrom(socketNO, UdpReciveBuffer, length, srcip, &destport);
                        if(ret > 0)
                        {
                            w5500_sendto(socketNO, UdpReciveBuffer, length, DefautIp, 8080);
                        }
                    }    
                }while(true);
								
                break;
			case SOCK_CLOSED:	
				if ((ret = w5500_socket(socketNO, Sn_MR_UDP, 8080, 0)) != socketNO)
				{
					rt_kprintf("udpserver close!\n");
				} 
				//rt_thread_delay(1);
				break;

			default:
				break;
		}
		
		
    }
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