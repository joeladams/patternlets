/* parallelLoopEqualChunks.c
 * ... illustrates the parallel for loop pattern in MPI 
 *	in which processes perform the loop's iterations in equal-sized 'chunks' 
 *	(preferable when loop iterations access memory/cache locations) ...
 * Joel Adams, Calvin College, November 2009.
 *
 * Usage: mpirun -np N ./parallelForEqualChunks
 *
 * Exercise:
 * - Compile and run, varying N: 1, 2, 3, 4, 5, 6, 7, 8
 * - Change REPS to 16, save, recompile, rerun, varying N again.
 * - Explain how this pattern divides the iterations of the loop
 *    among the processes.
 */

#include <stdio.h> // printf()
#include <mpi.h>   // MPI
#include <math.h>  // ceil()

int main(int argc, char** argv) {
    const int REPS = 8;
    int id = -1, numProcesses = -1, i = -1,
        start = -1, stop = -1, chunkSize = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    chunkSize = (int)ceil(((double)REPS) / numProcesses); // find chunk size
    start = id * chunkSize;                               // find starting index
                                                          // find stopping index:
    if ( id < numProcesses - 1 ) {                        //  if not the last process
        stop = (id + 1) * chunkSize;                      //   stop where next process starts
    } else {                                              //  else 
        stop = REPS;                                      //   last process does leftovers 
    }

    for (i = start; i < stop; i++) {                      // iterate through our range 
        printf("Process %d is performing iteration %d\n", id, i);
    }

    MPI_Finalize();
    return 0;
}

