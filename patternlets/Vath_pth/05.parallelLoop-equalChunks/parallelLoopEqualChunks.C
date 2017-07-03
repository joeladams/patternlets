
/*
 * Using Victor Alessandrini's vath_pth library.
 *
 * parallelLoopEqualChunks.C
 * ... illustrates the use of parallel for loop in which
 * 		threads iterate through equal sized chunks of the index range
 * 		Ex.  thread 1 [0, 1, 2 ... 7], thread 2 [8, 9 ... 15]
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./parallelLoopEqualChunks [numThreads]
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

// -------------------
// Worker threads code
// -------------------

void thread_fct(void *idp)  {
    // ThreadRange() computes index range for worker thread from index range.
    // Function knows rank of worker thread and number of threads.
    int beg, end;
    beg = 0;                    // initialize [beg, end) to global range
    end = REPS;

    TH->ThreadRange(beg, end);  // now [beg, end) is sub-range for this thread

    int rank = TH->GetRank();

    for (int i = beg; i < end; i++)  {
        printf("Thread %d performed iteration %d\n", rank, i);
    }
}

int main(int argc, char **argv)  {
    int numThreads;

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;

    TH = new SPool(numThreads);
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();


    delete TH;
    return 0;
}
