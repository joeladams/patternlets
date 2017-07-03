
/*
 * Using Victor Alessandrini's vath_pth library.
 *
 * parallelLoopChunksOf1.C
 * ... illustrates the use of parallel for loop in which
 * 		threads iterate through chunks of size 1 of the index range
 * 		Ex.  thread 1 [0, 2, ... 14], thread 2 [1, 3 ... 15]
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./parallelLoopChunksOf1 [numThreads]
 *
 * Exercise
 * - Compile and run, comparing output to source code
 * - try with different numbers of threads, e.g.: 2, 3, 4, 6, 8
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>
#include <pthread.h>

SPool *TH;
const int REPS = 16;
int numThreads;

// -------------------
// Worker threads code
// -------------------

void thread_fct(void *idp)  {

    int rank = TH->GetRank();

    for (int i = rank - 1; i < REPS; i+=numThreads)  {         //Section A
        printf("Thread %d performed iteration %d\n", rank, i);
    }
}

int main(int argc, char **argv)  {

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    TH = new SPool(numThreads);
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();

    delete TH;
    return 0;
}
