/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的相关任务
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */


/*****************************include***********************************/
#include "lwip/api.h"
#include "NetFinshApp.h"
#include "NetFinshThread.h"
#include "tcp_server.h"
#include "rtconfig.h"
#include ".\MultiThread\multi_thread.h"
#include "station_manager.h"
#include "w5500_server.h"

#include "lwip/sockets.h"
#include "lwip/ip.h"
#include <lwip/netdb.h>
/*****************************宏定义************************************/
#define REMOTE_ADDRESS 				"192.168.10.111"			/*远程IP的地址*/
#define FINSH_LOCAL_PORT			5533						//finsh和网口打印输出的本地端口
#define FINSH_REMOTE_PORT			FINSH_LOCAL_PORT			//finsh接收命令的远端端口
#define UDP_SERVE_LOCAL_PORT		5555						//UDP通信服务本地端口号
#define UDP_SERVE_REMOTE_PORT		UDP_SERVE_LOCAL_PORT		//UDP通信服务远端端口号
#define MAINTACE_SERVE_LOCAL_PORT	5500						//维护服务本地端口号
#define MAINTACE_SERVE_REMOTE_PORT	5555						//维护服务远端端口号
#define PRINT_BUFFER_SIZE			512		//打印输出的缓冲区大小





/*******************************Function********************************/

/**
  * @brief : net finsh thread entry
  * @param : none
  * @return: none
  * @update: [2018-09-10][李  磊][创建]
  * 
  */  
#if 0
static void rt_net_finsh_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint8_t printBuffer[PRINT_BUFFER_SIZE] = {0};
	uint32_t receviceNum = 0;
	
	/* 接收FIFO和发送FIFO申请动态内存以及初始化 */
	ret = FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);
	if(!ret)
	{
		ret = FifoMallocAndInit(&PrintfFifoHandle, &PrintfBuffer, NET_PRINTF_BUFSIZE, &PrintfBufferPack);
	}
	
	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);	

		g_NetFinshNetconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		
		if(g_NetFinshNetconn != NULL)  //创建UDP链接成功
		{
			g_NetFinshNetconn->recv_timeout = 100;
			err = netconn_bind(g_NetFinshNetconn, IP_ADDR_ANY, FINSH_LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //构造目的IP地址
			netconn_connect(g_NetFinshNetconn, &destipAddr, FINSH_REMOTE_PORT); 	//连接到远端主机
			if(err == ERR_OK)//绑定完成
			{
				/*UDP链接已经创建，之后可以使用网口的打印函数了*/
				NetFinshFlag = true;
				rt_kprintf("Net finsh Init Success\r\n");
				
				while(1)
				{
					/*等待接收，将接收到的字符入队*/
					receviceNum = UDP_NetconnReceiveString(g_NetFinshNetconn, FinshReceiveFifoHandle);
					
					/*将队列中的内容发送出去*/
					if(true == NetFinshFlag)
					{
						memset(printBuffer, 0, PRINT_BUFFER_SIZE);
						for(i=0; (i<PRINT_BUFFER_SIZE) && (PrintfFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(PrintfFifoHandle);
						}
						if(0 != i)
						{
							UDP_NetconnSendString(g_NetFinshNetconn, printBuffer);
						}
						
					}
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		NetFinshFlag = false;
	}
	
	FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/*释放接收和发送的队列所用到的动态分配的内存*/
	FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
}
#endif

/**
  * @brief : net finsh thread entry
  * @param : none
  * @return: none
  * @update: [2018-10-09][李  磊][创建]
  * 
  */  
#if RT_USING_NET_FINSH
static void rt_net_finsh_thread_entry(void *param)
{
	volatile uint32_t receiveNum;
	uint8_t buffer[PRINT_BUFFER_SIZE] = {0};
	uint32_t addressLenth;
	struct sockaddr_in localAddress, remoteAddress;
	uint8_t ret = 0;
	uint32_t i = 0;

	/* 本地和远程IP、端口的设置,socket的建立和绑定 */
	IpAddressInit(&localAddress, FINSH_LOCAL_PORT, &remoteAddress, FINSH_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_NetFinshSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);
	
	/* 接收FIFO和发送FIFO申请动态内存以及初始化 */
	ret += FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);
	ret += FifoMallocAndInit(&PrintfFifoHandle, &PrintfBuffer, NET_PRINTF_BUFSIZE, &PrintfBufferPack);
	if(ret)
	{
		FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/* fifo创建失败,则进行释放 */
		FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
		rt_kprintf("Net Finsh Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}
	
	/*UDP链接已经创建，之后可以使用网口的打印函数了*/
	NetFinshFlag = true;
	rt_kprintf("Net finsh Init Success\r\n");			
	
	while (1)
	{
		/* 发送数据 */
		memset(buffer, 0, PRINT_BUFFER_SIZE);
		for(i=0; (i<PRINT_BUFFER_SIZE) && (PrintfFifoHandle->fifo.count); i++)
		{
			buffer[i] = FifoCharDequeue(PrintfFifoHandle);
		}
		if(0 != i)
		{
			/* 将rt_kprintf的内容实际发送出去 */
			lwip_sendto(g_NetFinshSocket, buffer, i, 0, (struct sockaddr*)&remoteAddress, sizeof(struct sockaddr));
		}
		
		/* 接收数据 */
		memset(buffer, 0, PRINT_BUFFER_SIZE);
		lwip_recvfrom(g_NetFinshSocket, buffer, PRINT_BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		receiveNum = strlen((char*)buffer);
		if((receiveNum > 0) && (receiveNum < PRINT_BUFFER_SIZE))
		{
			/* 将接收到的数据入队，等待finsh读取 */
			FifoStringEnqueue(FinshReceiveFifoHandle, buffer, receiveNum);
		}
		rt_thread_delay(10);
	}
	
	lwip_close(g_NetFinshSocket);
	NetFinshFlag = false;
	return;
}
#endif

/**
  * @brief : udp serve thread entry
  * @param : none
  * @return: none
  * @update: [2018-09-12][李  磊][创建]
  * 
  */
#if 0
static void rt_udp_serve_thread_entry(void *param)
{
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[UDP_SERVE_BUFSIZE] = {0};

	// NVIC_Configuration();
	
	ret = FifoMallocAndInit(&UDP_ServeFifoHandle, &UDP_ServeBuffer, UDP_SERVE_BUFSIZE, &UDP_ServeBufferPack);  /*初始化fifo*/
	ip_addr_t iplocal;
	IP4_ADDR(&iplocal, 192, 168, 10, 129);
	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);

		g_UDP_ServeNetconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		if(g_UDP_ServeNetconn != NULL)  //创建UDP链接成功
		{
			g_UDP_ServeNetconn->recv_timeout = 100;
			err = netconn_bind(g_UDP_ServeNetconn, IP_ADDR_ANY, UDP_SERVE_LOCAL_PORT);
		//	err = netconn_bind(g_UDP_ServeNetconn, &iplocal, UDP_SERVE_LOCAL_PORT);
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //构造目的IP地址
			netconn_connect(g_UDP_ServeNetconn, &destipAddr, UDP_SERVE_REMOTE_PORT); 	//连接到远端主机
			ip_set_option(g_UDP_ServeNetconn->pcb.ip, SO_BROADCAST);
			if(err == ERR_OK)//绑定完成
			{
				/*UDP链接已经创建，之后可以使用网口的打印函数了*/
				UDP_ServeFlag = true;
				rt_kprintf("UDP Communbicate Serve Init Success\r\n");
				
				while(1)
				{
					receviceNum = UDP_NetconnReceiveString(g_UDP_ServeNetconn, UDP_ServeFifoHandle);
					
//					memset(printBuffer, 0, PRINT_BUFFER_SIZE);
//					for(i=0; (i<PRINT_BUFFER_SIZE) && (UDP_ServeFifoHandle->fifo.count); i++)
//					{
//						printBuffer[i] = FifoCharDequeue(UDP_ServeFifoHandle);
//					}
//					if(0 != i)
//					{
//						UDP_NetconnSendString(g_UDP_ServeNetconn, printBuffer);
//					}

					if(receviceNum > 0)
                   {
                       memset(printBuffer, 0, UDP_SERVE_BUFSIZE);
						for(i=0; (i<UDP_SERVE_BUFSIZE) && (UDP_ServeFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(UDP_ServeFifoHandle);
						}
						if(0 != i)
						{
							StationPointFrameDeal(printBuffer, i);
							rt_kprintf("StationPointFrameDeal success!\r\n");
						}
                   }
					
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		UDP_ServeFlag = false;
	}
	/*释放接收和发送的队列所用到的动态分配的内存*/
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);
}
#endif

/**
  * @brief : udp serve thread entry
  * @param : none
  * @return: none
  * @update: [2018-10-08][李  磊][创建]
  * 
  */
#if RT_USING_UDP_SERVE
static void rt_udp_serve_thread_entry(void *param)
{
	uint32_t sock;
	int receiveNum;
	char *recv_data;
	rt_uint32_t addressLenth;
	struct sockaddr_in localAddress, remoteAddress;

	/* 分配接收用的数据缓冲 */
	recv_data = rt_malloc(1024);
	if (recv_data == RT_NULL)
	{
		/* 分配内存失败，返回 */
		rt_kprintf("No memory\n");
		return;
	}

	/* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
	if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("Socket error\n");
		/* 释放接收用的数据缓冲 */
		rt_free(recv_data);
		return;
	}
	
	/* 初始化服务端地址 */
	localAddress.sin_family = AF_INET;
	localAddress.sin_port = htons(5555);
	localAddress.sin_addr.s_addr = INADDR_ANY;
	rt_memset(&(localAddress.sin_zero), 0, sizeof(localAddress.sin_zero));

	/* 绑定socket到服务端地址 */
	if (lwip_bind(sock, (struct sockaddr*) &localAddress, sizeof(struct sockaddr))== -1)
	{
		/* 绑定地址失败 */
		rt_kprintf("Bind error\n");
		/* 释放接收用的数据缓冲 */
		rt_free(recv_data);
		return;
	}
	addressLenth = sizeof(struct sockaddr);
	rt_kprintf("UDPServer Waiting for client on port 5555...\n");
//	udpclient("192.168.10.111", 5555, "123456\n");
	while (1)
	{
		/* 从sock中收取最大1024字节数据 */
		receiveNum = lwip_recvfrom(sock, recv_data, 1024, 0, (struct sockaddr*)&remoteAddress, &addressLenth);

		/* UDP不同于TCP，它基本不会出现收取的数据失败的情况，除非设置了超时等待 */
		recv_data[receiveNum] = '\0'; /* 把末端清零 */

		/* 输出接收的数据 */
		rt_kprintf("\n(%s , %d) said : ", inet_ntoa(remoteAddress.sin_addr), ntohs(remoteAddress.sin_port));
		rt_kprintf("%s", recv_data);
		lwip_sendto(sock, recv_data, strlen(recv_data), 0, (struct sockaddr*)&remoteAddress, sizeof(struct sockaddr));

		/* 如果接收数据是exit，退出 */
		if (strcmp(recv_data, "exit") == 0)
		{
			lwip_close(sock);
			/* 释放接收用的数据缓冲 */
			rt_free(recv_data);
			break;
		}
	}
	return;
}
#endif

/**
  * @brief : dp82848实现的维护服务的任务入口
  * @param : none
  * @return: none
  * @update: [2018-09-17][李  磊][创建]
  * 
  */
static void rt_maintenance_serve_thread_entry(void *param)
{
	return;
	err_t err = 0;
	uint8_t ret = 0;
	struct ip_addr destipAddr;
	rt_base_t level;
	struct lwip_dev lwipDev;
	uint32_t i = 0;
	uint32_t receviceNum = 0;
	uint8_t printBuffer[MAINTENANCE_SERVE_BUFSIZE] = {0};
	
	ret = FifoMallocAndInit(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, \
							MAINTENANCE_SERVE_BUFSIZE, &MaintenanceServeBufferPack);/*初始化fifo*/

	g_StationManger.isMaintanceRun = false;

	while(!ret)
	{
		rt_thread_delay(1000);
		DP83848_ServeIpSet(&lwipDev);
		
		LWIP_UNUSED_ARG(param);

		g_MaintenanceServeNetconn = netconn_new(NETCONN_UDP);  //创建一个UDP链接
		
		if(g_MaintenanceServeNetconn != NULL)  //创建UDP链接成功
		{
			g_MaintenanceServeNetconn->recv_timeout = 100;
			err = netconn_bind(g_MaintenanceServeNetconn, IP_ADDR_ANY, MAINTACE_SERVE_LOCAL_PORT); 
			IP4_ADDR(&destipAddr, lwipDev.remoteip[0], lwipDev.remoteip[1], lwipDev.remoteip[2], lwipDev.remoteip[3]); //构造目的IP地址
			netconn_connect(g_MaintenanceServeNetconn, &destipAddr, MAINTACE_SERVE_REMOTE_PORT); 	//连接到远端主机
			if(err == ERR_OK)//绑定完成
			{
				MaintenanceServe = true;
				g_StationManger.isMaintanceRun = true;

				/*UDP链接已经创建，之后可以使用网口的打印函数了*/
				rt_kprintf("UDP Maintenance Serve Init Success\r\n");
				
				while(1)
				{
					/*等待接收，将接收到的字符入队*/
					receviceNum = UDP_NetconnReceiveString(g_MaintenanceServeNetconn, MaintenanceServeFifoHandle);
					
					if(receviceNum > 0)
                    {
                        memset(printBuffer, 0, MAINTENANCE_SERVE_BUFSIZE);
						for(i=0; (i<MAINTENANCE_SERVE_BUFSIZE) && (MaintenanceServeFifoHandle->fifo.count); i++)
						{
							printBuffer[i] = FifoCharDequeue(MaintenanceServeFifoHandle);
						}
						if(0 != i)
						{
							MantaiceFrameDeal(printBuffer, i);
						}
						
                    }
				}
			}
			else
			{
				rt_kprintf("UDP bind faliure\r\n");
			}
		}
		else
		{
			rt_kprintf("UDP connect failure\r\n");
		}
		MaintenanceServe = false;
		g_StationManger.isMaintanceRun = false;
	}
	/*释放接收和发送的队列所用到的动态分配的内存*/
	FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);
}


/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @update: [2018-09-10][李  磊][创建]
  * 
  */
#if RT_USING_NET_FINSH
static void NetFinsh_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(NET_FINSH_THREAD_NAME, 
                           rt_net_finsh_thread_entry, 
                           param, 
                           NET_FINSH_THREAD_STACK_SIZE, 
                           NET_FINSH_THREAD_PRIORITY, 
                           NET_FINSH_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("Net finsh thread start \r\n"); 
    }   
}
#endif


/**
  * @brief : Start udp finsh thread
  * @param : none
  * @return: none
  * @update: [2018-09-12][李  磊][创建]
  * 
  */
#if RT_USING_UDP_SERVE
static void UDP_CommunicateServe_thread_start(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(UDP_SERVE_THREAD_NAME, 
                           rt_udp_serve_thread_entry, 
                           param, 
                           UDP_SERVE_THREAD_STACK_SIZE, 
                           UDP_SERVE_THREAD_PRIORITY, 
                           UDP_SERVE_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("UDP serve thread start \r\n"); 
    }   
}
#endif


/**
  * @brief : dp82848实现维护服务
  * @param : none
  * @return: none
  * @update: [2018-09-17][李  磊][创建]
  * 
  */
void DP83848_MaintenanceServiceInit(void* param)
{
	rt_thread_t tid; 

    tid = rt_thread_create(MAINTENANCE_SERVE_THREAD_NAME, 
                           rt_maintenance_serve_thread_entry, 
                           param, 
                           MAINTENANCE_SERVE_THREAD_STACK_SIZE, 
                           MAINTENANCE_SERVE_THREAD_PRIORITY, 
                           MAINTENANCE_SERVE_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("Maintenance Serve thread start \r\n"); 
    }   
}


/**
  * @brief : 使用dp83848和UDP协议完成finsh远程登录的功能
  * @param : none
  * @return: 0:成功; 1:失败
  * @update: [2018-09-10][李  磊][创建]
  * 
  */
#if RT_USING_NET_FINSH
uint8_t rt_NetFinsh_thread_start(void)
{
    NetFinsh_thread_start(RT_NULL);
	
	return RT_EOK;
}
INIT_APP_EXPORT(rt_NetFinsh_thread_start);
#endif


/**
  * @brief : 使用dp83848和UDP协议完成与电脑端通信的功能
  * @param : none
  * @return: 0:成功; 1:失败
  * @update: [2018-09-12][李  磊][创建]
  * 
  */
#if RT_USING_UDP_SERVE
uint8_t rt_UDP_CommunicateServe_thread_start(void)
{
	UDP_CommunicateServe_thread_start(RT_NULL);

	return RT_EOK;
}
INIT_APP_EXPORT(rt_UDP_CommunicateServe_thread_start);
#endif



/*****************************File End**********************************/

