#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include <stdint.h>
#include "rtdef.h"

extern rt_mutex_t g_ethernet_mutex;




void EthernetHookInit(void);
void EthernetInput(uint8_t* pData, uint16_t len);
rt_err_t EthernetOutput( uint8_t* pData, uint16_t count);


void EhernetOuputMutex_OnLock(void);
void EhernetOuputMutex_OffLock(void) ;
#endif
