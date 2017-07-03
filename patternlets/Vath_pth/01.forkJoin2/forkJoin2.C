/*
 * forkJoin2.C
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... illustrates the fork-join pattern and setting number of threads.
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./forkJoin2
 *
 * Exercise:
 * - Compile & run
 * - Rebuild and rerun using 2, 3, 4 threads
 * - What do you notice about the number of times statements are printed?
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>

SPool TH(1);    // initialize threads, global

void thread_fct(void *P)  {
    printf("\n  Part Completed");
}

void runWorkerThreads(int n)  {
    for (int i = 0; i < n; i++){
      TH.Dispatch(thread_fct, NULL);
      TH.WaitForIdle();
    }
}

int main(int argc, char **argv)  {

    printf("\n Beginning");
    // -------------------------------------------------------
    runWorkerThreads(2);
    // -------------------------------------------------------
    printf("\n Between I and II... ");
    runWorkerThreads(3);
    // -------------------------------------------------------
    printf("\n Between II and III... ");
    runWorkerThreads(1);
    // -------------------------------------------------------
    printf("\n End \n");

    return 0;
}
