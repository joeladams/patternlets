// --------------------------------------------------------
// Common.h
// --------
// This module implements a simple Pthreads interface, with 
// implicit error handling 
// ---------------------------------------------------------

#ifndef COMMONS_H
#define COMMONS_H

#include <pthread.h>

#define  JOINABLE    1
#define  DETACHABLE  2

#ifdef __c_plus_plus
 extern "C"
   {
#endif

   void Pthread_Create( pthread_t *thread,  pthread_attr_t *attr,
                     void *(*thread_fct)(void *), void *arg );
   void Pthread_Join(pthread_t thread, void **data); 
   void Pthread_Mutex_Lock(pthread_mutex_t *mutex); 
   void Pthread_Mutex_LockBis(pthread_mutex_t *mutex, const char *s); 
   void Pthread_Mutex_Unlock(pthread_mutex_t *mutex); 
   void Pthread_Spin_Lock(pthread_spinlock_t *mutex); 
   void Pthread_Spin_Unlock(pthread_spinlock_t *mutex); 
   void Pthread_Cond_Wait(pthread_cond_t *cond, pthread_mutex_t *mutex); 
   void Pthread_Cond_Signal(pthread_cond_t *cond); 
   void Pthread_Cond_Broadcast(pthread_cond_t *cond);
   void Pthread_Init_Attr(pthread_attr_t *attr, int type, float stack);

#ifdef __c_plus_plus
   }
#endif

#endif  // COMMONS_H

