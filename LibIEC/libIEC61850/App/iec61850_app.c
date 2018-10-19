/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      iec61850_app.c
  * @brief:     61850应用
  * @version:   V0.0.0 
  * @author:    Zhang Yufei
  * @date:      2018-08-13
  * @update:    
  */

#include "extern_interface.h"
#include "server_model.h"

#include "distribution_app.h"
#include "file_operate.h"
#include "distribution_config.h"
#include "rthw.h"

static rt_thread_t iec61850_thread;//61850线程
static struct exception_stack_frame *exception_stack;

rt_err_t exception_handle(void *context)
{
    exception_stack = (struct exception_stack_frame *)context;
    while(true);
    return RT_EOK;
}


static void iec61850_thread_entry(void* parameter)
{    
//	rt_kprintf("(thread iec61850App start)");
    
    rt_hw_exception_install(exception_handle);
    
	rt_thread_delay(1000);
	file_operate_Init();
	rt_thread_delay(1000);
	DistributionAppInit();
	WaitEnterCodition();
	Iec61850Server();
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
		THREAD_61850_NAME,                       
        iec61850_thread_entry,           
        RT_NULL,                      
        THREAD_61850_STACK_SIZE,     		
		THREAD_61850_PRIORITY,                            
		THREAD_61850_TIMESLICE);                          
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

