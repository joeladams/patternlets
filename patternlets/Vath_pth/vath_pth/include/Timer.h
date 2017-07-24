// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
/* =======================================================
 * Timer.h
 * Puts a thread in an uncoditional wait for a number of
 * miliseconds.
 * ======================================================*/

#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>

class Timer
    {
    private:
      pthread_mutex_t lock;
      pthread_cond_t  cond;
    
    public:
     Timer();
     ~Timer();
     void Wait(long time_wait);
    }; 
	
#endif
