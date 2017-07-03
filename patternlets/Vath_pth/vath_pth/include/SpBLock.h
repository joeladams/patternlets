// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
/* ==============================================================
 * SpBLock.h
 * Spinning Boolean Lock interface, used to encapsulate a busy wait.
 * The basic SpinLock used here is the native spinlock in Pthreads,
 * or the custom atomic implementations in C++11 and Windows.
 * =============================================================*/

#ifndef SP_BLOCK_H
#define SP_BLOCK_H

#include <SpinLock.h>

class SpBLock
    {
    private:

    bool state;
    SpinLock block;

    public:
	
    SpBLock()
       { state = false; }

    SpBLock(bool B)
       { state = B; }

    ~SpBLock() { }

    void SetState( bool value )
       {
       block.Lock();
       state = value;
       block.Unlock();
       }

    bool GetState()
       {
       bool retval;
       block.Lock();
       retval = state;
       block.Unlock();
       return retval;
       }

    void  Wait_Until(bool S)
       {
       bool my_state = !S;
       while(my_state == !S)
          {
          block.Lock();
          my_state = state;
          block.Unlock();
          }
       }
    };

#endif
