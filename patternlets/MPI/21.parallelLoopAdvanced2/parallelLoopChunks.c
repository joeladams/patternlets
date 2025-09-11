/* parallelLoopChunks.c illustrates the parallel loop pattern in MPI
 *  in which processes perform the loop's iterations in 'chunks' whose
 *  sizes differ by at most 1 (useful when the number of iterations
 *  are not evenly divisible by the number of processes).
 *
 *  By Joel Adams, Calvin University, Sept 2025.
 *
 * Usage: mpirun -np N ./parallelLoopChunks
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 3, 4, 5, 6, 7, 8, 9
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 */

#include "chunks.h"

int main(int argc, char** argv) {
    const unsigned long REPS = 8;
    int id = -1, numProcesses = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    // find start and stop values of my chunk of the iterations 
    unsigned long start = 0, stop = 0;
    getChunkStartStopValues(id, numProcesses, REPS, &start, &stop);

    for (int i = start; i < stop; ++i) { // iterate through my chunk
        printf("Process %d is performing iteration %d\n", id, i);
    }
      
    MPI_Finalize();
    return 0;
}

