/* parallelLoopChunksOf1.c
 * ... illustrates how to make OpenMP map threads to 
 *	parallel loop iterations in chunks of size 1
 *	(use when not accesssing memory).
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./parallelLoopChunksOf1 [numThreads]
 *
 * Exercise:
 * 1. Compile and run, comparing output to source code,
 *    and to the output of the 'equal chunks' version.
 * 2. Uncomment the "commented out" code below,
 *    and verify that both loops produce the same output.
 *    The first loop is simpler but more restrictive;
 *    the second loop is more complex but less restrictive.
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const int REPS = 16;

    printf("\n");
    if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
    }

    #pragma omp parallel for schedule(static,1)
    for (int i = 0; i < REPS; i++) {
        int id = omp_get_thread_num();
        printf("Thread %d performed iteration %d\n", 
                 id, i);
    }

/*
    printf("\n---\n\n");

    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        for (int i = id; i < REPS; i += numThreads) {
            printf("Thread %d performed iteration %d\n", 
                     id, i);
        }
    }
*/
    printf("\n");
    return 0;
}

