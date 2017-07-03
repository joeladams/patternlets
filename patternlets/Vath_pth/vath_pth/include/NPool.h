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

// NPool.h 
// -------
// Master class for the NPool thread pool utility
// *********************************************

#ifndef NPOOL_H
#define NPOOL_H

#include <list>
#include <map>
#include <ThDeque.h>
#include <BLock.h>
#include <Task.h>
#include <ThreadMgr.h>
#include <JobMgr.h>
#include <SafeCounter.h>


// *************
// Class NPool
// *************
class NPool
   {
   private:

   // pool characteristics 
   // --------------------
   int      nThreads;          // number of threads in the pool
   int      nWorkers;          // number of active threads at a given time
   int      maxQsize;          // maximum request queue size
   int      last_key;          // tracks keys for the map container

   ThDeque<TaskGroup*>         *QJ;  // reference to internal job queue
   ThDeque<Task*>               *Q;  // reference to internal task queue
   std::map<int,JobMgr*>         M;  // management of task groups
   ThreadManager               *TM;  // managment of thread-task mapping
   JobCounter                   JC;  // counts running jobs  

   // For debugging purposes
   // ----------------------
   SafeCounter    SC; 

   // pool synchronization 
   // --------------------
   pthread_mutex_t  qLock;     // to ensure mutual exclusion on class data
   //sem_t  Sem;                 // semaphore, used in waiting for idle.
   
   void  ClosePool();
   int   Suspend_And_Run_Task();
   bool  FlushJobQueue();
   void  FlushTaskGroup(TaskGroup *t);
   void  TPool_Thread();       // internal thread function
    
   public:

   NPool(int nThreads, int nMax=100);	
   ~NPool();

   int   SubmitJob(TaskGroup *tg);
   int   SubmitJob(Task *t);                     // individual tasks
   bool  JobStatus(int id); 
   void  WaitForJob(int gid);              

   int   SpawnTask(Task *t, bool iswaited=true); // synchronized task
   void  TaskWait();                        // wait for all spawned tasks
   void  WaitForIdle();                     // wait for idle pool

   friend void *ThreadFunction(void *arg);
   };

#endif
