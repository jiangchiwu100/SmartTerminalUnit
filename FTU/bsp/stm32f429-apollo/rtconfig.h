 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      rtconfig.h
  * @brief:     This file is part of RT-Thread RTOS.
  * @version:   V03.001(first implementation)
  * @author:    Bernard
  * @date:      2009-01-05
  * @updata:    [2014-04-27][Bernard][make the code cleanup]
  * @updata:    [2018-01-23][Lexun][make the code cleanup]
  * @updata:    [2018-02-28][Lexun][add the application config]
  */
#ifndef __RT_CONFIG_H__
#define __RT_CONFIG_H__


/* DEFINES -------------------------------------------------------------------*/
/* Automatically generated file; DO NOT EDIT. */
/* RT-Thread Configuration */

#define BOARD_STM32F429_APPOLO
#define SOC_STM32F429IG

/* RT-Thread Kernel ----------------------------------------------------------*/
#define RT_NAME_MAX                                                    8
#define RT_NAME_MIN                                                    3
#define RT_ALIGN_SIZE                                                  4

/* RT_THREAD_PRIORITY_8 is not set */
#define RT_THREAD_PRIORITY_32

/* RT_THREAD_PRIORITY_256 is not set */
//#define RT_DEBUG
#define RT_DEBUG_INIT                                                  1
#define RT_DEBUG_THREAD                                                0

#define RT_THREAD_PRIORITY_MAX                                         32
#define RT_TICK_PER_SECOND                                             1000
#define IDLE_THREAD_STACK_SIZE                                         1024

#define RT_USING_OVERFLOW_CHECK
#define RT_USING_HOOK
#define RT_USING_TIMER_SOFT
#define RT_SOFT_TIMER_1MS_NAME                                         "timer1ms"

/* Inter-Thread communication */
#define RT_USING_SEMAPHORE
#define RT_USING_MUTEX
#define RT_USING_EVENT
#define RT_USING_MAILBOX
#define RT_USING_MESSAGEQUEUE
/* RT_USING_SIGNALS is not set */

/* Memory Management */
#define RT_USING_MEMPOOL
#define RT_USING_MEMHEAP
/* RT_USING_NOHEAP is not set */
#define RT_USING_SMALL_MEM
/* RT_USING_SLAB is not set */
/* RT_USING_MEMHEAP_AS_HEAP is not set */
#define RT_USING_HEAP

/* Kernel Device Object */
#define RT_USING_DEVICE
/* RT_USING_INTERRUPT_INFO is not set */
#define RT_USING_CONSOLE

#define RT_CONSOLEBUF_SIZE                                             128
//#define RT_CONSOLE_DEVICE_NAME "uart"
/* RT_USING_MODULE is not set */

#define ARCH_ARM
#define ARCH_ARM_CORTEX_M
#define ARCH_ARM_CORTEX_M4

/* RT-Thread Components */
#define RT_USING_COMPONENTS_INIT
/* RT_USING_USER_MAIN is not set */

/* C++ features */

/* RT_USING_CPLUSPLUS is not set */

/* Command shell -------------------------------------------------------------*/
#define RT_USING_FINSH
#define FINSH_USING_HISTORY
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION
#define FINSH_THREAD_PRIORITY                                          20
#define FINSH_THREAD_STACK_SIZE                                        4096
#define FINSH_CMD_SIZE                                                 80
//#define FINSH_USING_MSH
//#define FINSH_USING_MSH_ONLY

#define FINSH_USING_AUTH
#define FINSH_DEFAULT_PASSWORD                                         "sojo"

/* Device virtual file system ------------------------------------------------*/
#define RT_USING_DFS
#define DFS_USING_WORKDIR
#define RT_USING_DFS_ELMFAT
#define DFS_FILESYSTEMS_MAX                                            4
#define DFS_FILESYSTEM_TYPES_MAX                                       4
#define DFS_FD_MAX                                                     4

/* elm-chan's FatFs, Generic FAT Filesystem Module */
#define RT_DFS_ELM_WORD_ACCESS
#define RT_DFS_ELM_USE_LFN_0
#define RT_DFS_ELM_CODE_PAGE                                           437
/* RT_DFS_ELM_USE_LFN_1 is not set */
/* RT_DFS_ELM_USE_LFN_2 is not set */
/* RT_DFS_ELM_USE_LFN_3 is not set */
#define RT_DFS_ELM_USE_LFN                                             3
#define RT_DFS_ELM_MAX_LFN                                             255
#define RT_DFS_ELM_DRIVES                                              1
#define RT_DFS_ELM_MAX_SECTOR_SIZE                                     4096

/* RT_DFS_ELM_USE_ERASE is not set */
//#define RT_DFS_ELM_REENTRANT
#define RT_USING_DFS_DEVFS
#define RT_USING_DFS_NET
#define HAVE_SYS_SELECT_H
/* HAVE_SYS_SOCKET_H is not set */
#define RT_USING_DFS_ROMFS
//#define RT_USING_DFS_UFFS
/* RT_USING_DFS_RAMFS is not set */
/* RT_USING_DFS_UFFS is not set */
/* RT_USING_DFS_NFS is not set */


/* Device Drivers ------------------------------------------------------------*/
#define RT_USING_DEVICE_IPC

/* Using USART framework */
#define RT_USING_SERIAL
#define RT_USING_USART1
//#define RT_USING_USART2
#define RT_USING_USART3
#define RT_USING_UART4
//#define RT_USING_UART5
#define RT_USING_USART6

#define RT_USART1_NAME                                                 "usart1"
//#define RT_USART2_NAME                                                 "usart2"
#define RT_USART3_NAME                                                 "usart3"
#define RT_UART4_NAME                                                  "uart4"
#define RT_UART5_NAME                                                  "uart5"
#define RT_USART6_NAME                                                 "usart6"

/* RT_USING_CAN is not set */

/* RT_USING_HWTIMER is not set */
#define RT_USING_I2C
#define RT_USING_I2C_BITOPS
#define RT_I2C0_NAME                                                   "i2c0"
#define RT_I2C_PCF8563_NAME                                            "pcf8563"

/* Using GPIO pin framework */
#define RT_USING_PIN
#define RT_PIN_NAME                                                    "pin"

/* RT_USING_MTD_NOR is not set */
#define RT_USING_MTD_NAND
#define RT_MTD_NAND_NAME                                               "nand0"

/* RT_MTD_NAND_DEBUG is not set */

/* Using RTC framework */
#define RT_USING_RTC
#define RT_RTC_NAME                                                    "rtc"

/* Using SDRAM framework */
#define RT_USING_EXT_SDRAM

/* RT_USING_SDIO is not set */
#define RT_SDIO_SD_NAME                                                "sd0"

/* Using SPI framework */
#define RT_USING_SPI
#define RT_USING_SPI2
#define RT_USING_SPI4
#define RT_USING_SPI5

#define RT_USING_IO_SPI1  //AUTO识别

#define RT_SPI2_DEVICE_NAME                                            "spi20"
#define RT_SPI2_BUS_NAME                                               "spi2"
#define RT_SPI4_DEVICE_NAME                                            "spi40"
#define RT_SPI4_BUS_NAME                                               "spi4"
#define RT_SPI5_DEVICE_NAME                                            "spi50"
#define RT_SPI5_BUS_NAME                                               "spi5"
#define RT_IO_SPI1_DEVICE_NAME                                         "io_spi10"
#define RT_IO_SPI1_BUS_NAME                                            "io_spi1"

#define RT_SPI_FRAM_NAME                                               "fram0"
#define RT_SPI_ETH_NAME                                                "w5500"
#define RT_SPI_ENCRYPTION_NAME                                         "sc1161y"
#define RT_SPI_FLASH_NAME                                              "W25Q256FV"

#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_USING_FLASH_INFO_TABLE

/* RT_SFUD_USING_FLASH_INFO_TABLE is not set */
/* RT_SFUD_DEBUG is not set */
/* RT_USING_W25QXX is not set */
/* RT_USING_GD is not set */
/* RT_USING_ENC28J60 is not set */
/* RT_USING_SPI_WIFI is not set */
/* RT_USING_WDT is not set */
/* RT_USING_WIFI is not set */

/* Using USB */
/* RT_USING_USB_HOST is not set */
/* RT_USING_USB_DEVICE is not set */

/* Using LCD */
#define RT_LCD_NAME                                                    "lcd"

/* POSIX layer and C standard library ----------------------------------------*/
#define RT_USING_LIBC
/* RT_USING_PTHREADS is not set */
#define RT_USING_POSIX
/* RT_USING_POSIX_MMAP is not set */
/* RT_USING_POSIX_TERMIOS is not set */





/* Modbus master and slave stack ---------------------------------------------*/
/* RT_USING_MODBUS is not set */
/* RT_USING_NETUTILS is not set */


/* RT-Thread UI Engine -------------------------------------------------------*/
/* RT_USING_GUIENGINE is not set */


/* VBUS(Virtual Software BUS) ------------------------------------------------*/
/* RT_USING_VBUS is not set */


/* RT-Thread online packages -------------------------------------------------*/
/* system packages */
/* PKG_USING_PARTITION is not set */
/* PKG_USING_SQLITE is not set */
/* PKG_USING_RTI is not set */


/* IoT - internet of things --------------------------------------------------*/
/* PKG_USING_PAHOMQTT is not set */
/* PKG_USING_WEBCLIENT is not set */
/* PKG_USING_MONGOOSE is not set */
/* PKG_USING_WEBTERMINAL is not set */
/* PKG_USING_CJSON is not set */
/* PKG_USING_EZXML is not set */
/* PKG_USING_NANOPB is not set */


/* security packages ---------------------------------------------------------*/
/* PKG_USING_MBEDTLS is not set */
/* PKG_USING_libsodium is not set */


/* language packages ---------------------------------------------------------*/
/* PKG_USING_JERRYSCRIPT is not set */
/* PKG_USING_MICROPYTHON is not set */


/* multimedia packages -------------------------------------------------------*/


/* tools packages ------------------------------------------------------------*/
/* PKG_USING_CMBACKTRACE is not set */
/* PKG_USING_EASYLOGGER is not set */
/* PKG_USING_SYSTEMVIEW is not set */


/* miscellaneous packages ----------------------------------------------------*/
/* PKG_USING_FASTLZ is not set */
/* PKG_USING_MINILZO is not set */


/* example package: hello ----------------------------------------------------*/
/* PKG_USING_HELLO is not set */


/* APP CONFIGURATION ---------------------------------------------------------*/
/* Using event*/
#define EVENT_RUN                                                      0x00000001
#define EVENT_REC_IRQ_W5500                                            0x00000002
#define EVENT_GOOSE_HAVE_CHANGE                                        0x00000004
#define EVENT_INIT_PROTECT                                             0x00000010
#define EVENT_INIT_WATCH                                               0x00000020
#define EVENT_INIT_CAL                                                 0x00000040
#define EVENT_INIT_SLAVE101                                            0x00000080
#define EVENT_INIT_SLAVE104                                            0x00000100
#define EVENT_INIT_HMICOM                                              0x00000200
#define EVENT_INIT_FTUIDLE                                             0x00000400
#define EVENT_INIT_DP83848_2404                                        0x00000800
#define EVENT_INIT_W5500_2404                                          0x00001000

/* Using Hardware Timer framework */  
#define RT_USING_HWTIMER                                               (1) // 硬件定时器
#define RT_USING_SFTIMER                                               (1) // 软件件定时器

/* Using Func */ 
#define RT_USING_TELEMETRY_SET                                         (1) // 遥测置数

/* thread cfg */

#define RT_USING_PROTECT                                               (1) // 保护任务
#define RT_USING_WATCH                                                 (1) // 监听任务
#define RT_USING_CAL                                                   (1) // 计算任务
#define RT_USING_SLAVE101                                              (1) // 101从站通讯任务
#define RT_USING_SLAVE104                                              (1) // 104从站通讯任务
#define RT_USING_DP83848                                               (1) // LWIP通讯任务DP83848_2404
#define RT_USING_W5500                                                 (1) // LWIP通讯任务W5500_2404
#define RT_USING_HMICOM                                                (0) // HMI通讯任务
#define RT_USING_FTUIDLE                                               (1) // 空闲任务
#define RT_USING_NET_FINSH                                             (1) // UDP网络实现finsh
#define RT_USING_UDP_SERVE                                             (1) // UDP通信
#define RT_USING_NETCONN                                               (0) // 使用netconn接口
#define RT_USING_SOCKET                                                (1) // 使用socket接口

/* thread parameter Configuration */
#define RT_USING_STATIC_THREAD

/* Thread priority -----------------------------------------------------------
#define RT_TIMER_THREAD_PRIORITY                                       0 
#define RT_LWIP_SLIPIFTHREAD_PRIORITY                                  1 
#define RT_LWIP_TCPTHREAD_PRIORITY                                     10 
#define RT_LWIP_ETHTHREAD_PRIORITY                                     12 
#define FINSH_THREAD_PRIORITY                                          20
 * -------------------------------------------------------------------------*/ 
#define INIT_THREAD_NAME                                               "init"
#define INIT_THREAD_STACK_SIZE                                         (4096)
//#define INIT_THREAD_PRIORITY                                           (RT_THREAD_PRIORITY_MAX / 3)
#define INIT_THREAD_PRIORITY                                           (RT_THREAD_PRIORITY_MAX / 2)
#define INIT_THREAD_TIMESLICE                                          (20)

#define W5500_UDP_RX_THREAD_NAME                                       "w5500_udp_rx"
#define W5500_UDP_RX_THREAD_PRIORITY                                   (5)
#define W5500_UDP_RX_THREAD_STACK_SIZE                                 (1024)
#define W5500_UDP_RX_THREAD_TIMESLICE                                  (20)

#define W5500_UDP_TX_THREAD_NAME                                       "w5500_udp_tx"
#define W5500_UDP_TX_THREAD_PRIORITY                                   (7)
#define W5500_UDP_TX_THREAD_STACK_SIZE                                 (1024)
#define W5500_UDP_TX_THREAD_TIMESLICE                                  (20)

#define CAL_THREAD_NAME                                                "cal"
#define CAL_THREAD_PRIORITY                                            (8)
#define CAL_THREAD_STACK_SIZE                                          (4096)
#define CAL_THREAD_TIMESLICE                                           (20)

#define PROTECT_THREAD_NAME                                            "protect"
#define PROTECT_THREAD_PRIORITY                                        (9)
#define PROTECT_THREAD_STACK_SIZE                                      (4096)
#define PROTECT_THREAD_TIMESLICE                                       (20)

#define WATCH_THREAD_NAME                                              "watch"
#define WATCH_THREAD_PRIORITY                                          (18)
#define WATCH_THREAD_STACK_SIZE                                        (8000)
#define WATCH_THREAD_TIMESLICE                                         (20)

#define SLAVE101_THREAD_NAME                                           "iec101"
#define SLAVE101_THREAD_PRIORITY                                       (24)
#define SLAVE101_THREAD_STACK_SIZE                                     (4096)
#define SLAVE101_THREAD_TIMESLICE                                      (20)

#define DP83848_2404_THREAD_NAME                                       "dp83848"
#define DP83848_2404_THREAD_PRIORITY                                   (21)
#define DP83848_2404_THREAD_STACK_SIZE                                 (4096)
#define DP83848_2404_THREAD_TIMESLICE                                  (20)

#define SLAVE104_THREAD_NAME                                           "iec104"
#define SLAVE104_THREAD_PRIORITY                                       (23)
#define SLAVE104_THREAD_STACK_SIZE                                     (4096)
#define SLAVE104_THREAD_TIMESLICE                                      (20)

#define HMICOM_THREAD_NAME                                             "hmi"
#define HMICOM_THREAD_PRIORITY                                         (25)
#define HMICOM_THREAD_STACK_SIZE                                       (4096)
#define HMICOM_THREAD_TIMESLICE                                        (20)

#define FTUIDLE_THREAD_NAME                                            "idle"
#define FTUIDLE_THREAD_PRIORITY                                        (28)
#define FTUIDLE_THREAD_STACK_SIZE                                      (4096)
#define FTUIDLE_THREAD_TIMESLICE                                       (20)

#define MBSLAVE_THREAD_NAME                                            "mb"
#define MBSLAVE_THREAD_PRIORITY                                        (19) // 18
#define MBSLAVE_THREAD_STACK_SIZE                                      (4096)
#define MBSLAVE_THREAD_TIMESLICE                                       (20)


/* INTERRUPT PRIORITIES CONFIGURATION ----------------------------------------*/
/* !!![INT_SYSTICK_PRIO] systick priority cannot be modified!!! */ 
#define INT_SYSTICK_PRIO                                               ((0x00 << 4) | 0x00) 
#define INT_EXTI15_10_PRIO                                             ((0x00 << 4) | 0x01) // AD7616BUSY
#define INT_EXTI3_PRIO                                                 ((0x00 << 4) | 0x02) // w5500 
#define INT_ETH_PRIO                                                   ((0x00 << 4) | 0x03) // Ethernet
#define INT_RTC_WKUP_PRIO                                              ((0x00 << 4) | 0x04) // RTC WKUP
//#define INT_EXTI9_5_PRIO                                               ((0x00 << 4) | 0x04) // pcf8563
#define INT_TIM3_PRIO                                                  ((0x01 << 4) | 0x01) // frequency measurement
#define INT_TIM6_PRIO                                                  ((0x01 << 4) | 0x00) // 1ms timer
#define INT_TIM2_PRIO                                                  ((0x01 << 4) | 0x02) // none
#define INT_TIM5_PRIO                                                  ((0x01 << 4) | 0x03) // none
#define INT_TIM7_PRIO                                                  ((0x01 << 4) | 0x04) // none
#define INT_USART1_PRIO                                                ((0x03 << 4) | 0x04) // xiansuo
#define INT_USART2_PRIO                                                ((0x03 << 4) | 0x05) // none
#define INT_USART3_PRIO                                                ((0x03 << 4) | 0x01) // IEC101
#define INT_UART4_PRIO                                                 ((0x03 << 4) | 0x02) // config
#define INT_UART5_PRIO                                                 ((0x03 << 4) | 0x03) // shell
#define INT_USART6_PRIO                                                ((0x03 << 4) | 0x00) // HMI
#define INT_SPI_DMA_TX_PRIO                                            ((0x00 << 4) | 0x06) // none
#define INT_SPI_DMA_RX_PRIO                                            ((0x00 << 4) | 0x00) // none
#define INT_RTC_ALARM_PRIO                                             ((0x01 << 4) | 0x02) // RTC alarm 
#define INT_SDMMC1_PRIO                                                ((0x02 << 4) | 0x00) // SDMMC1 
#define INT_DMA2_STREAM3_PRIO                                          ((0x03 << 4) | 0x00) // DMA2_Stream3,none
#define INT_DMA2_STREAM6_PRIO                                          ((0x03 << 4) | 0x00) // DMA2_Stream6,none


#endif /* __RT_CONFIG_H__ */


/* END OF FILE ---------------------------------------------------------------*/
