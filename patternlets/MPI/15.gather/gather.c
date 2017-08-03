/* gather.c
 * ... illustrates the use of MPI_Gather()...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./gather
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 4, 8.
 * - Trace execution through source.
 * - Explain behavior of MPI_Gather().
 */

#include <mpi.h>       // MPI
#include <stdio.h>     // printf()
#include <stdlib.h>    // malloc()

void print(int id, char* arrName, int* arr, int arrSize);

#define SIZE 3

int main(int argc, char** argv) {
   int  computeArray[SIZE];                          // array1
   int* gatherArray = NULL;                          // array2
   int  numProcs = -1, myRank = -1,
        totalGatheredVals = -1;

   MPI_Init(&argc, &argv);                           // initialize
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
                                                     // all processes:
   for (int i = 0; i < SIZE; i++) {                  //  load array1 with
      computeArray[i] = myRank * 10 + i;             //   3 distinct values
   }

   print(myRank, "computeArray", computeArray,       //  show array1
           SIZE);

   if (myRank == 0) {                                // master:
      totalGatheredVals = SIZE * numProcs;           //  allocate array2
      gatherArray = (int*) malloc( totalGatheredVals * sizeof(int) );
   }

   MPI_Gather(computeArray, SIZE, MPI_INT,           //  gather array1 vals
               gatherArray, SIZE, MPI_INT,           //   into array2
               0, MPI_COMM_WORLD);                   //   at master process

   if (myRank == 0) {                                // master process:
      print(myRank, "gatherArray",                   //  show array2
             gatherArray, totalGatheredVals);
      free(gatherArray);                             // clean up
   }


   MPI_Finalize();
   return 0;
}

void print(int id, char* arrName, int* arr, int arrSize) {
    printf("Process %d, %s: ", id, arrName);
    for (int i = 0; i < arrSize; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}
