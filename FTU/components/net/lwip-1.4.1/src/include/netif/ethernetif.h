 /**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      ethernetif.h
  * @brief:     This file is part of RT-Thread RTOS.
  * @version:   V03.001(first implementation)
  * @author:    Bernard
  * @date:      2009-01-05
  * @updata:    [2014-04-27][Bernard][make the code cleanup]
  * @updata:    [2018-01-23][Lexun][make the code cleanup]
  */
#ifndef __NETIF_ETHERNETIF_H__
#define __NETIF_ETHERNETIF_H__


/* INCLUDE FILES -------------------------------------------------------------*/
#include "lwip/netif.h"
#include <rtthread.h>


/* DEFINE --------------------------------------------------------------------*/	
#define NIOCTL_GADDR		0x01
#ifndef RT_LWIP_ETH_MTU
#define ETHERNET_MTU		1500
#else
#define ETHERNET_MTU		RT_LWIP_ETH_MTU
#endif

/* eth flag with auto_linkup or phy_linkup */
#define ETHIF_LINK_AUTOUP	0x0000
#define ETHIF_LINK_PHYUP	0x0100


/* STRUCT --------------------------------------------------------------------*/	
struct eth_device
{
    /* inherit from rt_device */
    struct rt_device parent;

    /* network interface for lwip */
    struct netif *netif;
    struct rt_semaphore tx_ack;

    rt_uint16_t flags;
    rt_uint8_t  link_changed;
    rt_uint8_t  link_status;

    /* eth device interface */
    struct pbuf* (*eth_rx)(rt_device_t dev);
    rt_err_t (*eth_tx)(rt_device_t dev, struct pbuf* p);
};


/* PUBLIC FUNCTION -----------------------------------------------------------*/
rt_err_t eth_device_ready(struct eth_device* dev);
rt_err_t eth_device_init(struct eth_device * dev, char *name);
rt_err_t eth_device_init_with_flag(struct eth_device *dev, char *name, rt_uint16_t flag);
rt_err_t eth_device_linkchange(struct eth_device* dev, rt_bool_t up);

int eth_system_device_init(void);


#endif /* __NETIF_ETHERNETIF_H__ */


/* END OF FILE ---------------------------------------------------------------*/

