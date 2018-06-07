 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      startup.c
  * @brief:     This file is part of RT-Thread RTOS.
  * @version:   V03.001(first implementation)
  * @author:    Bernard
  * @date:      2006-08-31
  * @updata:    [2011-06-05][Bernard][modify for STM32F107 version]
  * @updata:    [2018-01-23][Lexun][make the code clean up]
  */


/* INCLUDE FILES -------------------------------------------------------------*/
#include <rthw.h>
#include <rtthread.h>
#include "stm32f4xx.h"
#include "board.h"

#include "file_operate.h"

/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern int rt_application_init(void);

#ifdef RT_USING_FINSH
extern int finsh_system_init(void);
extern void finsh_set_device(const char* device);
#endif /* RT_USING_FINSH */

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define STM32_SRAM_BEGIN    (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#define STM32_SRAM_BEGIN    (__segment_end("HEAP"))
#else
extern int __bss_end;
#define STM32_SRAM_BEGIN    (&__bss_end)
#endif


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/

/**
  * @brief : Reports the name of the source file and the source line number
  *          where the assert error has occurred.
  * @param : [file] pointer to the source file name.
  * @param : [line] assert error line source number.
  * @return: None. 
  */  
void assert_failed(uint8_t* file, uint32_t line)
{
    rt_kprintf("\n\r Wrong parameter value detected on\r\n");
    rt_kprintf("       file  %s\r\n", file);
    rt_kprintf("       line  %d\r\n", line);

    while (1) ;
}

/**
  * @brief : This function will startup RT-Thread RTOS.
  * @param : None.
  * @return: None. 
  */  
void rtthread_startup(void)
{
    /* init board */
    rt_hw_board_init();

    /* show version */
    rt_show_version();

    /* init tick */
    rt_system_tick_init();

    /* init kernel object */
    rt_system_object_init();

    /* init timer system */
    rt_system_timer_init();

#ifdef RT_USING_EXT_SDRAM
    rt_system_heap_init((void*)EXT_SDRAM_BEGIN, (void*)EXT_SDRAM_END);
	memset((void*)EXT_SDRAM_BEGIN, 0, EXT_SDRAM_END - EXT_SDRAM_BEGIN);
#else
    rt_system_heap_init((void*)HEAP_BEGIN, (void*)HEAP_END);
#endif /* RT_USING_EXT_SDRAM */

    /* init scheduler system */
    rt_system_scheduler_init();

    /* init application */
    rt_application_init();

    /* init timer thread */
    rt_system_timer_thread_init();

    /* init idle thread */
    rt_thread_idle_init();

    /* start scheduler */
    rt_system_scheduler_start();
}

/**
  * @brief : This main function of RT-Thread RTOS.
  * @param : None.
  * @return: None. 
  */  
int main(void)
{
    /* disable interrupt first */
    rt_hw_interrupt_disable();

    /* startup RT-Thread RTOS */
    rtthread_startup();

    return(0);
}


/* END OF FILE ---------------------------------------------------------------*/

