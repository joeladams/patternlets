/* parallelLoopEqualChunks.c
 * ... illustrates the use of OpenMP's default parallel for loop in which
 *  	threads iterate through equal sized chunks of the index range
 *	(cache-beneficial when accessing adjacent memory locations).
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./parallelLoopEqualChunks [numThreads]
 *
 * Exercise
 * - Compile and run, comparing output to source code
 * - try with different numbers of threads, e.g.: 2, 3, 4, 6, 8
 */

#include <stdio.h>    // printf()
#include <stdlib.h>   // atoi()
#include <omp.h>      // OpenMP

int main(int argc, char** argv) {
    const int REPS = 16;

    printf("\n");
    if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
    }

    #pragma omp parallel for  
    for (int i = 0; i < REPS; i++) {
        int id = omp_get_thread_num();
        printf("Thread %d performed iteration %d\n", 
                 id, i);
    }

    printf("\n");
    return 0;
}

