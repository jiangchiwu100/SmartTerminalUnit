/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshThread.c
  * @brief:     使用网络接口，UDP实现finsh，上位机下发操作命令，下载配置文件的相关任务
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






/*******************************Function********************************/

/**
  * @brief : 使用dp83848和UDP协议完成finsh远程登录和打印输出的功能
  * @param : none
  * @return: none
  * @update: [2018-09-10][李  磊][创建]
  * 		 [2018-10-09][李  磊][改为socket接口实现]
  * 
  */  
#if RT_USING_NET_FINSH
static void rt_net_finsh_thread_entry(void *param)
{
	int32_t receiveNum;		//接收到的字节数
	uint8_t buffer[PRINT_BUFFER_SIZE] = {0};		//接收和发送缓冲区
	uint32_t addressLenth;		//地址长度
	struct sockaddr_in localAddress, remoteAddress;		//本地IP地址和远程IP地址
	uint8_t ret = 0;			//函数执行的返回值,用于判断是否执行成功
	uint32_t i = 0;

	/* 本地和远程IP、端口的设置,socket的建立和绑定 */
	IpAddressInit(&localAddress, FINSH_LOCAL_PORT, &remoteAddress, FINSH_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_NetFinshSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);
	
	/* 接收FIFO和发送FIFO申请动态内存以及初始化 */
	ret += FifoMallocAndInit(&FinshReceiveFifoHandle, &FinshBuffer, NET_FINSH_BUFSIZE, &FinshBufferPack);
	ret += FifoMallocAndInit(&PrintfFifoHandle, &PrintfBuffer, NET_PRINTF_BUFSIZE, &PrintfBufferPack);
	if(ret)		//上边三个函数执行成功返回0,执行失败返回值大于0,只要有一个失败,则表示失败
	{
		FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/* fifo创建失败,则进行释放 */
		FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
		rt_kprintf("Net Finsh Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}
	
	/*UDP链接已经创建，之后可以使用网口的打印函数了*/
	NetFinshFlag = true;
	rt_kprintf("Net finsh Init Success");			
	
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
		receiveNum = lwip_recvfrom(g_NetFinshSocket, buffer, PRINT_BUFFER_SIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < PRINT_BUFFER_SIZE))
		{
			/* 将接收到的数据入队，等待finsh读取 */
			FifoStringEnqueue(FinshReceiveFifoHandle, buffer, receiveNum);
		}
		rt_thread_delay(10);		/* 释放cpu资源，让低优先级任务得以运行 */
	}
	
	lwip_close(g_NetFinshSocket);
	FifoFree(&FinshReceiveFifoHandle, &FinshBuffer, &FinshBufferPack);		/*释放接收和发送的队列所用到的动态分配的内存*/
	FifoFree(&PrintfFifoHandle, &PrintfBuffer, &PrintfBufferPack);
	NetFinshFlag = false;
	return;
}
#endif	/* RT_USING_NET_FINSH */


/**
  * @brief : 使用dp83848和UDP协议完成上位机下发配置的功能
  * @param : none
  * @return: none
  * @update: [2018-09-12][李  磊][创建]
  * 		 [2018-10-09][李  磊][改为socket接口实现]
  * 
  */
#if RT_USING_UDP_SERVE
static void rt_udp_serve_thread_entry(void *param)
{
	int32_t receiveNum;		//接收到的字节数
	uint8_t buffer[UDP_SERVE_BUFSIZE] = {0};		//接收和发送缓冲区
	uint32_t addressLenth;		//地址长度
	struct sockaddr_in localAddress, remoteAddress;		//本地IP地址和远程IP地址
	uint8_t ret = 0;			//函数执行的返回值,用于判断是否执行成功
	uint32_t i = 0;
    
	/* 本地和远程IP、端口的设置,socket的建立和绑定 */
	IpAddressInit(&localAddress, UDP_SERVE_LOCAL_PORT, &remoteAddress, UDP_SERVE_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_UDP_ServeSocket, (struct sockaddr*)&localAddress);
    
    
	addressLenth = sizeof(struct sockaddr);

	/* 上位机下发配置的FIFO申请动态内存以及初始化 */
	ret += FifoMallocAndInit(&UDP_ServeFifoHandle, &UDP_ServeBuffer, UDP_SERVE_BUFSIZE, &UDP_ServeBufferPack);
	if(ret)		//上边三个函数执行成功返回0,执行失败返回值大于0,只要有一个失败,则表示失败
	{
		FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);		/* fifo创建失败,则进行释放 */
		rt_kprintf("Udp Serve Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}

	/*上位机下发配置的端口已经创建*/
	UDP_ServeFlag = true;
//	rt_kprintf("\nUDP Communbicate Serve Init Success");

	while (1)
	{
		/* 接收数据 */
		memset(buffer, 0, UDP_SERVE_BUFSIZE);
		receiveNum = lwip_recvfrom(g_UDP_ServeSocket, buffer, UDP_SERVE_BUFSIZE, 0, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < UDP_SERVE_BUFSIZE))
		{
			StationPointFrameDeal(buffer, receiveNum);
		}
		rt_thread_delay(10);		/* 释放cpu资源，让低优先级任务得以运行 */
	}

	lwip_close(g_UDP_ServeSocket);
	FifoFree(&UDP_ServeFifoHandle, &UDP_ServeBuffer, &UDP_ServeBufferPack);	/*释放接收和发送的队列所用到的动态分配的内存*/
	UDP_ServeFlag = false;
	return;
}
#endif	/* RT_USING_UDP_SERVE */

/**
  * @brief : dp82848实现的维护服务的任务入口
  * @param : none
  * @return: none
  * @update: [2018-09-17][李  磊][创建]
  * 		 [2018-10-09][李  磊][改为socket接口实现]
  * 
  */
static void rt_maintenance_serve_thread_entry(void *param)
{
	int32_t receiveNum;		//接收到的字节数
	uint8_t buffer[MAINTENANCE_SERVE_BUFSIZE] = {0};		//接收和发送缓冲区
	uint32_t addressLenth;		//地址长度
	struct sockaddr_in localAddress, remoteAddress;		//本地IP地址和远程IP地址
	uint8_t ret = 0;			//函数执行的返回值,用于判断是否执行成功
	uint32_t i = 0;

	/* 本地和远程IP、端口的设置,socket的建立和绑定 */
	IpAddressInit(&localAddress, MAINTACE_SERVE_LOCAL_PORT, &remoteAddress, MAINTACE_SERVE_REMOTE_PORT, REMOTE_ADDRESS);
	ret += UdpSocketInit(&g_MaintenanceServeSocket, (struct sockaddr*)&localAddress);
	addressLenth = sizeof(struct sockaddr);

	/* 上位机下发配置的FIFO申请动态内存以及初始化 */
	ret += FifoMallocAndInit(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer,
							MAINTENANCE_SERVE_BUFSIZE, &MaintenanceServeBufferPack);	/*初始化fifo*/
	if(ret)		//上边三个函数执行成功返回0,执行失败返回值大于0,只要有一个失败,则表示失败
	{
		FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);//fifo创建失败,则进行释放
		rt_kprintf("Maintenance Serve Thread: Fifo Malloc And Init Faliure\r\n");
		return;
	}

	/*上位机下发配置文件的维护端口已经创建*/
	MaintenanceServe = true;
	g_StationManger.isMaintanceRun = true;
	rt_kprintf("UDP Maintenance Serve Init Success\r\n");

	while (1)
	{
		/* 接收数据 */
		memset(buffer, 0, MAINTENANCE_SERVE_BUFSIZE);
		receiveNum = lwip_recvfrom(g_MaintenanceServeSocket, buffer, MAINTENANCE_SERVE_BUFSIZE, MSG_DONTWAIT, (struct sockaddr*)&remoteAddress, &addressLenth);
		if((receiveNum > 0) && (receiveNum < MAINTENANCE_SERVE_BUFSIZE))
		{
			MantaiceFrameDeal(buffer, receiveNum);
		}
		rt_thread_delay(10);		/* 释放cpu资源，让低优先级任务得以运行 */
	}

	lwip_close(g_MaintenanceServeSocket);
	FifoFree(&MaintenanceServeFifoHandle, &MaintenanceServeBuffer, &MaintenanceServeBufferPack);//fifo创建失败,则进行释放
	MaintenanceServe = false;
	g_StationManger.isMaintanceRun = false;
	return;
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
        
        THREAD_PRINTF("UDP Communicate serve thread start"); 
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
  * @brief : 使用dp83848和UDP协议完成finsh远程登录和打印输出的功能
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
  * @brief : 使用dp83848和UDP协议完成上位机下发配置的功能
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

