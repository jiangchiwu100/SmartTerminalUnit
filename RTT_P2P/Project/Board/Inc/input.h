#ifndef _INPUT_H_
#define _INPUT_H_

#include "rtdef.h"

#include "distribution_enum.h"

/* DEFINE --------------------------------------------------------------------*/	
#ifdef  DEBUG
#define INPUT_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define INPUT_PRINTF(...)
#endif /* DEBUG */



#define INPUT_NUM                 8


extern int rt_hw_input_init(void);
extern void rt_hw_input_check_task(rt_uint8_t clock);
extern void StationSwtichCheck(void);



#endif


