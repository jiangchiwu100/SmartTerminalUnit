/** ----------------------------------------------------------------------------
  *             Copyright (C) SOJO Electric CO., Ltd. 
  *             http://www.sojoline.com
	*             2017-2018. All right reserved.
  * @file:      bsp_cfg.h
  * @brief:     The configuration file of board surport packege. 
  * @version:   V1.0.0 
  * @author:    LEX
  * @date:      2017.11.23
  * @update:    [YYYY-MM-DD][NAME][DESCRIPTION]
 ** --------------------------------------------------------------------------*/
#ifndef  _BSP_CFG_H_
#define  _BSP_CFG_H_ 


/* DEFINE --------------------------------------------------------------------*/


/* BOOT VERSION --------------------------------------------------------------*/
#define  BOOTVersion                                       ("3.01")


/* TASK STACK NAMES CONFIGURATION --------------------------------------------*/
#define  LWIP_TASK_TCPIP_THREAD_NAME                       ("Taks of tcpip_thread")
#define  LWIP_TASK_DHCP_NAME                               ("Task of Shell")
#define  LWIP_TASK_DP83848_NAME                            ("Task of DP83848 input thread")
#define  LWIP_TASK_W5500_NAME                              ("Task of W5500 input thread")
#define  APP_TASK_START_NAME                               ("Task of Start")
#define  APP_TASK_WATCH_NAME                               ("Task of watch")
#define  APP_TASK_DLT634_5104_NAME                         ("Task of DL/T634.5104-2009")
#define  APP_TASK_TCP_SERVER_2404_NAME                     ("Task of TCP SERVER(2404)")
#define  APP_TASK_TCP_SERVER_8000_NAME                     ("Task of TCP SERVER(8000)")
#define  APP_TASK_DLT634_5101_MASTER_NAME                  ("Task of DL/T634.5101-2002 MASTER")
#define  APP_TASK_DLT634_5101_LINELOSS_NAME                ("Task of DL/T634.5101-2002 LINELOSS")
#define  APP_TASK_DLT634_5101_SLAVE_NAME                   ("Task of DL/T634.5101-2002 SLAVE")
#define  APP_TASK_DLT634_5104_SLAVE_NAME                   ("Task of DL/T634.5104-2002 SLAVE")
#define  APP_TASK_SDM_MASTER_NAME                          ("Task of SDM MASTER")
#define  APP_TASK_SDM_SLAVE_NAME                           ("Task of SDM SLAVE")
#define  APP_TASK_MODBUS_NAME                              ("Task of MODBUS")
#define  APP_TASK_SHELL_NAME                               ("Task of Shell")


/* TASK NAMES CONFIGURATION --------------------------------------------------*/
#define  INIT_TASK_NAME                                    "init"


/* DEVICE NAMES CONFIGURATION ------------------------------------------------*/
#define  RS232_DEVICE_NAME                                 "rs232_device"
#define  RS485_DEVICE_NAME                                 "rs485_device"
#define  PIN_DEVICE_NAME                                   "pin_device"
#define  IO_DEVICE_NAME                                    "io_device"
#define  FRAM_DEVICE_NAME                                  "fm25l04_device"
#define  RTC_DEVICE_NAME                                   "rtc"
#define  LED_DEVICE_NAME                                   "led_device"
#define  LCD_DEVICE_NAME                                   "lcd_device"
#define  WATCHDOG_DEVICE_NAME                              "watchdog_device"
#define  SOE_DEVICE_NAME                                   "soe_device"
#define  ADC_DEVICE_NAME                                   "ad7606_device"
#define  DIAL_DEVICE_NAME                                  "dial_device"
#define  W5500_DEVICE_NAME                                 "w5500_device"


/* SEMAPHORE NAMES CONFIGURATION ---------------------------------------------*/
#define  RS232_RECEIVE_SEM_NAME                            "uart2_recv_sem"
#define  RS485_RECEIVE_SEM_NAME                            "uart3_recv_sem"


/* TASK STACK SIZE CONFIGURATION ---------------------------------------------*/ 
#define  INIT_TASK_STACK_SIZE                              (2048) // 设置初始化启动任务堆栈大小


/* TASK PRIORITIES CONFIGURATION ---------------------------------------------*/ 
#define	 INIT_TASK_PRIORITY                                (RT_THREAD_PRIORITY_MAX / 3) // 设置初始化启动任务优先级[32/3]


/* TASK STACK SIZE CONFIGURATION ---------------------------------------------*/
#define  INIT_TASK_TIME_SLICE                              (20) // 设置初始化启动任务时间片


/* INTERRUPT PRIORITIES CONFIGURATION ----------------------------------------*/
/* 优先级:（1）抢占优先级（PreemptionPriority）占用高四位；
**        （2）子优先级（SubPriority） 占用低四位；
** ---------------------------------------------------------------------------*/
#define  INT_PREEMPPRIO_SIZE                               (4U) // 抢占优先级（PreemptionPriority）占用高四位
#define  INT_SUBPRIO_SIZE                                  (4U) // 子优先级（SubPriority）占用第四位
#define  INT_TICK_PRIO                                     ((0x00<<4)|0x00) // 滴答时钟中断优先级
#define  INT_ETHERNET_PRIO									               ((0x00<<4)|0x01) // 以太网中断优先级最高
#define  INT_USART1_PRIO									                 ((0x03<<4)|0x03) // RS485/RS232外部通信接口
#define  INT_USART2_PRIO									                 ((0x03<<4)|0x01) // RS232内部通信接口，与面板CPU通信
#define  INT_USART3_PRIO									                 ((0x03<<4)|0x04) // RS232外部维护接口
#define  INT_UART4_PRIO									                   ((0x03<<4)|0x04) // RS485/RS232外部通信接口
#define  INT_UART5_PRIO									                   ((0x03<<4)|0x01) // 未用
#define  INT_USART6_PRIO									                 ((0x03<<4)|0x02) // RS232内部通信接口，与测控CPU通信
#define  INT_UART7_PRIO									                   ((0x03<<4)|0x07) // 未用
#define  INT_UART8_PRIO									                   ((0x03<<4)|0x08) // 未用
#define  INT_EXTI9_5_PRIO									                 ((0x05<<4)|0x01) // 无线遥控模块
#define  INT_EXTI15_10_PRIO									               ((0x05<<4)|0x02) // 无线遥控模块
#define  INT_IIC1_PRIO                                     ((0x01<<2)|0x06) // 未用
#define  INT_SPI1_PRIO									                   ((0x01<<2)|0x02) // 未用
#define  INT_SPI2_PRIO									                   ((0x01<<2)|0x02) // FLASH接口
#define  INT_SPI3_PRIO									                   ((0x01<<2)|0x02) // 未用
#define  INT_SPI4_PRIO									                   ((0x01<<2)|0x02) // 未用
#define  INT_SPI5_PRIO									                   ((0x01<<4)|0x01) // W5500接口
#define  INT_SPI6_PRIO									                   ((0x01<<2)|0x02) // 未用
#define  INT_IO_SPI7_PRIO                                  ((0x01<<2)|0x02) // FRAM
#define  INT_IO_SPI8_PRIO									                 ((0x01<<2)|0x02) // 2.4G无线模块
#define  INT_TIM0_PRIO									                   ((0x01<<2)|0x04) // 未用
#define  INT_TIM1_PRIO									                   ((0x01<<2)|0x05) // 未用


/* DRIVER CONFIGURATION ------------------------------------------------------*/


/* APPLICATION PROTOCOL CONFIGURATION ----------------------------------------*/
#define  APP_TASK_FA_CFG                                   (0) // 就地分布式FA配置，暂未使用
#define  APP_TASK_DNP30_SEC_CFG                            (0) // DNP从站配置，暂未使用
#define	 APP_TASK_DLT634_5101_SLAVE_CFG                    (1) // DL/T634.5101从站配置
#define	 APP_TASK_DLT634_5101_MASTER_CFG                   (1) // DL/T634.5101主站配置
#define  APP_TASK_DLT634_5101_LINELOSS_CFG                 (1) // DL/T634.5101线损模块配置
#define	 APP_TASK_DLT634_5104_SLAVE_CFG                    (1) // DL/T634.5104从站配置
#define	 APP_TASK_DLT645_MASTER_CFG                        (0) // DL/T645主站配置，暂未使用
#define  APP_TASK_MODBUS_MASTER_CFG                        (0) // MODBUS协议主站配置，暂未使用
#define  APP_TASK_SDM_MASTER_CFG                           (0) // 内部规约SDM(V3.01)Master模式配置（适用于FTU_V3.01,与主控CPU通信）
#define  APP_TASK_SDM_SLAVE_CFG                            (0) // 内部规约SDM(V3.01)Slave模式配置（适用于FTU_V3.01,与显示CPU及维护接口通信）
#define  APP_TASK_WATCH_CFG                                (1) // 监听任务配置

/* APPLICATION PROTOCOL CONFIGURATION ----------------------------------------*/


#endif /* END _BSP_CFG_H_ */


/* END OF FILE ---------------------------------------------------------------*/



