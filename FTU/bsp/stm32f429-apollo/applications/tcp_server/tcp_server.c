/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      tcp_server.c
  * @brief:     The tcp server application.
  * @version:   V03.001
  * @author:    Lexun
  * @date:      2017-12-06
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  * @update:    [2018-03-01][Lexun][modify the variables names]
  */
  
  
/* INCLUDE FILES -------------------------------------------------------------*/
#include "stdio.h"
#include "tcp_server.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "queue.h"
#include "goose.h"
#include "drv_w5500.h"
#include "common_data.h"
#include "lwip/sockets.h"
#ifdef RT_USING_W5500
#include ".\MultiThread\multi_thread.h"
#include "drv_w5500_socket.h"
#endif /* RT_USING_W5500 */


/* PRIVATE VARIABLES ---------------------------------------------------------*/

#if RT_USING_DP83848 
static unsigned char *DP83848TcpServerRxBuf; // [TCP_SERVER_2404_RX_BUFSIZE] __attribute__((at(0x00000000 + SDRAM_ADDR_104))); // TCP服务端数据接收缓冲区
static unsigned char *DP83848TcpServerTxBuf; // [TCP_SERVER_2404_TX_BUFSIZE]; // TCP服务端数据发送缓冲区
static DataQueue DP83848TcpServerRxCB; // TCP SERVER接收缓冲队列控制块
static unsigned char DP83848TcpServerFlag; // TCP服务器数据发送标志位
static unsigned short DP83848TcpServerTxLen; // TCP服务器发送数据长度
#endif /* END RT_USING_DP83848 */

#if RT_USING_W5500 
static unsigned char *W5500_UDP_RxBuf; // [TCP_SERVER_2404_RX_BUFSIZE] __attribute__((at(0x00001000 + SDRAM_ADDR_104))); // TCP服务端数据接收缓冲区
static unsigned char *W5500_UDP_TxBuf; // [TCP_SERVER_2404_TX_BUFSIZE]; // TCP服务端数据发送缓冲区
static DataQueue W5500TcpServerRxCB; // TCP SERVER接收缓冲队列控制块
static unsigned char W5500TcpServerFlag; // TCP服务器数据发送标志位
static unsigned short W5500TcpServerTxLen; // TCP服务器发送数据长度
static unsigned short W5500_UDP_TxLen; // TCP服务器发送数据长度
#endif /* END RT_USING_W5500 */


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/

#ifdef RT_USING_W5500
/**
  * @brief : Initiliaztion the tcp server of W5500.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
static inline void w5500_udp_init(void)
{    
    W5500_UDP_RxBuf = (unsigned char *)rt_malloc(UDP_8080_RX_BUFSIZE);
    W5500_UDP_TxBuf = (unsigned char *)rt_malloc(UDP_8080_TX_BUFSIZE);
	
//    if (QueueCreate(&W5500TcpServerRxCB, W5500_UDP_RxBuf, UDP_8080_RX_BUFSIZE, NULL, NULL) == QUEUE_ERR) 
//    {
//        return; // 消息队列创建错误 
//    }
    memset(W5500_UDP_RxBuf, 0, UDP_8080_RX_BUFSIZE); // 数据接收缓冲区清零
    memset(W5500_UDP_TxBuf, 0, UDP_8080_TX_BUFSIZE); // 数据发送缓冲区清零
}

/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
/**
  * @brief : The thread body of tcp server for W5500.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void rt_w5500_udp_thread_entry(void *param)
{
    rt_err_t result;
    int32_t ret;
    uint8_t socketNO = UDPSERVER2404_SOCKET1;
    uint16_t length = 0;
	uint8_t goose_have_change = 0;
    uint8_t buf[256];
    uint8_t srcip[4];
	uint8_t defautip[4] = {192,168,60,255};	
    uint16_t destport;	
    w5500_udp_init();
	
    setSIMR(0x01);//使能S0
    setSn_IMR(socketNO, Sn_IR_RECV); //使能接收中断
	
    for (;;)
    {   		
        result = rt_event_recv(&w5500_event, EVENT_REC_IRQ_W5500 | EVENT_RUN | EVENT_GOOSE_HAVE_CHANGE, RT_EVENT_FLAG_OR, RT_WAITING_FOREVER, RT_NULL); 
		
		switch (getSn_SR(socketNO))
		{
			case SOCK_UDP:
				if ((w5500_event.set & EVENT_RUN) || (w5500_event.set & EVENT_GOOSE_HAVE_CHANGE))
				{
					if (w5500_event.set & EVENT_GOOSE_HAVE_CHANGE)
					{
						goose_have_change = 1;
					    w5500_event.set &= ~EVENT_GOOSE_HAVE_CHANGE;
					}
					
					if (w5500_event.set & EVENT_RUN)
					{
					    w5500_event.set &= ~EVENT_RUN;
					}					
					
					W5500_UDP_TxLen = goose_publisher_process(1, (struct TagGooseLink *)W5500_UDP_TxBuf, goose_have_change);
					
					if (goose_have_change)
					{
						goose_have_change = 0;
					}
					
					if (W5500_UDP_TxBuf[0])
					{
						TIM7->CNT = 0;
						w5500_sendto(socketNO, W5500_UDP_TxBuf, W5500_UDP_TxLen, defautip, 8080);		
						memset(W5500_UDP_TxBuf, 0, UDP_8080_TX_BUFSIZE);
					}					
				}	
				
				if (w5500_event.set & EVENT_REC_IRQ_W5500)						
				{
					w5500_event.set &= ~EVENT_REC_IRQ_W5500;	
					
					if (getSn_IR(socketNO) & Sn_IR_RECV)
					{
					    setSn_IR(socketNO, Sn_IR_RECV);
					}					
					
					while ((length = getSn_RX_RSR(socketNO)) > 0)
					{				
                        length = length > UDP_8080_RX_BUFSIZE ? UDP_8080_RX_BUFSIZE : length;
					
						ret = w5500_recvfrom(socketNO, W5500_UDP_RxBuf, length, srcip, &destport);					
						
						if (ret <= 0)
						{
							break;
						}
						else
						{
							goose_receiver_processe(W5500_UDP_RxBuf, srcip);						
						}						
					}										
				}				

			case SOCK_CLOSED:				
				if ((ret = w5500_socket(socketNO, Sn_MR_UDP, 8080, 0)) != socketNO)
				{
					
				} 
				//rt_thread_delay(1);
				break;

			default:
				break;
		}
    }
}

/**
  * @brief : Check the link state of tcp server for W5500.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint8_t w5500_tcpserver_linkstate(void)
{
    if ((W5500TcpServerFlag & LWIP_TCP_SERVER_LINKUP) == LWIP_TCP_SERVER_LINKUP)
    {
        return (1);
    }
    else
    {
        return (0);
    }										
}

/**
  * @brief : Disconnect the link of tcp server by W5500.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void w5500_tcpserver_disconnect(void)
{
    W5500TcpServerFlag |= LWIP_TCP_SERVER_DISCONNECT;			
}

/**
  * @brief : Get the data from tcp server by W5500.
  * @param : [str] The data which be read
  * @param : [length] The length of data 
  * @return: Return the length of data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint16_t w5500_tcpserver_getc(uint8_t * str, uint16_t length)
{	
    uint16_t temp = 0;
	
    if ((str == 0) || (length == 0))
    {
        return (0);
    }
		
    for (temp = 0; temp < length; temp++, str++)
    {				
        if (QueueRead(str, &W5500TcpServerRxCB) == QUEUE_EMPTY)
        {
            break;
        }
    }
				
    return (uint16_t)(temp);
}

/**
  * @brief : Put the data to tcp server by W5500.
  * @param : [str] The data which be read
  * @param : [length] The length of data 
  * @return: Return the length of data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint16_t w5500_tcpserver_putc(uint8_t *str, uint16_t length)
{	
    if ((str == 0) || (length == 0) || (W5500TcpServerTxLen != 0))
    {
        return (uint16_t)(0);
    }
		
    if (length < TCP_SERVER_2404_TX_BUFSIZE)
    {
        //memcpy(W5500TcpServerTxBuf, str, length);
        W5500TcpServerTxLen = length;
        W5500TcpServerFlag |= LWIP_SEND_DATA;
			
        return (W5500TcpServerTxLen);
    }
		
    return (uint16_t)(0);
}
#endif /* RT_USING_W5500 */

#ifdef RT_USING_DP83848
/**
  * @brief : Initiliaztion the tcp server of DP83848.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
static inline void dp83848_tcpserver_init(void)
{	    
    DP83848TcpServerFlag = 0; // TCP SERVER对应端口2404标志位
    
    DP83848TcpServerRxBuf = (unsigned char *)rt_malloc(TCP_SERVER_2404_RX_BUFSIZE);
    DP83848TcpServerTxBuf = (unsigned char *)rt_malloc(TCP_SERVER_2404_TX_BUFSIZE);
	
    if (QueueCreate(&DP83848TcpServerRxCB, DP83848TcpServerRxBuf, TCP_SERVER_2404_RX_BUFSIZE, NULL, NULL) == QUEUE_ERR) 
    {
        return; // 消息队列创建错误 
    }
    memset(DP83848TcpServerRxBuf, 0, TCP_SERVER_2404_RX_BUFSIZE); // 数据接收缓冲区清零
    memset(DP83848TcpServerTxBuf, 0, TCP_SERVER_2404_TX_BUFSIZE); // 数据发送缓冲区清零
}

/**
  * @brief : The thread body of tcp server of DP83848.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void rt_dp83848_tcpserver_thread_entry(void *param)
{
    int sock;
    rt_uint32_t sin_size = sizeof(struct sockaddr_in); 
    int accept_sock; 
    rt_int32_t ret = 0, recv_size = 0;
    rt_bool_t running = RT_TRUE;
    rt_uint32_t timeout = 10;
    uint8_t buf[512];
    err_t err;

    struct sockaddr_in server_addr, client_addr;
    fd_set recvset,sentset;
//    fd_set sentset;
    struct timeval tv;
		
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    dp83848_tcpserver_init();
		
    if ((sock = lwip_socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2404);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    rt_memset(&(server_addr.sin_zero), 8, sizeof(server_addr.sin_zero));

    if (lwip_bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        return;
    }

    if (lwip_listen(sock, 1) == -1)
    {
        return;
    }
		
    while (1)
    {
        accept_sock = lwip_accept(sock, (struct sockaddr *)&client_addr, &sin_size);

        lwip_setsockopt(accept_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        running = RT_TRUE;
        
        DP83848TcpServerFlag = 0x00;
        DP83848TcpServerFlag |= LWIP_TCP_SERVER_LINKUP;
		DP83848TcpServerTxLen = 0;
				
        while (running)
        {
            FD_ZERO(&recvset);
            FD_SET(accept_sock, &recvset);
//            FD_ZERO(&sentset);
//            FD_SET(accept_sock, &sentset);
            
            ret = lwip_select(accept_sock+1, &recvset, 0, 0, &tv);
          
            if (ret < 0)
            {
                running = RT_FALSE;
            }
          
            if (ret == 0)
            {
                if ((DP83848TcpServerFlag & LWIP_SEND_DATA) == LWIP_SEND_DATA) 
                {
                    DP83848TcpServerFlag &= ~LWIP_SEND_DATA;
                    err = lwip_send(accept_sock, DP83848TcpServerTxBuf, DP83848TcpServerTxLen, 0);
                    DP83848TcpServerTxLen = 0;
                    if (err == -1) 
                    {
                        running = RT_FALSE;
                    }
                }
            }
						
            if (ret > 0)
            {
                if (FD_ISSET(accept_sock, &recvset))
                {
                    recv_size = lwip_recv(accept_sock, buf, 512, 0);
                    
                    if((recv_size <= 0)||(recv_size > 512))
                    {
                        running = RT_FALSE;
                    }
                    else
                    {
                        QueueWriteBlock(&DP83848TcpServerRxCB, buf, recv_size);
                    }
                }
/*
                if (FD_ISSET(accept_sock, &sentset))
                {
                    if (iec104_client->ctx.time_out_flag==1||iec104_client->ctx.time_out_flag == 2)
                    {
                        running = RT_FALSE;
                    }

                    if (iec104_client_worker_entry(iec104_client, 0) == 0)
                    {
                        syslog(LOG_COMMUNICATE, 1, socketname, 1);
                        rt_kprintf("iec104 send error.\n");
                        running = RT_FALSE;
                    }
                }
*/
            }
			         
            if ((DP83848TcpServerFlag & LWIP_TCP_SERVER_DISCONNECT) == LWIP_TCP_SERVER_DISCONNECT)
            {
                DP83848TcpServerFlag &= ~LWIP_TCP_SERVER_DISCONNECT;
                running = RT_FALSE;
            }
        }
        lwip_close(accept_sock);
        DP83848TcpServerFlag &= ~LWIP_TCP_SERVER_LINKUP;
        rt_thread_delay(500);
    }
}


/**
  * @brief : Check the link state of tcp server of DP83848.
  * @param : none
  * @return: [1] tcp server linkup.
  * @return: [0] tcp server linkdown.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint8_t dp83848_tcpserver_linkstate(void)
{
    if ((DP83848TcpServerFlag & LWIP_TCP_SERVER_LINKUP) == LWIP_TCP_SERVER_LINKUP)
    {
        return (1);
    }
    else
    {
        return (0);
    }										
}

/**
  * @brief : Disconnect the link of tcp server of DP83848.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
void dp83848_tcpserver_disconnect(void)
{
    DP83848TcpServerFlag |= LWIP_TCP_SERVER_DISCONNECT;			
}

/**
  * @brief : Get the data from tcp server of DP83848.
  * @param : [str] The data
  * @param : [length] The length of data
  * @return: return the length of data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint16_t dp83848_tcpserver_getc(uint8_t * str, uint16_t length)
{	
    rt_uint16_t temp = 0;
	
    if ((str == 0) || (length == 0))
    {
        return (0);
    }

    for (temp = 0; temp < length; temp++, str++)
    {				
        if (QueueRead(str, &DP83848TcpServerRxCB) == QUEUE_EMPTY)
        {
            break;
        }
    }
				
    return (uint16_t)(temp);
}

/**
  * @brief : Put the data to tcp server of DP83848.
  * @param : [str] The data
  * @param : [length] The length of data
  * @return: return the length of data.
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
uint16_t dp83848_tcpserver_putc(uint8_t * str, uint16_t length)
{	
    if ((str == 0) || (length == 0) || (DP83848TcpServerTxLen != 0))
    {
        return (uint16_t)(0);
    }
		
    if (length < TCP_SERVER_2404_TX_BUFSIZE)
    {
        memcpy(DP83848TcpServerTxBuf, str, length);
        DP83848TcpServerTxLen = length;
        DP83848TcpServerFlag |= LWIP_SEND_DATA;
			
        return (DP83848TcpServerTxLen);
    }
    return (uint16_t)(0);
}
#endif /* END RT_USING_DP83848 */ 


/* END OF FILE ---------------------------------------------------------------*/



