/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      tcp_server.h
  * @brief:     The tcp server application.
  * @version:   V1.0.1 
  * @author:    LEX
  * @date:      2017.12.06
  * @update:    [YYYY-MM-DD] [更改人姓名][变更描述]
  */
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


/* DEFINE --------------------------------------------------------------------*/
#define  TCPSERVER2404_SOCKET0                      0 // 定义SOCKET0用于建立TCP服务端，端口号为2404
#define  TCP_SERVER_PORT_2404                       2404 // 定义DL/T634.5104规约任务tcp server的端口

#define  TCP_SERVER_2404_RX_BUFSIZE                 (256*14) // 定义tcp server最大接收数据长度
#define  TCP_SERVER_2404_TX_BUFSIZE                 512 // 定义tcp server最大发送数据长度

#define  LWIP_SEND_DATA                             0X80 // 定义TCP server有数据发送
#define  LWIP_TCP_SERVER_LINKUP                     0X40 // 定义TCP server处于连接状态
#define  LWIP_TCP_SERVER_DISCONNECT                 0X20 // 定义TCP server断开连接状态


/* PUBLIC FUNCTION -----------------------------------------------------------*/
extern void rt_w5500_tcpserver_thread_entry(void *param); // TCP服务器任务线程 
extern uint8_t w5500_tcpserver_linkstate(void); // 判断链路状态
extern void w5500_tcpserver_disconnect(void); // 主动断开链路
extern uint16_t w5500_tcpserver_getc(uint8_t* str, uint16_t length);
extern uint16_t w5500_tcpserver_putc(uint8_t* str, uint16_t length);

extern void rt_dp83848_tcpserver_thread_entry(void *param); // TCP服务器任务线程 
extern uint8_t dp83848_tcpserver_linkstate(void); // 判断链路状态
extern void dp83848_tcpserver_disconnect(void); // 主动断开链路
extern uint16_t dp83848_tcpserver_getc(uint8_t* str, uint16_t length);
extern uint16_t dp83848_tcpserver_putc(uint8_t* str, uint16_t length);


#endif /* END __TCP_SERVER_H__ */


/* END OF FILE ---------------------------------------------------------------*/


