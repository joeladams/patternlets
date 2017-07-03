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

//  SPool.h
//  Very lightweight thread pool specialized 
//  for SPMD parallel codes
// *****************************************
#ifndef SPOOL_H
#define SPOOL_H

#include <pthread.h>
#include <BkBarrier.h>
#include <BLock.h>
    
class SPool
    {
    private:
     //pool characteristics
     //-------------------- 
     int                 nThreads;
     bool                shutdown;

     // current task
     // ------------
     void                   (*fct)(void *);
     void                   *arg;

     // pool state 
     // ----------
     pthread_t           *threads;
     pthread_attr_t      attr;
     bool                active;
        
     // pool synchronization 
     // --------------------
     pthread_mutex_t     qLock;
     BkBarrier           *BlBarrier;
     
     void PeerThread();  // called internally
     bool GetActive();
     void SetActive(bool state);

    public:
     SPool(int Nth, double d = 0.0);
     ~SPool();

     // Basic management
     // ----------------
     void Dispatch(void (*taskfct)(void *), void *arg);
     void WaitForIdle();
     void Join();

     // Utilities
     // ---------
     int  GetRank();                // returns rank of caller thread

     void CancelTeam();             // cancelling worker threads
     void SetCancellationPoint();   

     void ThreadRange(int& beg, int& end);       // work sharing
     void ThreadRange(double& beg, double& end);

     friend void *ThreadFunc(void *P);
    };

#endif
