/* ---------------------------------------------------------------------------
    Copyright 2014-2015 Victor Alessandrini.  All Rights Reserved.

    This file is part of the software support provided wih the book
    "Shared Mamory Application Programming".

    This code is free software; you can redistribute it and/or modify it under 
    the terms of the GNU General Public License version 2 as published by 
    the Free Software Foundation.

    This software is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
    for more details.

    You should have received a copy of the GNU General Public License
    along with Threading Building Blocks; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    As a special exception, you may use this file as part of a free software
    library without restriction.  Specifically, if other files instantiate
    templates or use macros or inline functions from this file, or you compile
    this file and link it with other files to produce an executable, this
    file does not by itself cause the resulting executable to be covered by
    the GNU General Public License.  This exception does not however
    invalidate any other reasons why the executable file might be covered by
    the GNU General Public License.
 --------------------------------------------------------------------------*/
// ThreadMgr.h 
// NPool package
// ------------------

#ifndef THREAD_MGR_H
#define THREAD_MGR_H

#include <Task.h>

// *************************************************************
// Thread managment class. Internal class. Member functions 
// are called by worker threads. 
//
// This is an auxiliary class that implements two services: 
// -) It launches and terminates the worker threads in the 
//    pool 
// -) It manages the mapping of tasks to threads. At any
//    moment one can know which task is being executed by
//    a given thread.
// 
// Tasks are ties to threads. Once a task is picked for
// execution by a thread, it remains tied to the owner thread.
// All that can happen is that the task execution is temporarily
// suspended.
//
// This auxiliary class plays an important role when threads
// suspend a task just before a "taskwait" and execute another
// task before returning to the suspended task.
// *************************************************************
class ThreadManager
   {
   private:
   //-----
    pthread_mutex_t  mgr_mutex;
    pthread_attr_t   attr;        // attribute of threads
    int              nThreads;    // number of workers in pool
    pthread_t        *threads;    // identifiers of threads in pool
    Task*            *curr_task;  // array that maps tasks to threads

    public:
    //----
     ThreadManager(int Nth, void *P);
     ~ThreadManager();
     int   GetRank();         
     Task *GetCurrTask();
     void SetCurrTask(Task *t); 
    };

#endif
