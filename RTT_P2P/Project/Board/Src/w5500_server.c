/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      w5500_udp.c
  * @brief:     
  * @version:   V0.0.1 
  * @author:    Zhang Yufei
  * @date:      2018-07-21 w5500 udp server专用
  * @update:    
  */
  
#include "ll_driver.h"  
#include "stm32f429xx.h" 
#include "w5500_server.h"
#include "drv_w5500_socket.h"
#include "drv_w5500.h"

#include "distribution.h"
#include "distribution_config.h"
#include "distribution_app.h"

#include "NetFinshApp.h"
#include "extern_interface.h"


uint8_t SocketNum = 0; //使用的W5500 Socket号
uint8_t SocketMaintanceNum = 1; //使用的维护 Socket号



static struct rt_thread udpserver_thread;

void MaintaceServer(void);

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_udpserver_thread_stack[THREAD_UDPSERVER_STACK_SIZE];//线程栈
ALIGN(RT_ALIGN_SIZE)
static uint8_t UdpReciveBuffer[1024];
ALIGN(RT_ALIGN_SIZE)
static uint8_t UdpSendBuffer[1024];
static uint8_t DefautIp[4];
static uint16_t LocalPort; 
static uint16_t LocalMaintancePort;
static uint16_t RemotePort;
static struct rt_mutex udp_mutex;
static struct rt_semaphore    w5500_int_sem;
#define ON_LOCK()   {     result = rt_mutex_take(&udp_mutex, RT_WAITING_FOREVER); }
#define OFF_LOCK()  {     if (result  == RT_EOK) {rt_mutex_release(&udp_mutex);}};



/**
  * @brief :W5500用于UDP通信
  * @param  void *param
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */  
static inline void W5500Init(void)
{
    MEMSET(UdpReciveBuffer, 0, sizeof(UdpReciveBuffer));
    MEMSET(UdpSendBuffer, 0, sizeof(UdpSendBuffer));
    DefautIp[0] = 192;
    DefautIp[1] = 168;
    DefautIp[2] = 10;
    DefautIp[3] = 111;
    LocalPort = 5555;
    RemotePort = 5555;
    LocalMaintancePort = 5500;
    rt_err_t err = rt_mutex_init (&udp_mutex, "udp_mutex", RT_IPC_FLAG_PRIO );
    if (err != RT_EOK)
    {
        while(1);//TODO:此种有问题
    }
    //先产生资源
   // rt_mutex_release(udp_mutex);
    
}


static void W5500_SetDefaultNetInfo( wiz_NetInfo* pNetinfo)
{

    if (g_StationManger.pWorkPoint)
    {
        pNetinfo->ip[0] =  GET_N_BYTE( g_StationManger.pWorkPoint->id, 3);
        pNetinfo->ip[1] =  GET_N_BYTE( g_StationManger.pWorkPoint->id, 2);
        pNetinfo->ip[2] =  GET_N_BYTE( g_StationManger.pWorkPoint->id, 1);
        pNetinfo->ip[3] =  GET_N_BYTE( g_StationManger.pWorkPoint->id, 0);
    
    }
    else
    {
        pNetinfo->ip[0] =  192;
        pNetinfo->ip[1] =  168;
        pNetinfo->ip[2] =  10;
        pNetinfo->ip[3] =  249; //默认IP
    }
    

	
    pNetinfo->mac[0] = 0x00;
    pNetinfo->mac[1] = 0x80;
    pNetinfo->mac[2] = 0xE1;
    pNetinfo->mac[3] = *(rt_uint8_t*)(UID_BASE + 0);
    pNetinfo->mac[4] = *(rt_uint8_t*)(UID_BASE + 2);
    pNetinfo->mac[5] = *(rt_uint8_t*)(UID_BASE + 4);
    

    pNetinfo->sn[0] = 255;
    pNetinfo->sn[1] = 255;
    pNetinfo->sn[2] = 255;
    pNetinfo->sn[3] = 0;
    pNetinfo->gw[0] = 0;
    pNetinfo->gw[1] = 0;
    pNetinfo->gw[2] = 0;
    pNetinfo->gw[3] = 0;
    pNetinfo->dns[0] = 114;
    pNetinfo->dns[1] = 114;
    pNetinfo->dns[2] = 114;
    pNetinfo->dns[3] = 114;
    
   


}
/**
  * @brief  w5500 device config.
  * @param  None.
  * @retval None.
  */
void w5500_config(void)
{
    wiz_NetInfo wiz_netinfo;
	uint8_t tmpstr[6];
//    if (g_EthW5500.ip[0] != 0)
//    {
//    memcpy(wiz_netinfo.mac, g_EthW5500.mac, 6);
//    memcpy(wiz_netinfo.ip, g_EthW5500.ip, 4);
//    memcpy(wiz_netinfo.sn , g_EthW5500.netmask, 4);
//    memcpy(wiz_netinfo.gw , g_EthW5500.gateway, 4);
//    memcpy(wiz_netinfo.dns, g_EthW5500.dns, 4);
//    }
//    else
    {
        W5500_SetDefaultNetInfo(&wiz_netinfo);
    }

	  // Setting default network information
		ctlnetwork(CN_SET_NETINFO, (void*)&wiz_netinfo);
	
	  // Get network configuration information
		ctlnetwork(CN_GET_NETINFO, (void*)&wiz_netinfo);

		// Display Network Information
		ctlwizchip(CW_GET_ID, (void*)tmpstr);
}

/**
  * @brief :W5500系统初始化
  * @param  void *param
  * @return: void
  * @update: [2018-07-31][张宇飞][创建]
  */
void EmmedNetInit(void)
{
    rt_err_t result;
    rt_hw_w5500_init();
    w5500_config();
    W5500Init();
     //TODO:加入初始化成功标识
    ON_LOCK();
    if((w5500_socket(SocketMaintanceNum, Sn_MR_UDP, LocalMaintancePort, 0)) == SocketMaintanceNum)
    {
        g_StationManger.isMaintanceRun = true;
        rt_kprintf("W5500 start init Sucess!\n");
    }
    result = rt_sem_init(&w5500_int_sem, "w5500_int", 0,  RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("w5500_int sem failure.\r\n");        
    }
    OFF_LOCK();
   
}    


void NVIC_Configuration(void)
{
    LL_EXTI_InitTypeDef EXTI_InitStruct;
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE3);
    
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_3;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init(&EXTI_InitStruct);
    
    LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
    LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
    
    
 //   LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTE, LL_SYSCFG_EXTI_LINE3);
    
  /* EXTI interrupt init*/
    NVIC_SetPriority(EXTI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),INT_ETH_PRIO, INT_ETH_PRIO));
    NVIC_EnableIRQ(EXTI3_IRQn);    
}
/**
  * @brief :W5500用于UDP通信
  * @param  void *param
  * @return: void
  * @update: [2018-07-21][张宇飞][创建]
  */
static void udpserver_thread_entry(void *param)
{  
    int32_t ret;
    uint16_t length = 0;    
    uint8_t srcip[4];
    uint8_t cn = 0;
    uint16_t destport;	
    rt_err_t result;


	NVIC_Configuration(); 
    setSIMR(0x01);//
    setSn_IMR(SocketNum, Sn_IR_RECV); //
	uint8_t get_result =0;
	
    rt_kprintf("udpserver start!\n");
   
    for (;;)
    {
       
        ON_LOCK()
		get_result = getSn_SR(SocketNum);
        OFF_LOCK();
		switch (get_result)
		{
			case SOCK_UDP:		
                result = rt_sem_take(&w5500_int_sem, 200);
                if (result == -RT_ETIMEOUT)
                {
                    break;
                }
                if (result != RT_EOK)
                {
                    perror("result = rt_sem_take() , ERROR: %d\n", result);
                    break;
                }                
                do
                {
                   
                    ON_LOCK();
                    if(getSn_IR(SocketNum) & Sn_IR_RECV)
					{
						setSn_IR(SocketNum, Sn_IR_RECV);// Sn_IR的RECV位置1
					}
                    
                    length = getSn_RX_RSR(SocketNum);
                    OFF_LOCK();
                                        
                    if (length <= 0)
                    {
                        break;
                    }
                    else
                    {
                        ON_LOCK();
                        ret = w5500_recvfrom(SocketNum, UdpReciveBuffer, length, srcip, &destport);
                        OFF_LOCK();
                        if(ret > 0)
                        {                            
                           StationPointFrameDeal(UdpReciveBuffer, ret);
                        //    w5500_sendto(SocketNum, UdpReciveBuffer, length, DefautIp, LocalPort);
                        }
                    }    
                }while(true);
								
                break;
			case SOCK_CLOSED:	
                ON_LOCK();
				if ((ret = w5500_socket(SocketNum, Sn_MR_UDP, LocalPort, 0)) != SocketNum)
				{
					rt_kprintf("udpserver close!\n");
				} 
                OFF_LOCK();
				//rt_thread_delay(1);
				break;

			default:
				break;
		}
		
       
       // rt_thread_delay(4);
       // if (cn ++ > 10)
        {
            cn = 0;
           //
            MaintaceServer();
        }
		
    }
}


/**
* @brief :维护服务
* @param  void
* @return: 0--正常
* @update: [2018-07-23][张宇飞][创建]
*/
 void MaintaceServer(void)
{    
    extern ProtocolAnylast LocalAnylast;
    uint8_t srcip[4];
    uint16_t destport;	
    int32_t ret;
    uint16_t length = 0; 
    rt_err_t result;
    ON_LOCK();    
    rt_err_t  get_result = getSn_SR(SocketMaintanceNum);
    OFF_LOCK();
    g_StationManger.isMaintanceRun = true;
    switch (get_result)
    {
        case SOCK_UDP:			
            do
            {
                g_StationManger.isMaintanceRun = true;
                 ON_LOCK();   
                if(getSn_IR(SocketMaintanceNum) & Sn_IR_RECV)
                {
                    setSn_IR(SocketMaintanceNum, Sn_IR_RECV);// Sn_IR的RECV位置1
                }
                length = getSn_RX_RSR(SocketMaintanceNum);	
                 OFF_LOCK();
                if (length <= 0)
                {
                    break;
                } 
                else
                {
                    ON_LOCK();   
                    ret = w5500_recvfrom(SocketMaintanceNum, UdpReciveBuffer, length, srcip, &destport);
                    OFF_LOCK();
                    if(ret > 0 )
                    {
                        MantaiceFrameDeal(UdpReciveBuffer, ret);                           
                    //    w5500_sendto(SocketNum, UdpReciveBuffer, length, DefautIp, LocalPort);
                    }
                        
                }    
            }while(true);
                            
            break;
        case SOCK_CLOSED:	
            ON_LOCK();   
            if ((ret = w5500_socket(SocketMaintanceNum, Sn_MR_UDP, LocalMaintancePort, 0)) != SocketNum)
            {
                g_StationManger.isMaintanceRun = false;
                rt_kprintf("udpserver close!\n");
            } 
            OFF_LOCK();
            //rt_thread_delay(1);
            break;

        default:
            break;
    }
}

#include "datagram.h"


/**
  * @brief :外部发送,不进行参数检查
  * @param void
  * @return: 0--正常
  * @update: [2018-07-23][张宇飞][创建]
  *          [2018-09-14][李  磊][将之前使用W5500改为使用dp83848发送]
*/
ErrorCode ExternSend(PointUint8* pPacket)
{
    rt_err_t result;
/* 
    uint8_t destId[4] = {0};
    destId[0] = 192;
    destId[1] = 168;
    destId[2] = pPacket->pData[FRAME_DEST_INDEX + 1];
    destId[3] = pPacket->pData[FRAME_DEST_INDEX];
    ON_LOCK();
    int ret = w5500_sendto(SocketNum, pPacket->pData, pPacket->len, destId, RemotePort);
    OFF_LOCK();
    if(ret == pPacket->len)
    {
        return ERROR_OK_NULL;
    }
    else
    {
        return ERROR_UNKONOW;
    }
 */

	// FifoStringEnqueue(UDP_ServeFifoHandle, pPacket->pData, pPacket->len);
//    UDP_NetconnSendString(g_UDP_ServeNetconn, pPacket->pData);
	return ERROR_OK_NULL;
}
/**
  * @brief :监控使用
  * @param void
  * @return: 0--正常
  * @update: [2018-07-23][张宇飞][创建]
  *          [2018-09-17][李  磊][将之前使用W5500改为使用dp83848发送，此处放入消息队列，在任务中进行出队发送]
*/
void Monitor(void)
{    
    extern DatagramTransferNode g_VirtualNode;
    rt_err_t result;
	RingQueue* ring = &(g_VirtualNode.reciveRing);
	DatagramFrame* frame;
    // DefautIp[0] = 192;
    // DefautIp[1] = 168;
    // DefautIp[2] = 10;
    // DefautIp[3] = 111;
	do
	{

		bool state = ring->Read(ring, (void**)&frame);
		if (state)
		{
            // ON_LOCK();   
			// w5500_sendto(SocketMaintanceNum, frame->pData, frame->size, DefautIp, RemotePort);	
            // OFF_LOCK();

//            UDP_NetconnSendString(g_MaintenanceServeNetconn, frame->pData);
			Datagram_Destory(frame);
		}
		else
		{
			break;
		}
	} while (true);
    
}

/**
* @brief :UDP任务初始化
* @param  void
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void rt_kprintf(const char *fmt, ...)
{
//    if (!g_StationManger.isMaintanceRun)
//    {
//        return;
//    }
    rt_err_t result;
    
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);

    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;

//    ON_LOCK();
    // w5500_sendto(SocketMaintanceNum, (uint8_t*)rt_log_buf, rt_strlen(rt_log_buf) + 1, DefautIp, 5533);

    if(true == NetFinshFlag)		/*UDP_FinshFlag为1说明UDP打印已经初始化，可以使用了*/
	{
		FifoStringEnqueue(PrintfFifoHandle, (uint8_t*)rt_log_buf, length);
	}
	else
	{
		#if defined(RT_USING_DEVICE) && defined(RT_USING_CONSOLE)
			static rt_device_t _console_device = RT_NULL;
		#endif

		_console_device = rt_console_get_device();			/*获取当前控制台输出的串口设备号*/
		rt_uint16_t old_flag = _console_device->open_flag;

        _console_device->open_flag |= RT_DEVICE_FLAG_STREAM;
        rt_device_write(_console_device, 0, rt_log_buf, length);
        _console_device->open_flag = old_flag;
		
	}
//    OFF_LOCK();
   
    
    va_end(args);
}




/**
* @brief  : 外部发送
* @param  : 队列句柄
* @param  ：DatagramTransferNode* node 
* @param  ：PointUint8* pPacket
* @return: ErrorCode
* @update: [2018-07-23][张宇飞][]
*/
ErrorCode Udp_SendPacketNode(DatagramTransferNode* node, PointUint8* pPacket)
{       
	CHECK_POINT_RETURN(node, NULL, ERROR_NULL_PTR);
	CHECK_POINT_RETURN(pPacket, NULL, ERROR_NULL_PTR);

	ErrorCode error = ExternSend(pPacket);    
    //释放内存
	FREE(pPacket->pData);
	return error;
}


void EXTI3_IRQHandler(void)
{
  rt_interrupt_enter();
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
   //Intflag = getSn_IR(0);  
    rt_sem_release(&w5500_int_sem);
  }
  rt_interrupt_leave();
}


/**
* @brief :UDP任务初始化
* @param  void
* @return: 0--正常
* @update: [2018-07-21][张宇飞][创建]
*/
void UdpServerAppInit(void)
{
    
	rt_thread_init(&udpserver_thread,                 //线程控制块
		THREAD_UDPSERVER_NAME,                       //线程名字，在shell里面可以看到
		udpserver_thread_entry,            //线程入口函数
		RT_NULL,                      //线程入口函数参数
		rt_udpserver_thread_stack,     //线程栈起始地址
		sizeof(rt_udpserver_thread_stack), //线程栈大小
		THREAD_UDPSERVER_PRIORITY,                            //线程的优先级
		THREAD_UDPSERVER_TIMESLICE);                          //线程时间片

	rt_thread_startup(&udpserver_thread);

	
}



