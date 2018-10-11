/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     使用网络接口，UDP实现finsh，上位机下发操作命令，下载配置文件的相关应用函数
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2018-09-06
  * @update:    
  */
 

/*****************************include***********************************/
#include "NetFinshApp.h"
#include <string.h>
#include "board.h"
#include "rtdef.h"
#include "stm32f429xx.h"
#include <lwip/sockets.h>
#include <finsh.h>


/****************************全局变量***********************************/
int32_t g_NetFinshSocket = 0;				/* finsh的socket号 */
int32_t g_UDP_ServeSocket = 0;				/* 上位机下发操作命令的socket号 */
int32_t g_MaintenanceServeSocket = 0;		/* 上位机下发配置文件的socket号 */

bool NetFinshFlag = false;							/* 用于finsh和输出调试打印信息的网络UDP通道连通标志 */
bool UDP_ServeFlag = false;							/* 用于UDP通信服务的网络UDP通道连通标志 */
bool MaintenanceServe = false;						/* 用于维护服务的网络UDP通道连通标志 */

/* finsh接收FIFO相关 */
FifoHandle* FinshReceiveFifoHandle = NULL;
uint8_t* FinshBuffer = NULL;
PointUint8* FinshBufferPack = NULL;

/* 打印输出FIFO相关 */
FifoHandle* PrintfFifoHandle = NULL;
uint8_t* PrintfBuffer = NULL;
PointUint8* PrintfBufferPack = NULL;

/* 上位机下发操作命令服务的FIFO相关 */
FifoHandle* UDP_ServeFifoHandle = NULL;
uint8_t* UDP_ServeBuffer = NULL;
PointUint8* UDP_ServeBufferPack = NULL;

/* 上位机下发配置文件的维护服务FIFO相关 */
FifoHandle* MaintenanceServeFifoHandle = NULL;
uint8_t* MaintenanceServeBuffer = NULL;
PointUint8* MaintenanceServeBufferPack = NULL;



/*****************************Function**********************************/

/**
  * @brief : 使用dp83848通信服务中的队列动态内存释放
  * @param : fifoHandle 需要释放内存的FIFO的句柄
  * @param : buffer 对应的队列的缓冲区
  * @param : bufferPack 对应的缓冲区结构体
  * @return: none
  * @update: [2018-09-17][李  磊][创建]
  * 
  */
void FifoFree(FifoHandle** fifoHandle, uint8_t** buffer, PointUint8** bufferPack)
{
	if(NULL != *fifoHandle)
	{
		rt_free(*fifoHandle);
		*fifoHandle = NULL;
	}
	if(NULL != *buffer)
	{
		rt_free(*buffer);
		*buffer = NULL;
	}
	if(NULL != *bufferPack)
	{
		rt_free(*bufferPack);
		*bufferPack = NULL;
	}
}


/**
  * @brief : 使用dp83848通信的FIFO初始化
  * @param : fifoHandle FIFO的句柄
  * @param : buffer 队列的缓冲区
  * @param : bufSize 缓冲区大小
  * @param : bufferPack 缓冲区结构体,供初始化FIFO使用
  * @return: 0:成功    1:失败
  * @update: [2018-09-17][李  磊][创建]
  * 
  */
uint8_t FifoMallocAndInit(FifoHandle** fifoHandle, uint8_t** buffer, uint32_t bufSize, PointUint8** bufferPack)
{
	uint8_t err = 0;
	*fifoHandle = rt_malloc(sizeof(FifoHandle));
	if(NULL == fifoHandle)
	{
		rt_kprintf("%s:[%s] #-%d: fifoHandle Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	*buffer = rt_malloc(sizeof(uint8_t) * bufSize);
	if(NULL == buffer)
	{
		rt_kprintf("%s:[%s] #-%d: buffer Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}
	*bufferPack = rt_malloc(sizeof(PointUint8));
	if(NULL == bufferPack)
	{
		rt_kprintf("%s:[%s] #-%d: bufferPack Malloc Fail\r\n", __FILE__, __FUNCTION__, __LINE__);
		err = 1;
	}

	if(0 == err)
	{
		(*bufferPack)->len = bufSize;
		(*bufferPack)->pData = *buffer;
		FifoInit(*fifoHandle, *bufferPack);
	}
	else
	{
		FifoFree(fifoHandle, buffer, bufferPack);
	}

	return err;
}


/**
  * @brief : finsh接收到的多个字符入队
  * @param : handle FIFO句柄
  * @param : indata 要入队的字符串
  * @param : size 要入队的字节大小
  * @return: none
  * @update: [2018-09-11][李  磊][创建]
  */
void FifoStringEnqueue(FifoHandle *handle, uint8_t* indata, uint32_t size)
{
	uint32_t i = 0;
	for(i=0; i<size; i++)
	{
		handle->Enqueue(handle, indata[i]);
	}
}


/**
  * @brief : finsh的FIFO出队
  * @param : handle FIFO句柄
  * @return: 出队的字符
  * @update: [2018-09-11][李  磊][创建]
  */
char FifoCharDequeue(FifoHandle *handle)
{
	uint8_t dedata = 0;
	uint8_t ret = 0;
	ret = handle->Dequeue(handle, &dedata);
	if(false == ret)
	{
		return 0;
	}
	else if(true == ret)
	{
		return dedata;
	}
}


/**
  * @brief : 使用UDP获取字符
  * @param : none
  * @return: 出队的字符
  * @update: [2018-09-10][李  磊][创建]
  */
char NetGetchar(void)
{
	return FifoCharDequeue(FinshReceiveFifoHandle);
}


/**
  * @brief : 使用UDP的发送函数
  * @param : socket 发送时使用的socket号
  * @param : data 发送的数据
  * @param : lenth 发送的数据长度
  * @param : remoteAddressString 远程IP地址的字符串形式,例如“192.168.10.111”
  * @param : remotePort 远程端口号
  * @return: -1：发送错误	其他：发送完成的字节数
  * @update: [2018-10-09][李  磊][创建]
  */
int32_t UDP_SocketSendString(int32_t socket, uint8_t* data, uint32_t lenth, uint8_t* remoteAddressString, uint32_t remotePort)
{
	int32_t sendNum = 0;
	struct hostent* host;
	struct sockaddr_in remoteAddress;

	if(socket <= 0)
	{
		rt_kprintf("UDP Send Error: Socket is Invalid\r\n");
		return -1;
	}

	/* 通过函数入口参数获得host地址（如果是域名，会做域名解析） */
	host = (struct hostent*)gethostbyname(remoteAddressString);

	/* 初始化预连接的服务端地址 */
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_port = htons(remotePort);
	remoteAddress.sin_addr = *((struct in_addr *) host->h_addr);
	memset(&(remoteAddress.sin_zero), 0, sizeof(remoteAddress.sin_zero));

	/* 发送数据到服务远端 */
	sendNum = lwip_sendto(socket, data, lenth, 0, (struct sockaddr*)&remoteAddress, sizeof(struct sockaddr));
	
	return sendNum;
}


/**
  * @brief : 使用UDP的IP地址设置
  * @param : localAddress 本地IP地址的socket结构体指针,可以赋值为NULL
  * @param : localPort 本地端口,当localAddress为NULL，则此值不起作用
  * @param : remoteAddress 远程IP地址的socket结构体指针,可以赋值为NULL
  * @param : remotePort 远程端口,当remoteAddress为NULL，则此值不起作用
  * @param : remoteAddressString 远程IP地址的字符串形式,例如“192.168.10.111”,当remoteAddress为NULL，则此值不起作用
  * @return: none
  * @update: [2018-10-09][李  磊][创建]
  */
void IpAddressInit(struct sockaddr_in* localAddress, uint32_t localPort,
	struct sockaddr_in* remoteAddress, uint32_t remotePort, uint8_t* remoteAddressString)
{
	struct hostent* host;
	
	if(NULL != localAddress)
	{
		/* 初始化本地地址 */
		localAddress->sin_family = AF_INET;
		localAddress->sin_port = htons(localPort);
		localAddress->sin_addr.s_addr = INADDR_ANY;
		memset(&(localAddress->sin_zero), 0, sizeof(localAddress->sin_zero));
	}
	
	if(NULL != remoteAddress)
	{
		/* 初始化远程地址 */
		host = (struct hostent*)gethostbyname(remoteAddressString);
		remoteAddress->sin_family = AF_INET;
		remoteAddress->sin_port = htons(remotePort);
		remoteAddress->sin_addr = *((struct in_addr *) host->h_addr);
		memset(&(remoteAddress->sin_zero), 0, sizeof(remoteAddress->sin_zero));
	}
	
}


/**
  * @brief : 使用UDP的socket建立和绑定
  * @param : socketNum socket号的指针,作为输出参数
  * @param : socketAddress 本地地址的指针
  * @return: 0：成功	1：socket创建失败	2：绑定失败
  * @update: [2018-10-09][李  磊][创建]
  */
uint32_t UdpSocketInit(uint32_t* socketNum, struct sockaddr* socketAddress)
{
	/* 创建一个socket，类型是SOCK_DGRAM，UDP类型 */
	if ((*socketNum = lwip_socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		rt_kprintf("g_NetFinshSocket lwip_socket error\n");
		return 1;
	}
	
	/* 绑定socket到服务端地址 */
	if (lwip_bind(*socketNum, socketAddress, sizeof(struct sockaddr))== -1)
	{
		/* 绑定地址失败 */
		rt_kprintf("Bind error\n");
		return 2;
	}
	
	return 0;
}


/**
  * @brief : 打印ip地址等信息的finsh命令
  * @return: none
  * @update: [2018-10-11][李  磊][创建]
  */
void ifconfig(void)
{
	rt_kprintf("DP83848:\r\n");
	rt_kprintf("  IPv4 Address: %d.%d.%d.%d\r\n", g_EthDP83848.ip[0], g_EthDP83848.ip[1],
												  g_EthDP83848.ip[2], g_EthDP83848.ip[3]);
	rt_kprintf("  MAC: %X:%X:%X:%X:%X:%X\r\n", g_EthDP83848.mac[0], g_EthDP83848.mac[1], g_EthDP83848.mac[2],
											   g_EthDP83848.mac[3], g_EthDP83848.mac[4], g_EthDP83848.mac[5]);
	rt_kprintf("  Netmask: %d.%d.%d.%d\r\n", g_EthDP83848.netmask[0], g_EthDP83848.netmask[1],
											 g_EthDP83848.netmask[2], g_EthDP83848.netmask[3]);
	rt_kprintf("  Gateway: %d.%d.%d.%d\r\n", g_EthDP83848.gateway[0], g_EthDP83848.gateway[1],
											 g_EthDP83848.gateway[2], g_EthDP83848.gateway[3]);
	rt_kprintf("  DNS: %d.%d.%d.%d\r\n", g_EthDP83848.dns[0], g_EthDP83848.dns[1],
									   	 g_EthDP83848.dns[2], g_EthDP83848.dns[3]);
	rt_kprintf("  Remote IP: %s\r\n", REMOTE_ADDRESS);

	rt_kprintf("W5500:\r\n");
	rt_kprintf("  IPv4 Address: %d.%d.%d.%d\r\n", g_EthW5500.ip[0], g_EthW5500.ip[1],
												  g_EthW5500.ip[2], g_EthW5500.ip[3]);
	rt_kprintf("  MAC: %X:%X:%X:%X:%X:%X\r\n", g_EthW5500.mac[0], g_EthW5500.mac[1], g_EthW5500.mac[2],
											   g_EthW5500.mac[3], g_EthW5500.mac[4], g_EthW5500.mac[5]);
	rt_kprintf("  Netmask: %d.%d.%d.%d\r\n", g_EthW5500.netmask[0], g_EthW5500.netmask[1],
										   	 g_EthW5500.netmask[2], g_EthW5500.netmask[3]);
	rt_kprintf("  Gateway: %d.%d.%d.%d\r\n", g_EthW5500.gateway[0], g_EthW5500.gateway[1],
										   	 g_EthW5500.gateway[2], g_EthW5500.gateway[3]);
	rt_kprintf("  DNS: %d.%d.%d.%d\r\n", g_EthW5500.dns[0], g_EthW5500.dns[1],
									   	 g_EthW5500.dns[2], g_EthW5500.dns[3]);

}
FINSH_FUNCTION_EXPORT(ifconfig, net massage)


/**
  * @brief : 修改打印输出为网口或串口
  * @return: none
  * @update: [2018-10-11][李  磊][创建]
  */
void printfset(uint8_t* argv)
{
	if(argv == NULL)
	{
		if(true == NetFinshFlag)
		{
			rt_kprintf("The rt_kprintf function of the current device is network output\r\n");
			rt_kprintf("You can use the command 'printfset(\"-s\")' to set device to serial output\r\n");
		}
		else
		{
			rt_kprintf("The rt_kprintf function of the current device is serial output\r\n");
			rt_kprintf("You can use the command 'printfset(\"-n\")' to set device to network output\r\n");
		}
	}
	else
	{
		if(!strcmp(argv, "-s"))
		{
			rt_kprintf("SET OK!\r\n");
			NetFinshFlag = false;
			rt_kprintf("\r\nThe rt_kprintf function of the current device is serial output\r\n");
		}
		else if(!strcmp(argv, "-n"))
		{
			rt_kprintf("SET OK!\r\n");
			NetFinshFlag = true;
			rt_kprintf("\r\nThe rt_kprintf function of the current device is network output\r\n");
		}
		else
		{
			rt_kprintf("usage:\r\n");
			rt_kprintf("\tprintfset(\"-s\") : set serial output\r\n");
			rt_kprintf("\tprintfset(\"-n\") : set network output\r\n");
		}
	}

	return;
}
FINSH_FUNCTION_EXPORT(printfset, printf output set)

/*****************************File End**********************************/


