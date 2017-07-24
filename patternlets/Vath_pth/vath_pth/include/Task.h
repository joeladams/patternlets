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

// Task.h 
// **************************************
#ifndef TASK_H
#define TASK_H

#include <list>
#include <BLock.h>

// *********************************************************
// The Task class describes a task. 
//
// Task objects carry a lot of information:
// - identifier of job to which the task belongs
// - pointer to parent task: valid pointer if parent task is
//   run by worker thread (explicit task in OpenMP language), 
//   null pointer if it is run by external thread (implicit
//   task in OMP language)
// - number of child tasks
// - bool that informa is a parent task is waiting for this 
//   one   
// *********************************************************
class Task
   {
   private:
    pthread_mutex_t task_mutex;
    int       TGid;             // ID of job this task belongs
    Task      *parent;          // ponter to parent task. NULL by default
    int       refcount;         // number of my childs
    int       rank;             // rank of owner thread
    BLock     *BL;              // used for taskwait
    bool      iswaited;         // is my parent waiting for me?
    int       taskID;           // a unique integer, used for debugging only                              
   public: 
    Task();
    virtual ~Task();   // destructor
    virtual void ExecuteTask() = 0;    // interface

    void IncreaseRefcount();           // increases my refcount
    void DecreaseRefcount();           // decreases parent refcount
    void WaitForChilds();

    void SetWaited(bool B);
    bool Am_I_Waited();

    int GetJobid();
    void SetJobid(int jid);
 
    Task *GetParent(const char *msg);          
    void SetParent(Task *T);
 
    int GetOwnerRank();
    void SetOwnerRank(int R);
 
    int  GetTaskID();
    void SetTaskID(int R);

    BLock *GetBLock();
    void ResetBLock();
    };


// ********************************************************
// The TaskGroup structure encapsulates a group of pointers 
// to tasks as a list of Task objects
// ****************************************************

struct TaskGroup
   {
   std::list<Task*> LT;
   TaskGroup() {}
   ~TaskGroup() { LT.clear(); }
   void Attach(Task *T) { LT.push_back(T); }
   void Clear() { LT.clear(); }
   int  Size() { return LT.size(); }
   };

#endif
