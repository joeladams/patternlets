/* pthreadBarrier.h implements the Barrier pattern using pthreads.
 *
 * Joel Adams, Calvin College, Fall 2013.
 */

#include <pthread.h>    // various pthread functions

// Shared Variables used to implement the barrier
   pthread_mutex_t barrierMutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_cond_t allThreadsPresent = PTHREAD_COND_INITIALIZER;
   double barrierThreadCount = 0;

/* the Barrier pattern for pthreads
 * params: numThreads, the number of threads being synchronized
 * postcondition: all of those threads have reached this call
 *                 and are now ready to proceed.
 */
void pthreadBarrier(unsigned long numThreads) {
   pthread_mutex_lock( &barrierMutex );
   barrierThreadCount++;
   if (barrierThreadCount == numThreads) {
      barrierThreadCount = 0;
      pthread_cond_broadcast( &allThreadsPresent );
   } else {
      while ( pthread_cond_wait( &allThreadsPresent, &barrierMutex) != 0 );
   }
   pthread_mutex_unlock( &barrierMutex );
}

void barrierCleanup() {
   pthread_mutex_destroy(&barrierMutex);
   pthread_cond_destroy(&allThreadsPresent);
}

