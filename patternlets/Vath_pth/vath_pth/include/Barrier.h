// ***********************************************
// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// ***********************************************
// Barrier.h
// This class implements a traditional barrier
// algorithm. There are native barriers in
// Pthreads and Windows, but this class is useful
// for portability.
// ----------------------------------------------

#ifndef  BARRIER_H
#define  BARRIER_H

#include <pthread.h>

class  Barrier
   {
   private:
     pthread_mutex_t   mutex;          // Controls access to barrier data 
     pthread_cond_t    cond;           // condition variable         
     int               nTh;            // number of threads required
     int               counter;        // counts active threads      
     bool              cycle;          // toggles between successive calls
 
   public:
     Barrier(int count);
     ~Barrier();
     int Wait();
   };

#endif  /* BARRIER_H */
