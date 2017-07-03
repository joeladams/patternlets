// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// Reduction.h
// Template class to perform a reduction in a thread pool
// environment
// ------------------------------------------------------
#ifndef REDUCTION_H
#define REDUCTION_H

#include <pthread.h>

template<typename T>
class Reduction
   {
   private:
   T data;
   pthread_mutex_t lock; 

   public:
   Reduction()
      {
      pthread_mutex_init(&lock, NULL);
      }

   void Accumulate(T input) 
      {
      pthread_mutex_lock(&lock); 
      data += input; 
      pthread_mutex_unlock(&lock); 
      }
      
   void Multiply(T input) 
      {
      pthread_mutex_lock(&lock); 
      data *= input; 
      pthread_mutex_unlock(&lock); 
      }   
   
   T Data()
      {
      return data;
      }

   void Reset()
      {
      data -= data;
      }
   }; 

#endif
