
/*
 * spmd.C
 *
 * Using Victor Alessandrini's vath_pth library.
 * ... illustrates the single-program-multiple-data (SPMD) pattern
 *
 * Modeled from code provided by Joel Adams, Calvin College, November 2009.
 * Hannah Sonsalla, Macalester College, 2017.
 *
 * Usage: ./spmd
 *
 * Exercise:
 * - Compile & run multiple times - what do you observe about the
 *   order of the printed lines?
 */

#include <stdlib.h>
#include <stdio.h>
#include <SPool.h>

SPool TH(8);

void thread_fct(void *P)  {

    int rank = TH.GetRank();
    printf("Hello from thread %d \n", rank);

}

int main(int argc, char **argv)  {

    TH.Dispatch(thread_fct, NULL);
    TH.WaitForIdle();

    return 0;
}
