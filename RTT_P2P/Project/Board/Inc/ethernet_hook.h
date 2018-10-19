#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include <stdint.h>
#include <stdbool.h>
#include "rtdef.h"

//#define USE_PBUF
#define RING_MEM_POOL
#ifdef USE_PBUF

#include <netif/ethernetif.h>
extern bool EthernetInputPbuf(struct pbuf *p, uint16_t len);
#endif

extern rt_mutex_t g_ethernet_mutex;

extern bool EthernetInputPool(uint8_t* pData, uint16_t len);



void EthernetHookInit(void);

bool EthernetInput(uint8_t* pData, uint16_t len);
rt_err_t EthernetOutput( uint8_t* pData, uint16_t count);
uint16_t MacRawInputBlock(uint8_t* pData, uint16_t size);


void EthernetHookEnable(void);
void EhernetOuputMutex_OnLock(void);
void EhernetOuputMutex_OffLock(void) ;
#endif
