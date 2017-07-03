// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// SynchP.h
// Template code.
// This is a synchronization point among several threads
// with one producer that writes a value of type T, and
// several consumers
// Different synchronization strategy than SynchPoint<T>
//
// In this scheme:
//
// *) state = false means that object is ready to be reused
//    because previos data has been read.
//
// *) Producer thread only waits if state=true (object not
//    ready). Once value is written, it does not wait. The
//    Put() function specifies how many threads must read
//    the data.
//
// *) Consumes will eventually wait for true, read the data
//    and decrease the counter. When counter=0, state is
//    changed to false and the change signaled.
//
// ********************************************************

#ifndef SYNCH_P
#define SYNCH_P

#include <pthread.h>
#include <errors.h>

template <class T>
class SynchP
   {
   private:

   T      element;
   bool   state;
   int    counter;
   pthread_mutex_t  slock;
   pthread_cond_t   scond;

   public:

   // Constructor and destructor
   // --------------------------
   SynchP()
      {
      int st;

      state = false;
      counter = 0;
      st = pthread_mutex_init(&slock, NULL);
      if(st) err_abort(st, "Mutex Init");
      st = pthread_cond_init(&scond, NULL);
      if(st) err_abort(st, "CV Init");
      }


   ~SynchP()
      {
      int st;
      st = pthread_mutex_destroy(&slock);
      st = pthread_cond_destroy(&scond);
      }


   /***********************************************
    * This function posts a value. It does not wait
    ***********************************************/

   void Post(T& elem, int nReaders)
      {
      int st, retval;

      st = pthread_mutex_lock(&slock);
      if(st) err_abort(st, "Mutex Lock");
      
      // wait if object not ready
      // ------------------------
      while(state==true)
         pthread_cond_wait(&scond, &slock);
      
      // here, state is false
      // --------------------
      element = elem; 
      counter = nReaders;
      state = true;
      pthread_cond_broadcast(&scond);
      
      st = pthread_mutex_unlock(&slock);
      if(st) err_abort(st, "Mutex unlock");
      }    



   /********************************************
    * This function waits at the synchronization
    * point and gets a T value
    *******************************************/   
       
   T Get()
      {
      int st;
      T retval = T();

      st = pthread_mutex_lock(&slock);
      if(st) err_abort(st, "Mutex Lock");

      // wait for true, signaled after a write
      // -------------------------------------
      while(state==false)
         pthread_cond_wait(&scond, &slock);
      
      // here, state is true, read
      // -------------------------
      if(state==true)
          {
          retval = element;
          counter--;
          }
      
      // If I am the last to read, change state to
      // false and signal the change
      // -----------------------------------------
      if(counter==0)
          {
          state = false;
          pthread_cond_signal(&scond);
          }

      st = pthread_mutex_unlock(&slock);
      if(st) err_abort(st, "Mutex Lock");
      return retval;
      }    

   // end of class
   };
          
// *************************************************

 
#endif  // SYNCH_P
