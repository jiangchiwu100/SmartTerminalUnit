#ifndef _INPUT_H_
#define _INPUT_H_

#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "distribution.h"

/* DEFINE --------------------------------------------------------------------*/	
#ifdef  DEBUG
#define INPUT_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define INPUT_PRINTF(...)
#endif /* DEBUG */

extern SwitchProperty curStation;

#define INPUT_NUM                 8


extern int rt_hw_input_init(void);
extern void rt_hw_input_check_task(rt_uint8_t clock);
extern void OpeningClosing(void);



#endif


