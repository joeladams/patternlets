
/*
 * barrier.c
 *
 * AUTHOR: Victor Alessandrini, 2015
 * Example in book "Shared Memory Application Programming"
 * Edited by Hannah Sonsalla, Macalester College, 2017.
 *
 * ... illustrates the use of the barrier command,
 * 	   using the commandline to control the number of threads...
 *
 * Shows how to construct Barrier synchronization using the Pthreads
 * idle wait protocol. All threads write "before" message, wait
 * on the barrier, write "after" message, and exit.
 *
 * Usage: ./barrier [numThreads]
 *
 * Exercise:
 * - Compile & run several times, noting interleaving of outputs.
 * - Uncomment the BarrierWait function on line A, recompile, rerun,
 *    and note the change in the outputs.
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>

#include "pthreadBarrier.h"

int numThreads;
SPool *TH;

// -------------------
// Worker threads code
// -------------------

void thread_fct(void *idp)  {

    int rank = TH->GetRank();
    printf("Thread %d of %d is BEFORE barrier\n", rank, numThreads);
    //BarrierWait(rank);                                // A
    printf("Thread %d of %d is AFTER barrier\n", rank, numThreads);

}

int main(int argc, char **argv)  {

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 2;
    count = numThreads;

    // Create worker threads
    // ------------------------------
    TH = new SPool(numThreads);

    // Launch worker threads
    // -----------------------------
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();
    return 0;

}
