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

#include "distribution_app.h"


static rt_thread_t iec61850_thread;//缁捐法鈻奸幒褍鍩楅崸锟?








static void iec61850App(void);


static void iec61850_thread_entry(void* parameter)
{    
	rt_kprintf("thread iec61850App start.\r\n");
    rt_thread_delay(1000);
    DistributionAppInit();
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

    iec61850_thread = NULL;
    iec61850_thread = rt_thread_create(              
		"61850",                       
        iec61850_thread_entry,           
        RT_NULL,                      
        1024*10,     		
		14,                            
		20);                          
     if (iec61850_thread)
     {
         rt_thread_startup(iec61850_thread);  
     }         
    else
    {
        perror(" rt_thread_startup(iec61850_thread) failure\n");
    }
    
    return;

}

