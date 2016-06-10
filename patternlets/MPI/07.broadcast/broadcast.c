/* broadcast.c
 * ... illustrates the use of MPI_Bcast() with a scalar value...
 *      (compare to array version).
 * Joel Adams, Calvin College, April 2016.
 *
 * Usage: mpirun -np N ./broadcast
 *
 * Exercise:
 * - Compile and run several times,
 *     using 2, 4, and 8 processes
 * - Use source code to trace execution and output
 *     (noting contents of file "data.txt");
 * - Explain behavior/effect of MPI_Bcast().
 */

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MASTER 0

int main(int argc, char** argv) {
    int answer = 0, length = 0;
    int myRank = 0;

    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Get_processor_name (myHostName, &length);

    if (myRank == MASTER) {
       FILE *filePtr = fopen("data.txt", "r"); 
       assert( filePtr != NULL );
       fscanf(filePtr, " %d", &answer);
       fclose(filePtr);
    }

    printf("BEFORE the broadcast, process %d on host '%s' has answer = %d\n",
             myRank, myHostName, answer);

    MPI_Bcast(&answer, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("AFTER the broadcast, process %d on host '%s' has answer = %d\n",
             myRank, myHostName, answer);

    MPI_Finalize();

    return 0;
}

