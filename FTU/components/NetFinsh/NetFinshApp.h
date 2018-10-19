/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     使用网络接口，UDP实现finsh，上位机下发操作命令，下载配置文件的相关应用函数
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
  
#ifndef __NET_FINSH_APP_H__
#define __NET_FINSH_APP_H__

#include "common_data.h"
#include "lwip/netbuf.h"
#include "lwip/api.h"
#include "buffer.h"
#include <stdbool.h>
#include <sys/types.h>
#include <lwip/netdb.h>

/*****************************宏定义************************************/
#define REMOTE_ADDRESS 					"192.168.10.111"		/*远程IP的地址*/
#define FINSH_LOCAL_PORT				5533					//finsh和网口打印输出的本地端口
#define FINSH_REMOTE_PORT				FINSH_LOCAL_PORT		//finsh接收命令的远端端口
#define UDP_SERVE_LOCAL_PORT			5555					//UDP通信服务本地端口号
#define UDP_SERVE_REMOTE_PORT			UDP_SERVE_LOCAL_PORT	//UDP通信服务远端端口号
#define MAINTACE_SERVE_LOCAL_PORT		5500					//维护服务本地端口号
#define MAINTACE_SERVE_REMOTE_PORT		5555					//维护服务远端端口号

#define PRINT_BUFFER_SIZE				512						//打印输出的缓冲区大小
#define NET_FINSH_BUFSIZE				512						//finsh的FIFO接收缓冲区大小
#define NET_PRINTF_BUFSIZE				2048					//打印的FIFO输出缓冲区大小
#define UDP_SERVE_BUFSIZE				1024					//UDP通信服务的FIFO缓冲区大小
#define MAINTENANCE_SERVE_BUFSIZE		1024					//维护服务的FIFO缓冲区大小

extern int32_t g_NetFinshSocket;
extern int32_t g_UDP_ServeSocket;
extern int32_t g_MaintenanceServeSocket;

extern bool NetFinshFlag;
extern bool UDP_ServeFlag;
extern bool MaintenanceServe;

extern FifoHandle* FinshReceiveFifoHandle;
extern uint8_t* FinshBuffer;
extern PointUint8* FinshBufferPack;
extern FifoHandle* PrintfFifoHandle;
extern uint8_t* PrintfBuffer;
extern PointUint8* PrintfBufferPack;
extern FifoHandle* UDP_ServeFifoHandle;
extern uint8_t* UDP_ServeBuffer;
extern PointUint8* UDP_ServeBufferPack;
extern FifoHandle* MaintenanceServeFifoHandle;
extern uint8_t* MaintenanceServeBuffer;
extern PointUint8* MaintenanceServeBufferPack;



void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack); //dp83848相关服务的队列动态内存释放
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack);  //队列初始化
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);    //字符串入队操作
char FifoCharDequeue(FifoHandle *handle);    //字符出队操作
char NetGetchar(void);    //获取一个字符,在finsh中使用,代替getchar函数
int32_t UDP_SocketSendString(int32_t socket, uint8_t* data, uint32_t lenth,
	uint8_t* remoteIP, uint32_t remotePort);		//udp使用socket接口进行发送数据的函数
void IpAddressInit(struct sockaddr_in* localAddress, uint32_t localPort,
	struct sockaddr_in* remoteAddress, uint32_t remotePort, uint8_t* remoteAddressString);	//IP地址设置
uint32_t UdpSocketInit(uint32_t* socketNum, struct sockaddr* socketAddress);	//使用UDP的socket建立和绑定
void ifconfig(void);	//打印ip地址等信息的finsh命令


#endif

