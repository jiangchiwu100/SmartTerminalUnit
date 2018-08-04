/*
 *  ethernet_rtt.c
 *
 */

#include "stack_config.h"

#include <string.h>

#include "hal_ethernet.h"

#include "libiec61850_platform_includes.h"
#include <stdint.h>
#include "extern_interface.h"

#include "stm32f4xx_ll_utils.h"


struct sEthernetSocket {
    //pcap_t* rawSocket;
    //struct bpf_program etherTypeFilter;


	uint8_t data;
};

struct sEthernetHandleSet {
    //HANDLE *handles;
    //int nhandles;
	uint8_t data;
};

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
   
}

void
Ethernet_getInterfaceMACAddress(const char* interfaceId, uint8_t* addr)
{

	addr[0]=0x00;//
	addr[1]=0x80;
	addr[2]=0xE1;
	addr[3]= *(uint8_t*)(UID_BASE + 0);
	addr[4]= *(uint8_t*)(UID_BASE + 2);
	addr[5]= *(uint8_t*)(UID_BASE + 4);
}


EthernetSocket
Ethernet_createSocket(const char* interfaceId, uint8_t* destAddress)
{

	uint8_t dest[6];
	memcpy(destAddress, destAddress, 6);

    EthernetSocket ethernetSocket = (EthernetSocket)calloc(1, sizeof(struct sEthernetSocket));


    return ethernetSocket;
}

void
Ethernet_destroySocket(EthernetSocket ethSocket)
{
	free(ethSocket);
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
