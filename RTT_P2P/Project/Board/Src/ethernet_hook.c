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
}









