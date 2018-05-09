/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      ftu_config.h
  * @brief:     RT-Thread config file.
  * @version:   V1.0.1 
  * @author:    XXX
  * @date:      2017.12.06
  * @update:    [2017-12-07][Lexun][Add the IRQ interrupt priority]
  */
	
#ifndef __FTU_CFG_H__
#define __FTU_CFG_H__


/* DEFINE --------------------------------------------------------------------*/

///* Using event*/
//#define EVENT_RUN                                                      0x00000001
//#define EVENT_INIT_PROTECT                                             0x00000010
//#define EVENT_INIT_WATCH                                               0x00000020
//#define EVENT_INIT_CAL                                                 0x00000040
//#define EVENT_INIT_SLAVE101                                            0x00000080
//#define EVENT_INIT_SLAVE104                                            0x00000100
//#define EVENT_INIT_HMICOM                                              0x00000200
//#define EVENT_INIT_FTUIDLE                                             0x00000400
//#define EVENT_INIT_DP83848_2404                                        0x00000800
//#define EVENT_INIT_W5500_2404                                          0x00001000

///* Using Hardware Timer framework */  
//#define RT_USING_HWTIMER                                               (1) // 硬件定时器
//#define RT_USING_SFTIMER                                               (1) // 软件件定时器

///* Using Func */ 
//#define RT_USING_TELEMETRY_SET                                         (1) // 遥测置数

///* thread cfg */
//#define RT_USING_PROTECT                                               (1) // 保护任务
//#define RT_USING_WATCH                                                 (1) // 监听任务
//#define RT_USING_CAL                                                   (1) // 计算任务
//#define RT_USING_SLAVE101                                              (1) // 101从站通讯任务
//#define RT_USING_SLAVE104                                              (1) // 104从站通讯任务
//#define RT_USING_DP83848                                               (1) // LWIP通讯任务DP83848_2404
//#define RT_USING_W5500                                                 (1) // LWIP通讯任务W5500_2404
//#define RT_USING_HMICOM                                                (1) // HMI通讯任务
//#define RT_USING_FTUIDLE                                               (1) // 空闲任务

///* thread parameter Configuration */

//#define RT_USING_STATIC_THREAD

//#define INIT_THREAD_NAME                                               "init"
//#define INIT_THREAD_STACK_SIZE                                         (4096)
//#define INIT_THREAD_PRIORITY                                           (RT_THREAD_PRIORITY_MAX / 3)
//#define INIT_THREAD_TIMESLICE                                          (20)

//#define CAL_THREAD_NAME                                                "cal"
//#define CAL_THREAD_STACK_SIZE                                          (4096)
//#define CAL_THREAD_PRIORITY                                            (5)
//#define CAL_THREAD_TIMESLICE                                           (20)

//#define PROTECT_THREAD_NAME                                            "protect"
//#define PROTECT_THREAD_STACK_SIZE                                      (4096)
//#define PROTECT_THREAD_PRIORITY                                        (7)
//#define PROTECT_THREAD_TIMESLICE                                       (20)

//#define WATCH_THREAD_NAME                                              "watch"
//#define WATCH_THREAD_STACK_SIZE                                        (8000)
//#define WATCH_THREAD_PRIORITY                                          (19)
//#define WATCH_THREAD_TIMESLICE                                         (20)

//#define SLAVE101_THREAD_NAME                                           "slave101"
//#define SLAVE101_THREAD_STACK_SIZE                                     (4096)
//#define SLAVE101_THREAD_PRIORITY                                       (24)
//#define SLAVE101_THREAD_TIMESLICE                                      (50)

//#define DP83848_2404_THREAD_NAME                                       "dp83848_2404"
//#define DP83848_2404_THREAD_STACK_SIZE                                 (4096)
//#define DP83848_2404_THREAD_PRIORITY                                   (21)
//#define DP83848_2404_THREAD_TIMESLICE                                  (50)

//#define W5500_2404_THREAD_NAME                                         "w5500_2404"
//#define W5500_2404_THREAD_STACK_SIZE                                   (4096)
//#define W5500_2404_THREAD_PRIORITY                                     (22)
//#define W5500_2404_THREAD_TIMESLICE                                    (50)

//#define SLAVE104_THREAD_NAME                                           "slave104"
//#define SLAVE104_THREAD_STACK_SIZE                                     (4096)
//#define SLAVE104_THREAD_PRIORITY                                       (23)
//#define SLAVE104_THREAD_TIMESLICE                                      (50)

//#define HMICOM_THREAD_NAME                                             "hmi_com"
//#define HMICOM_THREAD_STACK_SIZE                                       (4096)
//#define HMICOM_THREAD_PRIORITY                                         (25)
//#define HMICOM_THREAD_TIMESLICE                                        (20)

//#define FTUIDLE_THREAD_NAME                                            "ftu_idle"
//#define FTUIDLE_THREAD_STACK_SIZE                                      (4096)
//#define FTUIDLE_THREAD_PRIORITY                                        (28)
//#define FTUIDLE_THREAD_TIMESLICE                                       (20)


///* INTERRUPT PRIORITIES CONFIGURATION ----------------------------------------*/
//#define INT_ETH_PRIO                                                   ((0x00 << 4) | 0x02)
//#define INT_USART1_PRIO                                                ((0x03 << 4) | 0x03)
//#define INT_USART2_PRIO                                                ((0x03 << 4) | 0x02)
//#define INT_USART3_PRIO                                                ((0x03 << 4) | 0x03)
//#define INT_UART4_PRIO                                                 ((0x03 << 4) | 0x03)
//#define INT_UART5_PRIO                                                 ((0x03 << 4) | 0x02)
//#define INT_USART6_PRIO                                                ((0x03 << 4) | 0x02)
//#define INT_EXTI9_5_PRIO                                               ((0x00 << 4) | 0x01) // sd2405
//#define INT_EXTI15_10_PRIO                                             ((0x00 << 4) | 0x00) // AD7616BUSY
//#define INT_TIMER3_PRIO                                                ((0x01 << 4) | 0x02) // frequency measurement
//#define INT_TIMER6_PRIO                                                ((0x01 << 4) | 0x00) // 1ms timer
//#define INT_TIMER7_PRIO                                                ((0x01 << 4) | 0x01) // none


#endif /* END __FTU_CFG_H__ */


/* END OF FILE ---------------------------------------------------------------*/

