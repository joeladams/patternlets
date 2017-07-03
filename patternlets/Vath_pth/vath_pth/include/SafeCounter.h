// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// SafeCounter.h
// Thread safe counter. Returns 1, 2, 3, ...
// ---------------------------------------

#ifndef SAFECOUNTER_H
#define SAFECOUNTER_H

#include <pthread.h>

class SafeCounter
   {
   private:
   int data;
   pthread_mutex_t lock; 

   public:
   SafeCounter()
      {
      data = 0;
      pthread_mutex_init(&lock, NULL);
      }

   ~SafeCounter()
      {
      pthread_mutex_destroy(&lock);
      }

   int Next() 
      {
      int retval;
      pthread_mutex_lock(&lock); 
      data ++;
      retval = data; 
      pthread_mutex_unlock(&lock);
      return retval; 
      }
   
   void Reset()
      {
      pthread_mutex_lock(&lock); 
      data = 0;
      pthread_mutex_unlock(&lock);
      }
   }; 

#endif
