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

// JobMgr.h
// --------
// This is an auxiliary class, part of the NPool 
// utility. It corresponds o the job manager that
// is created for each submitted job, monitoring 
// the job execution and synchronizing with the
// client threads
// **************************************

#ifndef JOB_MGR_H
#define JOB_MGR_H

#include <BLock.h>
#include <Task.h>

// *********************************************
// Class JobCounter
// ----------------
// The purpose of this class is to keep track of
// the number of jobs in execution in the pool, and 
// to signal to external client threads that the 
// pool is idle when there are no more ongoing
// jobs. This class implements the WaitforIdle
// utility.
//
// Internal Boolean lock state: 
//  - true if pool is inactive
//  - false if there are ongoing jobs
// *********************************************
class JobCounter
   {
   private: 
    int nJobs;
    pthread_mutex_t jc_lock;
    BLock *jcBLock;

   public:
     JobCounter()
        {
        nJobs = 0;  
        int status = pthread_mutex_init( &jc_lock, NULL);
        if(status != 0) 
           {
           std::cout << "\nError in JobCounter mutex initialization\n" 
                     << std::endl;
           exit(0);
           }
        jcBLock = new BLock(true);         // mark "pool idle"
        }
   
     ~JobCounter()
         {
         int status = pthread_mutex_destroy(&jc_lock);
         if(status != 0) 
            {
            std::cout << "\nError in JobCounter mutex destruction \n" 
                      << std::endl;
            exit(0);
            }
         delete jcBLock; 
        }

     void RegisterJob()
        {
        Pthread_Mutex_LockBis(&jc_lock, "RegisterJob()");
        if(nJobs==0) jcBLock->SetState(false);
        nJobs++;
        Pthread_Mutex_Unlock(&jc_lock);
        }

     void UnregisterJob()
        {
        Pthread_Mutex_LockBis(&jc_lock, "UnregisterJob()");
        nJobs--;
        Pthread_Mutex_Unlock(&jc_lock);
        if(nJobs==0)
           jcBLock->Set_And_Notify_All(true);
        }

     BLock *GetIdleBLock() { return jcBLock; }
     };


// **********************< JobMgr class>****************************
// The purpose of the JobMgr class is to manage a group of tasks, and 
// in particular to run a boolean lock associated to the parallel job,
// that will remain in a "true" state as long as the whole group of
// tasks in the job is still active.
//
// The group of tasks is dynamic. This job manager takes into account
// the tasts that are initially submitted to the pool, the tasks that
// are dynamically created (spawned), as well as the tasks that exit
// the group at termination. When there are no more tasks, the "end 
// of job" is  signalled to client threads.
//
// NOTICE: in this class, the internal mutex "gmutex" protects the 
// shared state variable "n_tasks", which is the number of tasks still
// active in the group. But the public Boolean Lock is not protected 
// by this mutex, because this object has internally its own mutex 
// that ensures its thread safety. 
//
// Boolean lock state: "true" means that the associated job is running.
// When the monitored job terminates, the BLock state is set to "false" 
// and the change is notified. Client threads waiting for job termination 
// wait for "false".
// *********************************************************************

class JobMgr
   {
   private:
   ///////
   TaskGroup       *tgr;
   pthread_mutex_t gmutex;
   int             n_tasks;
   JobCounter      *JC;
 

   public:
   //////
   BLock gBlock;
   
   JobMgr(JobCounter *jc, int nt=0)
      {
      JC = jc;
      n_tasks = nt;
      int status = pthread_mutex_init( &gmutex, NULL);
      if(status != 0) 
         {
         std::cout << "\nError in mutex initialization\n" << std::endl;
         exit(0);
         }
      gBlock.SetState(true);         // mark "in use"
      }
   
   ~JobMgr()
      {
      int status = pthread_mutex_destroy(&gmutex);
      if(status != 0) 
         {
         std::cout << "\nError in mutex destruction \n" << std::endl;
         exit(0);
         }

      // Deallocate all the tasks in the group
      // -------------------------------------
      std::list<Task*>::iterator pos;
      for(pos=tgr->LT.begin(); pos!=tgr->LT.end(); pos++)
         {
         Task *t = *pos;      //  recover the Task* pointed by pos
         delete t; 
         }
      // Deallocate the task group itself
      // --------------------------------
      tgr->Clear();
      delete tgr;
      }


   void SetTaskGroup(TaskGroup *tg)
      {  tgr = tg; }

   // -------------------------------------------
   // The next function is called by the worker
   // threads in a thread pool, after they finish
   // a task.
   // -------
   // NOTICE: if the mutex lock incorporates the 
   // notification, it is possible that a thread
   // waiting for the job wakes up and tries to 
   // destroy the JobMgr BEFORE the mutex is
   // unlocked. In this case, the destruction of
   // a locked mutex gives an error.
   // ------------------------------------------
   void DecreaseActive()
      {
      // ----------------------------
      pthread_mutex_lock(&gmutex);
      n_tasks--;
      pthread_mutex_unlock( &gmutex);
      if(n_tasks==0)
         {
         // Signal end of job
         // -----------------
         gBlock.Set_And_Notify_All(false);
         // Unregister job from JobCounter
         // ------------------------------
         JC->UnregisterJob();
         }
      // ----------------------------
      }

   // ------------------------------------------
   // The next function is called by the worker
   // threads in a thread pool, when they post
   // a child task. The child task is added to 
   // the initial TaskGroup.
   // ------------------------------------------
   void IncreaseActive(Task *t)
      {
      pthread_mutex_lock( &gmutex);
      tgr->Attach(t);
      n_tasks++;
      pthread_mutex_unlock( &gmutex);
      }
   };

#endif
