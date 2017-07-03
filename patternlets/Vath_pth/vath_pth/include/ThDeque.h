// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
/*  ThDeque.h
 *
 *  Safe deque class, adapted to the needs of the thread pool utility.
 *  We will use this utility to queue tasks. 
 *
 *  When the queue is empty, threads go to sleep on a CV. 
 *
 *  We do not need a capacity for the usage of this class. The Add
 *  operation never blocks.
 *
 *  Remove is called by idle threads, and operates like the
 *  same function in the ThQueue class. TryRemove is called by
 *  threads that have suspeded a task, and removes items from
 *  the back. 
 * ************************************************************/

#ifndef TH_DEQUE
#define TH_DEQUE

#include <deque>
#include <pthread.h>
#include <iostream>
#include <errors.h>
#include <Common.h>

using namespace std;

template <class T>
class ThDeque
    {
    protected:

    std::deque<T> c;          // container for the elements
    bool active;
    pthread_mutex_t qlock;
    pthread_cond_t  not_empty;

    int count_add;
    int count_remove;
    int count_trf;
    int count_trb;
    int count_close;
    
    public:

    int size;

    // ************
    // Constructor
    // ************

    ThDeque()   
        {
        int retval;

        size = 0;
        active = true;

        count_add = 0;
        count_remove = 0;
        count_trf = 0;
        count_trb = 0;
        count_close = 0;

	retval = pthread_mutex_init( &qlock, NULL); 
	if(retval != 0) err_abort(retval, "Mutex init");
	retval = pthread_cond_init( &not_empty, NULL); 
	if(retval != 0) err_abort(retval, "CV init");
        }


    // **********
    // Destructor
    // **********
     ~ThDeque()  // destructor
         {
         int retval;
        
         /* 
         cout << "\n Mutex locking status: "
              << "\nAdd        : " << count_add
              << "\nRemove     : " << count_remove
              << "\nTry_back   : " << count_trb
              << "\nTry_front  : " << count_trf
              << "\nClose      : " << count_close << endl;
         */

	 retval = pthread_mutex_destroy( &qlock); 
	 if(retval != 0) err_abort(retval, "Mutex destroy");
	 retval = pthread_cond_destroy( &not_empty); 
	 if(retval != 0) err_abort(retval, "CV destroy");
         }   
  
    int GetSize()
        {
        Pthread_Mutex_Lock(&qlock);
        int ret = size;        
        Pthread_Mutex_Unlock(&qlock);
        return ret;
        }


    // ******************************      
    // insert element into the queue
    // ******************************

    void Add (const T& elem) 
        {
        int retval;
        
        Pthread_Mutex_Lock(&qlock);
        count_add++;
        // .....................................................
        c.push_back(elem);
        size++;

        // If queue was empty, signal add event
        // ------------------------------------ 
        if(size==1)
            {
            retval = pthread_cond_broadcast(&not_empty);
	    if(retval != 0) err_abort(retval, "CV broadcast");
            }
        // ....................................................
        count_add--;
        Pthread_Mutex_Unlock(&qlock);
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
        int status;
        T retval;
        bool state;

        Pthread_Mutex_Lock(&qlock);
        count_remove++;
        // ----------------------------------------
        // wait if queue is empty but still active
        while(size==0 && active==true ) 
	    pthread_cond_wait(&not_empty, &qlock);
        state = (size==0) && (active==false) ;
        if(state) flag = false;
        // ---------------------------------------
        //count_remove--;                move away
        //Pthread_Mutex_Unlock(&qlock);
        
       // ------------------------------------------------------
       // I reached here for several reasons
       // -) size=0, inactive (state=true): return a false value
       // -) size >0, active or inactive (state=false) : pop and 
       //    return, signaling "queue not full if appropriate
       // -------------------------------------------------------
       if(state)
           {
           // -----------------------------------------------------
           // Before returning, remember that other threads may be
           // waiting for "queue not empty", signal the condition
           // so that they may also wake up and return a fake value
           // -----------------------------------------------------
           status = pthread_cond_broadcast(&not_empty);
	   if(status != 0) err_abort(status, "CV broadcast");
           count_remove--;               // moved away
           Pthread_Mutex_Unlock(&qlock);
           }
        else
           {
           // ----------------------------------------------------
           // Here we are sure that thread woke up by genuine post
           // Remove and prepare return value
           // ----------------------------------------------------
           //Pthread_Mutex_Lock(&qlock);
           //count_remove++;
           // ----------------------------------------
           T elem(c.front());
           c.pop_front();
           size--;
           retval = elem;
           flag = true;
           // ----------------------------------------
           count_remove--;
           Pthread_Mutex_Unlock(&qlock);
           }
        return retval;
        }
    
    // ***************************************************
    // Tries to remove head element from the deque, but 
    // does not wait if the deque is empty
    // The boolean flag indicates if the return value T
    // is valid. 
    // ***************************************************
    T TryRemoveFront(bool& flag) 
        {
        /* -----------------------------------------------
        * Strategy
        * If size==0, returns a fake value. Otherwise, returns
        * a valid value.
        * -----------------------------------------------*/
        T retval;
        Pthread_Mutex_Lock(&qlock);
        count_trf++;
        if(size == 0) flag = false;
        else
           { 
           // ---------------------------------------------
           // Here we are sure that there is a head element
           // ---------------------------------------------
           T elem(c.front());
           flag = true;
           c.pop_front();
           size--; 
           retval = elem;
           }
        count_trf--;
        Pthread_Mutex_Unlock(&qlock);
        return retval;
        }
    

    // **************************************************
    // Same function as before, but now the remove takes
    // place at the tail of the deque.
    // ************************************************** 
    T TryRemoveBack(bool& flag) 
        {
        /* -----------------------------------------------
         * Strategy
         * If size==0, returns a fake value. Otherwise, returns
         * a valid value.
         * -----------------------------------------------*/
        T retval;
        Pthread_Mutex_Lock(&qlock);
        count_trb++;
        if(size == 0) flag = false;
        else
           { 
           // ---------------------------------------------
           // Here we are sure that there is a head element
           // ---------------------------------------------
           T elem(c.back());
           flag = true;
           c.pop_back();
           size--; 
           retval = elem; 
           }
       count_trb--;
       Pthread_Mutex_Unlock(&qlock);
       return retval;
       }

   // **************************************
   // Close queue. The only effect of this
   // call is to stop all Add() actions from
   // producer threads
   // **************************************
   void CloseQueue()
       {
        int retval;
        Pthread_Mutex_Lock(&qlock);
        count_close++;
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
        count_close--;
        Pthread_Mutex_Unlock(&qlock);
        }
   };

#endif // TH_DEQUE 
