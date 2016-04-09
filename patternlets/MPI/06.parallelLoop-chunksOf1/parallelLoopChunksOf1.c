/* parallelLoopChunksOf1.c
 * ... illustrates the parallel for loop pattern in MPI 
 *	in which processes perform the loop's iterations in 'chunks' 
 *      of size 1 (simple, and useful when loop iterations 
 *      do not access memory/cache locations) ...
 * Note this is much simpler than the 'equal chunks' loop.
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./parallelForSlices
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 3, 4, 5, 6, 7, 8
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 */

#include <stdio.h>  // printf()
#include <mpi.h>    // MPI

int main(int argc, char** argv) {
    const int REPS = 8;
    int id = -1, numProcesses = -1, i = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    for (i = id; i < REPS; i += numProcesses) {
        printf("Process %d is performing iteration %d\n", id, i);
    }

    MPI_Finalize();
    return 0;
}

