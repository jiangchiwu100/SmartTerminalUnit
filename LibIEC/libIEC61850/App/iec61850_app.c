/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      iec61850_app.c
  * @brief:     61850娴犺濮?
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-13
  * @update:    
  */

#include "extern_interface.h"
#include "server_model.h"




static struct rt_thread iec61850_thread;//缁捐法鈻奸幒褍鍩楅崸锟?



ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_iec61850_thread_stack[2048];//




static void iec61850App(void);


static void iec61850_thread_entry(void* parameter)
{    
	rt_kprintf("thread iec61850App start.\r\n");
    rt_thread_delay(1000);
//    subscriber_example();
	Iec61850Server();
    //TestGooseBeat();
}
  




/**
  * @brief :iec61850App
  * @param  Svoid
  * @return: 0--
  * @update: [2018-08-13][创建]
  */
static void iec61850App(void)
{    

        
    
}





/**
  * @brief :IEC61850AppInit
  * @param  void
  * @return: void
  * @update: [2018-08-13][创建]
  */
void IEC61850AppInit(void)
{

    rt_thread_init(&iec61850_thread,                 
		"61850",                       
                   iec61850_thread_entry,           
                   RT_NULL,                      
                   &rt_iec61850_thread_stack,     
		sizeof(rt_iec61850_thread_stack), 
		14,                            
		20);                          
                               
    rt_thread_startup(&iec61850_thread);  
    


    return;

}

