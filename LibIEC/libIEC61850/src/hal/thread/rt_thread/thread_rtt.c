﻿/*
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






Thread
Thread_create(ThreadExecutionFunction function, void* parameter, bool autodestroy)
{

	Thread thread;

	return thread;
}

void
Thread_start(Thread thread)
{
	
}

void
Thread_destroy(Thread thread)
{
	
}

void
Thread_sleep(int millies)
{
	
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
