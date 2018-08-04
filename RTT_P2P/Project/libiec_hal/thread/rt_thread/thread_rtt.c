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


#include "rtthread.h"

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
    
	Thread thread =(Thread) GLOBAL_MALLOC(sizeof(struct sThread));
    thread->parameter = parameter;
	thread->parameter = parameter;
	thread->function = function;
	thread->state = 0;
	thread->autodestroy = autodestroy;

	thread->handle = rt_thread_create("googse",
								function,
								parameter,
	                            2048,
	                            2,
	                            20);

	if (thread->handle == NULL)
	{
		GLOBAL_FREEMEM(thread);
		printf("Creat faulure!");
	}
	else
	{
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

Semaphore
Semaphore_create(int initialValue)
{
    

    return NULL;
}

/* Wait until semaphore value is greater than zero. Then decrease the semaphore value. */
void
Semaphore_wait(Semaphore self)
{
   
}

void
Semaphore_post(Semaphore self)
{
   
}

void
Semaphore_destroy(Semaphore self)
{
    
}
