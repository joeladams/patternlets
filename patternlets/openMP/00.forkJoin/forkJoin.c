/* forkJoin.c
 * ... illustrates the fork-join pattern 
 *      using OpenMP's parallel directive.
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./forkJoin
 *
 * Exercise:
 * - Compile & run, uncomment the pragma,
 *    recompile & run, compare results.
 */

#include <stdio.h>     // printf()
#include <omp.h>       // OpenMP

int main(int argc, char** argv) {

    printf("\nBefore...\n");

//    #pragma omp parallel 
    printf("\nDuring...");

    printf("\n\nAfter...\n\n");

    return 0;
}

