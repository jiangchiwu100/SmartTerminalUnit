#include "ethernet_hook.h"

#include "distribution_enum.h"
#include "common_def.h"

#include "string.h"

#include "rtthread.h"
#include "extern_interface.h"
#include "RingQueue.h"
//#define EXSIT_VLAN_TAG 1

#if EXSIT_VLAN_TAG

#define TPID_INDEX 12  //0x81
#define ETHERNET_TYPE_INDEX 16  

#else

#define ETHERNET_TYPE_INDEX 12  //0x81

#endif

#define TPID_LOW                 0x81  //0x81
#define TPID_HIGHT               0x00  //0x81
#define ETHERNET_TYPE_LOW        0x88  //0x81
#define ETHERNET_TYPE_HIGHT      0xB8  //0x81


#define MAX_RECIVE_COUNT 1518
//static uint8_t EthernetReciveBuffer[MAX_RECIVE_COUNT]; 
//static uint16_t EthernetReciveCount;

/**
*以太网发送使用的互斥信号量
*/
static struct rt_mutex ethernet_mutex;
rt_mutex_t g_ethernet_mutex;
/**
*以太网帧接收使用信号量
*/
static rt_sem_t mac_raw_sem;

/**
*动态分配内存池，静态使用
*/
static RingQueuePool* RingPool;
/**
*消息邮箱
*/
static  rt_mailbox_t MacRawReciveMb;


static PointUint8*  MakePacketMacRawMessage(uint8_t *pData, uint16_t len);



/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: true--发送成功
* @update: [2018-08-2][张宇飞][]
*[2018-08-06][张宇飞][添加邮箱发送，返回值为true]
*[2018-08-28][张宇飞][适应有无VLAN标记两种情况]
*/
bool EthernetInput(uint8_t* pData, uint16_t len)
{
    uint16_t bufPos = 0;
    if (pData == NULL)
    {
        return false;
    }

    bufPos = 12;

    //检测TPID是否是0x8100
    if ((pData[bufPos] == 0x81) && (pData[bufPos + 1] == 0x00)) 
    {
        bufPos += 4; /* skip VLAN tag */        
    }
    
     //检测协议类型是否是0x88B8
    if((pData[bufPos++] != ETHERNET_TYPE_LOW) 
        || (pData[bufPos++] != ETHERNET_TYPE_HIGHT))
    {
       return false;
    }   
    
    if (len > MAX_RECIVE_COUNT)
    {
        return false;
    }
  
	PointUint8* pPacket = MakePacketMacRawMessage(pData, len);
	if (!pPacket)
	{
		perror("MakePacketMacRawMessage Make failure\n");
        return false;
	}

    
    
	rt_err_t err = rt_mb_send(MacRawReciveMb, (rt_uint32_t)(pPacket));
	if (err != RT_EOK)
	{
		perror("rt_mb_send(MacRawReciveMb, (rt_uint32_t)(pPacket)), error: %d.\n", err);
        return false;
	}
    return true;
    

}

#ifdef USE_PBUF
/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: true--发送成功
* @update: [2018-09-13][张宇飞][]
*/
bool EthernetInputPbuf(struct pbuf *p, uint16_t len)
{
    uint8_t* pData = p->payload;
    uint16_t bufPos = 0;
    if (pData == NULL)
    {
        return false;
    }

    bufPos = 12;

    //检测TPID是否是0x8100
    if ((pData[bufPos] == 0x81) && (pData[bufPos + 1] == 0x00)) 
    {
        bufPos += 4; /* skip VLAN tag */        
    }
    
     //检测协议类型是否是0x88B8
    if((pData[bufPos++] != ETHERNET_TYPE_LOW) 
        || (pData[bufPos++] != ETHERNET_TYPE_HIGHT))
    {
       return false;
    }   
    
    if (len > MAX_RECIVE_COUNT)
    {
        return false;
    }
    
	    
	rt_err_t err = rt_mb_send(MacRawReciveMb, (rt_uint32_t)(p));
	if (err != RT_EOK)
	{
		perror("rt_mb_send(MacRawReciveMb, (rt_uint32_t)(pPacket)), error: %d.\n", err);
        return false;
	}
    return true;
    

}

#endif

/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: true--发送成功
* @update: [2018-09-13][张宇飞][]
*/
bool EthernetInputPool(uint8_t* pData, uint16_t len)
{
    uint16_t bufPos = 0;
    if (pData == NULL)
    {
        return false;
    }

    bufPos = 12;

    //检测TPID是否是0x8100
    if ((pData[bufPos] == 0x81) && (pData[bufPos + 1] == 0x00))
    {
        bufPos += 4; /* skip VLAN tag */
    }

     //检测协议类型是否是0x88B8
    if((pData[bufPos++] != ETHERNET_TYPE_LOW)
        || (pData[bufPos++] != ETHERNET_TYPE_HIGHT))
    {
       return false;
    }

    if (len > MAX_RECIVE_COUNT)
    {
        return false;
    }
    
   
    bool reslut = RingQueuePool_Write(RingPool, pData, len);

	if (!reslut)
	{
		perror("RingQueuePool_Write failure\n");
        return false;
	}

	rt_err_t err = rt_sem_release(mac_raw_sem);
	if (err != RT_EOK)
	{
		perror("rt_sem_release(mac_raw_sem, (rt_uint32_t)(RingPool)), error: %d.\n", err);
        return false;
	}
    return true;


}


/**
* @brief : 生成m打包信息，有动态内存分配
* @param : uint8_t *pData 数据指针
* @param : uint16_t len 数据长度
* @return: PointUint8* pPacket 数据
* @update: [2018-08-2][张宇飞][]
*/
static PointUint8*  MakePacketMacRawMessage(uint8_t *pData, uint16_t len)
{
    PointUint8* pPacket = (PointUint8*)MALLOC(sizeof(PointUint8));
    if (!pPacket)
    {
        return NULL;
    }
    pPacket->pData = (uint8_t*)MALLOC(len * sizeof(uint8_t));
    if (!pPacket->pData)
    {
        FREE(pPacket);
        return NULL;
    }
    MEMCPY(pPacket->pData, pData, len);
    pPacket->len = len;
    return pPacket;
}

/**
* @brief :mac 阻塞输入,有内存释放
* @param :uint8_t* pData 保存的地址
* @param :size 缓冲尺寸
* @return: uint16_t 实际接收尺寸
* @update: [2018-08-03][张宇飞][]
*[2018-08-06][张宇飞][修改形参为]
*/
uint16_t MacRawInputBlock(uint8_t* pData, uint16_t size)
{
#if defined( USE_PBUF)
    struct pbuf *p;
    uint16_t count;
	rt_err_t err = rt_mb_recv(MacRawReciveMb, (rt_uint32_t*)(&p), RT_WAITING_FOREVER);    
    if (err == RT_EOK)
	{
        count = p->len;
		if (count > size || count== 0)
		{
			return 0;
		}

		MEMCPY(pData, p->payload, count);
        pbuf_free(p);
        
		return count;		
	}
    
#elif defined( RING_MEM_POOL)
    PointUint8 out;
    out.len = size;
    out.pData = pData;
    rt_err_t err = rt_sem_take(mac_raw_sem, RT_WAITING_FOREVER);

    if (err == RT_EOK)
	{
    	bool reslut = RingQueuePool_Read(RingPool, &out);

		if (!reslut)
		{
			perror("RingQueuePool_Read failure\n");
			return 0;
		}
		return out.len;
	}


#else
	PointUint8* pPacket;
    uint16_t count;
	rt_err_t err = rt_mb_recv(MacRawReciveMb, (rt_uint32_t*)(&pPacket), RT_WAITING_FOREVER);
	if (err == RT_EOK)
	{
        count = pPacket->len;
		if (count > size || count== 0)
		{
			return 0;
		}

		MEMCPY(pData, pPacket->pData, pPacket->len);
        FREE(pPacket->pData);
        FREE(pPacket);
        
		return count;		
	}
#endif
    
	return 0;
}




/**
* @brief : 上锁
* @param : void
* @return: void
* @update: [2018-08-2][张宇飞][]
*/
void EhernetOuputMutex_OnLock(void)   
{
    if (g_ethernet_mutex) 
    {        
        rt_err_t result = rt_mutex_take(g_ethernet_mutex, RT_WAITING_FOREVER); 
        if (result) 
            rt_kprintf("error:rt_mutex_take\n");
    }
    else
    {
        rt_kprintf("error:EhernetOuputMutex_OnLock g_ethernet_mutex is null\n");
    }

}

/**
* @brief : 解锁
* @param : void
* @return: void
* @update: [2018-08-2][张宇飞][]
*/
void EhernetOuputMutex_OffLock(void) 
{
    if (g_ethernet_mutex)
    {
        rt_mutex_release(g_ethernet_mutex);
    }
    else
    {
        rt_kprintf("error:EhernetOuputMutex_OffLock g_ethernet_mutex is null\n");
    }
}



/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: void
* @update: [2018-08-2][张宇飞][]
*[2018-08-06][张宇飞][添加接收邮箱]
*[2018-08-16][张宇飞][修改邮箱容量100->500]
*/

PointUint8 point;
uint8_t tx[64];
uint8_t rx[64];

void EthernetHookInit(void)
{

    rt_err_t err = rt_mutex_init (&ethernet_mutex, "udp_mutex", RT_IPC_FLAG_PRIO );
    if (err != RT_EOK)
    {
        rt_kprintf("rt_mutex_init g_ethernet_mutex err != RT_EOK");
        g_ethernet_mutex = NULL;
    }
    else
    {
        g_ethernet_mutex = &ethernet_mutex;
    }


    MacRawReciveMb = rt_mb_create ("macraw", 500, RT_IPC_FLAG_FIFO);
    if (!MacRawReciveMb)
    {
    	perror("rt_mb_create failure\n");

    }
    mac_raw_sem = rt_sem_create("macraw", 0, RT_IPC_FLAG_FIFO);
    if (!mac_raw_sem)
	{
		perror("rt_sem_create failure\n");

	}
    RingPool = RingQueuePool_Create(500, 1524);
    
//    for(uint8_t i = 0; i < 64; i++)
//    {
//        tx[i] = i;
//        rx[i] = 0;
//    }
//    for(uint8_t i = 0; i < 64; i++)
//    {
//        tx[0] = 0;
//        MEMSET(rx, 0, 64);
//        point.len =64;
//        point.pData =rx;
//        bool reslut = RingQueuePool_Write(RingPool, tx, 64);
//        reslut = RingQueuePool_Read(RingPool, &point);
//    }
    
    
    if (!RingPool)
    {

    	perror("RingQueuePool_Init failure\n");
    }
}
