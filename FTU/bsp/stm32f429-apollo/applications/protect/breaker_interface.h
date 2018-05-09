#ifndef _BREAKER_CONNECT_H_
#define _BREAKER_CONNECT_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_it.h"

enum
{
	BRE_DEV0,				// ??0
	BRE_DEVMAXNUM
};

extern void BreakerCtrlInit(void);
extern void BreakerCtrlClock(void);
extern void BreakerCtrlReset(uint8_t pdrv);
#endif

