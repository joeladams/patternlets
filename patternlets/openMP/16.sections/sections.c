/* sections.c
 * ... illustrates the use of OpenMP's parallel section/sections directives,
 *      which can be used for task parallelism...
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./sections
 *
 * Exercise: Compile, run (several times), compare output to source code.
 */

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    printf("\nBefore...\n\n");

    #pragma omp parallel sections num_threads(4)
    {
        #pragma omp section 
        {
            printf("Task/section A performed by thread %d\n", 
                    omp_get_thread_num() ); 
        }
        #pragma omp section 
        {
            printf("Task/section B performed by thread %d\n",
                    omp_get_thread_num() ); 
        }
        #pragma omp section
        {
            printf("Task/section C performed by thread %d\n",
                    omp_get_thread_num() ); 
        }
        #pragma omp section 
        {
                printf("Task/section D performed by thread %d\n", 
                         omp_get_thread_num() ); 
        }
    }

    printf("\nAfter...\n\n");

    return 0;
}

