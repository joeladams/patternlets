/* reduction.c
 * ... illustrates the OpenMP parallel-for loop's reduction clause
 *
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: ./reduction 
 *
 * Exercise:
 * - Compile and run.  Note that correct output is produced.
 * - Uncomment #pragma in function parallelSum(), 
 *    but leave its reduction clause commented out
 * - Recompile and rerun.  Note that correct output is NOT produced.
 * - Uncomment 'reduction(+:sum)' clause of #pragma in parallelSum()
 * - Recompile and rerun.  Note that correct output is produced again.
 */

#include <stdio.h>   // printf()
#include <omp.h>     // OpenMP
#include <stdlib.h>  // rand()

void initialize(int* a, int n);
int sequentialSum(int* a, int n);
int parallelSum(int* a, int n);

#define SIZE 1000000

int main(int argc, char** argv) {
   int array[SIZE];

   if (argc > 1) {
        omp_set_num_threads( atoi(argv[1]) );
   }

   initialize(array, SIZE);
   printf("\nSeq. sum: \t%d\nPar. sum: \t%d\n\n",
            sequentialSum(array, SIZE),
            parallelSum(array, SIZE) );

   return 0;
} 

/* fill array with random values */
void initialize(int* a, int n) {
   int i;
   for (i = 0; i < n; i++) {
      a[i] = rand() % 1000;
   }
}

/* sum the array sequentially */
int sequentialSum(int* a, int n) {
   int sum = 0;
   int i;
   for (i = 0; i < n; i++) {
      sum += a[i];
   }
   return sum;
}

/* sum the array using multiple threads */
int parallelSum(int* a, int n) {
   int sum = 0;
   int i;
//   #pragma omp parallel for // reduction(+:sum)
   for (i = 0; i < n; i++) {
      sum += a[i];
   }
   return sum;
}

