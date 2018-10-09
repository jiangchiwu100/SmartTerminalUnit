/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.h
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的应用函数
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


#define NET_FINSH_BUFSIZE		512			//finsh的FIFO接收缓冲区大小
#define NET_PRINTF_BUFSIZE		2048		//打印的FIFO输出缓冲区大小
#define UDP_SERVE_BUFSIZE		1024		//UDP通信服务的FIFO缓冲区大小
#define MAINTENANCE_SERVE_BUFSIZE	1024	//维护服务的FIFO缓冲区大小

extern struct netconn* g_NetFinshNetconn;
extern bool NetFinshFlag;
extern struct netconn* g_UDP_ServeNetconn;
extern bool UDP_ServeFlag;
extern struct netconn* g_MaintenanceServeNetconn;
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

extern uint32_t g_NetFinshSocket;
extern uint32_t g_UDP_ServeSocket;
extern uint32_t g_MaintenanceServeSocket;


void DP83848_ServeIpSet(struct lwip_dev* lwip);  //dp83848相关服务IP地址设置
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString);  //UDP发送字符串
uint32_t UDP_NetconnReceiveString(struct netconn* udpNetConn, FifoHandle* fifoHandle);   //UDP接收字符串
void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack); //dp83848相关服务的队列动态内存释放
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack);  //队列初始化
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size);    //字符串入队操作
char FifoCharDequeue(FifoHandle *handle);    //字符出队操作
char NetGetchar(void);    //获取一个字符,在finsh中使用,代替getchar函数
void UDP_SocketSendString(uint8_t* remoteAddressString, uint32_t port, uint8_t* sendData);	//udp使用socket接口进行发送数据的函数
//void NetFinsh_kprintf(const char *fmt, ...);


#endif

