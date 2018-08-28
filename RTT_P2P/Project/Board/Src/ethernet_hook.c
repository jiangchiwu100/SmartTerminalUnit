#include "ethernet_hook.h"

#include "distribution_enum.h"
#include "common_def.h"

#include "string.h"

#include "rtthread.h"
#include "extern_interface.h"

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
static uint8_t EthernetReciveBuffer[MAX_RECIVE_COUNT]; 
static uint16_t EthernetReciveCount;

/**
*以太网发送使用的互斥信号量
*/
static struct rt_mutex ethernet_mutex;
rt_mutex_t g_ethernet_mutex;



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
    
//    EthernetReciveCount = len;
//    
//    //调试打印输出
//    rt_kprintf("ReciveData rxCount : %d HEX:\n", len);
//    uint16_t i;
//    for(i = 0; i < len; i++)
//    {
//        rt_kprintf("%2X ", EthernetReciveBuffer[i]);
//    }
//    rt_kprintf("\r\n");
//    
//    pData[0] = 0xFF;
//    pData[1] = 0xFF;
//    pData[2] = 0xFF;
//    pData[3] = 0xFF;
//    pData[4] = 0xFF;
//    pData[5] = 0xFF;
//    EthernetOutput(pData, len);
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




