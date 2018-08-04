/*
 *  ethernet_rtt.c
 *
 */

#include "stack_config.h"

#include <string.h>

#include "hal_ethernet.h"

#include "libiec61850_platform_includes.h"




//#include "w5500_demo.h"

#ifndef DEBUG_HAL_ETHERNET
#define DEBUG_HAL_ETHERNET 1
#endif

#if (CONFIG_INCLUDE_ETHERNET_WINDOWS == 1)











EthernetHandleSet
EthernetHandleSet_new(void)
{

}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    
}

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
{
    
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
    
}

static char*
getInterfaceName(int interfaceIndex)
{
   
    char* interfaceName = NULL;

    
    return interfaceName;
}

static void
getAdapterMacAddress(char* pcapAdapterName, uint8_t* macAddress)
{
    uint32_t sn0=*(__IO uint32_t*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字 
    macAddress[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
    macAddress[1]=0;
    macAddress[2]=0;
    macAddress[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
    macAddress[4]=(sn0>>8)&0XFFF;
    macAddress[5]=sn0&0XFF; 
    
}
/**
*  @brief :获取本地MAC
  * @param const char* interfaceId
  * @param cuint8_t* addr
  * @return: void
  * @update: [2018-07-24][张宇飞][创建]
*/
void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
    uint32_t sn0=*(__IO uint32_t*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字 
    addr[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
    addr[1]=0;
    addr[2]=0;
    addr[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
    addr[4]=(sn0>>8)&0XFFF;
    addr[5]=sn0&0XFF; 
}


EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{


    EthernetSocket ethernetSocket ;

 

    return ethernetSocket;
}

void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
  
}

void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize)
{
   
   MacRawOutput(buffer, packetSize);
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
  
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    PointUint8 *pPacket = 0;
   ReciveInputBlock(&pPacket);
    
    if (pPacket)
    {
        memcpy(buffer, pPacket->pData, pPacket->len);
    }
     
    uint16_t reciveLen = pPacket->len;
    MX_TIM10_Init();
    StartTimer();
    rt_free(pPacket->pData);
    rt_free(pPacket);
    return reciveLen;
    
}

bool
Ethernet_isSupported()
{
    return true;
}

#else

bool
Ethernet_isSupported()
{
    return false;
}

EthernetHandleSet
EthernetHandleSet_new(void)
{
    return NULL;
}

void
EthernetHandleSet_addSocket(EthernetHandleSet self, const EthernetSocket sock)
{
}

int
EthernetHandleSet_waitReady(EthernetHandleSet self, unsigned int timeoutMs)
{
    return 0;
}

void
EthernetHandleSet_destroy(EthernetHandleSet self)
{
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{
}

EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{
    return NULL;
}

void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
}

void
Ethernet_sendPacket(EthernetSocket ethSocket, uint8_t* buffer, int packetSize)
{
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    return 0;
}

#endif /* (CONFIG_INCLUDE_ETHERNET_WINDOWS == 1) */
