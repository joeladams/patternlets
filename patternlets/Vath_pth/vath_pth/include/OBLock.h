// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
// OBLock.h
//
// OpenMP implementation of the portable Boolean Lock interface
// encapsulating a BUSY (SPIN) wait.
// ************************************************************

#ifndef OMP_BLOCK_H
#define OMP_BLOCK_H

#include <omp.h>

class OBLock
    {
    private:

    bool state;
    omp_lock_t block;

    public:
	
    OBLock()
       {
       omp_init_lock(&block);
       state = false;
       }

    OBLock(bool B)
       {
       omp_init_lock(&block);
       state = B;
       }

    ~OBLock()
       {  omp_destroy_lock(&block); }

    void SetState( bool value )
       {
       omp_set_lock(&block);
       state = value;
       omp_unset_lock(&block);
       }

    bool GetState()
       {
       bool retval;
       omp_set_lock(&block);
       retval = state;
       omp_unset_lock(&block);
       return retval;
       }

    void  Wait_Until(bool S)
       {
       bool my_state = !S;
       while(my_state == !S)
          {
          omp_set_lock(&block);
          my_state = state;
          omp_unset_lock(&block);
          }
       }
    };

#endif
