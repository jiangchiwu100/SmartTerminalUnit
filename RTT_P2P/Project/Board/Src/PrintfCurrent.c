#include "PrintfCurrent.h"
#include "stm32f429xx.h"
#include "calculator.h"
#include "common_data.h"
#include "extern_interface.h"
#include "input.h"


static struct rt_thread rt_thread_print_cur_val;
static rt_uint8_t *rt_thread_print_cur_val_stack;


static void rt_print_current_val_thread_entry(void *param)
{
	for(;;)
	{
		rt_kprintf(" Ia  =  %d \r\n", (int32_t)g_TelemetryDB[g_TelemetryAddr.Ia]);
  
        rt_kprintf("---------------%d\r\n", curStation.state);
		rt_thread_delay(2000);
	}
}
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

