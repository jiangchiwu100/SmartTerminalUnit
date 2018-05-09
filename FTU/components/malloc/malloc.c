/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      malloc.c
  * @brief:     SDRAM memory dynamic allocation management.
  * @version:   V03.001
  * @author:    SUN
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
	

/* INCLUDE FILES -------------------------------------------------------------*/
#include "malloc.h"	    


/* PRIVATE FUNCTIONS ---------------------------------------------------------*/
static rt_uint16_t _mem_perused(rt_uint8_t memx); 
//rt_uint32_t _mem_malloc(rt_uint8_t memx,rt_uint32_t size); 
//rt_uint8_t _mem_free(rt_uint8_t memx,rt_uint32_t offset); 


/* PRIVATE VARIABLES ---------------------------------------------------------*/
/* memory pool(32 byte alignment) */ 
//static __align(32) rt_uint8_t mem1_pool[MEM1_MAX_SIZE]; // internal SRAM
static __align(32) rt_uint8_t mem2_pool[MEM2_MAX_SIZE] __attribute__((at(0xC0600000))); // external SDRAM
//static __align(32) rt_uint8_t mem3_pool[MEM3_MAX_SIZE] __attribute__((at(0X10000000))); // internal CCM 

/* memory management table */
//rt_uint32_t mem1_map[MEM1_ALLOC_TABLE_SIZE]; // internal SRAM memory pool MAP
static rt_uint32_t mem2_map[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0xC0600000 + MEM2_MAX_SIZE))); // external SDRAM memory pool MAP
//rt_uint32_t mem3_map[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE))); // internal CCM memory pool MAP

/* memory management parameters */ 	   
const rt_uint32_t mem_table_size[SRAMBANK] = {MEM2_ALLOC_TABLE_SIZE};	// table size
const rt_uint32_t mem_block_size[SRAMBANK] = {MEM2_BLOCK_SIZE}; // block size
const rt_uint32_t mem_total_size[SRAMBANK] = {MEM2_MAX_SIZE}; // memory total size

/* memory management controller */ 
struct _m_mallco_dev mallco_dev =
{
    _mem_init, // memory initialization
    _mem_perused, // memory usage
    mem2_pool, // memory pool
    mem2_map, // memory management table
    0, // memory management unable
};


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
/**
  * @brief  memory copy.
  * @param  [des] destination address.
  * @param  [src] source address.
  * @param  [n] The length of memory that needs to be copied(In bytes).
  * @retval None.
  */
static void _mem_memcpy(void *des, void *src, rt_uint32_t n)  
{  
    rt_uint8_t *xdes = des;
    rt_uint8_t *xsrc = src; 
    while (n--)
    {
        *xdes++ = *xsrc++;  
    }
}  

/**
  * @brief  memory set.
  * @param  [s] head address of memory block.
  * @param  [c] data that need to set.
  * @param  [count] The size of memory that needs to be copied(In bytes).
  * @retval None.
  */
static void _mem_memset(void *s, rt_uint8_t c, rt_uint32_t count)  
{  
    rt_uint8_t *xs = s;  
	
    while (count--)
    {
        *xs++ = c;  
    }
}	

/**
  * @brief  memory management initialization.
  * @param  [memx] memory block.
  * @retval None.
  */
void _mem_init(rt_uint8_t memx)  
{  
    _mem_memset(mallco_dev.memmap[memx], 0, mem_table_size[memx]*4); // clear memory state table
    _mem_memset(mallco_dev.membase[memx], 0, MEM2_MAX_SIZE); // clear memory state table
 
   	mallco_dev.memrdy[memx] = 1; // memory management initialization OK  
}  

/**
  * @brief  gain memory usage rate.
  * @param  [memx] memory block.
  * @retval return usage rate.(Expand 10 times,0~1000,express 0.0%~100.0%)
  */
static rt_uint16_t _mem_perused(rt_uint8_t memx)
{  
    rt_uint32_t used = 0;  
    rt_uint32_t i = 0;  
    for (i = 0; i < mem_table_size[memx]; i++)  
    {  
        if (mallco_dev.memmap[memx][i])
        {
            used++; 
        }
    } 
    return (used*1000)/(mem_table_size[memx]);  
}  

/**
  * @brief  memory allocation.
  * @param  [memx] memory block.
  * @param  [size] the size of the memory to be allocated(byte).
  * @retval [0xFFFFFFFF] error;others express memory offset address.
  */
static rt_uint32_t _mem_malloc(rt_uint8_t memx, rt_uint32_t size)  
{  
    signed long offset = 0;  
    rt_uint32_t nmemb = 0;
    rt_uint32_t cmemb = 0;
    rt_uint32_t i = 0;  
	
    if (!mallco_dev.memrdy[memx])
    {
        mallco_dev.init(memx); // uninitialized, initialization first
    }
		
    if (size == 0)
    {
        return 0xFFFFFFFF; // non memory allocation
    }
		
    nmemb = size / mem_block_size[memx]; // get assigned contiguous memory blocks
		
    if (size % mem_block_size[memx])
    {
        nmemb++; 
    }
		
    for (offset = mem_table_size[memx]-1; offset >= 0; offset--) // search the whole memory
    {     
        if (!mallco_dev.memmap[memx][offset])
        {
            cmemb++; // empty memory block increase
        }
        else
        {
            cmemb = 0; // empty memory block clear
        }
				
        if (cmemb == nmemb) // found <nmemb> continuous blocks of memory.
        {
            for (i = 0; i < nmemb; i++) // memory block is not empty
            {  
                mallco_dev.memmap[memx][offset+i] = nmemb;  
            }  
            return (offset*mem_block_size[memx]); // memory offset address
        }
    } 
		
    return (0xFFFFFFFF); // non memory allocation
}  

/**
  * @brief  memory release.
  * @param  [memx] memory block.
  * @param  [offset] memory offset address.
  * @retval [0] sucess;[1] error.
  */
static rt_uint8_t _mem_free(rt_uint8_t memx,rt_uint32_t offset)  
{  
    int i = 0; 
	
    if (!mallco_dev.memrdy[memx])
    {
        mallco_dev.init(memx);    
        return (1); // initialization failed
    }  
    if (offset < mem_total_size[memx])
    {  
        int index = offset / mem_block_size[memx];
        int nmemb = mallco_dev.memmap[memx][index];	// number of memory block
        for (i = 0; i < nmemb; i++) // memory block zero
        {  
            mallco_dev.memmap[memx][index+i]=0;  
        }  
        return (0);  
    }
		else 
    {
        return (2);  
    }
} 


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief  memory release.
  * @param  [memx] memory block.
  * @param  [ptr] memory first address.
  * @retval None.
  */ 
void rt_mem_free(rt_uint8_t memx, void *ptr)  
{  
    rt_uint32_t offset;   
	
    if (ptr == NULL)
    {
        return; // address is zero. 
    }
		
    offset = (rt_uint32_t)ptr - (rt_uint32_t)mallco_dev.membase[memx];     
    _mem_free(memx, offset); // memory release. 
}  

/**
  * @brief  memory allocated.
  * @param  [memx] memory block.
  * @param  [size] memory size(byte).
  * @retval return the memory address.
  */ 
void *rt_mem_malloc(rt_uint8_t memx,rt_uint32_t size)  
{  
    rt_uint32_t offset = 0;   
	
    offset = _mem_malloc(memx, size);  
	
    if (offset == 0XFFFFFFFF)
    {
        MALLOC_PRINTF("rt_mem_malloc failed\n"); 
        return(NULL);  
    }
    else
    {
        return (void*)((rt_uint32_t)mallco_dev.membase[memx]+offset);  
    }
}  




/**
  * @brief  memory allocation redistribution.
  * @param  [memx] memory block.
  * @param  [ptr]  old memory block address.
  * @param  [size] memory size(byte).
  * @retval return the new memory address.
  */ 
void *rt_mem_realloc(rt_uint8_t memx,void *ptr,rt_uint32_t size)  
{  
    rt_uint32_t offset = 0;    
    offset = _mem_malloc(memx, size);   
	
    if (offset == 0xFFFFFFFF)
    {
        return NULL;  
    }			
    else  
    {  									   
        _mem_memcpy((void*)((rt_uint32_t)mallco_dev.membase[memx] + offset), ptr, size);  
        rt_mem_free(memx, ptr);
        return (void*)((rt_uint32_t)mallco_dev.membase[memx] + offset);
    }  
}

void *atom_mem_malloc(rt_uint32_t size)  
{
    return rt_mem_malloc(0 , size) ;
}
void *atom_mem_realloc(void *ptr,rt_uint32_t size)  
{
    return rt_mem_realloc(0, ptr, size);
}
void atom_mem_free(void *ptr)  
{
     rt_mem_free(0, ptr) ;
}

/* END OF FILE ---------------------------------------------------------------*/

