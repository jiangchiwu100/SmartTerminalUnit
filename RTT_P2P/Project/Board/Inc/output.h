#ifndef _OUTPUT_H
#define _OUTPUT_H


#include "rtthread.h"


#ifdef  DEBUG
#define DO_PRINTF(...)         rt_kprintf(__VA_ARGS__)
#else
#define DO_PRINTF(...)
#endif /* DEBUG */




rt_uint8_t rt_hw_output_operate(rt_uint16_t addr, rt_uint8_t operate_type);
void OpeningclosingOperate(rt_uint8_t type);

#endif /*_OUTPUT_H*/


