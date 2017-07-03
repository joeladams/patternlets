/*
 * forkJoin.C
 *
 * Using Victor Alessandrini's vath_pth library
 * ... illustrates the fork-join pattern
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./forkJoin
 *
 * Exercise:
 * - Compile & run
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>

SPool TH(2);    // initialize threads, global

void thread_fct(void *P)  {
    printf("\n During... \n");
}

int main(int argc, char **argv)  {

    printf("\n Before... \n");
    // -------------------------------------------------------
    TH.Dispatch(thread_fct, NULL); // activates worker threads
    TH.WaitForIdle();   // joins all worker threads
    // -------------------------------------------------------
    printf("\n After... \n\n");

    return 0;
}
