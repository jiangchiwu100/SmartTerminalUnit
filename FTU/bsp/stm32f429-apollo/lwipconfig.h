#ifndef __LWIPCONFIG_H__
#define __LWIPCONFIG_H__

/* Network stack -------------------------------------------------------------*/
/* light weight TCP/IP stack */
#define RT_USING_LWIP
/* RT_USING_LWIP141 is not set */
#define RT_USING_LWIP141
/* RT_LWIP_IGMP is not set */
//#define RT_LWIP_IGMP
#define RT_LWIP_ICMP
/* RT_LWIP_SNMP is not set */
#define RT_LWIP_DNS

#define RT_LWIP_ETH_NAME                                               "e0"
//#define RT_LWIP_DHCP
#define IP_SOF_BROADCAST                                               1
#define IP_SOF_BROADCAST_RECV                                          1

/* LWIP_USING_DHCPD is not set */
/* Static IPv4 Address */
#define RT_LWIP_IPADDR                                                 "192.168.60.100"
#define RT_LWIP_GWADDR                                                 "192.168.60.1"
#define RT_LWIP_MSKADDR                                                "255.255.255.0"
#define RT_LWIP_UDP
#define RT_LWIP_TCP
/* RT_LWIP_RAW is not set */
/* RT_LWIP_PPP is not set */
/* RT_LWIP_PPPOE is not set */
/* RT_LWIP_PPPOS is not set */
#define RT_LWIP_PBUF_NUM                                               16
#define RT_LWIP_RAW_PCB_NUM                                            4
#define RT_LWIP_UDP_PCB_NUM                                            4
#define RT_LWIP_TCP_PCB_NUM                                            4
#define RT_LWIP_TCP_SEG_NUM                                            40
#define RT_LWIP_TCP_SND_BUF                                            8196
#define RT_LWIP_TCP_WND                                                8196
#define RT_LWIP_TCPTHREAD_PRIORITY                                     12//12
#define RT_LWIP_SLIPIFTHREAD_PRIORITY                                  1
#define RT_LWIP_TCPTHREAD_MBOX_SIZE                                    8
#define RT_LWIP_TCPTHREAD_STACKSIZE                                    1024
#define RT_LWIP_ETHTHREAD_PRIORITY                                     2//10
#define RT_LWIP_ETHTHREAD_STACKSIZE                                    1024
#define RT_LWIP_ETHTHREAD_MBOX_SIZE                                    32
#define RT_LWIP_ETHTHREAD_TIMESLICE                                    16
#define RT_LWIP_ETHTHREAD_RX_NAME                                      "erx"
#define RT_LWIP_ETHTHREAD_TX_NAME                                      "etx"
//以太网接收优先级
#define RT_LWIP_ETHTHREAD_RX_PRIORITY                               3
#define RT_LWIP_ETHTHREAD_TX_PRIORITY                               11




/* RT_LWIP_REASSEMBLY_FRAG is not set */
#define LWIP_NETIF_STATUS_CALLBACK                                     1
#define SO_REUSE                                                       1
#define LWIP_SO_RCVTIMEO                                               1
#define LWIP_SO_SNDTIMEO                                               1
#define LWIP_SO_RCVBUF                                                 1

#define CHECKSUM_GEN_ICMP 0
#define CHECKSUM_GEN_IP 0
#define CHECKSUM_GEN_UDP 0
#define CHECKSUM_GEN_TCP 0
#define CHECKSUM_CHECK_IP 0
#define CHECKSUM_CHECK_UDP 0
#define CHECKSUM_CHECK_TCP 0
#endif /* __LWIPOPTS_H__ */
