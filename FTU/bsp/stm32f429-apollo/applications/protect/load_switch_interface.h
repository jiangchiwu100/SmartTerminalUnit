#ifndef _LOAD_SWITCH_CONNECT_H_
#define _LOAD_SWITCH_CONNECT_H_

#include "stm32f4xx_hal.h"

enum
{
	LOAD_DEV0,				// 
	LOAD_DEVMAXNUM
};

extern void LoadSwitchCtrlInit(void);
extern void LoadSwitchCtrlClock(void);
extern void LoadSwitchCtrlReset(uint8_t pdrv);
#endif
