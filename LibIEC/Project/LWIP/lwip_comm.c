#include "lwip_comm.h" 

#include "delay.h"

#include "ethernetif.h"
#include "rtthread.h"
   
__lwip_dev lwipdev;						//lwip控制结构体 





//用于以太网中断调用
void lwip_pkt_handle(void)
{
	ethernetif_input();
}


//lwip 默认IP设置
//lwipx:lwip控制结构体指针
void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
	u32 sn0;
	sn0=*(vu32*)(0x1FFF7A10);//获取STM32的唯一ID的前24位作为MAC地址后三字节
	//默认远端IP为:192.168.1.111
	lwipx->remoteip[0]=192;	
	lwipx->remoteip[1]=168;
	lwipx->remoteip[2]=10;
	lwipx->remoteip[3]=111;
	//MAC地址设置(高三字节固定为:2.0.0,低三字节用STM32唯一ID)
	lwipx->mac[0]=2;//高三字节(IEEE称之为组织唯一ID,OUI)地址固定为:2.0.0
	lwipx->mac[1]=0;
	lwipx->mac[2]=0;
	lwipx->mac[3]=(sn0>>16)&0XFF;//低三字节用STM32的唯一ID
	lwipx->mac[4]=(sn0>>8)&0XFFF;
	lwipx->mac[5]=sn0&0XFF; 
	//默认本地IP为:192.168.1.30
	lwipx->ip[0]=192;	
	lwipx->ip[1]=168;
	lwipx->ip[2]=10;
	lwipx->ip[3]=100;
	//默认子网掩码:255.255.255.0
	lwipx->netmask[0]=255;	
	lwipx->netmask[1]=255;
	lwipx->netmask[2]=255;
	lwipx->netmask[3]=0;
	//默认网关:192.168.1.1
	lwipx->gateway[0]=192;	
	lwipx->gateway[1]=168;
	lwipx->gateway[2]=60;
	lwipx->gateway[3]=1;	
	lwipx->dhcpstatus=0;//没有DHCP	
} 

/**
* @brief : 网卡硬件初始化
* @param  void
* @return: 0--成功
* @update: [2018-08-02][张宇飞][]
*/
uint8_t NetHardInit(void)
{
    uint8_t retry=0;

    lwip_comm_default_ip_set(&lwipdev);	//设置默认IP等信息
   
	while(DP83848_Init())		       
    {
        retry++;
        if(retry>5) 
        {
            retry=0; 
            return 3; //DP83848初始化失败
        } 
    }
    rt_kprintf("MAC:\n");
    uint16_t i;
    for(i = 0; i < 6; i++)
    {
        rt_kprintf("%2X ", lwipdev.mac[i]);
    }
     rt_kprintf("\n");
    
    ethernetif_init();
    return 0;
}
