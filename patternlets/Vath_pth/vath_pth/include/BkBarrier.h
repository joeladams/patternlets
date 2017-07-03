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

// BkBarrier.h
//
// A Barrier in which threads are released by an 
// external client, not by the last arriving thread.
// The external client interacts with the worker 
// thread team: waits for them to reach the barrier,
// and releases the team.
// ------------------------------------------------

#ifndef  BLOCKBARRIER_H
#define  BLOCKBARRIER_H

#include <pthread.h>

class BkBarrier
   {
   private:
     pthread_mutex_t   mutex;          // Controls access to barrier 
     pthread_cond_t    qtask;          // condition variable         
     pthread_cond_t    qidle;          // condition variable         
     int               nTh;            // number of threads required
     int               counter;        // counts active threads      
     bool              cycle;          // toggles between successive calls
     bool              is_idle;        // flag, true if all threads waiting
 
   public:
     BkBarrier(int count);
     ~BkBarrier();
     int Wait();                       // called by worker threads
     void WaitForIdle();               // called by client threads
     void ReleaseThreads();            // called by client threads
     bool State();                     // returns is_idle
     void WakeUpClient();              // NEW, needed in SPool
   };

#endif  /* BLOCKBARRIER_H */
