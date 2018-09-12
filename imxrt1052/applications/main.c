/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-10-10     Tanek        first version
 */

#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_DFS
#include <dfs_file.h>
#endif

#ifdef RT_USING_DEVICE
#include <rtdevice.h>
#endif

#include <board.h>
#define LED_PIN 46
void rt_hw_led_init(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
}


static void IEC61850AppInit(void);

int main(void)
{
    rt_uint32_t result;

    rt_kprintf("build time: %s %s\n", __DATE__, __TIME__);

#if defined(RT_USING_DFS) && defined(RT_USING_SDIO)
    result = mmcsd_wait_cd_changed(RT_TICK_PER_SECOND);
    if (result == MMCSD_HOST_PLUGED)
    {
        /* mount sd card fat partition 1 as root directory */
        if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
            rt_kprintf("File System initialized!\n");
        else
            rt_kprintf("File System init failed!\n");
    }
    else
    {
        rt_kprintf("sdcard init fail or timeout: %d!\n", result);
    }
#endif
    IEC61850AppInit();
    rt_hw_led_init();
    while (1)
    {
        rt_pin_write(LED_PIN, 0);
        rt_thread_delay(500);//RT_TICK_PER_SECOND
        rt_pin_write(LED_PIN, 1);
        rt_thread_delay(500);
    }
}




static rt_thread_t test_thread;


/**
  * @brief :IEC61850AppInit
  * @param  void
  * @return: void
  * @update: [2018-08-13][??]
  */
static void IEC61850AppInit(void)
{
    extern void publisher_main(void* paramter);
  
    test_thread = rt_thread_create(              
		"test",                       
        publisher_main,           
        RT_NULL,                      
        1024*4,     		
		14,                            
		20);                          
     if (test_thread)
     {
         rt_thread_startup(test_thread);  
     }         
    else
    {
        perror(" test_thread(publisher_main) failure\n");
    }
    
    return;

}



/*@}*/
