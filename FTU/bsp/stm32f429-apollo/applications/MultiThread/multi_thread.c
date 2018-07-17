/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      create_thread.c
  * @brief:     Create all threads.
  * @version:   V1.0.0 
  * @author:    Mr.Sun
  * @date:      2017-10-26
  * @update:    [2017-12-07][Lexun][make the code cleanup]
  */
  
/* INCLUDE FILES -------------------------------------------------------------*/
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include ".\MultiThread\multi_thread.h"
#include <dfs_posix.h>
#include "uffs/uffs.h"
#include "drv_timer.h"
#include "calculator.h"
#include "breaker_interface.h"
#include "load_switch_interface.h"
#include "distributed_interface.h"
#include "other_protect.h"
#include "common_data.h"
#include "drv_w5500.h"
#include "drv_led.h"
#include "drv_do.h"
#include "drv_ad7616.h"
#include "drv_handheld.h"
#include "dlt634_5101slave_disk.h"
#include "dlt634_5104slave_disk.h"
#include "tcp_server.h"
#include "file_operate.h"
#include "channel_monitor.h"
#include "gui_common.h"
#include "gui_101_cmd.h"
#include "hmi_101_disk.h"
#include "GUIdisplay.h"

/* PRIVATE VARIABLES ---------------------------------------------------------*/
//static struct rt_thread rt_thread_system;
//static rt_uint8_t rt_thread_system_stack[INIT_THREAD_STACK_SIZE]; 

#ifdef RT_USING_STATIC_THREAD

#if RT_USING_SLAVE101  
struct rt_event slave101_event; // 101从站事件
static struct rt_thread *rt_thread_slave101;
static rt_uint8_t *rt_thread_slave101_stack;
#endif /* END RT_USING_SLAVE101 */

#if RT_USING_SLAVE104 
struct rt_event slave104_event; // 104从站事件
static struct rt_thread *rt_thread_slave104;
static rt_uint8_t *rt_thread_slave104_stack;
#endif /* END RT_USING_SLAVE104 */

#if RT_USING_HMICOM
static struct rt_thread *rt_thread_hmicom;
static rt_uint8_t *rt_thread_hmicom_stack;
#endif /* END RT_USING_HMICOM */

#if RT_USING_WATCH
struct rt_event pcf8563_event; // pcf8563事件
struct rt_semaphore watch_sem; // watch semaphore
static struct rt_thread *rt_thread_watch;
static rt_uint8_t *rt_thread_watch_stack;
#endif /* END RT_USING_WATCH */

#if RT_USING_FTUIDLE 
static struct rt_thread *rt_thread_ftuidle;
static rt_uint8_t *rt_thread_ftuidle_stack;
#endif /* END RT_USING_FTUIDLE */

#if RT_USING_DP83848 
static struct rt_thread rt_thread_dp83848;
static rt_uint8_t rt_thread_dp83848_stack[DP83848_2404_THREAD_STACK_SIZE];
//static struct rt_thread *rt_thread_dp83848;
//static rt_uint8_t *rt_thread_dp83848_stack;
#endif /* END RT_USING_DP83848 */

#if RT_USING_W5500 
static struct rt_thread rt_thread_w5500_udp_rx;
static rt_uint8_t rt_thread_w5500_udp_rx_stack[W5500_UDP_RX_THREAD_STACK_SIZE];
static struct rt_thread rt_thread_w5500_udp_tx;
static rt_uint8_t rt_thread_w5500_udp_tx_stack[W5500_UDP_RX_THREAD_STACK_SIZE];
struct rt_semaphore w5500_sem; // w5500 semaphore
struct rt_event w5500_event; // w5500
struct rt_event w5500_irq_event; // w5500
//static struct rt_thread *rt_thread_w5500;
//static rt_uint8_t *rt_thread_w5500_stack;
/* 消息队列控制块 */
struct rt_messagequeue mq;
/* 消息队列中用到的放置消息的内存池 */
char msg_pool[20];

#endif /* END RT_USING_W5500 */

#if RT_USING_CAL
struct rt_event calculate_event; // 计算事件
static struct rt_thread *rt_thread_cal;
static rt_uint8_t *rt_thread_cal_stack;
#endif /* END RT_USING_CAL */

#if RT_USING_PROTECT    
struct rt_semaphore protect_sem; // protect semaphore
static struct rt_thread *rt_thread_protect;
static rt_uint8_t *rt_thread_protect_stack;
#endif /* END RT_USING_PROTECT */

#endif /* RT_USING_STATIC_THREAD */


/* PRIVATE FUNCTION PROTOTYPES -----------------------------------------------*/
///**
//  * @brief : The thread body of init_thread.
//  * @param : [parameter] The parameter.
//  * @return: none
//  * @update: [2017-12-07][Lexun][make code cleanup]
//  */  
//static void rt_init_thread_entry(void* parameter)
//{ 	
//    /* initialization RT-Thread Components */
//#ifdef RT_USING_COMPONENTS_INIT
//    rt_components_init();
//#endif
//	        
//    /* GDB STUB */
//#ifdef RT_USING_GDB
//    gdb_set_device("uart6");
//    gdb_start();
//#endif
//	
//#ifdef RT_USING_DFS  
//    #ifdef RT_USING_DFS_UFFS
//        /* mount nand flash partition 0 as root directory */
//        if (dfs_mount(RT_MTD_NAND_NAME, "/", "uffs", 0, 0) == 0)
//        {
//            mkdir("/sdcard",0);
//            THREAD_PRINTF("uffs initialized!\n");
//        }
//        else        
//        {
//            THREAD_PRINTF("uffs initialzation failed!\n");
//        }
//            
//    #endif /* RT_USING_DFS_UFFS */    
//    #ifdef RT_USING_DFS_ELMFAT
//        /* mount sd card fat partition 0 as root directory */
//        if (dfs_mount("sd0", "/sdcard", "elm", 0, 0) == 0)
//        if (dfs_mount(RT_SDIO_SD_NAME, "/sdcard", "elm", 0, 0) == 0)
//        {
//            THREAD_PRINTF("File System initialized!\n");        
//        }
//        else
//        {
//            THREAD_PRINTF("File System initialzation failed!\n");
//        }
//            
//        /* mount sd card fat partition 0 as root directory */
//        if (dfs_mount("W25Q256", "/spi", "elm", 0, 0) == 0)
//        {
//            THREAD_PRINTF("spi flash mount to /spi !\n");
//        }
//        else
//        {
//            THREAD_PRINTF("spi flash mount to /spi failed!\n");
//        }
//    #endif /* RT_USING_DFS_ELMFAT */
//#endif /* DFS */
//				
////    file_operate_Init(); // file system initialization

//#ifdef RT_USING_GUIENGINE
//	{
//		rt_device_t device;

//		device = rt_device_find(RT_LCD_NAME);
//		
//		/* re-set graphic device */
//		rtgui_graphic_set_device(device);
//        
//    rt_gui_demo_init();
//	}
//#endif /*  RT_USING_GUIENGINE */
//}

/**
  * @brief : Calculate thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
#if RT_USING_CAL
static void rt_cal_thread_entry(void *param)
{ 
    rt_err_t result;               
    
	for (;;)
    {
        /* 永久等待事件 */
        result = rt_event_recv(&calculate_event, EVENT_RUN, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);              
        if (result == RT_EOK)  
        {
			g_ThreadRunSta |= THREAD_RUN_CAL;
			
            CalcultateTask();
        }         
    }    
}
#endif /* RT_USING_CAL */

/**
  * @brief : Protect thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_PROTECT
static void rt_protect_thread_entry(void *param)
{
    rt_err_t result;
    
	distributInit();
	
    BreakerCtrlInit();
	
    LoadSwitchCtrlInit(); 
    
    rt_sem_control(&protect_sem, RT_IPC_CMD_RESET, 0);
    
    for (;;)
    {
        result = rt_sem_take(&protect_sem, RT_WAITING_FOREVER); 
        if (result == RT_EOK)  
        {
			g_ThreadRunSta |= THREAD_RUN_PROTECT;
			
//            if (g_Parameter[SWITCH_TYPE] == TYPE_BREAKER_COMMON) 
            {
                /* 断路器主保护逻辑 */ 					
                BreakerCtrlClock();	  					    
            }
//            else
            {
                /* 负荷开关主保护逻辑 */ 
                LoadSwitchCtrlClock();  
            }
            
            if(g_Parameter[DISTRIBUTE_SWITCH] == SWITCH_ON)
            {
                DBWriteSOE(g_TelesignalAddr.p2p_communication_switch,ON);
            }
            else
            {
                DBWriteSOE(g_TelesignalAddr.p2p_communication_switch,OFF);            
            }

            distributClock();			
        }
    }  
}
#endif /* RT_USING_PROTECT */


/**
  * @brief : Slave101 thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
#if RT_USING_SLAVE101
static void rt_slave101_thread_entry(void *param)
{ 
    rt_err_t result;
    
	DLT634_HMI_SLAVE_INIT();
    DLT634_5101_SLAVE_INIT();
       
    for (;;)
    {
        /* 永久等待事件 */
        result = rt_event_recv(&slave101_event, EVENT_RUN, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);              
        if (result == RT_EOK)  
        {
			g_ThreadRunSta |= THREAD_RUN_SLAVE101;
			
            DLT634_5101_SlaveTask();
			
			DLT634_HMI_SlaveTask();
        } 		
    }    
}
#endif /* RT_USING_SLAVE101 */

/**
  * @brief : Slave104 thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
#if RT_USING_SLAVE104
static void rt_slave104_thread_entry(void *param)
{ 
    rt_err_t result; 
    
    DLT634_5104_ParaInit();
    
    for (;;)
    {
        /* 永久等待事件 */
        result = rt_event_recv(&slave104_event, EVENT_RUN, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);              
        if (result == RT_EOK)  
        {
			g_ThreadRunSta |= THREAD_RUN_SLAVE104;
			
            DLT634_5104_SlaveTask();
        }     
    }    
}
#endif /* RT_USING_SLAVE104 */

/**
  * @brief : Hmicom thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_HMICOM
static void rt_hmicom_thread_entry(void *param)
{
	time_static_init();
	Hmi101Init();
	GUIDisplayInit();
	for (;;){ 				
		GUIDisplayMian();
//	g_ThreadRunSta |= THREAD_RUN_HMI;
		rt_thread_delay(20);		
	}    
}
#endif /* RT_USING_HMICOM */

/**
  * @brief : Watch thread entry
  * @param : [param] the parameter.
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_WATCH
static void rt_watch_thread_entry(void *param)
{  
    rt_err_t result;
    
    other_protect_init();
    
    rt_sem_control(&watch_sem, RT_IPC_CMD_RESET, 0);
    
    for (;;)
    {        
        /* 永久等待信号量 */
        result = rt_sem_take(&watch_sem, RT_WAITING_FOREVER);  
        if (result == RT_EOK)  
        {
			g_ThreadRunSta |= THREAD_RUN_WATCH;
			
            /* 其他保护 */
            other_protect_clock();
			
            /* 指示灯 */
            rt_hw_led_task();

            /* 变化遥测监测 */
            DB_NVA_Check(); 
			
            /* 获取频率 */
			GetFrequency();          
			
			/* 通道监听 */
			rt_channel_monitor_task();					
        }           
    }  
}
#endif /* RT_USING_WATCH */


/**
  * @brief : Ftuidle thread entry
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */  
#if RT_USING_FTUIDLE
static void rt_ftuidle_thread_entry(void *param)
{   
	static uint8_t s_run_state = 0;

    file_operate_Init();
    
    for (;;)
    {      
		g_ThreadRunSta |= THREAD_RUN_IDLE;
		
        file_operate();  

        if (s_run_state == 0)
        {   
            s_run_state = 1;			
            rt_multi_common_data_fram_update_state_write(RUN_NOMAL); // 运行正常，写FRAM
        }        
    }    
}
#endif /* RT_USING_FTUIDLE */

///**
//  * @brief : hook function
//  * @param : none
//  * @return: none
//  * @updata: [2017-12-07][Lexun][make the code cleanup]
//  */  
//#ifdef  RT_USING_HOOK
//static void rt_ftu_idle_hook(void)
//{   
//}
//#endif /* RT_USING_HOOK */

/**
  * @brief : Start calculate thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_CAL  
static void cal_thread_start(void *param)
{    
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
    result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_cal,
                            CAL_THREAD_NAME, 
                            rt_cal_thread_entry,
                            RT_NULL,
                            rt_thread_cal_stack,
                            CAL_THREAD_STACK_SIZE,
                            CAL_THREAD_PRIORITY,
                            CAL_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_cal);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("claculate thread start \r\n");  
									 
#else 
									 
    rt_thread_t tid; 
   
    tid = rt_thread_create(CAL_THREAD_NAME, 
						   rt_cal_thread_entry, 
						   RT_NULL, 
						   CAL_THREAD_STACK_SIZE, 
						   CAL_THREAD_PRIORITY, 
						   CAL_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("claculate thread start \r\n");
    }  
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* END RT_USING_CAL */  

/**
  * @brief : Start protect thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_PROTECT  
static void protect_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_protect,
                            PROTECT_THREAD_NAME, 
                            rt_protect_thread_entry,
                            RT_NULL,
                            rt_thread_protect_stack,
                            PROTECT_THREAD_STACK_SIZE,
                            PROTECT_THREAD_PRIORITY,
                            PROTECT_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_protect);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("protect thread start \r\n");  
									 
#else
									 
    rt_thread_t tid; 
  
    tid = rt_thread_create(PROTECT_THREAD_NAME, 
                           rt_protect_thread_entry, 
                           param, 
                           PROTECT_THREAD_STACK_SIZE,
                           PROTECT_THREAD_PRIORITY, 
                           PROTECT_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
         
        THREAD_PRINTF("protect thread start \r\n");  
    }   
#endif /* END RT_USING_STATIC_THREAD */									 
}
#endif /* RT_USING_PROTECT */  

/**
  * @brief : Start tcp server (DP83848) thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_DP83848 
static void dp83848_2404_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(&rt_thread_dp83848,
                            DP83848_2404_THREAD_NAME, 
                            rt_dp83848_tcpserver_thread_entry,
                            RT_NULL,
                            &rt_thread_dp83848_stack[0],
                            sizeof(rt_thread_dp83848_stack),
                            DP83848_2404_THREAD_PRIORITY,
                            DP83848_2404_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(&rt_thread_dp83848);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("tcp server(dp83848) thread start \r\n");

#else
														
    rt_thread_t tid; 

    tid = rt_thread_create(DP83848_2404_THREAD_NAME, 
                           rt_dp83848_tcpserver_thread_entry, 
                           param, 
                           DP83848_2404_THREAD_STACK_SIZE, 
                           DP83848_2404_THREAD_PRIORITY, 
                           DP83848_2404_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("tcp server(dp83848) thread start \r\n");
    }
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_DP83848_2404_THREAD */

/**
  * @brief : Start W5500 thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_W5500 
static void w5500_8080_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(&rt_thread_w5500_udp_rx,
                            W5500_UDP_RX_THREAD_NAME, 
                            rt_w5500_udp_rx_thread_entry,
                            RT_NULL,
                            &rt_thread_w5500_udp_rx_stack[0],
                            sizeof(rt_thread_w5500_udp_rx_stack),
                            W5500_UDP_RX_THREAD_PRIORITY,
                            W5500_UDP_RX_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(&rt_thread_w5500_udp_rx);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("tcp server(w5500) thread start \r\n");

//    /* initialize thread */
//    result = rt_thread_init(&rt_thread_w5500_udp_tx,
//                            W5500_UDP_TX_THREAD_NAME, 
//                            rt_w5500_udp_tx_thread_entry,
//                            RT_NULL,
//                            &rt_thread_w5500_udp_tx_stack[0],
//                            sizeof(rt_thread_w5500_udp_tx_stack),
//                            W5500_UDP_TX_THREAD_PRIORITY,
//                            W5500_UDP_TX_THREAD_TIMESLICE);

//    /* startup */
//    RT_ASSERT(result == RT_EOK);

//    result = rt_thread_startup(&rt_thread_w5500_udp_tx);
//														
//    RT_ASSERT(result == RT_EOK);
//    THREAD_PRINTF("udp rx(w5500) thread start \r\n");								
#else
    rt_thread_t tid; 

    tid = rt_thread_create(W5500_8080_THREAD_NAME, 
                           rt_w5500_tcpserver_thread_entry, 
                           param, 
                           W5500_8080_THREAD_STACK_SIZE,
                           W5500_8080_THREAD_PRIORITY, 
                           W5500_8080_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("tcp server(w5500) thread start \r\n");
    }
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_W5500 */

/**
  * @brief : Start slave101 thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_SLAVE101 
static void slave101_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_slave101,
                            SLAVE101_THREAD_NAME, 
                            rt_slave101_thread_entry,
                            RT_NULL,
                            rt_thread_slave101_stack,
                            SLAVE101_THREAD_STACK_SIZE,
                            SLAVE101_THREAD_PRIORITY,
                            SLAVE101_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_slave101);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("slave101 thread start \r\n");
									 
#else 
									 
    rt_thread_t tid; 

    tid = rt_thread_create(SLAVE101_THREAD_NAME, 
                           rt_slave101_thread_entry, 
                           param, 
                           SLAVE101_THREAD_STACK_SIZE, 
                           SLAVE101_THREAD_PRIORITY, 
                           SLAVE101_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("slave101 thread start \r\n");
    }   
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_SLAVE101 */

/**
  * @brief : Start slave104 thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_SLAVE104 
static void slave104_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_slave104,
                            SLAVE104_THREAD_NAME, 
                            rt_slave104_thread_entry,
                            RT_NULL,
                            rt_thread_slave104_stack,
                            SLAVE104_THREAD_STACK_SIZE,
                            SLAVE104_THREAD_PRIORITY,
                            SLAVE104_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_slave104);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("slave104 thread start \r\n");

#else 
									 
    rt_thread_t tid; 

    tid = rt_thread_create(SLAVE104_THREAD_NAME, 
                           rt_slave104_thread_entry, 
                           param, 
                           SLAVE104_THREAD_STACK_SIZE, 
                           SLAVE104_THREAD_PRIORITY, 
                           SLAVE104_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("slave104 thread start \r\n");
    } 
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_SLAVE104 */

/**
  * @brief : Start watch thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_WATCH 
static void watch_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_watch,
                            WATCH_THREAD_NAME, 
                            rt_watch_thread_entry,
                            RT_NULL,
                            rt_thread_watch_stack,
                            WATCH_THREAD_STACK_SIZE,
                            WATCH_THREAD_PRIORITY,
                            WATCH_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_watch);
														
    RT_ASSERT(result == RT_EOK);
		THREAD_PRINTF("watch thread start \r\n"); 

#else
    rt_thread_t tid; 

    tid = rt_thread_create(WATCH_THREAD_NAME, 
                           rt_watch_thread_entry, 
                           param, 
                           WATCH_THREAD_STACK_SIZE, 
                           WATCH_THREAD_PRIORITY, 
                           WATCH_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("watch thread start \r\n"); 
    }  
#endif /* END RT_USING_STATIC_THREAD */									 
}
#endif /* RT_USING_WATCH */

/**
  * @brief : Start hmicom thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_HMICOM 
static void hmicom_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
    /* initialize thread */
    result = rt_thread_init(rt_thread_hmicom,
                            HMICOM_THREAD_NAME, 
                            rt_hmicom_thread_entry,
                            RT_NULL,
                            rt_thread_hmicom_stack,
                            HMICOM_THREAD_STACK_SIZE,
                            HMICOM_THREAD_PRIORITY,
                            HMICOM_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_hmicom);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("hmicom thread start \r\n");
														
#else 
														
    rt_thread_t tid; 

    tid = rt_thread_create(HMICOM_THREAD_NAME, 
                           rt_hmicom_thread_entry,
                           param, 
                           HMICOM_THREAD_STACK_SIZE,
                           HMICOM_THREAD_PRIORITY,
                           HMICOM_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("hmicom thread start \r\n");
    }  
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_HMICOM */

/**
  * @brief : Start ftuidle thread
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
#if RT_USING_FTUIDLE 
static void ftuidle_thread_start(void *param)
{
#ifdef RT_USING_STATIC_THREAD
    rt_err_t result = RT_EOK;
	result = result;
	
    /* initialize thread */
    result = rt_thread_init(rt_thread_ftuidle,
                            FTUIDLE_THREAD_NAME, 
                            rt_ftuidle_thread_entry,
                            RT_NULL,
                            rt_thread_ftuidle_stack,
                            FTUIDLE_THREAD_STACK_SIZE,
                            FTUIDLE_THREAD_PRIORITY,
                            FTUIDLE_THREAD_TIMESLICE);

    /* startup */
    RT_ASSERT(result == RT_EOK);

    result = rt_thread_startup(rt_thread_ftuidle);
														
    RT_ASSERT(result == RT_EOK);
    THREAD_PRINTF("ftuidle thread start \r\n");
														
#else 
    rt_thread_t tid; 

    tid = rt_thread_create(FTUIDLE_THREAD_NAME, 
                           rt_ftuidle_thread_entry, 
                           param, 
                           FTUIDLE_THREAD_STACK_SIZE, 
                           FTUIDLE_THREAD_PRIORITY, 
                           FTUIDLE_THREAD_TIMESLICE);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
        THREAD_PRINTF("ftuidle thread start \r\n"); 
    }   
#endif /* END RT_USING_STATIC_THREAD */
}
#endif /* RT_USING_FTUIDLE */


/* PUBLIC FUNCTION PROTOTYPES ------------------------------------------------*/
///**
//  * @brief : System initialization thread
//  * @param : [param] the parameter of thread.
//  * @return: none
//  * @updata: [2017-12-07][Lexun][make the code cleanup]
//  */
//void rt_system_thread_start(void)
//{
//#ifdef RT_USING_STATIC_THREAD
//    rt_err_t result = RT_EOK;
//	
//    /* initialize thread */
//    result = rt_thread_init(&rt_thread_system,
//                            INIT_THREAD_NAME, 
//                            rt_init_thread_entry,
//                            RT_NULL,
//                            &rt_thread_system_stack[0],
//                            sizeof(rt_thread_system_stack),
//                            INIT_THREAD_PRIORITY,
//                            INIT_THREAD_TIMESLICE);

//    /* startup */
//    RT_ASSERT(result == RT_EOK);

//    result = rt_thread_startup(&rt_thread_system);
//														
//    RT_ASSERT(result == RT_EOK);
//    THREAD_PRINTF("system_init thread start \r\n");
//									 
//#else 
//    rt_thread_t tid; 

//    tid = rt_thread_create(INIT_THREAD_NAME, 
//                           rt_init_thread_entry, 
//                           RT_NULL, 
//                           INIT_THREAD_STACK_SIZE, 
//                           INIT_THREAD_PRIORITY, 
//                           INIT_THREAD_TIMESLICE);

//    if (tid != RT_NULL)
//    {
//        rt_thread_startup(tid);
//        
//        THREAD_PRINTF("system_init thread start \r\n");
//    }   
//#endif /* END RT_USING_STATIC_THREAD */
//}

/**
  * @brief : system all thread creation
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][make the code cleanup]
  */
int rt_multi_thread_start(void)
{	
  #if RT_USING_CAL 	
    rt_thread_cal = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_cal == NULL)
	{
	    THREAD_PRINTF("rt_thread_cal malloc failed"); 
	}
	
	rt_thread_cal_stack = (rt_uint8_t *)rt_malloc(CAL_THREAD_STACK_SIZE);
	
	if (rt_thread_cal_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_cal_stack malloc failed");
	}	
    cal_thread_start(RT_NULL); 
  #endif /* RT_USING_CAL */
  
  #if RT_USING_PROTECT  
    rt_thread_protect = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_protect == NULL)
	{
	    THREAD_PRINTF("rt_thread_protect malloc failed"); 
	}
	
	rt_thread_protect_stack = (rt_uint8_t *)rt_malloc(PROTECT_THREAD_STACK_SIZE);
	
	if (rt_thread_protect_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_protect_stack malloc failed");
	}	
    protect_thread_start(RT_NULL); 
  #endif /* RT_USING_PROTECT */
    
  #if RT_USING_WATCH 
    rt_thread_watch = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_watch == NULL)
	{
	    THREAD_PRINTF("thread_watch malloc failed"); 
	}
	
	rt_thread_watch_stack = (rt_uint8_t *)rt_malloc(WATCH_THREAD_STACK_SIZE);
	
	if (rt_thread_watch_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_watch_stack malloc failed");
	}
	
    watch_thread_start(RT_NULL);     
  #endif /* RT_USING_WATCH */  
     
  #if RT_USING_DP83848   
//    rt_thread_dp83848 = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
//	
//	if (rt_thread_dp83848 == NULL)
//	{
//	    THREAD_PRINTF("rt_thread_w5500 malloc failed"); 
//	}
//	
//	rt_thread_dp83848_stack = rt_malloc(DP83848_2404_THREAD_STACK_SIZE);
//	
//	if (rt_thread_dp83848_stack == NULL)
//	{
//	    THREAD_PRINTF("rt_thread_w5500_stack malloc failed");
//	}	
    dp83848_2404_thread_start(RT_NULL); 
  #endif /* RT_USING_DP83848 */

  #if RT_USING_W5500  
//    rt_thread_w5500 = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
//	
//	if (rt_thread_w5500 == NULL)
//	{
//	    THREAD_PRINTF("rt_thread_w5500 malloc failed"); 
//	}
//	
//	rt_thread_w5500_stack = rt_malloc(W5500_2404_THREAD_STACK_SIZE);
//	
//	if (rt_thread_w5500_stack == NULL)
//	{
//	    THREAD_PRINTF("rt_thread_w5500_stack malloc failed");
//	}	
//    w5500_8080_thread_start(RT_NULL); 
  #endif /* RT_USING_W5500 */
  
  #if RT_USING_SLAVE101 
    rt_thread_slave101 = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_slave101 == NULL)
	{
	    THREAD_PRINTF("rt_thread_slave101 malloc failed"); 
	}
	
	rt_thread_slave101_stack = rt_malloc(SLAVE101_THREAD_STACK_SIZE);
	
	if (rt_thread_slave101_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_slave101_stack malloc failed");
	}	
    slave101_thread_start(RT_NULL); 
  #endif /* RT_USING_SLAVE101 */

  #if RT_USING_SLAVE104  
    rt_thread_slave104 = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_slave104 == NULL)
	{
	    THREAD_PRINTF("rt_thread_slave104 malloc failed"); 
	}
	
	rt_thread_slave104_stack = rt_malloc(SLAVE104_THREAD_STACK_SIZE);
	
	if (rt_thread_slave104_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_slave104_stack malloc failed");
	}	
    slave104_thread_start(RT_NULL); 
  #endif /* RT_USING_SLAVE104 */ 

  #if RT_USING_HMICOM 
    rt_thread_hmicom = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_hmicom == NULL)
	{
	    THREAD_PRINTF("rt_thread_hmicom malloc failed"); 
	}
	
	rt_thread_hmicom_stack = rt_malloc(HMICOM_THREAD_STACK_SIZE);
	
	if (rt_thread_hmicom_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_hmicom_stack malloc failed");
	}	
    hmicom_thread_start(RT_NULL);     
  #endif /* RT_USING_HMICOM */ 	
	
  #if RT_USING_FTUIDLE 
    rt_thread_ftuidle = rt_malloc(RT_THREAD_PER_STRUCT_SIZE);
	
	if (rt_thread_ftuidle == NULL)
	{
	    THREAD_PRINTF("rt_thread_hmicom malloc failed"); 
	}
	
	rt_thread_ftuidle_stack = rt_malloc(FTUIDLE_THREAD_STACK_SIZE);
	
	if (rt_thread_ftuidle_stack == NULL)
	{
	    THREAD_PRINTF("rt_thread_ftuidle_stack malloc failed");
	}	
    ftuidle_thread_start(RT_NULL);     
  #endif /* RT_USING_FTUIDLE */
    return(RT_EOK);    
}
INIT_APP_EXPORT(rt_multi_thread_start);


/**
  * @brief : Event initialization.
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][Make the code cleanup]
  */
int rt_multi_event_init(void)
{
    rt_err_t result;
    
  #if RT_USING_CAL 	
    result = rt_event_init(&calculate_event, "calculate", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("calculate_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    }  
  #endif /* RT_USING_CAL */   

  #if RT_USING_SLAVE101 	
    result = rt_event_init(&slave101_event, "IEC101_slave", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("slave101_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    } 
  #endif /* RT_USING_SLAVE101 */     

  #if RT_USING_SLAVE104     
    result = rt_event_init(&slave104_event, "IEC104_slave", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("slave104_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    }   
  #endif /* RT_USING_SLAVE104 */
    
  #if RT_USING_WATCH 	
    result = rt_event_init(&pcf8563_event, "pcf8563", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("sd2405_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    } 
  #endif /* RT_USING_WATCH */ 

  #if RT_USING_W5500 	
    result = rt_event_init(&w5500_event, "w5500", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("w5500_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    } 	

    result = rt_event_init(&w5500_irq_event, "w5500", RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
      #if APP_DEBUG   
        THREAD_PRINTF("w5500_irq_event rt_event failed.\n");
      #endif /* APP_DEBUG */  
    } 	
  #endif /* RT_USING_W5500 */ 
	
    return(RT_EOK);    
}
INIT_BOARD_EXPORT(rt_multi_event_init);

/**
  * @brief : Semaphore initialization
  * @param : none
  * @return: none
  * @updata: [2017-12-07][Lexun][Make the code cleanup]
  */
int rt_multi_sem_init(void)
{
    rt_err_t result;

  #if RT_USING_PROTECT
    result = rt_sem_init(&protect_sem, "protect", 0, RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
        THREAD_PRINTF("protect_sem semaphore failed.\n");
    }
  #endif /* RT_USING_PROTECT */     

  #if RT_USING_W5500
    result = rt_sem_init(&w5500_sem, "w5500", 0, RT_IPC_FLAG_PRIO);
    
    if (result != RT_EOK)
    {
        THREAD_PRINTF("w5500_sem semaphore failed.\n");
    }
    else
	{
	    rt_sem_release(&w5500_sem);
	}		
  #endif /* RT_USING_W5500 */    

  #if RT_USING_WATCH    
    result = rt_sem_init(&watch_sem, "watch", 0, RT_IPC_FLAG_FIFO);    
    
    if (result != RT_EOK)
    {
        THREAD_PRINTF("watch_sem semaphre failed.\n");
    } 
  #endif /* RT_USING_WATCH */  
			   
    return(RT_EOK); 
}
INIT_BOARD_EXPORT(rt_multi_sem_init);


/* END OF FILE ---------------------------------------------------------------*/

