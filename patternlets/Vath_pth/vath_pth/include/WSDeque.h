// AUTHOR: Victor Alessandrini, 2016
// VATH libray,in book "Shared Memory Application
// Programming"
// ***********************************************
//  WSDeque.h
//
//  Thread safe Deque class, implementing work stealing. 
//  Algorithm is taken from "The art of multiprocessor
//  programming", page 387.
//
//  This class encapsulates the std::deque class, adding
//  the necessary functionality (mutex locking) to
//  enforce thread safety. This queue operates in the
//  following way:
//
//  The queue is "owned" by one specific thread in a 
//  pool. Each worker thread has its own queue.
//
//  The owner queue inserts tasks by calling push().
//  No other thread can insert tasks in the queue.
//
//  The owner thread retrieves the most recently
//  queued task from the bottom of the queue, by
//  calling try_pop().
//
//  Other threads steal the oldest tasks in the queue 
//  by popping from the top, calling try_steal().
//
//  Notice that, in the absence of task stealing,
//  the queue is accessed only by the owner thread,
//  and would naturally be thread safe.
// *************************************************

#ifndef WS_DEQUE
#define WS_DEQUE

#include <pthread.h>
#include <iostream>
#include <deque>
#include <Task.h>
#include <errors.h>

class WSDeque
   {
   private:
   ///////
    std::deque<Task*> the_queue;
    pthread_mutex_t the_mutex;

   public:
   //////

    WSDeque() 
       {
       int status = pthread_mutex_init(&the_mutex, NULL);
       if (status != 0) err_abort(status, "pthread_mutex_init");
       }                                // default constructor

    ~WSDeque()
       {
       pthread_mutex_destroy (&the_mutex);
       }
        
    bool isEmpty()
       { return the_queue.empty(); }

    void push(Task *t)
       {
       pthread_mutex_lock(&the_mutex);
       the_queue.push_front(t);
       pthread_mutex_unlock(&the_mutex);
       }
   
    Task* try_pop()
       {
       Task *retval;
       pthread_mutex_lock(&the_mutex);
       if(the_queue.empty()) retval = NULL; 
       else
           {
           retval = the_queue.front();
           the_queue.pop_front();
           }
       pthread_mutex_unlock(&the_mutex);
       return retval;
       }
   
   Task* try_steal()
       {
       Task *retval;
       pthread_mutex_lock(&the_mutex);
       if(the_queue.empty()) retval = NULL;
       else
           {
           retval = the_queue.back();
           the_queue.pop_back();
           }
       pthread_mutex_unlock(&the_mutex);
       return retval; 
       }

    /*
    Task* try_pop()
       {
       pthread_mutex_lock(&the_mutex);
       if(the_queue.empty()) 
           {
           pthread_mutex_unlock(&the_mutex);
           return NULL;
           }
       else
           {
           Task *t = the_queue.front();
           the_queue.pop_front();
           pthread_mutex_unlock(&the_mutex);
           return t;
           }
       }
   
   Task* try_steal()
       {
       pthread_mutex_lock(&the_mutex);
       if(the_queue.empty())
           { 
           pthread_mutex_unlock(&the_mutex);
           return NULL;
           }
       else
           {
           Task *t = the_queue.back();
           the_queue.pop_back();
           pthread_mutex_unlock(&the_mutex);
           return t;
           }
       }
    */

    };

#endif // WS_DEQUE 
