#include "rtthread.h"
#include "main.h"
#include "w5500_demo.h"
#include "config.h"
#include "buffer.h"

void udp_interrupt(void);
void udp_interrupt_test_time(void);
void udp_loop(void);

static void set_network_check(void);
static uint16_t data_check(uint16_t recv_len);

//w5500 中断信号
static struct rt_semaphore    w5500_int_sem;

void mac_test(void);

extern uint8_t txsize[];										// 引用外部变量，声明Socket发送缓存大小
extern uint8_t rxsize[];										// 引用外部变量，声明Socket接收缓存大小

uint8_t buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据


//#define W5500_INT
const uint16_t send_len = 800;

uint8_t dest_ip[4]={192,168,60,120};			// 配置远程IP地址
uint8_t dest_buff[send_len + 100] = "";
uint8_t recv_buff[send_len + 100] = "";

uint8_t remote_ip[4]={192,168,60,111};			// 配置远程IP地址

uint16_t remote_port=5000;								// 配置远程端口
uint16_t  local_port=5000;									// 初始化一个本地端口
uint16 len = 0, result = 0;

uint16_t DataLen = 0;
uint16_t TimeCn = 0;


rt_mailbox_t MacRawReciveMb;



uint8_t UDPTest(void)
{
    for(uint16_t i = 0; i < send_len; i++)
    {
        dest_buff[i] = i + 1;
        recv_buff[i] = 0;
    }
    result = rt_sem_init(&w5500_int_sem, "w5500_int", 0,  RT_IPC_FLAG_FIFO);
    if (result != RT_EOK)
    {
        rt_kprintf("w5500_int sem failure.\r\n"); 
        rt_thread_delay(500);
    }
    /***** GPIO、SPI初始化 *****/
   
   w5500_spi_init();

    
    /***** 硬重启W5500 *****/
    Reset_W5500();


    

#ifdef W5500_INT
    //udp_interrupt();
    udp_interrupt_test_time();
#else   
    //udp_loop();
    mac_test();
#endif
    return 0;
}
static void set_network_check(void)
{
    do
    {
        set_default();
        result = set_network();	// 配置初始化IP信息并打印，初始化8个Socket
        if(result)
        {
             rt_kprintf("w5500 Init Failure.\r\n"); /* 打印线程计数值输出 */
             rt_thread_delay(500);
        }
        else
        {
             rt_kprintf("w5500 Init Success.\r\n"); /* 打印线程计数值输出 */
            break;
        }
        
    } while(1);
}
static uint16_t data_check(uint16_t recv_len)
{
    if (send_len <= recv_len )
    {
        for(uint16_t i = 0; i < send_len; i++)
        {
            if(dest_buff[i] !=  recv_buff[i] )
            {
                return i + 1;
            }
        }
        return 0;
    }
    else
    {
        return 0xFFFF;
    }
        
    
    
}
void udp_interrupt_test_time(void)
{
    MX_TIM10_Init();
            /***** W5500的IP信息初始化 *****/
    set_default(); 	// 设置默认MAC、IP、GW、SUB、DNS
    ConfigMsg.lip[0] = 192;
    ConfigMsg.lip[1] = 168;
    ConfigMsg.lip[2] = 60;
    ConfigMsg.lip[3] = 130;
    remote_ip[0] = 192;
    remote_ip[1] = 168;
    remote_ip[2] = 60;
    remote_ip[3] = 120;
    rt_kprintf("w5500 初始化.\r\n");
    
    set_network_check();
    
    MX_TIM10_Init();
    NVIC_Configuration(); 
    setSIMR(0x01);//使能S0
    setSn_IMR(0, Sn_IR_RECV); //使能接收中断
    do
    {
        switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_UDP:																							// Socket处于初始化完成(打开)状态
                    rt_thread_delay(100);
                    len  = send_len;
                     StartTimer();
                    INDICATE_LED_OFF();
                    sendto(0, dest_buff, len, dest_ip, remote_port);		  		
                   
                    result = rt_sem_take(&w5500_int_sem, RT_WAITING_FOREVER);
                    if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
					}
                    
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((len=getSn_RX_RSR(0))>0)
					{ 
						memset(recv_buff, 0, len+1);
                        
						recvfrom(0,recv_buff, len, remote_ip,&remote_port);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						StopTimer();	                       
                        INDICATE_LED_ON();
                        result = data_check(len);
                        if (!result)
                        {
                            rt_kprintf("T1, %d\r\n", TimeCn);
                        }
                        else
                        {
                            rt_kprintf("ERROR, %d\r\n", result);
                        }
					}
                    
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
					socket(0,Sn_MR_UDP,local_port,0);												// 打开Socket0，并配置为UDP模式，打开一个本地端口
			break;
		}
        
        
    }
    while(1);				   
    
}

    /*Socket状态机，MCU通过读Sn_SR(0)的值进行判断Socket应该处于何种状态
    	Sn_SR状态描述：
    	0x00		SOCK_CLOSED
    	0x13		SOCK_INIT
    	0x14		SOCK_LISTEN
    	0x17		SOCK_ESTABLISHED
    	0x1C		SOCK_CLOSE_WAIT
    	0x22		SOCK_UDP
    */
void udp_interrupt(void)
{
        /***** W5500的IP信息初始化 *****/
    set_default(); 	// 设置默认MAC、IP、GW、SUB、DNS
    ConfigMsg.lip[0] = 192;
    ConfigMsg.lip[1] = 168;
    ConfigMsg.lip[2] = 60;
    ConfigMsg.lip[3] = 120;
    remote_ip[0] = 192;
    remote_ip[1] = 168;
    remote_ip[2] = 60;
    remote_ip[3] = 130;
    
    rt_kprintf("w5500 初始化.\r\n");
    
    set_network_check();
    
    NVIC_Configuration(); 
    setSIMR(0x01);//使能S0
    setSn_IMR(0, Sn_IR_RECV); //使能接收中断
    do
    {
        switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_UDP:																							// Socket处于初始化完成(打开)状态
                   
                    result = rt_sem_take(&w5500_int_sem, RT_WAITING_FOREVER);
                    if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
					}
                    
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((len=getSn_RX_RSR(0))>0)
					{ 
						memset(buffer,0,len+1);
						recvfrom(0,buffer, len, remote_ip,&remote_port);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						//printf("%s\r\n",buffer);															// 串口打印接收到的数据
						sendto(0,buffer,len, remote_ip, remote_port);		  		// 接收到数据后再回给远程上位机，完成数据回环
					}
                    
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
					socket(0,Sn_MR_UDP,local_port,0);												// 打开Socket0，并配置为UDP模式，打开一个本地端口
			break;
		}
        
        
    }
    while(1);				
    
}
char strPool[2000];
void mac_test(void)
{
    MacRawReciveMb = rt_mb_create ("macraw", 100, RT_IPC_FLAG_FIFO);
    
    
    if (!MacRawReciveMb)
    {
        rt_kprintf("rt_mb_create faliure\n");
    }
    
    set_network_check();
    
  
    
    while(1)																							// Socket状态机
	{
         
		switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_MACRAW:																							// Socket处于初始化完成(打开)状态
					
//					if(getSn_IR(0) & Sn_IR_RECV)
//					{
//						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
//					}
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((len= getSn_RX_RSR(0))>0)
					{ 
                       
                        
						uint16_t reciveLen = recvfrom(0,buffer, len, remote_ip, &remote_port);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						
                        if (reciveLen > 0)
                        {
                            PointUint8* pPacket = rt_calloc(1, sizeof(reciveLen));
                            if (!pPacket)
                            {
                                rt_kprintf(" rt_calloc failure\n");
                                break;
                            }
                            pPacket->pData = rt_malloc(reciveLen * sizeof(uint8_t));
                            if (!pPacket->pData)
                            {
                                rt_kprintf(" rt_calloc failure\n");
                                rt_free(pPacket);
                                break;
                            }
                            rt_memcpy(pPacket->pData, buffer, reciveLen);
                            pPacket->len = reciveLen;
                            
                            rt_mb_send (MacRawReciveMb , (rt_uint32_t)(pPacket));
                        }
//                        rt_kprintf("ReciveData len : %d HEX:\n", reciveLen);
//                        uint16_t i;
//                        for(i = 0; i < reciveLen; i++)
//                        {
//                            rt_kprintf("%2X ", buffer[i]);
//                        }
//                    
//                        rt_kprintf("\r\n\n");
                       
					//	sendto(0,buffer,len, remote_ip, remote_port);		  		// 接收到数据后再回给远程上位机，完成数据回环
					//	memset(buffer,0,len+1);
						
						
						
					}
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
			{		
                uint8 ret = socket(0,Sn_MR_MACRAW,local_port,0);		
                if (ret)
                {
                    rt_kprintf("MACRAW Open Sucess!\n");
                }
                else
                {
                    rt_kprintf("MACRAW Open failure!\n");
                }
                break;
            }
		}
        
       // publisher_main();
        rt_thread_delay(200);
	}
}

/**
* @brief :mac输出
* @param void
* @return: uint16_t
* @update: [2018-08-03][张宇飞][]
*/
uint16_t MacRawOutput(uint8_t* pData, uint16_t len)
{
    return  sendto(0, pData, len, remote_ip, 5500);
    
}
/**
* @brief :mac 阻塞输入
* @param void
* @return: uint16_t
* @update: [2018-08-03][张宇飞][]
*/
uint16_t ReciveInputBlock(PointUint8** pPacket)
{
    
    rt_err_t err=  rt_mb_recv(MacRawReciveMb , (rt_uint32_t*)(pPacket), RT_WAITING_FOREVER);
    if (err == RT_EOK)
    {
        
    }
    return err;
}

void udp_loop(void)
{
    set_network_check();
    while(1)																							// Socket状态机
	{
         
		switch(getSn_SR(0))																						// 获取socket0的状态
		{
			case SOCK_UDP:																							// Socket处于初始化完成(打开)状态
					rt_thread_delay(100);
					if(getSn_IR(0) & Sn_IR_RECV)
					{
						setSn_IR(0, Sn_IR_RECV);															// Sn_IR的RECV位置1
					}
					// 数据回环测试程序：数据从远程上位机发给W5500，W5500接收到数据后再回给远程上位机
					if((len=getSn_RX_RSR(0))>0)
					{ 
						memset(buffer,0,len+1);
						recvfrom(0,buffer, len, remote_ip,&remote_port);			// W5500接收来自远程上位机的数据，并通过SPI发送给MCU
						//printf("%s\r\n",buffer);															// 串口打印接收到的数据
						sendto(0,buffer,len, remote_ip, remote_port);		  		// 接收到数据后再回给远程上位机，完成数据回环
					}
			break;
			case SOCK_CLOSED:																						// Socket处于关闭状态
					socket(0,Sn_MR_UDP,local_port,0);												// 打开Socket0，并配置为UDP模式，打开一个本地端口
			break;
		}
        
        
        rt_thread_delay(5);
	}
}
void MX_TIM10_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM10);

  TIM_InitStruct.Prescaler = 179;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xFFFF;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM10, &TIM_InitStruct);
  
    
}
void StartTimer(void)
{
   
    LL_TIM_SetCounter(TIM10, 0);
    LL_TIM_EnableCounter(TIM10);
}
void StopTimer(void)
{
    TimeCn = LL_TIM_GetCounter(TIM10);
    LL_TIM_DisableCounter(TIM10);
   
    rt_kprintf("duty time: %d\n", TimeCn);
   
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
