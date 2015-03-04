/* masterWorker.c
 * ... illustrates the master-worker pattern in OpenMP
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./masterWorker
 *
 * Exercise: 
 * - Compile and run as is.
 * - Uncomment #pragma directive, re-compile and re-run
 * - Compare and trace the different executions.
 */

#include <stdio.h>   // printf()
#include <stdlib.h>   // atoi()
#include <omp.h>     // OpenMP

int main(int argc, char** argv) {
    printf("\n");
    if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
    }

//    #pragma omp parallel 
    {
        int id = omp_get_thread_num();
        int numThreads = omp_get_num_threads();

        if ( id == 0 ) {  // thread with ID 0 is master
            printf("Greetings from the master, # %d of %d threads\n",
			    id, numThreads);
        } else {          // threads with IDs > 0 are workers 
            printf("Greetings from a worker, # %d of %d threads\n",
			    id, numThreads);
        }
    }

    printf("\n");

    return 0;
}

