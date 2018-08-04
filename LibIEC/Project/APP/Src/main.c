#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "rtthread.h"
#include "w5500_demo.h"
#include "lwip_comm.h" 
//重映射串口1到rt_kprintf
void rt_hw_console_output(const char *str)
{
    /* empty console output */
	
	//rt_enter_critical();

	while(*str!='\0')
	{
		if(*str=='\n')
		{
			USART6->DR = (u8)  '\r'; 
			while((USART6->SR&0X40)==0);
		}
		USART6->DR =*str++;
		while((USART6->SR&0X40)==0);	
	}

//	rt_exit_critical();
}

static struct rt_thread blink_thread;//线程控制块
static struct rt_thread udp_thread;

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_blink_thread_stack[1024];//线程栈
static rt_uint8_t rt_udp_thread_stack[1024];//线程栈
  
//线程LED0
static void blink_thread_entry(void* parameter)
{

	rt_kprintf("thread blink start.\r\n"); /* 打印线程计数值输出 */
    rt_thread_delay(1000);
    subscriber_main();
    while (1)
    {       
        rt_thread_delay(100);   //延时500个tick
        
       // INDICATE_LED_TOGGLE();
				 		
		
    }
}
  
static void udp_thread_entry(void* parameter)
{
    rt_kprintf("thread udp start.\r\n"); /* 打印线程计数值输出 */
    
     UDPTest();
//    uint8_t ret = NetHardInit();
//    if (ret != 0)
//    {
//        rt_kprintf("Net Dp38848 Init Faulure\n");
//    }
//    else
//    {
//        rt_kprintf("Net Dp38848 Init Sucess\n");        
//    }
//    while(1)
//    {
//        rt_thread_delay(1000);
//    }
    
    
}
 
int my_main(void)
{
    // 创建静态线程
    rt_thread_init(&blink_thread,                 //线程控制块
                   "blink",                       //线程名字，在shell里面可以看到
                   blink_thread_entry,            //线程入口函数
                   RT_NULL,                      //线程入口函数参数
                   &rt_blink_thread_stack[0],     //线程栈起始地址
                   sizeof(rt_blink_thread_stack), //线程栈大小
                   7,                            //线程的优先级
                   20);                          //线程时间片
                               
    rt_thread_startup(&blink_thread);             //启动线程led0_thread，开启调度
									 
  
    rt_thread_init(&udp_thread,                 //线程控制块
                   "udp",                       //线程名字，在shell里面可以看到
                   udp_thread_entry,            //线程入口函数
                   RT_NULL,                      //线程入口函数参数
                   &rt_udp_thread_stack[0],     //线程栈起始地址
                   sizeof(rt_udp_thread_stack), //线程栈大小
                   3,                            //线程的优先级
                   20);                          //线程时间片
                               
    rt_thread_startup(&udp_thread);             //启动线程led0_thread，开启调度 
     return  0;                 
}




 

void rt_init_thread_entry(void* parameter)
{    
     /* initialization RT-Thread Components */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_init();
#endif
    my_main();

}
int rt_application_init()
{
    rt_thread_t tid;


    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
    


    return 0;
}



