/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      UDP_FinshApp.c
  * @brief:     使用网络接口，UDP协议实现远程登录开发板finsh的应用函数
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

/****************************全局变量***********************************/
struct netconn* g_NetFinshNetconn = NULL;			/* 用于finsh和输出调试打印信息的netconn接口句柄 */
bool NetFinshFlag = false;							/* 用于finsh和输出调试打印信息的网络UDP通道连通标志 */
struct netconn* g_UDP_ServeNetconn = NULL;			/* 用于UDP通信服务的netconn接口句柄 */
bool UDP_ServeFlag = false;							/* 用于UDP通信服务的网络UDP通道连通标志 */
struct netconn* g_MaintenanceServeNetconn = NULL;	/* 用于维护服务的netconn接口句柄 */
bool MaintenanceServe = false;						/* 用于维护服务的网络UDP通道连通标志 */

/* finsh接收FIFO相关 */
FifoHandle* FinshReceiveFifoHandle = NULL;
uint8_t* FinshBuffer = NULL;
PointUint8* FinshBufferPack = NULL;

/* 打印输出FIFO相关 */
FifoHandle* PrintfFifoHandle = NULL;
uint8_t* PrintfBuffer = NULL;
PointUint8* PrintfBufferPack = NULL;

/* UDP通信服务FIFO相关 */
FifoHandle* UDP_ServeFifoHandle = NULL;
uint8_t* UDP_ServeBuffer = NULL;
PointUint8* UDP_ServeBufferPack = NULL;

/* 维护服务FIFO相关 */
FifoHandle* MaintenanceServeFifoHandle = NULL;
uint8_t* MaintenanceServeBuffer = NULL;
PointUint8* MaintenanceServeBufferPack = NULL;


/*****************************Function**********************************/

/**
  * @brief : udp finsh thread IP address set
  * @param : lwip lwip地址设置结构体指针
  * @return: none
  * @update: [2018-09-10][李  磊][创建]
  * 
  */
void DP83848_ServeIpSet(struct lwip_dev* lwip)
{
	uint8_t i = 0;
	
	/*设置远端IP*/
	lwip->remoteip[0] = g_EthDP83848.remoteip[0];
	lwip->remoteip[1] = g_EthDP83848.remoteip[1];
	lwip->remoteip[2] = g_EthDP83848.remoteip[2];
	lwip->remoteip[3] = g_EthDP83848.remoteip[3];
	
	/*设置本地IP*/
	lwip->ip[0] = g_EthDP83848.ip[0];
	lwip->ip[1] = g_EthDP83848.ip[1];
	lwip->ip[2] = g_EthDP83848.ip[2];
	lwip->ip[3] = g_EthDP83848.ip[3];
	
	/*设置子网掩码*/
	lwip->netmask[0] = g_EthDP83848.netmask[0];
	lwip->netmask[1] = g_EthDP83848.netmask[1];
	lwip->netmask[2] = g_EthDP83848.netmask[2];
	lwip->netmask[3] = g_EthDP83848.netmask[3];
	
	/*设置网关*/
	lwip->gateway[0] = g_EthDP83848.gateway[0];
	lwip->gateway[1] = g_EthDP83848.gateway[1];
	lwip->gateway[2] = g_EthDP83848.gateway[2];
	lwip->gateway[3] = g_EthDP83848.gateway[3];
	
	/*设置MAC地址*/
	lwip->mac[0] = g_EthDP83848.mac[0];
	lwip->mac[1] = g_EthDP83848.mac[1];
	lwip->mac[2] = g_EthDP83848.mac[2];
	lwip->mac[3] = g_EthDP83848.mac[3];
	lwip->mac[4] = g_EthDP83848.mac[4];
	lwip->mac[5] = g_EthDP83848.mac[5];
	
	lwip->dhcpstatus = g_EthDP83848.dhcpstatus;
	
}


/**
  * @brief : 使用UDP协议，netconn接口向PC机发送信息
  * @param : udpconn netconn接口的结构体指针
  * @param : sendBuf 要发送的字符串
  * @return: 0：成功  1：传参错误  2：netbuf创建失败或动态内存申请失败  其他：发送错误
  * @update: [2018-09-10][李  磊][创建]
  * 
  */
int8_t UDP_NetconnSendString(struct netconn* udpNetconn, uint8_t* sendString)
{
	struct netbuf* sendNetbuf = NULL;
	void* pointer = NULL;
	int8_t error = 0;

	sendNetbuf = netbuf_new();
	netbuf_alloc(sendNetbuf,strlen((char *)sendString));
	memcpy(sendNetbuf->p->payload,(void*)sendString,strlen((char*)sendString));
	error = netconn_send(udpNetconn, sendNetbuf);
	if(error != ERR_OK)
	{
		rt_kprintf("send error\r\n");
		netbuf_delete(sendNetbuf);
	}
	netbuf_delete(sendNetbuf);

	
	return 0;
}


/**
  * @brief : 使用UDP协议，netconn接口接收来自PC机的字符串
  * @param : udpconn netconn接口的结构体指针
  * @param : fifoHandle 要入的队列指针
  * @return: 接收到的字符数
  * @update: [2018-09-10][李  磊][创建]
  * 
  */
uint32_t UDP_NetconnReceiveString(struct netconn* udpNetConn, FifoHandle* fifoHandle)
{
	static struct netbuf* recvNetBuf = NULL;
	rt_base_t level;			/*开关中断的返回值*/
	struct pbuf* q = NULL;
	uint32_t dataLenth = 0;
	
	netconn_recv(udpNetConn, &recvNetBuf); //接收数据
	if(recvNetBuf != NULL)          //接收到数据
	{
		level = rt_hw_interrupt_disable(); //关中断
		
		for(q=recvNetBuf->p; q!=NULL; q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到fifoHandle中的数据是否大于NET_FINSH_BUFSIZE的剩余空间,如果大于
			//的话就只拷贝fifoHandle中剩余长度的数据,否则的话机拷贝所有的数据
			if(q->len > (NET_FINSH_BUFSIZE-dataLenth))
			{
				FifoStringEnqueue(fifoHandle, q->payload, (NET_FINSH_BUFSIZE-dataLenth));
			}
			else
			{
				FifoStringEnqueue(fifoHandle, q->payload, q->len);
			}
			dataLenth += q->len;	
			if(dataLenth > NET_FINSH_BUFSIZE)
			{
				dataLenth = NET_FINSH_BUFSIZE;
				break; //超出TCP客户端接收数组,跳出
			}
		}
		rt_hw_interrupt_enable(level);  //开中断
		netbuf_delete(recvNetBuf);      //删除buf
	}
	return dataLenth;
}


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
  * @brief : 使用UDP的打印函数
  * @param : 打印的信息
  * @return: none
  * @update: [2018-09-11][李  磊][创建]
  */
void NetFinsh_kprintf(const char *fmt, ...)
{
	va_list args;
    rt_size_t length;
    char rt_log_buf[RT_CONSOLEBUF_SIZE] = {0};

    va_start(args, fmt);
	
	length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;

	if(true == NetFinshFlag)		/*UDP_FinshFlag为1说明UDP打印已经初始化，可以使用了*/
	{
		FifoStringEnqueue(PrintfFifoHandle, (uint8_t*)rt_log_buf, length);
	}
	else
	{
		#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
			static rt_device_t _console_device = RT_NULL;
		#endif

		_console_device = rt_console_get_device();			/*获取当前控制台输出的串口设备号*/
		rt_uint16_t old_flag = _console_device->open_flag;

        _console_device->open_flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(_console_device, 0, rt_log_buf, length);
        _console_device->open_flag = old_flag;
		
	}
	
	va_end(args);
}




/*****************************File End**********************************/


