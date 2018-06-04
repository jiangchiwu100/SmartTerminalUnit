#ifndef _DISTRIBUTED_CONNECT_H_
#define _DISTRIBUTED_CONNECT_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_it.h"
#include "distributed_protect.h"

enum
{
	DISTRIBUT_DEV0,				// ??0
	DISTRIBUT_DEVMAXNUM
};

extern uint8_t g_dataHaveChange;
extern DevStr g_SelfSts[DISTRIBUT_DEVMAXNUM];;

extern void distributInit(void);
extern void distributClock(void);
extern void distributUpdataQueue(uint8_t pdrv,uint8_t ip,uint32_t *pdata);
extern void distributReset(uint8_t pdrv);

#endif

