/* barrier.c
 * ... illustrates the behavior of MPI_Barrier() ...
 * Joel Adams, Calvin College, May 2013.
 *
 * Usage: mpirun -np 8 ./barrier
 *
 * Exercise: 
 *  - Compile; then run the program several times, 
 *     noting the intermixed outputs
 *  - Uncomment the MPI_Barrier() call; then recompile and rerun,
 *     noting how the output changes.
 *  - Explain what effect MPI_Barrier() has on process behavior.
 */

#include <stdio.h>   // printf()
#include <mpi.h>     // MPI

int main(int argc, char** argv) {
    int id = -1, numProcesses = -1, length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Get_processor_name (myHostName, &length);

    printf("Process #%d of %d on %s is BEFORE the barrier.\n",
             id, numProcesses, myHostName);

     MPI_Barrier(MPI_COMM_WORLD); 

    printf("Process #%d of %d on %s is AFTER the barrier.\n",
             id, numProcesses, myHostName);

    MPI_Finalize();
    return 0;
}

