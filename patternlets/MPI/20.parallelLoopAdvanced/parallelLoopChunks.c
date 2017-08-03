/* parallelLoopChunks.c
 *
 *  illustrates the parallel for loop pattern in MPI
 *	in which processes perform the loop's iterations in 'chunks'
 *  whose size differs by at most 1 (useful when iterations is not divisible
 *  by the number of processes)
 *
 *   by Libby Shoop, Macalester College, July 2017
 *
 * Usage: mpirun -np N ./parallelForChunks
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
    int id = -1, numProcesses = -1,
        start = -1, stop = -1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (numProcesses > REPS) {
      if (id == 0) {
          printf("Please run with -np less than or equal to %d\n.", REPS);
      }
    } else {

      // find chunk size for part of processes
      int chunkSize1 = (int)ceil(((double)REPS) / numProcesses);
      // chunk size to spread among rest of processes
      int chunkSize2 = chunkSize1 - 1;
      int remainder = REPS % numProcesses;

      // When remainder is 0, we have equal-sized chunks for all processes.
      // When remainder is not zero and the process id  is lower than remainder,
      // we use the higher chunk size.
      if (remainder == 0 || (remainder !=0 && id < remainder)) {
        start = id * chunkSize1;
        stop = start + chunkSize1;
      } else {
        // decrease chunk size by one to spread out across remaining
        // processes whose id is >= remainder
        start = (remainder * chunkSize1) + (chunkSize2 * (id - remainder));
        stop = start + chunkSize2;
      }

      for (int i = start; i < stop; i++) {     // iterate through our range
          printf("Process %d is performing iteration %d\n", id, i);
      }
      
    }
    MPI_Finalize();
    return 0;
}
