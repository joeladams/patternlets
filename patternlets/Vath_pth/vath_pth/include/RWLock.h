// AUTHOR: Victor Alessandrini, 2015
// VATH libray,in book "Shared Memory Application
// Programming"
// *********************************************
/* RWLock.h
 * This class encapsulates the Reader-Writer locks in
 * the different programming environments.
 * ================================================*/

#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

class RWLock
    {
    private:
      pthread_rwlock_t rw_lock;

    public:
      RWLock()   
         { pthread_rwlock_init(&rw_lock, NULL);}

      ~RWLock()
         { pthread_rwlock_destroy(&rw_lock); } 
 
      int  Lock(bool B)
         {
         int status;
         if(B) status = pthread_rwlock_wrlock( &rw_lock);
         else status = pthread_rwlock_rdlock( &rw_lock );
         return status;
         }

      int  Trylock(bool B)
         {
         int status;
         if(B) status = pthread_rwlock_trywrlock( &rw_lock);
         else  status = pthread_rwlock_tryrdlock( &rw_lock);
         return status;
         }

      int  Unlock(bool write=true)
         {
         int status = pthread_rwlock_unlock(&rw_lock);
         return status;
         }
    };

#endif /* RWLOCK_H */
