
/*
 * spmd2.C
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... illustrates the single-program-multiple-data (SPMD)
 *     using command line arguments to control the
 *     number of threads.
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./spmd2 [numThreads]
 *
 * Exercise:
 * - Compile & run with no commandline args
 * - Rerun with different commandline arg 4, 10, 20, etc.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>

SPool *TH;

void thread_fct(void *P)  {

    int rank = TH->GetRank();
    printf("Hello from thread %d \n", rank);

}

int main(int argc, char **argv)  {
    int numThreads;

    if(argc==2) numThreads = atoi(argv[1]);
    else numThreads = 4;     // default number of threads

    // Create worker threads
    // -----------------------------
    TH = new SPool(numThreads);

    // Launch worker threads
    // -----------------------------
    TH->Dispatch(thread_fct, NULL);
    TH->WaitForIdle();

    delete TH;
    return 0;
}
