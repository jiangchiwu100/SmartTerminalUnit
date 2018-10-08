/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2017-2018. All right reserved.
  * @file:      quick_break_protest.h
  * @brief:     The protect control logic.
  * @version:   V1.0.0
  * @author:    tianxiaoliang
  * @date:      2018.09.17
  * @update:    [YYYY-MM-DD][更改人姓名][变更描述]
  */
  
#include "PrintfCurrent.h"
#include "stm32f429xx.h"
#include "calculator.h"
#include "common_data.h"
#include "extern_interface.h"
#include "input.h"


static struct rt_thread rt_thread_print_cur_val;
static rt_uint8_t *rt_thread_print_cur_val_stack;

/**
  * @brief : 打印电流值任务回调函数
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][田晓亮][创建]
  */  
static void rt_print_current_val_thread_entry(void *param)
{
	for(;;)
	{
		rt_kprintf(" Ia  =  %d \r\n", (int32_t)g_TelemetryDB[g_TelemetryAddr.Ia]);
  
		OpeningClosing();
        rt_kprintf("---------------%d\r\n", curStation.state);
		rt_thread_delay(2000);
	}
}

/**
  * @brief : 打印电流值任务
  * @param : [none].
  * @return: [none] 
  * @updata: [2018-09-17][田晓亮][创建]
  */  
void printfCurrent(void)
{
	rt_thread_t tid;
    /* initialize thread */
    tid = rt_thread_create(PRINT_CUR_VAL_THREAD_NAME, 
                            rt_print_current_val_thread_entry,
                            RT_NULL,
                            PRINT_CUR_VAL_THREAD_STACK_SIZE,
                            PRINT_CUR_VAL_THREAD_PRIORITY,
                            PRINT_CUR_VAL_THREAD_TIMESLICE);

    /* startup */
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        
    } 
}

