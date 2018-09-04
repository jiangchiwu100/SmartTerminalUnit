/*
 *  ethernet_rtt.c
 *
 */

#include "stack_config.h"

#include <string.h>

#include "hal_ethernet.h"

#include "libiec61850_platform_includes.h"

//#include "stm32f429xx.h"

#include "ethernet_hook.h"
//#include "w5500_demo.h"

#ifndef DEBUG_HAL_ETHERNET
#define DEBUG_HAL_ETHERNET 1
#endif

#if (CONFIG_INCLUDE_ETHERNET_WINDOWS == 1)




rt_err_t EthernetOutput( uint8_t* pData, uint16_t count)
{
    perror("Unimplent!\n");
    return 0;
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

void
EthernetHandleSet_removeSocket(EthernetHandleSet self, const EthernetSocket sock)
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

static char*
getInterfaceName(int interfaceIndex)
{
   
    char* interfaceName = NULL;

    
    return interfaceName;
}

static void
getAdapterMacAddress(char* pcapAdapterName, uint8_t* macAddress)
{
   
  //TODO:应该提供统一接口
    /* OUI 00-80-E1 STMICROELECTRONICS. */    
    macAddress[0] = 0x00;
    macAddress[1] = 0x80;
    macAddress[2] = 0xE1;
	
    /* generate MAC addr from 96bit unique ID (only for test). */
    //macAddress[3] = *(uint8_t*)(UID_BASE+4);
   // macAddress[4] = *(uint8_t*)(UID_BASE+2);
   // macAddress[5] = *(uint8_t*)(UID_BASE+0);
    
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
    //TODO:应该提供统一接口
    /* OUI 00-80-E1 STMICROELECTRONICS. */    
    addr[0] = 0x00;
    addr[1] = 0x80;
    addr[2] = 0xE1;
	
    /* generate MAC addr from 96bit unique ID (only for test). */
//    addr[3] = *(uint8_t*)(UID_BASE+4);
//    addr[4] = *(uint8_t*)(UID_BASE+2);
//    addr[5] = *(uint8_t*)(UID_BASE+0);
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
       //  StopWatchInit();
      //  StopWatchStart();  
    EthernetOutput(buffer, packetSize);
   //MacRawOutput(buffer, packetSize);
  //  StopWatchStop();
}

void
Ethernet_setProtocolFilter(EthernetSocket ethSocket, uint16_t etherType)
{
  
}

int
Ethernet_receivePacket(EthernetSocket self, uint8_t* buffer, int bufferSize)
{
    return MacRawInputBlock(buffer, bufferSize);   
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
