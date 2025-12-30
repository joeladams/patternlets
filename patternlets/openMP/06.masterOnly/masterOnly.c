/* masterOnly.c
 * ... illustrates the master-only pattern in OpenMP
 *
 * Joel Adams, Calvin College, November 2025.
 *
 * Usage: ./masterOnly [numThreads]
 *
 * Exercise: 
 * - Compile and run as is.
 * - Uncomment #pragma directive, re-compile and re-run
 * - Compare and trace how the executions differ.
 */

#include <stdio.h>   // printf()
#include <stdlib.h>   // atoi()
#include <omp.h>     // OpenMP

int main(int argc, char** argv) {
    printf("\n");
    if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
    }

    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();

//        #pragma omp master
        printf("I am the master, # %d of %d threads!\n",
			    id, numThreads);
    }

    printf("\n");

    return 0;
}

