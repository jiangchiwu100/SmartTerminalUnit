/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      multi_thread.h
  * @brief:     Create all the threads.
  * @version:   V1.1.0 
  * @author:    Mr.Sun
  * @date:      2017.10.26
  * @update:    [2017.12.11][Lexun][Make the code cleanup]
  */
#ifndef __MULTIPE_THREAD_H__
#define __MULTIPE_THREAD_H__


/* DEFINES -------------------------------------------------------------------*/
#ifdef  DEBUG
#define THREAD_PRINTF(...)                       rt_kprintf(__VA_ARGS__)
#else
#define THREAD_PRINTF(...)
#endif /* DEBUG */

#define RT_THREAD_PER_STRUCT_SIZE                256*8
#define RT_THREAD_PER_STATCK_SIZE                8192*8

#define W5500_2404_THREAD_START_ADDR             0xC1100000
#define W5500_2404_THREAD_STACK_START_ADDR       (W5500_2404_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define SLAVE101_THREAD_START_ADDR               (W5500_2404_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define SLAVE101_THREAD_STACK_START_ADDR         (SLAVE101_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define SLAVE104_THREAD_START_ADDR               (SLAVE101_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define SLAVE104_THREAD_STACK_START_ADDR         (SLAVE104_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define HMICOM_THREAD_START_ADDR                 (SLAVE104_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define HMICOM_THREAD_STACK_START_ADDR           (HMICOM_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define CAL_THREAD_START_ADDR                    (HMICOM_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define CAL_THREAD_STACK_START_ADDR              (CAL_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define PROTECT_THREAD_START_ADDR                (CAL_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define PROTECT_THREAD_STACK_START_ADDR          (PROTECT_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define WATCH_THREAD_START_ADDR                  (PROTECT_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define WATCH_THREAD_STACK_START_ADDR            (WATCH_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define FTUIDLE_THREAD_START_ADDR                (WATCH_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define FTUIDLE_THREAD_STACK_START_ADDR          (FTUIDLE_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define DP83848_2404_THREAD_START_ADDR           (FTUIDLE_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define DP83848_2404_THREAD_STACK_START_ADDR     (DP83848_2404_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)

#define INIT_THREAD_START_ADDR                   (DP83848_2404_THREAD_STACK_START_ADDR + RT_THREAD_PER_STATCK_SIZE)
#define INIT_THREAD_STACK_START_ADDR             (INIT_THREAD_START_ADDR + RT_THREAD_PER_STRUCT_SIZE)


/* STRUCTS -------------------------------------------------------------------*/


/* PUBLIC VARIABLES ----------------------------------------------------------*/
extern struct rt_event calculate_event; 
extern struct rt_event slave101_event; 
extern struct rt_event slave104_event;
extern struct rt_event sd2405_event;

extern struct rt_semaphore w5500_sem; 
extern struct rt_semaphore protect_sem; 
extern struct rt_semaphore watch_sem;

/* PUBLIC FUNCTIONS ----------------------------------------------------------*/
extern int rt_multi_thread_start(void);


#endif /* END __MULTIPE_THREAD_H__ */


/* END OF FILE ---------------------------------------------------------------*/

