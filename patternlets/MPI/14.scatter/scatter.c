/* scatter.c
 * ... illustrates the use of MPI_Scatter()...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./scatter
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 4, 8
 * - Trace execution through source code.
 * - Explain behavior/effect of MPI_Scatter().
 */

#include <mpi.h>      // MPI
#include <stdio.h>    // printf(), etc.
#include <stdlib.h>   // malloc()

void print(int id, char* arrName, int* arr, int arrSize);

int main(int argc, char** argv) {
    const int MAX = 8;
    int* arrSend = NULL;
    int* arrRcv = NULL;
    int numProcs = -1, myRank = -1, numSent = -1;

    MPI_Init(&argc, &argv);                            // initialize
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0) {                                 // master process:
        arrSend = (int*) malloc( MAX * sizeof(int) );  //  allocate array1
        for (int i = 0; i < MAX; i++) {                //  load with values
            arrSend[i] = (i+1) * 11;
        }
        print(myRank, "arrSend", arrSend, MAX);        //  display array1
    }
     
    numSent = MAX / numProcs;                          // all processes:
    arrRcv = (int*) malloc( numSent * sizeof(int) );   //  allocate array2

    MPI_Scatter(arrSend, numSent, MPI_INT, arrRcv,     //  scatter array1 
                 numSent, MPI_INT, 0, MPI_COMM_WORLD); //   into array2

    print(myRank, "arrRcv", arrRcv, numSent);          // display array2

    free(arrSend);                                     // clean up
    free(arrRcv);
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

