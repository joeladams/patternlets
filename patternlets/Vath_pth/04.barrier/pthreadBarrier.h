/* pthreadBarrier.h implements the Barrier pattern using pthreads.
 *
 * AUTHOR: Victor Alessandrini, 2015
 * Example in book "Shared Memory Application Programming"
 * Edited by Hannah Sonsalla, Macalester College, 2017.
 */

#include <pthread.h>

int count; // count = shared counter

// Initialize mutex and conition variable
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t count_cond  = PTHREAD_COND_INITIALIZER;

// -------------------------------------------------------
// Function called by worker threads to wait for the
// arrival of all the cooperating threads
// ------------------------------------------------------
void BarrierWait(int R)  {
    // -----------------------------------------------------
    // Initially, count=N (number of cooperating threads)
    // Caller thread acquire mutex and decreases count.
    // - If count>0, wait on condition.
    // - If count==0, print rank and broadcast wake up.
    // ----------------------------------------------------
     pthread_mutex_lock(&count_lock);   // lock mutex
     count--;   // decrease counter
     if(count)  {
         while(count){
             pthread_cond_wait(&count_cond, &count_lock);
             }
         pthread_mutex_unlock(&count_lock);
     }
     else  {
         printf("\n Broadcast sent by thread %d\n\n", R);
         // broadcast condition to wake up all waiting threads
         pthread_cond_broadcast(&count_cond);
         pthread_mutex_unlock(&count_lock);   // unlock mutex
     }
 }
