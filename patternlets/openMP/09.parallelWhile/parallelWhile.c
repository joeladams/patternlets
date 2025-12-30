/* parallelWhile.c
 * shows how to implement parallel while loops in OpenMP,
 * which is useful when a parallel loop does not meet OpenMP's
 * requirement for a canonical for loop.
 *
 * Joel C. Adams, Calvin University, for CSinParallel, November 2025.
 *
 * Usage: OMP_NUM_THREADS=P ./parallelWhile
 *        where P is the number of threads to use.
 *
 * Exercise:
 * 1. Run using P = 1, 2, 3, 4, ..., 8, 9 threads;
 *    note the syntax each loop uses to produce its behavior.
 * 2. If still unclear, repeat #1 using REPS = 10, 12, 14, 16. 
 */

#include <stdio.h>                               // printf
#include <stdbool.h>                             // bool type
#include <omp.h>                                 // OpenMP functions

#define REPS 8                                   // iterations to perform

// Function implementing the 'slices' parallel while loop in OpenMP.
// Behavior is similar to #pragma omp parallel for schedule(static,1),
//  but allowing a more complex loop-control condition.
void slices() {
 bool notFound = true;                           // shared loop-control flag
 #pragma omp parallel                            // fork threads
 {
   int numThreads = omp_get_num_threads();       // how many workers?
   int id         = omp_get_thread_num();        // which one am I?
   long count     = id;                          // count from iteration id 
   while ( count < REPS && notFound) {
      printf("Thread %d performed iteration %ld\n", id, count);
      // ...do work that might set notFound to false...
      count += numThreads;                       // increment by P
   }
 }
}

#include "chunks.h"                              // getChunkStartStopValues()

// Function implementing the 'chunks' parallel while loop in OpenMP.
// Behavior is similar to #pragma omp parallel for
//  but allowing a more complex loop-control condition.
void chunks() {
 bool notFound = true;                           // shared loop-control flag
 #pragma omp parallel                            // fork threads
 {
   int numThreads = omp_get_num_threads();       // how many workers?
   int id         = omp_get_thread_num();        // which one am I?
   unsigned start = -1, stop = -1;               // get my start, stop values
   getChunkStartStopValues(id, numThreads, REPS, &start, &stop);
   long count    = start;                        // count from iteration 'start'
   while ( count < stop && notFound ) {
      printf("Thread %d performed iteration %ld\n", id, count);
      // ...do work that might set notFound to false...
      ++count;                                   // increment by 1
   }
 }
}

/* Print a labeled separator before slices and chunks output
 * @param: label, a char*
 * Precondition: label is either "slices" or "chunks"
 * Postcondition: a dashed line, the label, and a dashed line
 *                 have been displayed by the Master thread.
*/
void barrier(char* label) {
   #pragma omp barrier
   #pragma omp master
   printf("----- %s -----\n", label);
   #pragma omp barrier
}

int main() {
   barrier("slices");
   slices();
   barrier("chunks");
   chunks();
   return 0;
}

