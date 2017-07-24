// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// SpinLock.h
// Encapsulates native spinlock
// --------------------------------------------

#ifndef  SPIN_LOCK_H
#define  SPIN_LOCK_H

#include <pthread.h>

class SpinLock
    {
    private:
     pthread_spinlock_t lock;

    public:
    SpinLock() 
       {
       pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
       }

    ~SpinLock() 
       {
       pthread_spin_destroy(&lock);
       }

    void Lock()
       {
       int retval = pthread_spin_lock(&lock);
       }
	    
    void Unlock()
       {
       int retval = pthread_spin_unlock(&lock);
       }
    };

#endif  /* SPIN_LOCK_H */
