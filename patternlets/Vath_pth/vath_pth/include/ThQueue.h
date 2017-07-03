// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
/*  ThQueue.h
 *
 *  Safer queue class. When the queue is  empty, threads go 
 *  to sleep on a CV. 
 *  The STL is never full, but we define a capacity because it
 *  is useful to control permanent producers
 *  
 * Instances like ThQueue<double>, ThQueue<int>, and so on can
 * be used directly to build pipes between threads.
 *
 * This tool is also used for thread pools, to pipeline work
 * requests.
 *
 * NOTE: New version, adding Peek() and TryRemove() member
 * functions.
 * ************************************************************/

#ifndef THQUEUE
#define THQUEUE

#include <queue>
#include <pthread.h>
#include <iostream>
#include <errors.h>
#include <Common.h>

using namespace std;

template <class T>
class ThQueue 
    {
    protected:

    std::queue<T> c;        // container for the elements
    int        capacity;
    bool       active;
    pthread_mutex_t qlock;
    pthread_cond_t  not_empty;
    pthread_cond_t  not_full;
    
    public:

    int size;

    // ************
    // Constructor
    // ************

    ThQueue(int cap)   
        {
        int retval;

        capacity = cap;
        active = true;
        size = 0;

	retval = pthread_mutex_init( &qlock, NULL); 
	if(retval != 0) err_abort(retval, "Mutex init");
	retval = pthread_cond_init( &not_empty, NULL); 
	if(retval != 0) err_abort(retval, "CV init");
	retval = pthread_cond_init( &not_full, NULL); 
	if(retval != 0) err_abort(retval, "CV init");
        }


    // **********
    // Destructor
    // **********

     ~ThQueue(){}    // destructor
  
    int GetSize()
        {
        Pthread_Mutex_Lock(&qlock);
        int ret = size;        
        Pthread_Mutex_Unlock(&qlock);
        return ret;
        }


    // ******************************************      
    // insert element into the queue, but only if
    // the queue is active. Return 0 if the add
    // failed because the queue is inactive.
    // ******************************************

    int Add (const T& elem) 
        {
        int retval, cancel, tmp;
        
        Pthread_Mutex_Lock(&qlock);
        // ......................................................
        while(size==capacity)                    // wait if full
            pthread_cond_wait(&not_full, &qlock);

        // At this point, queue is not full. 
        // ---------------------------------
        if(active == true)           // check if queue is active
            {
            c.push(elem);
            size++;
            tmp = 1;

            // If queue was empty, signal add event
            // ------------------------------------ 
            if(size==1)
                {
                retval = pthread_cond_broadcast(&not_empty);
	        if(retval != 0) err_abort(retval, "CV broadcast");
                }
             }
        else tmp = 0;
        // ....................................................
        Pthread_Mutex_Unlock(&qlock);
        return tmp;
        }



    // ***************************************************
    // Removes element from the queue and return its value
    // The boolean flag indicates if the return value T
    // is valid. 
    // If remove with an inactive empty queue, it does
    // not waits, it returns an invalid value.
    // ***************************************************

    T Remove(bool& flag) 
        {
        int retval, cancel, tmp;;

        Pthread_Mutex_Lock(&qlock);
        // .......................
        // wait if queue is empty and active
        // ---------------------------------
        while(size==0 && active==true ) 
	  pthread_cond_wait(&not_empty, &qlock);
       
       /* -----------------------------------------------
        * I reached here for several reasons
        * -) size=0, inactive : return a false value
        * -) size >0, active or inactive : pop and return,
        *    signaling "queue not full if appropriate
        * -----------------------------------------------*/

       if(size == 0 && active==false)
           {
           // .................................
           Pthread_Mutex_Unlock(&qlock);
           T fake;
           flag = false;
         
           // -----------------------------------------------------
           // Before returning, remember that other threads may be
           // waiting for "queue not empty", signal the condition
           // so that they may also wake up and return a fake value
           // -----------------------------------------------------
           retval = pthread_cond_broadcast(&not_empty);
	   if(retval != 0) err_abort(retval, "CV broadcast");

           return fake;
           }
        
        // ----------------------------------------------------
        // Here we are sure that thread woke up by genuine post
        // Remove and return
        // ----------------------------------------------------
        T elem(c.front());
        c.pop();
        size--;

        // If queue was full, signal the remove event
        // ------------------------------------------
        if(size==(capacity-1))
            {
            retval = pthread_cond_broadcast(&not_full);
	    if(retval != 0) err_abort(retval, "CV broadcast");
            }

        // ..................................
        Pthread_Mutex_Unlock(&qlock);
        flag = true;
        return elem;
        }

    // ***************************************************
    // NEW CODE:
    // --------
    // Tries to remove head element from the queue, but does
    // not wait if the queue is empty
    // The boolean flag indicates if the return value T
    // is valid. 
    // ***************************************************

    T TryRemove(bool& flag) 
        {
        Pthread_Mutex_Lock(&qlock);

       /* -----------------------------------------------
        * Strategy
        * If size==0, returns a fake value. Otherwise, returns
        * a valid value.
        * -----------------------------------------------*/

       if(size == 0)
           {
           T fake;
           flag = false;
           Pthread_Mutex_Unlock(&qlock);
           return fake;
           }
       else
           { 
           // ---------------------------------------------
           // Here we are sure that there is a head element
           // ---------------------------------------------
           T elem(c.front());
           flag = true;
           c.pop();
           size--; 
           Pthread_Mutex_Unlock(&qlock);
           return elem;
           }
       }

    // ***************************************************
    // Reads element from the queue and return its value
    // The boolean flag indicates if the return value T
    // is valid. 
    // If reads with an inactive empty queue, it does
    // not wait, it returns an invalid value.
    // ***************************************************

    T Read(bool& flag) 
        {
        int retval, cancel, tmp;;

        retval = pthread_mutex_lock(&qlock);
	if(retval != 0) err_abort(retval, "Mutex lock");

        // wait if queue is empty and active
        while(size==0 && active==true ) 
	  pthread_cond_wait(&not_empty, &qlock);
       
       /* -----------------------------------------------
        * I reached here for several reasons
        * -) size=0, inactive : return a false value
        * -) size >0, active or inactive : return,
        *    DO NOT SIGNAL "queue not full" if appropriate
        * -----------------------------------------------*/

       if(size == 0 && active==false)
           {
           retval = pthread_mutex_unlock(&qlock);
	   if(retval != 0) err_abort(retval, "Mutex unlock");
           T fake;
           flag = false;
           return fake;
           }
        
        // ----------------------------------------------------
        // Here we are sure that thread woke up by genuine post
        // Remove and return
        // ----------------------------------------------------
        T elem(c.front());

        retval = pthread_mutex_unlock(&qlock);
	if(retval != 0) err_abort(retval, "Mutex unlock");
        
        flag = true;
        return elem;
        }

   // **************************************
   // Close queue. The only effect of this
   // call is to stop all Add() actions from
   // producer threads
   // **************************************
   
   void CloseQueue()
       {
        int retval;

        retval = pthread_mutex_lock(&qlock);
	if(retval != 0) err_abort(retval, "Mutex lock");
        active = false;   // queue is closed

        // ---------------------------------------------------------
        // From now on, no more adds are possible. But, notice that
        // it could happen that there are consumer threads waiting
        // for "non-empty" condition in a remove operation. Wake
        // them up so that they finish properly (nobody else will
        // do that because there are no more adds)
        // ---------------------------------------------------------
        if(size==0)
            {
            retval = pthread_cond_broadcast(&not_empty);
	    if(retval != 0) err_abort(retval, "CV broadcast");
            }

        retval = pthread_mutex_unlock(&qlock);
	if(retval != 0) err_abort(retval, "Mutex unlock");
        }

   // ***********************************
   // Drain the queue. called by consumer
   // threads, after closing the queue
   // ***********************************

   void DrainQueue()
      {
      T data;
      bool state;
      do
         {
         data = Remove(state);
         }while(state);
      }


   // ***************************************
   // This function is called only by cleanup
   // handlers, when threads holding a locked
   // mutex are cancelled
   // ***************************************

   void MutexRelease()
        {
        int retval = pthread_mutex_trylock(&qlock);
        //if(retval) err_abort(retval, "Trylock in Mutex release");
        retval = pthread_mutex_unlock(&qlock);
	if(retval) err_abort(retval, "Mutex unlock, cleanup handler");
        }
   };

#endif /* THQUEUE */
