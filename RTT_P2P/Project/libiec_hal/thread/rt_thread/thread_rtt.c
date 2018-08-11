/*
 *  thread_win32.c
 *
 *  Copyright 2013, 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */


#include "libiec61850_platform_includes.h"
#include "hal_thread.h"


#include "extern_interface.h"

struct sThread {
	ThreadExecutionFunction function;
	void* parameter;
	rt_thread_t handle;
	int state;
	bool autodestroy;
};

Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{
    static int count = 0;
	Thread thread =(Thread) GLOBAL_MALLOC(sizeof(struct sThread));
    thread->parameter = parameter;
	thread->parameter = parameter;
	thread->function = function;
	thread->state = 0;
	thread->autodestroy = autodestroy;

	thread->handle = rt_thread_create("googse",
								(void (*)(void *))function,
								parameter,
	                            2048,
	                            3,
	                            20);

	if (thread->handle == NULL)
	{
		GLOBAL_FREEMEM(thread);
		printf("Creat faulure!");
	}
	else
	{
        rt_kprintf("Thread_create  Taske NUM:%d\r\n", ++count);
		return thread;
	}

    

	return thread;
}

void
Thread_start(Thread thread)
{
	rt_thread_startup(thread->handle);
}

void
Thread_destroy(Thread thread)
{
	rt_thread_delete(thread->handle);
}

void
Thread_sleep(int millies)
{
	rt_thread_delay(millies);
}
static int mux_count = 0;
Semaphore
Semaphore_create(int initialValue)
{
    
    static char name[5] = "mux0";
    if (mux_count++ > 30)
    {
        perror("mux_count > 30) \n");
        return NULL;
    }
    rt_kprintf("Semaphore_create Count: %d\n", mux_count);
    name[3] ++;//支持10个名称
    
    
    rt_mutex_t sem = rt_mutex_create(name,  RT_IPC_FLAG_PRIO);
    return (void*)sem;
}

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
    rt_mutex_t sem = (rt_mutex_t)self;
    rt_err_t err =  rt_mutex_take (sem, RT_WAITING_FOREVER);
    if (err != RT_EOK)
    {
        perror("rt_sem_take(sem, RT_WAITING_FOREVER)\n");
    }
}

void
Semaphore_post(Semaphore self)
{
    rt_mutex_t sem = (rt_mutex_t)self;
    rt_err_t err = rt_mutex_release(sem);
    if (err != RT_EOK)
    {
        perror("rt_sem_release(sem)\n");
    }
}

void
Semaphore_destroy(Semaphore self)
{
    rt_mutex_t sem = (rt_mutex_t)self;
    rt_mutex_delete (sem);
    mux_count--;
}
