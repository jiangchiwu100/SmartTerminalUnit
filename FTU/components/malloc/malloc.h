/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      malloc.h
  * @brief:     SDRAM memory dynamic allocation management.
  * @version:   V03.001
  * @author:    SUN
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
#ifndef __MALLOC_H__
#define __MALLOC_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/
#ifdef  MALLOC_DEBUG 
#define MALLOC_PRINTF     rt_kprintf
#else
#define MALLOC_PRINTF(...)
#endif /* MALLOC_DEBUG */

#ifndef NULL
#define NULL                         0
#endif

/* define three memory pools */
#define SRAMIN                       0 // internal memory pool
#define SRAMEX                       0 // external memory pool(SDRAM)
#define SRAMCCM                      1 // internal memory pool(CCM,ONLY USE BY CPU))
#define SRAMBANK                     1 // the number of memory blocks	

/* MEMORY2 parameter setting.(internal memory(SRAM)) */ 
#define MEM1_BLOCK_SIZE              64 // per block 64 Bytes.
#define MEM1_MAX_SIZE                160 * 1024 // max memory is 160K
#define MEM1_ALLOC_TABLE_SIZE        MEM1_MAX_SIZE / MEM1_BLOCK_SIZE // size of memory table

/* MEMORY2 parameter setting.(external memory(SDRAM)) */ 
#define MEM2_BLOCK_SIZE              64 // per block 64 Bytes.
#define MEM2_MAX_SIZE                22480 * 1024 // max memory is 22480K
#define MEM2_ALLOC_TABLE_SIZE        MEM2_MAX_SIZE/MEM2_BLOCK_SIZE // size of memory table
		 
/* MEMORY3 parameter setting.(internal memory(CCM,ONLY USE BY CPU)) */ 
#define MEM3_BLOCK_SIZE              64 // per block 64 Bytes.
#define MEM3_MAX_SIZE                60 * 1024 //  max memory is 60K
#define MEM3_ALLOC_TABLE_SIZE        MEM3_MAX_SIZE/MEM3_BLOCK_SIZE // size of memory table
		 

/* STRUCT --------------------------------------------------------------------*/
/* memory management struct */
struct _m_mallco_dev
{
    void (*init)(rt_uint8_t); // initialization
    rt_uint16_t (*perused)(rt_uint8_t); // memory usage rate
    rt_uint8_t *membase[SRAMBANK];
    rt_uint32_t *memmap[SRAMBANK];
    rt_uint8_t memrdy[SRAMBANK];
};


/* PUBLIC VARIABLES ----------------------------------------------------------*/
//extern struct _m_mallco_dev mallco_dev;


/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
void _mem_init(rt_uint8_t memx); 
void rt_mem_free(rt_uint8_t memx, void *ptr);
void *rt_mem_malloc(rt_uint8_t memx, rt_uint32_t size);
void *rt_mem_realloc(rt_uint8_t memx, void *ptr, rt_uint32_t size);




void *atom_mem_malloc(rt_uint32_t size) ;
void *atom_mem_realloc(void *ptr,rt_uint32_t size)  ;
void  atom_mem_free(void *ptr)  ;

#endif /* END __MALLOC_H__ */


/* END OF FILE ---------------------------------------------------------------*/












