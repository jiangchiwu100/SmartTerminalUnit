#include "ethernet_hook.h"

#include "distribution_enum.h"
#include "common_def.h"

#include "string.h"
//#include "extern_interface.h"
#include "rtthread.h"


#if VLAN_TAG

#define TPID_INDEX 12  //0x81
#define ETHERNET_TYPE_INDEX 14  //0x81

#else

#define ETHERNET_TYPE_INDEX 12  //0x81

#endif

#define TPID_LOW                 0x81  //0x81
#define TPID_HIGHT               0x00  //0x81
#define ETHERNET_TYPE_LOW        0x88  //0x81
#define ETHERNET_TYPE_HIGHT      0xB8  //0x81


#define MAX_RECIVE_COUNT 1500
static uint8_t EthernetReciveBuffer[MAX_RECIVE_COUNT]; 
static uint16_t EthernetReciveCount;

/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: void
* @update: [2018-08-2][张宇飞][]
*/
void EthernetInput(uint8_t* pData, uint16_t len)
{
    if (pData == NULL)
    {
        return;
    }

    
 #if VLAN_TAG   
    //检测TPID是否是0x8100
    if((pData[TPID_INDEX] != TPID_LOW) 
        || (pData[TPID_INDEX + 1] != TPID_HIGHT))
    {
       return;
    }
#endif
    
     //检测协议类型是否是0x88B8
    if((pData[ETHERNET_TYPE_INDEX] != ETHERNET_TYPE_LOW) 
        || (pData[ETHERNET_TYPE_INDEX + 1] != ETHERNET_TYPE_HIGHT))
    {
       return;
    }   
    
    if (len > MAX_RECIVE_COUNT)
    {
        return;
    }
    
    rt_memcpy(EthernetReciveBuffer, pData, len);
    EthernetReciveCount = len;
    
    //调试打印输出
    rt_kprintf("ReciveData rxCount : %d HEX:\n", len);
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        rt_kprintf("%2X ", EthernetReciveBuffer[i]);
    }
    rt_kprintf("\r\n");
    
    pData[0] = 0xFF;
    pData[1] = 0xFF;
    pData[2] = 0xFF;
    pData[3] = 0xFF;
    pData[4] = 0xFF;
    pData[5] = 0xFF;
    EthernetOutput(pData, len);
}



static struct rt_mutex ethernet_mutex;
rt_mutex_t g_ethernet_mutex;
/**
* @brief : 嵌入以太网输入回调,嵌入在任务中，注意占用时间
* @param : uint8_t* pData 数据指针
* @param : uint16_t len 数据长度
* @return: void
* @update: [2018-08-2][张宇飞][]
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




