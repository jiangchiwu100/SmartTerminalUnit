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
#include "distribution.h"

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
	extern StationManger g_StationManger;
	while (!g_StationManger.pWorkPoint)
	{
		rt_kprintf("g_StationManger.pWorkPoint is NULL \r\n");
		rt_thread_delay(5000);
	}

	SwitchProperty* pswitch = g_StationManger.pWorkPoint->topology.localSwitch;

	for(;;)
	{
		rt_kprintf(" Ia  =   %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.Ia]));
		rt_kprintf(" Uab  =  %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.Uab]));//A
		rt_kprintf(" Ucb  =  %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.Ucb]));
		rt_kprintf(" Uac  =  %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.Uac]));
		rt_kprintf(" UAB  =  %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.UAB]));
		rt_kprintf(" UCB  =  %d \r\n", (uint32_t)(1000*g_TelemetryDB[g_TelemetryAddr.UCB]));

        if (pswitch->state == SWITCH_OPEN)
        {
             rt_kprintf("pos : %s\r\n", "open");
        }
        else if (pswitch->state == SWITCH_CLOSE)
        {
            rt_kprintf("pos : %s\r\n", "close");
        }
        else
        {
            rt_kprintf("pos : Unknow\r\n");
        }
        
       
		rt_thread_delay(3000);
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

