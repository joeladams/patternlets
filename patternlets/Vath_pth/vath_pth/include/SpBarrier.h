// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// SpBarrier.h
// Barrier with spin wait, made with spin locks
// --------------------------------------------

#ifndef  SPBARRIER_H
#define  SPBARRIER_H

#include <SpinLock.h>

class  SpBarrier
   {
   private:
     SpinLock   lock;           // Control access to barrier
     SpinLock   lwait;          // implementing busy wait
     int        nTh;            //  number of threads required
     int        counter;        // counts active threads      
     bool       cycle;          // toggles between successive calls
 
   public:
     SpBarrier(int count);
     ~SpBarrier();
     void Wait();
   };

#endif  /* SPBARRIER_H */
